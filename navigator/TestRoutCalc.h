#ifndef TESTROUTCALC_H
#define TESTROUTCALC_H
#include <QRect>
#include <QObject>
class Test_RouteCalculatorFromDB: public QObject{
    Q_OBJECT
public:
    explicit Test_RouteCalculatorFromDB(QObject* parent = nullptr);
private slots:
    void FindNeighborhood();
};
#endif // TESTROUTCALC_H
