#ifndef QTEST_CONNECOTTOSQL_H
#define QTEST_CONNECOTTOSQL_H
#include "connector_to_sql_declaration.h"
class Test_SQLConnector : public QObject{
    Q_OBJECT
public:
    explicit Test_SQLConnector(QObject* parent = nullptr);
private:
    std::list<QRectF> GetObstaclesFromDB(const QRectF curPos);
};

#endif // QTEST_CONNECOTTOSQL_H
