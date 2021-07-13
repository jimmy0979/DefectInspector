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
    private: System::Windows::Forms::TabPage^ tpInfo;
    private: System::Windows::Forms::TabPage^ tpSelectedDies;



    private: System::Windows::Forms::ListBox^ listDieInfo;
    private: System::Windows::Forms::TabControl^ tctrlImage;
    private: System::Windows::Forms::TabPage^ tpROI;
    private: System::Windows::Forms::TabPage^ tpStackImage;
    private: System::Windows::Forms::TabPage^ tpSelectedFrames;






    private: System::Windows::Forms::ListBox^ listFrames;
    private: System::Windows::Forms::PictureBox^ imgCompared;
    private: System::Windows::Forms::ComboBox^ Filter_comboBox;

    private: System::Windows::Forms::Label^ label1;
    private: System::Windows::Forms::TextBox^ FilterBox;

    private: System::Windows::Forms::Button^ filter_button;



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
            this->tpInfo = (gcnew System::Windows::Forms::TabPage());
            this->tpSelectedDies = (gcnew System::Windows::Forms::TabPage());
            this->listDieInfo = (gcnew System::Windows::Forms::ListBox());
            this->tpSelectedFrames = (gcnew System::Windows::Forms::TabPage());
            this->listFrames = (gcnew System::Windows::Forms::ListBox());
            this->tctrlImage = (gcnew System::Windows::Forms::TabControl());
            this->tpROI = (gcnew System::Windows::Forms::TabPage());
            this->tpStackImage = (gcnew System::Windows::Forms::TabPage());
            this->imgCompared = (gcnew System::Windows::Forms::PictureBox());
            this->Filter_comboBox = (gcnew System::Windows::Forms::ComboBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->FilterBox = (gcnew System::Windows::Forms::TextBox());
            this->filter_button = (gcnew System::Windows::Forms::Button());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imgROI))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imgMap))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartDies))->BeginInit();
            this->tctrlInfo->SuspendLayout();
            this->tpInfo->SuspendLayout();
            this->tpSelectedDies->SuspendLayout();
            this->tpSelectedFrames->SuspendLayout();
            this->tctrlImage->SuspendLayout();
            this->tpROI->SuspendLayout();
            this->tpStackImage->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imgCompared))->BeginInit();
            this->SuspendLayout();
            // 
            // imgROI
            // 
            this->imgROI->Location = System::Drawing::Point(7, 8);
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
            this->imgMap->BackColor = System::Drawing::SystemColors::ControlLightLight;
            this->imgMap->Location = System::Drawing::Point(1090, 14);
            this->imgMap->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
            this->imgMap->MinimumSize = System::Drawing::Size(240, 480);
            this->imgMap->Name = L"imgMap";
            this->imgMap->Size = System::Drawing::Size(240, 480);
            this->imgMap->TabIndex = 1;
            this->imgMap->TabStop = false;
            this->imgMap->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainForm::imgMap_MouseDown);
            // 
            // btnConnectSql
            // 
            this->btnConnectSql->Location = System::Drawing::Point(1612, 511);
            this->btnConnectSql->Name = L"btnConnectSql";
            this->btnConnectSql->Size = System::Drawing::Size(187, 56);
            this->btnConnectSql->TabIndex = 2;
            this->btnConnectSql->Text = L"Connect";
            this->btnConnectSql->UseVisualStyleBackColor = true;
            this->btnConnectSql->Click += gcnew System::EventHandler(this, &mainForm::btnConnectSql_Click);
            // 
            // lblInfo
            // 
            this->lblInfo->Location = System::Drawing::Point(17, 13);
            this->lblInfo->Name = L"lblInfo";
            this->lblInfo->Size = System::Drawing::Size(392, 418);
            this->lblInfo->TabIndex = 3;
            this->lblInfo->Text = L"NONE";
            // 
            // btnUpdate
            // 
            this->btnUpdate->Location = System::Drawing::Point(1366, 511);
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
            this->chartDies->Location = System::Drawing::Point(1090, 629);
            this->chartDies->Name = L"chartDies";
            series1->ChartArea = L"ChartArea1";
            series1->Legend = L"Legend1";
            series1->Name = L"Series1";
            this->chartDies->Series->Add(series1);
            this->chartDies->Size = System::Drawing::Size(637, 383);
            this->chartDies->TabIndex = 5;
            this->chartDies->Text = L"chartDies";
            // 
            // tctrlInfo
            // 
            this->tctrlInfo->Controls->Add(this->tpInfo);
            this->tctrlInfo->Controls->Add(this->tpSelectedDies);
            this->tctrlInfo->Controls->Add(this->tpSelectedFrames);
            this->tctrlInfo->Location = System::Drawing::Point(1366, 4);
            this->tctrlInfo->Name = L"tctrlInfo";
            this->tctrlInfo->SelectedIndex = 0;
            this->tctrlInfo->Size = System::Drawing::Size(433, 490);
            this->tctrlInfo->TabIndex = 6;
            // 
            // tpInfo
            // 
            this->tpInfo->Controls->Add(this->lblInfo);
            this->tpInfo->Location = System::Drawing::Point(4, 37);
            this->tpInfo->Name = L"tpInfo";
            this->tpInfo->Padding = System::Windows::Forms::Padding(3);
            this->tpInfo->Size = System::Drawing::Size(425, 449);
            this->tpInfo->TabIndex = 0;
            this->tpInfo->Text = L"info";
            this->tpInfo->UseVisualStyleBackColor = true;
            // 
            // tpSelectedDies
            // 
            this->tpSelectedDies->Controls->Add(this->listDieInfo);
            this->tpSelectedDies->Location = System::Drawing::Point(4, 37);
            this->tpSelectedDies->Name = L"tpSelectedDies";
            this->tpSelectedDies->Padding = System::Windows::Forms::Padding(3);
            this->tpSelectedDies->Size = System::Drawing::Size(425, 449);
            this->tpSelectedDies->TabIndex = 1;
            this->tpSelectedDies->Text = L"Selected Dies";
            this->tpSelectedDies->UseVisualStyleBackColor = true;
            // 
            // listDieInfo
            // 
            this->listDieInfo->FormattingEnabled = true;
            this->listDieInfo->ItemHeight = 28;
            this->listDieInfo->Location = System::Drawing::Point(21, 19);
            this->listDieInfo->Name = L"listDieInfo";
            this->listDieInfo->Size = System::Drawing::Size(386, 396);
            this->listDieInfo->TabIndex = 0;
            // 
            // tpSelectedFrames
            // 
            this->tpSelectedFrames->Controls->Add(this->listFrames);
            this->tpSelectedFrames->Location = System::Drawing::Point(4, 37);
            this->tpSelectedFrames->Name = L"tpSelectedFrames";
            this->tpSelectedFrames->Size = System::Drawing::Size(425, 449);
            this->tpSelectedFrames->TabIndex = 2;
            this->tpSelectedFrames->Text = L"Selected Frames";
            this->tpSelectedFrames->UseVisualStyleBackColor = true;
            // 
            // listFrames
            // 
            this->listFrames->FormattingEnabled = true;
            this->listFrames->ItemHeight = 28;
            this->listFrames->Location = System::Drawing::Point(15, 15);
            this->listFrames->Name = L"listFrames";
            this->listFrames->Size = System::Drawing::Size(393, 396);
            this->listFrames->TabIndex = 0;
            // 
            // tctrlImage
            // 
            this->tctrlImage->Alignment = System::Windows::Forms::TabAlignment::Left;
            this->tctrlImage->Controls->Add(this->tpROI);
            this->tctrlImage->Controls->Add(this->tpStackImage);
            this->tctrlImage->Location = System::Drawing::Point(-1, 0);
            this->tctrlImage->Multiline = true;
            this->tctrlImage->Name = L"tctrlImage";
            this->tctrlImage->SelectedIndex = 0;
            this->tctrlImage->Size = System::Drawing::Size(1060, 1024);
            this->tctrlImage->TabIndex = 7;
            this->tctrlImage->SelectedIndexChanged += gcnew System::EventHandler(this, &mainForm::tabControl1_SelectedIndexChanged);
            // 
            // tpROI
            // 
            this->tpROI->Controls->Add(this->imgROI);
            this->tpROI->Location = System::Drawing::Point(39, 4);
            this->tpROI->Name = L"tpROI";
            this->tpROI->Padding = System::Windows::Forms::Padding(3);
            this->tpROI->Size = System::Drawing::Size(1017, 1016);
            this->tpROI->TabIndex = 0;
            this->tpROI->Text = L"ROI";
            this->tpROI->UseVisualStyleBackColor = true;
            // 
            // tpStackImage
            // 
            this->tpStackImage->Controls->Add(this->imgCompared);
            this->tpStackImage->Location = System::Drawing::Point(39, 4);
            this->tpStackImage->Name = L"tpStackImage";
            this->tpStackImage->Padding = System::Windows::Forms::Padding(3);
            this->tpStackImage->Size = System::Drawing::Size(1017, 1016);
            this->tpStackImage->TabIndex = 1;
            this->tpStackImage->Text = L"stackImage";
            this->tpStackImage->UseVisualStyleBackColor = true;
            // 
            // imgCompared
            // 
            this->imgCompared->Location = System::Drawing::Point(6, 6);
            this->imgCompared->MaximumSize = System::Drawing::Size(1000, 1000);
            this->imgCompared->MinimumSize = System::Drawing::Size(1000, 1000);
            this->imgCompared->Name = L"imgCompared";
            this->imgCompared->Size = System::Drawing::Size(1000, 1000);
            this->imgCompared->TabIndex = 8;
            this->imgCompared->TabStop = false;
            // 
            // Filter_comboBox
            // 
            this->Filter_comboBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
            this->Filter_comboBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
            this->Filter_comboBox->Cursor = System::Windows::Forms::Cursors::Default;
            this->Filter_comboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->Filter_comboBox->FormattingEnabled = true;
            this->Filter_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"All Defeat", L"Normal Dies" });
            this->Filter_comboBox->Location = System::Drawing::Point(1090, 540);
            this->Filter_comboBox->Name = L"Filter_comboBox";
            this->Filter_comboBox->Size = System::Drawing::Size(240, 36);
            this->Filter_comboBox->TabIndex = 8;
            this->Filter_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &mainForm::Filter_comboBox_SelectedIndexChanged);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(1086, 511);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(155, 28);
            this->label1->TabIndex = 9;
            this->label1->Text = L"Filter Type";
            // 
            // FilterBox
            // 
            this->FilterBox->Location = System::Drawing::Point(1200, 508);
            this->FilterBox->Name = L"FilterBox";
            this->FilterBox->Size = System::Drawing::Size(129, 36);
            this->FilterBox->TabIndex = 10;
            // 
            // filter_button
            // 
            this->filter_button->Location = System::Drawing::Point(1090, 574);
            this->filter_button->Name = L"filter_button";
            this->filter_button->Size = System::Drawing::Size(75, 23);
            this->filter_button->TabIndex = 11;
            this->filter_button->Text = L"Enter";
            this->filter_button->UseVisualStyleBackColor = true;
            this->filter_button->Click += gcnew System::EventHandler(this, &mainForm::filter_button_Click);
            // 
            // mainForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(13, 28);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
            this->BackColor = System::Drawing::SystemColors::ActiveCaption;
            this->ClientSize = System::Drawing::Size(1820, 1024);
            this->Controls->Add(this->filter_button);
            this->Controls->Add(this->FilterBox);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->Filter_comboBox);
            this->Controls->Add(this->tctrlImage);
            this->Controls->Add(this->tctrlInfo);
            this->Controls->Add(this->chartDies);
            this->Controls->Add(this->btnUpdate);
            this->Controls->Add(this->btnConnectSql);
            this->Controls->Add(this->imgMap);
            this->Font = (gcnew System::Drawing::Font(L"Consolas", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->KeyPreview = true;
            this->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
            this->Name = L"mainForm";
            this->Text = L"mainForm";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &mainForm::mainForm_FormClosing);
            this->Load += gcnew System::EventHandler(this, &mainForm::mainForm_Load);
            this->Click += gcnew System::EventHandler(this, &mainForm::mainForm_Click);
            this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &mainForm::mainForm_KeyDown);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imgROI))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imgMap))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartDies))->EndInit();
            this->tctrlInfo->ResumeLayout(false);
            this->tpInfo->ResumeLayout(false);
            this->tpSelectedDies->ResumeLayout(false);
            this->tpSelectedFrames->ResumeLayout(false);
            this->tctrlImage->ResumeLayout(false);
            this->tpROI->ResumeLayout(false);
            this->tpStackImage->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imgCompared))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

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
        System::Void tabControl1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
        System::Void Filter_comboBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
        System::Void filter_button_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void mainForm_Click(System::Object^ sender, System::EventArgs^ e);

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