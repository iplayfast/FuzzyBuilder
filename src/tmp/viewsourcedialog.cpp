#include "viewsourcedialog.h"
#include "ui_viewsourcedialog.h"

ViewSourceDialog::ViewSourceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewSourceDialog)
{
    ui->setupUi(this);
}

ViewSourceDialog::~ViewSourceDialog()
{
    delete ui;
}

void ViewSourceDialog::SetSource(QString h, QString s)
{
    ui->Header->setText(h);
    ui->Source->setText(s);
}
