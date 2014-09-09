#include <QMessageBox>
#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "viewsourcedialog.h"
#include "addlogic.h"
#include "groups.h"
#include "nodefactory.h"
#include "fuzzynode.h"
#include "TFuzzy.h"
#include "Util.h"
FuzzyNode *fnp =0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    wp = this;
    gti = new GroupTableItem();
    gti->AddGroupItem(0,"All");
    ui->setupUi(this);
    Active = 0;
    SelectNode(0);
    Filename = "";
    createActions();
    createToolBars();
}

void MainWindow::FillCodeUI(Node *np)
{
    {
    QString s;
    QTextStream ts(&s);
    np->setSourceBeenWritten(false);
        np->WriteSourcePlainGuts(ts);
    ui->FunctionBody->setPlainText(s);
    s="";

        ui->FunctionParameters->setText(s);
        s = "";

        ui->FunctionReturn->setText(s);

        ui->FunctionHeader->setText(np->getName());
        s = "";
        //np->WriteSourceGutsReturn(ts);
        ui->FunctionBodyReturn->setText(s);

    }
}

void MainWindow::SelectNode(Node *np)
{
    bool visible = np!=0;
    ui->tabWidget->setVisible(visible);
    ui->LogicLabel->setVisible(visible);
    ui->NodeName->setVisible(visible);
    ui->ValueTable->setVisible(visible);
    ui->MinLabel->setVisible(false);
    ui->Min->setVisible(false);
    ui->MaxLabel->setVisible(false);
    ui->Max->setVisible(false);
    ui->MaxText->setVisible(false);
    ui->pidlabel->setVisible(false);
    ui->pid->setVisible(false);
    ui->SetPoint->setVisible(false);
    ui->ValueTable->setVisible(false);
    ui->Lock->setVisible(true);
    ui->Graph->setVisible(false);
    ui->MinScale->setVisible(false);
    ui->MaxScale->setVisible(false);
    ui->MinText->setVisible(false);
    ui->MaxText->setVisible(false);
    ui->MaxScaleLabel->setVisible(false);
    ui->MinScaleLabel->setVisible(false);

    if (np) {
        Active = np;
        QString ss;
        double v = Active->InValue; // incase invalue get's corrupted while setting limits
        visible = Active->UsesMin();

        ui->MinLabel->setVisible(visible);
        ui->Min->setVisible(visible);
        ui->MinLabel->setVisible(visible);
        ui->Min->setValue(Active->IOMin);
        ui->Min->setMinimum(0);
        on_MinScale_valueChanged(Active->IOMin > 16 ? 512 : 16);
        ui->MinScale->setVisible(Active->UsesMinScale());
        ui->MinScaleLabel->setVisible(Active->UsesMinScale());


        ui->Min->setStatusTip("The minimum expected value");
        ui->MinText->setVisible(visible);
        ss.sprintf("%05.5f",v);
        ui->MinText->setText(ss);
        if (visible) this->on_Min_valueChanged(Active->IOMin);

        visible = Active->UsesMax();
        ui->MaxLabel->setVisible(visible);
        ui->Max->setMinimum(0);
        ui->Max->setValue(Active->IOMax);
        on_MaxScale_valueChanged(Active->IOMax > 16 ? 512 : 16);
        ui->MaxScale->setVisible(Active->UsesMaxScale());
        ui->MaxScaleLabel->setVisible(Active->UsesMaxScale());


        ss.sprintf("%05.5f",Active->IOMax);
        ui->MaxText->setText(ss);
        ui->MaxText->setVisible(visible);
        ui->Max->setStatusTip("The maximum expected value");
        ui->MaxScale->setVisible(visible);
        if (visible) this->on_Max_valueChanged(Active->IOMax);
        switch(np->GetLogicType()) {
        case fSETUP:
            break;
        case fIN:
        case fOUT:
        {
            if (np->GetLogicType()==fIN) {  // needs to be set after min and max are set
                ui->pidlabel->setVisible(true);
                ui->pid->setMinimum(Active->IOMin * Active->SimulateScale());
                ui->pid->setMaximum(Active->IOMax * Active->SimulateScale());

                if (Active->InValue<Active->IOMin) Active->InValue = Active->IOMin;
                if (Active->InValue>Active->IOMax) Active->InValue = Active->IOMax;
                this->on_pid_valueChanged(Active->InValue * Active->SimulateScale());
                ui->pid->setVisible(true);
            }
        }
            break;

        case fAND:
            ui->MaxLabel->setVisible(true);
            ui->Max->setMinimum(0);
            ui->Max->setValue(np->getActiveValue());
            ui->Max->setMaximum(NODEHIGHVAL);
            ui->Max->setVisible(true);
            ui->MaxText->setVisible(visible);
            on_Max_valueChanged(np->getActiveValue());
            ui->Max->setStatusTip("The maximum of all inputs up to this level");
            break;
        case fOR:
            ui->MinLabel->setVisible(true);
            ui->Min->setMinimum(0);
            ui->Min->setMaximum(NODEHIGHVAL);
            ui->Min->setVisible(true);
            ui->Min->setStatusTip("The minimum of all inputs down to this level");
            on_Min_valueChanged(np->getActiveValue());
            break;
        case fNOT:
            break;
        case fFUZZY:
        {
            FuzzyNode *fnp = (FuzzyNode *)np;
            fnp->setHeaderData(0,Qt::Horizontal,"Cause");
            fnp->setHeaderData(1,Qt::Horizontal,"Effect");
            ui->ValueTable->setModel(fnp);
            ui->ValueTable->setVisible(true);
            ui->ValueTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
            ui->ValueTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
            ui->ValueTable->update();
            ui->MinLabel->setText("Cause");
            ui->MinLabel->setVisible(true);
            ui->Min->setMinimum(0);
            ui->Min->setMaximum(NODEHIGHVAL);


//            ui->Min->setValue(np->InValue);
            ui->Min->setVisible(true);
            ui->Min->setStatusTip("");

            //            ui->MinScale->setVisible(true);
            //            ui->MaxScale->setVisible(true);
            //            ui->MinScale->setValue(np->IOMin);
            //            ui->MaxScale->setValue(np->IOMax);

            ui->MaxLabel->setText("Effect");
            ui->MaxLabel->setVisible(true);
            ui->Max->setMinimum(0);
            ui->Max->setMaximum(NODEHIGHVAL);

//            ui->Max->setValue(fnp->fuzzy.Value(np->InValue));

            ui->Max->setVisible(true);
            ui->MaxText->setVisible(true);
            ui->Max->setStatusTip("");
            ui->SetPoint->setVisible(true);
            ui->Graph->setVisible(true);
            ui->ValueTable->setModel(fnp);
            ui->ValueTable->setVisible(true);
            on_Min_valueChanged(np->getActiveValue()*NODEHIGHVAL);

            UpdateTableAndGraph(fnp);
            }
            break;
        case fPID:
        {
            PidNode *pnp = (PidNode *)np;
            ui->MinLabel->setVisible(true);
            ui->MinLabel->setText("P");
            ui->Min->setVisible(true);
            ui->Min->setValue(pnp->get_p());
            on_Min_valueChanged(pnp->get_p());

            ui->MaxLabel->setVisible(true);
            ui->MaxLabel->setText("I");
            ui->Max->setValue(pnp->get_i());
            ui->Max->setVisible(true);
            ui->MaxText->setVisible(true);
            on_Max_valueChanged(pnp->get_i());

            ui->pidlabel->setVisible(true);
            ui->pidlabel->setText("P");
            ui->pid->setValue(pnp->get_d());
            ui->pid->setVisible(true);
            on_pid_valueChanged(pnp->get_d());
        }
            break;
        case fTIMER:
            break;
        }
        ui->NodeName->setText(np->getName());
        FillCodeUI(np);
    }
    else Active = 0;
}

