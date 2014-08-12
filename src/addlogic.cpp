#include "addlogic.h"
#include "ui_addlogic.h"
#include "Util.h"
AddLogic::AddLogic(MainWindow *mwp,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLogic)
{
    ui->setupUi(this);
    lt = fIN;
    wp = mwp;

    SelectType(fIN);
    ui->In->setChecked(true);
    ui->Setup->setVisible(wp->findNode("setup")==0);
}

AddLogic::~AddLogic()
{
    delete ui;
}

QString AddLogic::GetName() const
{
    return ui->Name->text();
}

LOGICTYPE AddLogic::GetLogicType() const
{
    return lt;
}


void AddLogic::Load(Node *v)
{
    switch(v->GetLogicType())
    {
    case fSETUP: ui->Setup->setChecked(true);
        break;
    case fIN: ui->In->setChecked(true); break;
    case fOUT: ui->Out->setChecked(true); break;
    case fFUZZY: ui->Fuzzy->setChecked(true); break;
    case fAND: ui->And->setChecked(true); break;
    case fOR: ui->Or->setChecked(true); break;
    case fNOT: ui->Not->setChecked(true); break;
    case fPID: ui->Pid->setChecked(true); break;
    case fTIMER:    ui->Timer->setChecked(true); break;
    }
    SelectType(v->GetLogicType());
    ui->Name->setText(v->getName());

    QString h,s;
    QTextStream hs(&h),ss(&s);
    v->setHeaderBeenWritten(false);
    v->setSourceBeenWritten(false);

    v->WriteSourceUserGuts(ss);


}

void AddLogic::HideType()
{
    ui->groupBox->setVisible(false);
}


void AddLogic::SelectType(LOGICTYPE ltv)
{    
    lt = ltv;

    switch(ltv)
    {
    case fSETUP:    
        ui->Name->setText(wp->SuggestName(fSETUP));    
        break;
    case fIN:        // input    
        ui->Name->setText(wp->SuggestName(fIN));        
        break;
    case fOUT: // output
        ui->Name->setText(wp->SuggestName(fOUT));        
        break;
    case fFUZZY: // fuzzyvalue
        ui->Name->setText(wp->SuggestName(fFUZZY));
        break;
    case fAND: // fuzzyand
        ui->Name->setText(wp->SuggestName(fAND));
        break;
    case fOR: // fuzzy or
        ui->Name->setText(wp->SuggestName(fOR));
        break;
    case fNOT: // fuzzy not
        ui->Name->setText(wp->SuggestName(fNOT));
        break;
    case fPID: // pid
        ui->Name->setText(wp->SuggestName(fPID));
        break;
    case fTIMER: // timer
        ui->Name->setText(wp->SuggestName(fTIMER));
    break;
    }
}

void AddLogic::on_buttonBox_accepted()
{

}

void AddLogic::on_In_clicked()
{
    SelectType(fIN);
}

void AddLogic::on_Out_clicked()
{
    SelectType(fOUT);
}

void AddLogic::on_And_clicked()
{
    SelectType(fAND);
}

void AddLogic::on_Or_clicked()
{
    SelectType(fOR);
}

void AddLogic::on_Not_clicked()
{
    SelectType(fNOT);
}

void AddLogic::on_Fuzzy_clicked()
{
    SelectType(fFUZZY);
}

void AddLogic::on_Pid_clicked()
{
    SelectType(fPID);
}

void AddLogic::on_Timer_clicked()
{
    SelectType(fTIMER);
}

void AddLogic::on_Setup_clicked()
{
    SelectType(fSETUP);
}

