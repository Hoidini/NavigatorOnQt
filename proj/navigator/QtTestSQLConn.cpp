#include "TestsSQLcon.h"
#include <QtTest/QTest>
#include "connector_to_sql_declaration.h"
Test_SQLConnector::Test_SQLConnector(QObject* parent) : QObject(parent){

}
void Test_SQLConnector::GetObstaclesFromDB(){
    SQLConnector conn;
    auto l1 = conn.GetObstaclesFromDB(QRectF(29.9, 2.75, 0.1, 0.1));
    auto l2 = conn.GetObstaclesFromDB(QRectF(31.1, 2.9, 0.1, 0.1));
    auto l3 = conn.GetObstaclesFromDB(QRectF(24.2, 3.5, 0.1, 0.1));
    QCOMPARE(l1.size(), 2);
    QCOMPARE(l2.size(), 1);
    QCOMPARE(l3.size(), 1);
}