MainWindow::~MainWindow()
{
    delete gti;
    delete ui;
}

void MainWindow::Load(QString filename)
{
    QFile h;

    int ext = filename.lastIndexOf(".");
    if (ext>-1)
        filename.chop(filename.length() - ext);
    h.setFileName(filename + ".h");

    if (h.open(QIODevice::ReadOnly | QIODevice::Text ))    {
        QTextStream hs(&h);
        on_actionNew_triggered();
        ui->graphicsView->ReadSource(hs);
        h.close();
    }
    Filename = filename;
    ui->actionSave->setText("Save " + Filename);
    ui->actionSave->setEnabled(true);
    ui->GroupSelect->clear();
    for(int i=0;i<gti->rowCount();i++) {
        ui->GroupSelect->addItem(gti->GetItem(i)->Name);
    }

}

void MainWindow::on_actionLoad_triggered()
{
    // load as C source
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter ( tr("C Header files(*.h *.H);;All Files(*.*)") );
    QString filename = dialog.getOpenFileName();
    dialog.close();
    if (filename.isEmpty())
        return;


    Load(filename);

}

void MainWindow::on_actionSave_triggered()
{
    //Menu Save
    if (Filename!="") {
         SaveFile(Filename);
    }
}

void MainWindow::WriteSource(QTextStream &h, QTextStream &s)
{
    this->getGroupTableItem()->WriteNodeInfo(s);
    ui->graphicsView->WriteSource(h,s);
}

