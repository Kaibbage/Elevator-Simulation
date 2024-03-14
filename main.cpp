#include "mainwindow.h"

#include <QApplication>

//Simple class, just creates a QApplication and a MainWindow and runs
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
