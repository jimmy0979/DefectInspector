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
    private: System::Windows::Forms::DataVisualization::Charting::Chart^ chartDies;

    private: System::Windows::Forms::TabControl^ tctrlInfo;

    private: System::Windows::Forms::TabPage^ tabPage1;
    private: System::Windows::Forms::TabPage^ tabPage2;
    private: System::Windows::Forms::ListBox^ listDieInfo;



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
            this->chartDies = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
            this->tctrlInfo = (gcnew System::Windows::Forms::TabControl());
            this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
            this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
            this->listDieInfo = (gcnew System::Windows::Forms::ListBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imgROI))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imgMap))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartDies))->BeginInit();
            this->tctrlInfo->SuspendLayout();
            this->tabPage1->SuspendLayout();
            this->tabPage2->SuspendLayout();
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
            this->btnConnectSql->Location = System::Drawing::Point(1647, 565);
            this->btnConnectSql->Name = L"btnConnectSql";
            this->btnConnectSql->Size = System::Drawing::Size(187, 56);
            this->btnConnectSql->TabIndex = 2;
            this->btnConnectSql->Text = L"Connect";
            this->btnConnectSql->UseVisualStyleBackColor = true;
            this->btnConnectSql->Click += gcnew System::EventHandler(this, &mainForm::btnConnectSql_Click);
            // 
            // lblInfo
            // 
            this->lblInfo->Location = System::Drawing::Point(16, 13);
            this->lblInfo->Name = L"lblInfo";
            this->lblInfo->Size = System::Drawing::Size(430, 459);
            this->lblInfo->TabIndex = 3;
            this->lblInfo->Text = L"NONE";
            // 
            // btnUpdate
            // 
            this->btnUpdate->Location = System::Drawing::Point(1428, 564);
            this->btnUpdate->Name = L"btnUpdate";
            this->btnUpdate->Size = System::Drawing::Size(187, 57);
            this->btnUpdate->TabIndex = 4;
            this->btnUpdate->Text = L"Update";
            this->btnUpdate->UseVisualStyleBackColor = true;
            this->btnUpdate->Click += gcnew System::EventHandler(this, &mainForm::btnUpdate_Click);
            // 
            // chartDies
            // 
            this->chartDies->BackColor = System::Drawing::Color::Transparent;
            chartArea1->Name = L"ChartArea1";
            this->chartDies->ChartAreas->Add(chartArea1);
            legend1->Name = L"Legend1";
            this->chartDies->Legends->Add(legend1);
            this->chartDies->Location = System::Drawing::Point(1037, 629);
            this->chartDies->Name = L"chartDies";
            series1->ChartArea = L"ChartArea1";
            series1->Legend = L"Legend1";
            series1->Name = L"Series1";
            this->chartDies->Series->Add(series1);
            this->chartDies->Size = System::Drawing::Size(639, 372);
            this->chartDies->TabIndex = 5;
            this->chartDies->Text = L"chart1";
            // 
            // tctrlInfo
            // 
            this->tctrlInfo->Controls->Add(this->tabPage1);
            this->tctrlInfo->Controls->Add(this->tabPage2);
            this->tctrlInfo->Location = System::Drawing::Point(1424, 14);
            this->tctrlInfo->Name = L"tctrlInfo";
            this->tctrlInfo->SelectedIndex = 0;
            this->tctrlInfo->Size = System::Drawing::Size(471, 532);
            this->tctrlInfo->TabIndex = 6;
            // 
            // tabPage1
            // 
            this->tabPage1->Controls->Add(this->lblInfo);
            this->tabPage1->Location = System::Drawing::Point(4, 37);
            this->tabPage1->Name = L"tabPage1";
            this->tabPage1->Padding = System::Windows::Forms::Padding(3);
            this->tabPage1->Size = System::Drawing::Size(463, 491);
            this->tabPage1->TabIndex = 0;
            this->tabPage1->Text = L"RealTime-info";
            this->tabPage1->UseVisualStyleBackColor = true;
            // 
            // tabPage2
            // 
            this->tabPage2->Controls->Add(this->listDieInfo);
            this->tabPage2->Location = System::Drawing::Point(4, 37);
            this->tabPage2->Name = L"tabPage2";
            this->tabPage2->Padding = System::Windows::Forms::Padding(3);
            this->tabPage2->Size = System::Drawing::Size(463, 491);
            this->tabPage2->TabIndex = 1;
            this->tabPage2->Text = L"Selected Feature";
            this->tabPage2->UseVisualStyleBackColor = true;
            // 
            // listDieInfo
            // 
            this->listDieInfo->FormattingEnabled = true;
            this->listDieInfo->ItemHeight = 28;
            this->listDieInfo->Location = System::Drawing::Point(21, 19);
            this->listDieInfo->Name = L"listDieInfo";
            this->listDieInfo->Size = System::Drawing::Size(422, 452);
            this->listDieInfo->TabIndex = 0;
            // 
            // mainForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(13, 28);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
            this->ClientSize = System::Drawing::Size(1917, 1036);
            this->Controls->Add(this->tctrlInfo);
            this->Controls->Add(this->chartDies);
            this->Controls->Add(this->btnUpdate);
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
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartDies))->EndInit();
            this->tctrlInfo->ResumeLayout(false);
            this->tabPage1->ResumeLayout(false);
            this->tabPage2->ResumeLayout(false);
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