void MainWindow::SaveFile(QString filename)
{
    QFile h;
    QFile s;

    int ext = filename.lastIndexOf(".");
    if (ext>-1)
        filename.chop(filename.length() - ext);
    h.setFileName(filename + ".h");
    s.setFileName(filename + ".c");
    if (h.exists()) h.remove();
    if (s.exists()) s.remove();

    if (h.open(QIODevice::WriteOnly | QIODevice::Text ))    {
        if (s.open(QIODevice::WriteOnly | QIODevice::Text ))            {
            QTextStream hs(&h),ss(&s);
            WriteSource(hs, ss);
        }
        h.close();
    }
    Filename = filename;
    ui->actionSave->setText("Save " + Filename);
    ui->actionSave->setEnabled(true);
}

void MainWindow::on_actionSave_As_C_Source_triggered()
{
    // save as C source
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter ( tr("C files(*.c *.C);;All Files(*.*)") );
    QString filename = dialog.getSaveFileName();
    dialog.close();
    if (filename.isEmpty())
        return;
    SaveFile(filename);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}




void MainWindow::on_AddLogic_clicked()
{
    // Add Logic
    AddLogic *al = new AddLogic(this);
    int r = al->exec();
    if (r==0) {
        delete al;
        return;
    }
    LOGICTYPE lt = al->GetLogicType();
    QString Name = al->getName();


    Node *node1 = NodeFactory::Create(ui->graphicsView,lt);
    node1->setName(Name);



    //int w = scene->width();
    //int h = scene->height();


    QRect exposedRect(ui->graphicsView->mapToScene(0,0).toPoint(), ui->graphicsView->viewport()->rect().size());
    //QPointF p = node1->pos();

    //QPointF p1 = node1->pos();
    switch(lt) {
    case fSETUP:

        break;
    case fIN:

    case fTIMER:
        break;
    case fOUT:
        break;
    case fFUZZY:
    case fOR:
    case fNOT:
    case fAND:
    case fPID:
        break;
    }
    delete al;
}


void MainWindow::on_ValueTable_clicked(const QModelIndex &/*index*/)
{
    //
}

void MainWindow::on_Min_valueChanged(int value)
{
    if (!Active) return;
    QString s;
    switch (Active->GetLogicType()) {
    case fIN:
    case fOUT:
    case fTIMER:
        Active->OnMinValueChanged(value,s);
        ui->MinText->setText(s);
        ui->MinLabel->setText(FormatLabel("Min",1.0 * ui->Min->minimum(),1.0 * value,1.0 * ui->Min->maximum()));
        ui->pid->setMinimum(value * Active->SimulateScale());
        if (value>=ui->Max->value())
            this->on_Max_valueChanged(value+1);
        break;

    case fAND:
        break;
    case fOR:
        ui->MinLabel->setText(FormatLabel("Minimum Output",ui->Min->minimum() / NODEHIGHVAL,1.0 * value / NODEHIGHVAL,1.0 * ui->Min->maximum()/ NODEHIGHVAL));
        Active->setActiveValue(value);
        break;
    case fFUZZY:
    {
        FuzzyNode *fnp = (FuzzyNode *) Active;
        ui->MinLabel->setText(FormatLabel("Cause",1.0 * ui->Min->minimum() / NODEHIGHVAL,1.0 * value/ NODEHIGHVAL,1.0 * ui->Min->maximum()/ NODEHIGHVAL));
        ui->Max->setValue(NODEHIGHVAL * fnp->fuzzy.Value(value / NODEHIGHVAL));
    }
        break;
    case fPID:
    {
        ((PidNode *)Active)->set_p(value);
        ui->MinLabel->setText(FormatLabel("P",1.0 * ui->Min->minimum()/ NODEHIGHVAL,1.0 * value/ NODEHIGHVAL,1.0 * ui->Min->maximum()/ NODEHIGHVAL));
    }
    default:
        break;

    }
    // update all outputs
    Active->update();

}

