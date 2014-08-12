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
    on_dial_sliderMoved(ui->dial->value());
    SelectType(fIN);
    ui->In->setChecked(true);
}

AddLogic::~AddLogic()
{
    delete ui;
}

QString AddLogic::GetName() const
{
    return ui->lineEdit->text();
}

LOGICTYPE AddLogic::GetLogicType() const
{
    return lt;
}
void AddLogic::on_lineEdit_editingFinished()
{
    //this->on_comboBox_currentIndexChanged(ui->comboBox->currentText());
}
QString AddLogic::GetInputGuts() const
{
QString Result;
        Result.sprintf(
"int Max = %d,Min = %d;\n/*add your code here to obtain and return the input*/\n\
double myinput = ; \
\n\
/* Normalize value to range 0<=Number<=1 */\
  myinput = (myinput - Min) / (Max - Min);\n\
  return myinput;\n",
            ui->Max->value(),ui->Min->value());
    ui->fguts->setPlainText(Result);
    return Result;
}
int AddLogic::GetMax() const
{
    return ui->Max->value();
}
int AddLogic::GetMin() const
{
    return ui->Min->value();
}

QString AddLogic::GetOutputGuts() const
{
    QString Result;

            Result.sprintf(
    "   int Max = %d,Min = %d;\n\n/*convert from range [0..1] to real world values */\n    result = ((Max-Min) * result)+Min; \n\
        /* add code here to send result to where you want it*/\n#error add your code\n",
                ui->Max->value(),ui->Min->value());
        ui->fguts->setPlainText(Result);
        return Result;

}

void AddLogic::on_Min_sliderMoved(int position)
{
    if (position>ui->Max->value())  {
        ui->Max->setValue(position+1);
        on_Max_sliderMoved(position+1);
    }
    ui->Min->setValue(position);
    QString s = FormatLabel("Expected Real World Minimum",0,position,ui->Min->maximum());
    ui->MinLabel->setText(s);
    if (ui->In->isChecked())
        GetInputGuts();
    else
        GetOutputGuts();
}

void AddLogic::on_Max_sliderMoved(int position)
{
    if (position<ui->Min->value()){
        ui->Min->setValue(position-1);
        on_Min_sliderMoved(position-1);
    }
    ui->Max->setValue(position);
    QString s = FormatLabel("Expected Real World Maximum",0,position,ui->Max->maximum());
    ui->MaxLabel->setText(s);
    if (ui->In->isChecked())
        GetInputGuts();
    else
        GetOutputGuts();
}

void AddLogic::on_dial_sliderMoved(int position)
{
QString ss;
    ss.sprintf("%d",position);
    ui->ScaleLabel->setText(ss);
    ui->Min->setMaximum(position);
    ui->Max->setMaximum(position);
    on_Min_sliderMoved(ui->Min->value());
    on_Max_sliderMoved(ui->Max->value());
}

void AddLogic::on_Max_valueChanged(int value)
{
    on_Max_sliderMoved(value);
}

void AddLogic::on_Min_valueChanged(int value)
{
    on_Min_sliderMoved(value);
}

void AddLogic::on_MinTextEdit_editingFinished()
{
QString s = ui->MinTextEdit->text();
double v = s.toDouble();
    s.sprintf("%05.5f",v);
    ui->MinTextEdit->setText(s);
    if (ui->Min->maximum()<v)
        ui->Min->setMaximum(v);
    if (ui->Min->minimum()>v)
        ui->Min->setMinimum(v);
    ui->Min->setValue(v);
}

void AddLogic::on_MaxTextEdit_editingFinished()
{
    QString s = ui->MaxTextEdit->text();
    double v = s.toDouble();
        s.sprintf("%05.5f",v);
        ui->MaxTextEdit->setText(s);
        if (ui->Max->maximum()<v)
            ui->Max->setMaximum(v);
        if (ui->Max->minimum()>v)
            ui->Max->setMinimum(v);
        ui->Max->setValue(v);
}

void AddLogic::SelectType(LOGICTYPE ltv)
{
    bool io = false;
    lt = ltv;
    switch(ltv)
    {
    case fIN:        // input
    {

        ui->lineEdit->setText(wp->SuggestName(fIN));
        on_Min_sliderMoved(ui->Min->value());
        on_Max_sliderMoved(ui->Max->value());
        ui->returnval->setText("double ");
        ui->declaration->setText(ui->lineEdit->text());
        ui->parameter->setText("(void)");
        GetInputGuts();
        ui->fguts->setPlainText(GetInputGuts());        
        io = true;
    }
        break;
    case fOUT: // output
    {
        ui->returnval->setText("void ");
        on_Min_sliderMoved(ui->Min->value());
        on_Max_sliderMoved(ui->Max->value());
        ui->lineEdit->setText(wp->SuggestName(fOUT));
        ui->declaration->setText(ui->lineEdit->text());
        ui->parameter->setText("(double result)");
        ui->fguts->setPlainText(GetOutputGuts());        
        on_dial_sliderMoved(ui->dial->value());
        io = true;
    }
        break;
    case fFUZZY: // fuzzyvalue
       {
        ui->returnval->setText("double ");
        ui->lineEdit->setText(wp->SuggestName(fFUZZY));

        ui->declaration->setText("inline code");
        ui->parameter->setText("(double input)");
        ui->fguts->setPlainText("return " + ui->lineEdit->text()+"(input);\n");        
    }
        break;
    case fAND: // fuzzyand
    {
        ui->returnval->setText("double ");
        ui->lineEdit->setText(wp->SuggestName(fAND));

        ui->declaration->setText(ui->lineEdit->text());
        ui->parameter->setText("(void)");
        ui->fguts->setPlainText("/* will return the least of any input attached to it */");        
    }
        break;
    case fOR: // fuzzy or
    {
        ui->returnval->setText("double ");
        ui->lineEdit->setText(wp->SuggestName(fOR));
        ui->declaration->setText(ui->lineEdit->text());
        ui->parameter->setText("(void)");
        ui->fguts->setPlainText("/* will return the most of any input attached to it */");        
    }
        break;
    case fNOT: // fuzzy not
    {
        ui->returnval->setText("double ");
        ui->lineEdit->setText(wp->SuggestName(fNOT));
        ui->declaration->setText(ui->lineEdit->text());
        ui->parameter->setText("(void)");
        ui->fguts->setPlainText("/* will return the 1 - n of (inverse of 0 to 1 range) */");                
    }
        break;
    case fPID: // pid
    {
        ui->returnval->setText("double ");
        ui->lineEdit->setText(wp->SuggestName(fPID));
        ui->declaration->setText(ui->lineEdit->text());        
    }
        break;
    case fTIMER: // timer
    {
        ui->returnval->setText("double ");
        ui->lineEdit->setText(wp->SuggestName(fTIMER));
        ui->declaration->setText(ui->lineEdit->text());        
    }
    break;
    }
    ui->fguts->setReadOnly(true);
    ui->dial->setVisible(io);
    ui->ScaleLabel->setVisible(io);
    ui->MinLabel->setVisible(io);
    ui->Min->setVisible(io);
    ui->MaxLabel->setVisible(io);
    ui->Max->setVisible(io);
    ui->MinTextEdit->setVisible(io);
    ui->MaxTextEdit->setVisible(io);
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
