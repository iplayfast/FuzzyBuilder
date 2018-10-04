#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "viewsourcedialog.h"
#include "addlogic.h"
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
    ui->setupUi(this);
    Active = 0;
    DisplayNode(0);

}

MainWindow::~MainWindow()
{
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
                ui->graphicsView->WriteSource(hs,ss);
            }
           h.close();
        }

}

void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::setMin(bool Vis,const char *text,double v)
{
    ui->MinLabel->setText(text);
    ui->MinLabel->setVisible(Vis);
    ui->Min->setMinimum(0);
    ui->Min->setMaximum(NODEHIGHVAL);


    ui->Min->setValue(v);
    ui->Min->setVisible(Vis);
    ui->MinScale->setVisible(Vis);
}

void MainWindow::setMax(bool Vis,const char *text,double v)
{
    ui->MaxLabel->setText(text);
    ui->MaxLabel->setVisible(Vis);
    ui->Max->setMinimum(0);
    ui->Max->setMaximum(NODEHIGHVAL);
    ui->Max->setValue(v);
    ui->Max->setVisible(Vis);
    ui->MaxScale->setVisible(Vis);
}

void MainWindow::setPID(bool Vis,const char *text,double v)
{
    ui->pidlabel->setVisible(Vis);
    ui->pidlabel->setText(text);
    ui->pid->setValue(v);
    ui->pid->setVisible(Vis);
}

void MainWindow::DisplayNode(Node *np)
{
    bool visible = np!=0;
    ui->LogicLabel->setVisible(visible);
    ui->NodeName->setVisible(visible);
    ui->UpdateName->setVisible(visible);
    ui->ValueTable->setVisible(visible);
    ui->MinLabel->setVisible(false);
    ui->Min->setVisible(false);
    ui->MaxLabel->setVisible(false);
    ui->Max->setVisible(false);
    ui->pidlabel->setVisible(false);
    ui->pid->setVisible(false);
    ui->SetPoint->setVisible(false);
    ui->ValueTable->setVisible(false);
    ui->Lock->setVisible(true);
    ui->Graph->setVisible(false);
    ui->MinScale->setVisible(false);
    ui->MaxScale->setVisible(false);
    if (np) {
        Active = np;
        switch(np->GetLogicType()) {
        case fIN:
        case fOUT:
        {
            double v = Active->InValue; // incase invalue get's corrupted while setting limits
             this->on_MinScale_valueChanged(np->IOMin > 16 ? 512 : 16);
             setMin(true,"Min",np->IOMin);

            this->on_MaxScale_valueChanged(np->IOMax > 16 ? 512 : 16);
                np->InValue = v;
             setMax(true,"Max",np->IOMax);

             if (np->GetLogicType()==fIN)   {
                ui->pid->setMinimum(np->IOMin * Active->SimulateScale());
                ui->pid->setMaximum(np->IOMax * Active->SimulateScale());
                if (np->InValue<np->IOMin) np->InValue = np->IOMin;
                if (np->InValue>np->IOMax) np->InValue = np->IOMax;
                this->on_pid_valueChanged(np->InValue * Active->SimulateScale());
                setPID(true,"Simulation",np->InValue);
             }
        }
             break;

        case fAND:
              setMax(true,"Max",np->getActiveValue());
              on_Max_valueChanged(np->getActiveValue());
              break;
        case fOR:
                setMin(true,"Min",np->getActiveValue());
                on_Min_valueChanged(np->getActiveValue());
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
            setMin(true,"Cause", fnp->fuzzy.Count()>0 ? fnp->fuzzy.IndexAt(0):0);
//            ui->MinScale->setVisible(true);
//            ui->MaxScale->setVisible(true);
//            ui->MinScale->setValue(np->IOMin);
//            ui->MaxScale->setValue(np->IOMax);

            ui->MaxLabel->setText("Effect");
            ui->MaxLabel->setVisible(true);
            ui->Max->setMinimum(0);
            ui->Max->setMaximum(NODEHIGHVAL);
            ui->Max->setValue(fnp->fuzzy.Count()>0 ? fnp->fuzzy.Value(ui->Min->value()):0);
            ui->Max->setVisible(true);

            ui->SetPoint->setVisible(true);
            ui->Graph->setVisible(true);
            ui->ValueTable->setModel(fnp);
            ui->ValueTable->setVisible(true);
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
        case fMAP:
             ui->MaxLabel->setVisible(true);
             ui->Max->setMinimum(0);
             ui->Max->setValue(np->IOMax);
             this->on_MaxScale_valueChanged(np->IOMax > 16 ? 512 : 16);
             ui->Max->setVisible(true);
             ui->MaxScale->setVisible(true);
            break;
        }
        ui->NodeName->setText(np->getName());
    }
    else Active = 0;
}