void MainWindow::on_Max_valueChanged(int value)
{
    if (!Active) return;
QString s;
    switch (Active->GetLogicType()) {
    case fIN:
    case fOUT:
        if (value<1) value = 1;
        Active->OnMaxValueChanged(value,s);
        ui->MaxText->setText(s);
        ui->MaxLabel->setText(FormatLabel("Max",1.0 * ui->Max->minimum(),1.0 * value,1.0 * ui->Max->maximum()));
        ui->Max->setValue(value);
        if (value<=ui->Min->value())
            this->on_Min_valueChanged(value-1);
        ui->pid->setMaximum(value * Active->SimulateScale());
        this->on_pid_valueChanged(ui->pid->value());
        break;
    case fAND:
        ui->MinLabel->setText(FormatLabel("Maximum Output",1.0 * ui->Min->minimum()/ NODEHIGHVAL,1.0 * value/ NODEHIGHVAL,1.0 * ui->Min->maximum() / NODEHIGHVAL));
        Active->setActiveValue(value/ NODEHIGHVAL);
        break;
    case fOR:
        break;
    case fFUZZY:
        ui->Max->setMaximum(NODEHIGHVAL);
        ui->MaxLabel->setText(FormatLabel("Effect",1.0 * ui->Max->minimum()/ NODEHIGHVAL,1.0 * value/ NODEHIGHVAL,1.0 * ui->Max->maximum()/ NODEHIGHVAL));
        break;
    case fPID:
        ((PidNode *)Active)->set_i(value);
        ui->MaxLabel->setText(FormatLabel("I",1.0 * ui->Max->minimum()/ NODEHIGHVAL,1.0 * value/ NODEHIGHVAL,1.0 * ui->Max->maximum()/ NODEHIGHVAL));
       case fTIMER:
        break;
    default:
        break;
    }
// update all outputs
    //Active->Simulate(Active->Current);
    Active->update();
}

void MainWindow::on_pid_valueChanged(int value)
{
   if (!Active) return;
    double v = value / NODEHIGHVAL;
   switch (Active->GetLogicType()) {
   case fIN:
   {
       ui->pidlabel->setText(FormatLabel("Simulate input Value",ui->pid->minimum() / Active->SimulateScale(),
                                            value / Active->SimulateScale(),ui->pid->maximum() / Active->SimulateScale()));
       Active->InValue = value / Active->SimulateScale();

       ui->graphicsView->simulate();
   }
       break;
   case fOUT:
       break;
   case fAND:
       break;
   case fOR:
       break;
   case fFUZZY:
       ui->MaxLabel->setText(FormatLabel("Effect",ui->Max->minimum(), v,ui->Max->maximum()));
       break;
   case fPID:
       ((PidNode *)Active)->set_d(value);
       ui->pidlabel->setText(FormatLabel("D",ui->pid->minimum()/ NODEHIGHVAL, v/ NODEHIGHVAL,ui->pid->maximum()/ NODEHIGHVAL));
       break;
   case fTIMER:
       break;
   default:
       break;
   }
// update all outputs
//   Active->Simulate(Active->Current);
   Active->update();
}

void MainWindow::UpdateTableAndGraph(FuzzyNode *fActive)
{
    ui->ValueTable->setModel(0);
    ui->ValueTable->setModel(fActive);
    ui->ValueTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->ValueTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->Graph->clearGraphs();
    ui->Graph->addGraph();
    {
        QVector<double> c(int(NODEHIGHVAL)),e(int(NODEHIGHVAL));
        for(int i=0;i<NODEHIGHVAL;i++) {
            c[i] = i/NODEHIGHVAL;
            e[i] = fActive->fuzzy.Value(c[i]);
        }
        ui->Graph->graph(0)->setData(c,e);
        ui->Graph->xAxis->setLabel("Cause");
        ui->Graph->yAxis->setLabel("Effect");
        ui->Graph->xAxis->setRange(0,1);
        ui->Graph->yAxis->setRange(0,1);
        ui->Graph->replot();

    }
    this->on_MaxScale_valueChanged(ui->Max->value());
}

