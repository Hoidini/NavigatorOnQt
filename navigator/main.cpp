#include "mainwindow.h"
#include <QtTest/QTest>
#include <QApplication>
#include "TestRoutCalc.h"
#include "QTest_ConnecotToSQl.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