void MainWindow::on_AddLogic_clicked()
{
    // Add Logic
    AddLogic *al = new AddLogic(this);
    int r = al->exec();
    if (r==0) return;
    LOGICTYPE lt = al->GetLogicType();
    QString Name = al->GetName();
Node *node1 = NodeFactory::Create(ui->graphicsView,lt);
    node1->setName(Name);
    switch(lt) {
    case fIN:
    case fTIMER:
        node1->IOMax = al->GetMax();
        node1->IOMin = al->GetMin();
        break;
    case fOUT:
        node1->IOMax = al->GetMax();
        node1->IOMin = al->GetMin();
        break;
    case fFUZZY:
    case fOR:
    case fAND:
    case fPID:
    case fMAP:
        break;
    }
    delete al;
}

void MainWindow::on_UpdateName_clicked()
{
    if (!Active) return;
    Active->setName( ui->NodeName->text());
    Active->update();
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
        ui->MinLabel->setText(FormatLabel("Min",1.0 * ui->Min->minimum(),1.0 * value,1.0 * ui->Min->maximum()));
        Active->IOMin  = value;
        ui->pid->setMinimum(value * Active->SimulateScale());
        if (value>=ui->Max->value())
            this->on_Max_valueChanged(value+1);
       // Active->Simulate(Active->InValue);
        // update all outputs
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
        break;

    }
    // update all outputs
    Active->update();

}

void MainWindow::on_Max_valueChanged(int value)
{
    if (!Active) return;

    switch (Active->GetLogicType()) {
    case fIN:
    case fOUT:
        if (value<1) value = 1;
        ui->MaxLabel->setText(FormatLabel("Max",1.0 * ui->Max->minimum(),1.0 * value,1.0 * ui->Max->maximum()));

        Active->IOMax = value;
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
        ui->MaxLabel->setText(FormatLabel("Effect",1.0 * ui->Max->minimum()/ NODEHIGHVAL,1.0 * value/ NODEHIGHVAL,1.0 * ui->Max->maximum()/ NODEHIGHVAL));
        break;
    case fPID:
        ((PidNode *)Active)->set_i(value);
        ui->MaxLabel->setText(FormatLabel("I",1.0 * ui->Max->minimum()/ NODEHIGHVAL,1.0 * value/ NODEHIGHVAL,1.0 * ui->Max->maximum()/ NODEHIGHVAL));
       case fTIMER:
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
   }
// update all outputs
//   Active->Simulate(Active->Current);
   Active->update();
}

void MainWindow::on_SetPoint_clicked()
{
    if (!Active) return;
    FuzzyNode *fActive = (FuzzyNode *) Active;
    double cause = ui->Min->value()  / NODEHIGHVAL;
    double effect = ui->Max->value() / NODEHIGHVAL;

    fActive->fuzzy.TFuzzyAddPoint(cause,effect);

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
    ui->graphicsView->WriteSource(hs,ss);
   vs->SetSource(h,s);
   vs->exec();

}


void MainWindow::on_DeleteBlock_clicked()
{
    QGraphicsScene *scene = this->ui->graphicsView->scene();
    foreach (QGraphicsItem *item, scene->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;
        if (node->getSelected())
        {
            node->ModifyEdges(true);
            scene->items().removeOne(item);
            delete node;
        }
    }
Load("test");
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
        {
            node->ModifyEdges(true);
            scene->items().removeOne(item);
            delete node;
        }
    }
}

void MainWindow::on_NodeName_editingFinished()
{
    if (Active) {
        Active->setName( ui->NodeName->text());
        Active->update();
    }
}

/*void MainWindow::on_NodeName_textChanged(const QString &arg1)
{
    if (Active) {
        Active->setName( ui->NodeName->text());
        Active->update();
   }
}*/

//void MainWindow::on_NodeName_textEdited(const QString &arg1)
//{

//}
void MainWindow::on_NodeName_returnPressed()
{

}

void MainWindow::on_graphicsView_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint)
{

}