void MainWindow::on_SetPoint_clicked()
{
    if (!Active) return;
    FuzzyNode *fActive = (FuzzyNode *) Active;
    double cause = ui->Min->value()  / NODEHIGHVAL;
    double effect = ui->Max->value() / NODEHIGHVAL;

    fActive->fuzzy.TFuzzyAddPoint(cause,effect);

    UpdateTableAndGraph(fActive);
    fActive->update();
}


QString MainWindow::SuggestName(LOGICTYPE t) const
{
    if (this==0) return "oops";
    return ui->graphicsView->SuggestName(t);
}

void MainWindow::on_Lock_clicked()
{
    this->Frozen = (ui->Lock->checkState() == Qt::Checked) || (ui->Lock->checkState() == Qt::PartiallyChecked);
}



void MainWindow::on_ValueTable_entered(const QModelIndex &/*index*/)
{
    FuzzyNode *fActive = (FuzzyNode *)Active;
    connect(fActive,SIGNAL(editCompleted(const QString &)),this,SLOT(setWindowTitle(QString)));
}

void MainWindow::Simulate()
{
    this->ui->graphicsView->simulate();
}


void MainWindow::on_MinScale_valueChanged(int value)
{
    ui->Min->setMaximum(value * 2);
    ui->MinScale->setValue(value);
    on_Min_valueChanged(ui->Min->value());
    ui->MinScale->update();
}

void MainWindow::on_MaxScale_valueChanged(int value)
{
    ui->Max->setMaximum(value * 2);
    ui->MaxScale->setValue(value);
    this->on_Max_valueChanged(ui->Max->value());
    ui->MaxScale->update();
}

void MainWindow::on_actionView_as_C_Source_triggered()
{
ViewSourceDialog *vs = new ViewSourceDialog(this);
QString h,s;
QTextStream hs(&h),ss(&s);
    WriteSource(hs,ss);
   vs->SetSource(h,s);
   vs->exec();
   delete vs;
}

#include <QDockWidget>
void MainWindow::on_DeleteBlock_clicked()
{
    QGraphicsScene *scene = this->ui->graphicsView->scene();
    foreach (QGraphicsItem *item, scene->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;
        if (node->getSelected())
        {
            node->DeleteAllEdges();
            scene->items().removeOne(item);
            delete node;
        }
    }
//Load("test");

}
bool MainWindow::getFrozen() const
{
    return Frozen;
}

void MainWindow::setFrozen(bool value)
{
    Frozen = value;
}


void MainWindow::on_actionNew_triggered()
{
    QGraphicsScene *scene = this->ui->graphicsView->scene();
    foreach (QGraphicsItem *item, scene->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;
        node->DeleteAllEdges();
    }
    foreach (QGraphicsItem *item, scene->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;
         scene->items().removeOne(item);
         delete node;
    }
}

void MainWindow::on_Group_clicked()
{
    Groups *g = new Groups(this);
    QGraphicsScene *scene = this->ui->graphicsView->scene();
    foreach (QGraphicsItem *item, scene->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;
        {
            g->addNode(node->getName());

        }
    }

    g->exec();
    delete g;
    ui->GroupSelect->clear();
    for(int i=0;i<gti->rowCount();i++) {
        ui->GroupSelect->addItem(gti->GetItem(i)->Name);
    }
}
GroupTableItem *MainWindow::getGroupTableItem() const
{
    return gti;
}

void MainWindow::setGti(GroupTableItem *value)
{
    gti = value;
}


Node *MainWindow::findNode(const QString &Name)
{
    QGraphicsScene *scene = this->ui->graphicsView->scene();
    foreach (QGraphicsItem *item, scene->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;
        if (node->getName()==Name)
               return node;
    }
    return 0;
}


void MainWindow::on_LoadTest_clicked()
{
    ui->Min->setValue(50);
    //Load("test");
}

void MainWindow::on_GroupSelect_currentIndexChanged(const QString &arg1)
{
    QGraphicsScene *scene = this->ui->graphicsView->scene();
   for(int i=0;i<gti->rowCount();i++)   {
       if (gti->GetItem(i)->Name==arg1) {
           int groupid = gti->GetItem(i)->GroupID;
           foreach (QGraphicsItem *item, scene->items()) {
               Node *node = qgraphicsitem_cast<Node *>(item);
               if (!node)
                   continue;
               node->setVisible(node->GroupMember(groupid));

           }
           return;
       }
   }
}

void MainWindow::on_DelLogic_clicked()
{
   if (Active!=0) {
       Active->DeleteAllEdges();
       delete Active;
       Active = 0;
   }
}

