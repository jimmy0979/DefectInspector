#pragma once

#include <windows.h>

#include "DieMapper.h"
#include "DiePainter.h"
#include "SqlCommunicator.h"

namespace DefectInspector {

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class mainForm : public System::Windows::Forms::Form {
private: 
    delegate void UpdateImage(Bitmap^ uImage);
    delegate void UpdateText(String^ uText, int mode);
public:
    mainForm(void) {
        InitializeComponent();
    }

protected:
    ~mainForm() {
        if (components) {
            delete components;
        }
    }

    private:
    System::Windows::Forms::PictureBox ^ imgROI;

    protected:
    private:
    System::Windows::Forms::PictureBox ^ imgMap;

    private:
    System::Windows::Forms::Button ^ btnConnectSql;

    private:
    System::Windows::Forms::Label ^ lblInfo;

    private:
    System::ComponentModel::Container ^ components;

#pragma region Windows Form Designer generated code
    void InitializeComponent(void) {
        this->imgROI = (gcnew System::Windows::Forms::PictureBox());
        this->imgMap = (gcnew System::Windows::Forms::PictureBox());
        this->btnConnectSql = (gcnew System::Windows::Forms::Button());
        this->lblInfo = (gcnew System::Windows::Forms::Label());
        (cli::safe_cast<System::ComponentModel::ISupportInitialize ^>(this->imgROI))->BeginInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize ^>(this->imgMap))->BeginInit();
        this->SuspendLayout();
        //
        // imgROI
        //
        this->imgROI->Location = System::Drawing::Point(34, 41);
        this->imgROI->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
        this->imgROI->MinimumSize = System::Drawing::Size(900, 900);
        this->imgROI->Name = L"imgROI";
        this->imgROI->Size = System::Drawing::Size(900, 900);
        this->imgROI->TabIndex = 0;
        this->imgROI->TabStop = false;
        //
        // imgMap
        //
        this->imgMap->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
        this->imgMap->Location = System::Drawing::Point(998, 41);
        this->imgMap->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
        this->imgMap->MinimumSize = System::Drawing::Size(240, 480);
        this->imgMap->Name = L"imgMap";
        this->imgMap->Size = System::Drawing::Size(240, 607);
        this->imgMap->TabIndex = 1;
        this->imgMap->TabStop = false;
        this->imgMap->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &mainForm::imgMap_MouseDown);
        //
        // btnConnectSql
        //
        this->btnConnectSql->Location = System::Drawing::Point(998, 885);
        this->btnConnectSql->Name = L"btnConnectSql";
        this->btnConnectSql->Size = System::Drawing::Size(187, 56);
        this->btnConnectSql->TabIndex = 2;
        this->btnConnectSql->Text = L"Connect";
        this->btnConnectSql->UseVisualStyleBackColor = true;
        this->btnConnectSql->Click += gcnew System::EventHandler(this, &mainForm::btnConnectSql_Click);
        //
        // lblInfo
        //
        this->lblInfo->Location = System::Drawing::Point(998, 685);
        this->lblInfo->Name = L"lblInfo";
        this->lblInfo->Size = System::Drawing::Size(240, 84);
        this->lblInfo->TabIndex = 3;
        this->lblInfo->Text = L"NONE";
        //
        // mainForm
        //
        this->AutoScaleDimensions = System::Drawing::SizeF(13, 28);
        this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        this->ClientSize = System::Drawing::Size(1297, 980);
        this->Controls->Add(this->lblInfo);
        this->Controls->Add(this->btnConnectSql);
        this->Controls->Add(this->imgMap);
        this->Controls->Add(this->imgROI);
        this->Font = (gcnew System::Drawing::Font(L"Consolas", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                                                  static_cast<System::Byte>(0)));
        this->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
        this->Name = L"mainForm";
        this->Text = L"mainForm";
        this->Load += gcnew System::EventHandler(this, &mainForm::mainForm_Load);
        (cli::safe_cast<System::ComponentModel::ISupportInitialize ^>(this->imgROI))->EndInit();
        (cli::safe_cast<System::ComponentModel::ISupportInitialize ^>(this->imgMap))->EndInit();
        this->ResumeLayout(false);
    }
#pragma endregion
    private:
    System::Void mainForm_Load(System::Object ^ sender, System::EventArgs ^ e);
    System::Void btnConnectSql_Click(System::Object ^ sender, System::EventArgs ^ e);
    System::Void imgMap_MouseDown(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e);

    System::Void connectToDb(System::Void);

    // for delegate use
    System::Void UpdateMapperBitmap(Bitmap^ uImage);
    System::Void UpdatePainterBitmap(Bitmap^ uImage);
    System::Void UpdateInfoText(String^ uText, int mode);
};
}   // namespace DefectInspector
