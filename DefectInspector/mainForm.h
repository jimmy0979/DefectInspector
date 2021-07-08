#pragma once

#include <windows.h>

#include "Map.h"
#include "ROI.h"
#include "DataController.h"
#include "SqlCommunicator.h"

namespace DefectInspector {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// MyForm 的摘要
    /// </summary>
    public ref class mainForm : public System::Windows::Forms::Form {
    private:
        delegate void UpdateImage(Bitmap^ uImage);
        delegate void UpdateText(String^ uText, int mode);
    public:
        mainForm(void) {
            InitializeComponent();
            //
            //TODO:  在此加入建構函式程式碼
            //
        }

    protected:
        /// <summary>
        /// 清除任何使用中的資源。
        /// </summary>
        ~mainForm() {
            if (components) {
                delete components;
            }
        }

    private:
        System::Windows::Forms::PictureBox^ imgROI;

    protected:
    private:
        System::Windows::Forms::PictureBox^ imgMap;

    private:
        System::Windows::Forms::Button^ btnConnectSql;

    private:
        System::Windows::Forms::Label^ lblInfo;
    private: System::Windows::Forms::Button^ btnUpdate;
    private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart1;

    private:
        /// <summary>
        /// 設計工具所需的變數。
        /// </summary>
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
        /// 這個方法的內容。
        /// </summary>
        void InitializeComponent(void) {
            System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
            System::Windows::Forms::DataVisualization::Charting::Legend^ legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
            System::Windows::Forms::DataVisualization::Charting::Series^ series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
            this->imgROI = (gcnew System::Windows::Forms::PictureBox());
            this->imgMap = (gcnew System::Windows::Forms::PictureBox());
            this->btnConnectSql = (gcnew System::Windows::Forms::Button());
            this->lblInfo = (gcnew System::Windows::Forms::Label());
            this->btnUpdate = (gcnew System::Windows::Forms::Button());
            this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imgROI))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imgMap))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
            this->SuspendLayout();
            // 
            // imgROI
            // 
            this->imgROI->Location = System::Drawing::Point(13, 14);
            this->imgROI->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
            this->imgROI->MinimumSize = System::Drawing::Size(1000, 1000);
            this->imgROI->Name = L"imgROI";
            this->imgROI->Size = System::Drawing::Size(1000, 1000);
            this->imgROI->TabIndex = 0;
            this->imgROI->TabStop = false;
            this->imgROI->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainForm::imgROI_MouseDown);
            // 
            // imgMap
            // 
            this->imgMap->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->imgMap->Location = System::Drawing::Point(1037, 14);
            this->imgMap->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
            this->imgMap->MinimumSize = System::Drawing::Size(240, 480);
            this->imgMap->Name = L"imgMap";
            this->imgMap->Size = System::Drawing::Size(360, 607);
            this->imgMap->TabIndex = 1;
            this->imgMap->TabStop = false;
            this->imgMap->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainForm::imgMap_MouseDown);
            // 
            // btnConnectSql
            // 
            this->btnConnectSql->Location = System::Drawing::Point(1437, 553);
            this->btnConnectSql->Name = L"btnConnectSql";
            this->btnConnectSql->Size = System::Drawing::Size(187, 56);
            this->btnConnectSql->TabIndex = 2;
            this->btnConnectSql->Text = L"Connect";
            this->btnConnectSql->UseVisualStyleBackColor = true;
            this->btnConnectSql->Click += gcnew System::EventHandler(this, &mainForm::btnConnectSql_Click);
            // 
            // lblInfo
            // 
            this->lblInfo->Location = System::Drawing::Point(1439, 14);
            this->lblInfo->Name = L"lblInfo";
            this->lblInfo->Size = System::Drawing::Size(192, 442);
            this->lblInfo->TabIndex = 3;
            this->lblInfo->Text = L"NONE";
            // 
            // btnUpdate
            // 
            this->btnUpdate->Location = System::Drawing::Point(1437, 480);
            this->btnUpdate->Name = L"btnUpdate";
            this->btnUpdate->Size = System::Drawing::Size(187, 57);
            this->btnUpdate->TabIndex = 4;
            this->btnUpdate->Text = L"Update";
            this->btnUpdate->UseVisualStyleBackColor = true;
            this->btnUpdate->Click += gcnew System::EventHandler(this, &mainForm::btnUpdate_Click);
            // 
            // chart1
            // 
            chartArea1->Name = L"ChartArea1";
            this->chart1->ChartAreas->Add(chartArea1);
            legend1->Name = L"Legend1";
            this->chart1->Legends->Add(legend1);
            this->chart1->Location = System::Drawing::Point(1037, 629);
            this->chart1->Name = L"chart1";
            series1->ChartArea = L"ChartArea1";
            series1->Legend = L"Legend1";
            series1->Name = L"Series1";
            this->chart1->Series->Add(series1);
            this->chart1->Size = System::Drawing::Size(587, 372);
            this->chart1->TabIndex = 5;
            this->chart1->Text = L"chart1";
            // 
            // mainForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(13, 28);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
            this->ClientSize = System::Drawing::Size(1643, 1036);
            this->Controls->Add(this->chart1);
            this->Controls->Add(this->btnUpdate);
            this->Controls->Add(this->lblInfo);
            this->Controls->Add(this->btnConnectSql);
            this->Controls->Add(this->imgMap);
            this->Controls->Add(this->imgROI);
            this->Font = (gcnew System::Drawing::Font(L"Consolas", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->KeyPreview = true;
            this->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
            this->Name = L"mainForm";
            this->Text = L"mainForm";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &mainForm::mainForm_FormClosing);
            this->Load += gcnew System::EventHandler(this, &mainForm::mainForm_Load);
            this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &mainForm::mainForm_KeyDown);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imgROI))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imgMap))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
            this->ResumeLayout(false);

        }
#pragma endregion
    private:
        System::Void mainForm_Load(System::Object^ sender, System::EventArgs^ e);
        System::Void btnConnectSql_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void btnUpdate_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void imgMap_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        System::Void imgROI_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        System::Void mainForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e);
        System::Void mainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
        System::Void connectToDb(System::Void);

        // for delegate use
        System::Void UpdateMapperBitmap(Bitmap^ uImage);
        System::Void UpdatePainterBitmap(Bitmap^ uImage);
        System::Void UpdateInfoText(String^ uText, int mode);

        // for drop & amplify
        System::Void Drop(int dir);
        System::Void Amplify(bool amplifyFlag);

        // Plots & Info lists
        System::Void updateRealTimeInfo();
        System::Void updateInfoList();
        System::Void updatePlot();
};
    // namespace DefectInspector
}