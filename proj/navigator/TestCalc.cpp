#include "TestRoutCalc.h"
#include <QtTest/QTest>
#include "route_Calculator.h"
Test_RouteCalculatorFromDB::Test_RouteCalculatorFromDB(QObject* parent) : QObject(parent){

}
void Test_RouteCalculatorFromDB::FindNeighborhood(){
    RouteCalculatorFromDB calc;
    //auto c1 = calc.FindNeighborhood(QRect(267, 55, 1, 1)); //8 соседей
    QRect r(300, 28, 1, 1);
    auto c2 = calc.FindNeighborhood(QRect(297, 27, 1, 1));//3
    //QCOMPARE(c1.size(), 8);
    QCOMPARE(c2.size(), 3);
}
