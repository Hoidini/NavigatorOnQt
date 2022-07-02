#ifndef TESTSSQLCON_H
#define TESTSSQLCON_H
#include <QRectF>
#include <QObject>
class Test_SQLConnector : public QObject{
    Q_OBJECT
public:
    explicit Test_SQLConnector(QObject* parent = nullptr);
private slots:
    void GetObstaclesFromDB();
};

#endif // TESTSSQLCON_H
