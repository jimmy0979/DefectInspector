#include "mainForm.h"

#include <time.h>

using namespace DefectInspector;
using namespace System::Threading;

//===============================
// Global Variables Declartion

// OpenCV MAT Image
DiePainter* ROI;
DieMapper* Map;
vector<vector<pair<int, int>>> Dies;

//===============================

//---------------------------------------------------------------------

System::Drawing::Bitmap ^ MatToBitmap(cv::Mat img) {
    // covert OpenCV::Mat to Bitmap^
    // in order to show on clr::pictureBox simply

    System::IntPtr ptr(img.ptr());
    System::Drawing::Bitmap ^ b = gcnew System::Drawing::Bitmap(img.cols, img.rows, img.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, ptr);
    return b;
}

//---------------------------------------------------------------------

System::Void mainForm::mainForm_Load(System::Object ^ sender, System::EventArgs ^ e) {
    // mainForm_load

    // initial Painter, Mapper
    ROI = new DiePainter(900, 900);
    Map = new DieMapper();
}

//---------------------------------------------------------------------

System::Void mainForm::btnConnectSql_Click(System::Object ^ sender, System::EventArgs ^ e) {
    // click to connect
    // connect to dataBase
    connectToDb();

    // TODO : use multi-thread to deal with connectToDb(),
    //          or it will cause GUI react like "no response"
    // Thread^ thr1 = gcnew ::Thread(gcnew::Threading::ThreadStart(connectToDb));
}

System::Void mainForm::imgMap_MouseDown(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e) {
    // trigger function
    // whenever selected region on Mapper changing, Painter should refresh to correspond die infor.

    // TODO : amplify function
    if (e->Button == System::Windows::Forms::MouseButtons::Left) {
        int region = (e->Y / 30) * 8 + (e->X / 30);
        cout << "Mouse move over the window - Region (" << region << ")" << endl;

        ROI->clear();
        for (int i = 0; i < Dies[region].size(); i++) {
            ROI->paint(Dies[region][i].first, Dies[region][i].second);
        }

        imgROI->Image = MatToBitmap(ROI->returnMat());
    }
}

//---------------------------------------------------------------------
// Datbase related
System::Void mainForm::connectToDb(System::Void) {
    // the return state of each ODBC command, initialize to Success
    SQLRETURN retCode = SQL_SUCCESS;

    // SQL ODBC Connect
    SQLBIGINT region, diex, diey;
    SQLLEN cbRegion = 0, cbDiex = 0, cbDiey = 0;

    // connect to database with its ConnectString
    SqlCommunicator* sql = new SqlCommunicator(L"Driver={ODBC Driver 17 for SQL Server};server=localhost;database=test;trusted_connection=Yes;");
    // make a query and get the statmentHandle
    SQLHSTMT hstmt = sql->sqlQuery(L"SELECT [Region], [DieX], [DieY] FROM [test].[dbo].[2274_DefectData_TEST_PartALL]");

    // seperate each column from HSTMT
    SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &region, 0, &cbRegion);
    SQLBindCol(hstmt, 2, SQL_C_SBIGINT, &diex, 0, &cbDiex);
    SQLBindCol(hstmt, 3, SQL_C_SBIGINT, &diey, 0, &cbDiey);

    // put each info into dataStructure
    clock_t start = clock();
    int each = 0;
    for (int i = 0;; i++) {
        retCode = SQLFetch(hstmt);
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
            // cout << i + 1 << " : " << "( " << diex << ", " << diey << " )" << endl;
            if (region > Dies.size()) {
                if (region >= 2) {
                    Map->paint(region - 2, each);
                    each = 0;
                }
                Dies.push_back({});
            }

            Dies[region - 1].push_back({ diex, diey });
            each++;
        }
        else {
            Map->paint(region - 1, each);
            each = 0;

            break;
        }
    }
    sql->close();

    // the consuming time used to load data
    float cost = (float)(clock() - start) / CLOCKS_PER_SEC;
    lblInfo->Text = cost.ToString() + "\n";

    // default area = 0 in Painter
    start = clock();
    int area = 0;
    for (int i = 0; i < Dies[area].size(); i++) {
        ROI->paint(Dies[area][i].first, Dies[area][i].second);
    }
    imgROI->Image = MatToBitmap(ROI->returnMat());
    imgMap->Image = MatToBitmap(Map->returnMat());

    // the consuming time that used to paint die on region 0
    cost = (float)(clock() - start) / CLOCKS_PER_SEC;
    lblInfo->Text += cost.ToString() + "\n";
}

//---------------------------------------------------------------------