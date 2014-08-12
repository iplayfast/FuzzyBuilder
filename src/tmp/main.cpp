#include "mainwindow.h"
#include <QApplication>
MainWindow *wp;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    wp = &w;
    w.show();

    return a.exec();
}