void MainWindow::on_EditLogic_clicked()
{
    if (Active) {
        AddLogic *al = new AddLogic(this);
        al->Load(Active);
        al->HideType();
        int r = al->exec();
        if (r==0) {
            delete al;
            return;
        }
        Active->setName(al->getName());
        delete al;
        return;
    }
}

void MainWindow::on_actionView_as_Arduino_Source_triggered()
{ // view as arduino source
    ViewSourceDialog *vs = new ViewSourceDialog(this);
    QString hs;
    QTextStream ss(&hs);
        WriteSource(ss,ss);
       vs->SetSource(hs);
       vs->exec();

}

void MainWindow::on_actionSave_as_Arduiono_Source_triggered()
{ // save as arduino source
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter ( tr("ino files(*.ino *.INO);;All Files(*.*)") );
    QString filename = dialog.getSaveFileName();
    dialog.close();
    if (filename.isEmpty())
        return;
    SaveFile(filename);
}

void MainWindow::on_FunctionHeader_editingFinished()
{
    if (!Active) return;
    Active->setName( ui->FunctionHeader->text());
    Active->update();
}

void MainWindow::on_regenerate_clicked()
{
    if (Active) {
        QString os;
        QTextStream ss(&os);
        Active->WriteSourcePlainGuts(ss);
        ui->FunctionBody->setPlainText(os);
        QString Return,Parameters,FunctionReturn;
        Active->FunctionData(Return,Parameters,FunctionReturn);
        Active->setUserGuts("");
        ui->FunctionReturn->setText(Return);
        ui->FunctionParameters->setText(Parameters);
        ui->FunctionBodyReturn->setText(FunctionReturn);
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (Active==0) return;// No node selected
    if (index==1) {// looking at code
        QString os;
        QTextStream ss(&os);
            Active->WriteSourcePlainGuts(ss);

        QString s;
        s = Active->getUserGuts();
        if (s=="") s = os;
        ui->FunctionBody->setPlainText(s);
        QString Return,Parameters,FunctionReturn;
        ui->FunctionHeader->setText(Active->getName());
        Active->FunctionData(Return,Parameters,FunctionReturn);
        ui->FunctionReturn->setText(Return);
        ui->FunctionParameters->setText(Parameters);
        ui->FunctionBodyReturn->setText(FunctionReturn);
        return;
    }
    if (index==0) {
       QString os;
       QTextStream ss(&os);
       Active->WriteSourcePlainGuts(ss);
       QString s;
       s = ui->FunctionBody->toPlainText();
       ui->NodeName->setText(Active->getName());
       //s = Active->getUserGuts();
       QString regen = Active->Regenerate();
       if (regen==os) // no change from regenerated
           Active->setUserGuts("");
        if (s!=os)
            Active->setUserGuts(s);
    }
}

void MainWindow::on_MinText_editingFinished()
{
    try {
        bool OK;
    int value = ui->MinText->text().toDouble(&OK);
        if (OK)
            on_Min_valueChanged(value);
        else {
            QMessageBox::information(NULL, "Bad Value","Integer Value Expected");
            if (!Active) return;
            QString ss;
            ss.sprintf("%05.5f",Active->IOMin);
            ui->MinText->setText(ss);
        }
    }
    catch(...)
    {

    }
}

void MainWindow::on_MaxText_editingFinished()
{
    try {
        bool OK;
    int value = ui->MaxText->text().toDouble(&OK);
        if (OK)
            on_Max_valueChanged(value);
        else {
            QMessageBox::information(NULL, "Bad Value","Integer Value Expected");
            if (!Active) return;
            QString ss;
            ss.sprintf("%05.5f",Active->IOMax);
            ui->MinText->setText(ss);
        }
    }
    catch(...)
    {

    }
}

void MainWindow::on_NodeName_editingFinished()
{
    if (Active==0) return;
    Active->setName(ui->NodeName->text());
    Active->update();
}

void MainWindow::AddIn()
{
    AddNode(fIN);
}

void MainWindow::AddOut()
{
    AddNode(fOUT);
}

void MainWindow::AddOr()
{
    AddNode(fOR);
}

void MainWindow::AddNot()
{
    AddNode(fNOT);
}

void MainWindow::AddAnd()
{
    AddNode(fAND);
}
void MainWindow::AddFuzzy()
{
    AddNode(fFUZZY);
}
void MainWindow::AddSetup()
{
    AddNode(fSETUP);
}

void MainWindow::AddPID()
{
    AddNode(fPID);
}

void MainWindow::AddTimer()
{
    AddNode(fTIMER);
}

void MainWindow::AddNode(LOGICTYPE lt)
{
    Node *node = NodeFactory::Create(ui->graphicsView,lt);
    node->setName(SuggestName(lt));
    QRect exposedRect(ui->graphicsView->mapToScene(0,0).toPoint(), ui->graphicsView->viewport()->rect().size());
}


void MainWindow::createToolBars()
{
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(inNodeAct);
    editToolBar->addAction(outNodeAct);
    editToolBar->addAction(orNodeAct);
    editToolBar->addAction(andNodeAct);
    editToolBar->addAction(notNodeAct);
    editToolBar->addAction(fuzzyNodeAct);
    editToolBar->addAction(setupNodeAct);
    editToolBar->addAction(pidNodeAct);
    editToolBar->addAction(timerNodeAct);

}

void MainWindow::createActions()
{
    Node *node;
    node = NodeFactory::Create(ui->graphicsView,fIN);
    inNodeAct = new QAction(node->generateIcon(), tr("&In"), this);
    //inNodeAct->setShortcuts(QKeySequence::Undo);
    inNodeAct->setStatusTip(tr("Add an Input Node"));
    connect(inNodeAct, SIGNAL(triggered()), this, SLOT(AddIn()));
    delete node;


    node = NodeFactory::Create(ui->graphicsView,fOUT);
    outNodeAct = new QAction(node->generateIcon(), tr("&Out"), this);
    outNodeAct->setStatusTip(tr("Add an Output Node"));
    connect(outNodeAct, SIGNAL(triggered()), this, SLOT(AddOut()));
    delete node;

    node = NodeFactory::Create(ui->graphicsView,fOR);
    orNodeAct = new QAction(node->generateIcon(), tr("&Or"), this);
    //orNodeAct->setShortcuts(QKeySequence::Undo);
    orNodeAct->setStatusTip(tr("Add an Or Node"));
    connect(orNodeAct, SIGNAL(triggered()), this, SLOT(AddOr()));
    delete node;

    node = NodeFactory::Create(ui->graphicsView,fAND);
    andNodeAct = new QAction(node->generateIcon(), tr("&And"), this);
    //andNodeAct->setShortcuts(QKeySequence::Undo);
    andNodeAct->setStatusTip(tr("Add an And Node"));
    connect(andNodeAct, SIGNAL(triggered()), this, SLOT(AddAnd()));
    delete node;

    node = NodeFactory::Create(ui->graphicsView,fNOT);
    notNodeAct = new QAction(node->generateIcon(), tr("&Not"), this);
    //notNodeAct->setShortcuts(QKeySequence::Undo);
    notNodeAct->setStatusTip(tr("Add a Not Node"));
    connect(notNodeAct, SIGNAL(triggered()), this, SLOT(AddNot()));
    delete node;

    node = NodeFactory::Create(ui->graphicsView,fFUZZY);
    fuzzyNodeAct = new QAction(node->generateIcon(), tr("&Fuzzy"), this);
    fuzzyNodeAct->setStatusTip(tr("Add a Fuzzy Node"));
    connect(fuzzyNodeAct, SIGNAL(triggered()), this, SLOT(AddFuzzy()));
    delete node;

    node = NodeFactory::Create(ui->graphicsView,fSETUP);
    setupNodeAct = new QAction(node->generateIcon(), tr("&Setup"), this);
    setupNodeAct->setStatusTip(tr("Add a setup node"));
    connect(setupNodeAct, SIGNAL(triggered()), this, SLOT(AddSetup()));
    delete node;

    node = NodeFactory::Create(ui->graphicsView,fPID);
    pidNodeAct = new QAction(node->generateIcon(), tr("&PID"), this);
    pidNodeAct->setStatusTip(tr("Add a PID node"));
    connect(pidNodeAct, SIGNAL(triggered()), this, SLOT(AddPID()));
    delete node;

    node = NodeFactory::Create(ui->graphicsView,fTIMER);
    timerNodeAct = new QAction(node->generateIcon(), tr("&Timer"), this);
    timerNodeAct->setStatusTip(tr("Add a timer node"));
    connect(timerNodeAct, SIGNAL(triggered()), this, SLOT(AddTimer()));
    delete node;

}
