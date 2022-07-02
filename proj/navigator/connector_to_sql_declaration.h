#ifndef CONNECTOR_TO_SQL_DECLARATION_H
#define CONNECTOR_TO_SQL_DECLARATION_H
#include <list>
#include <QRect>
#include <QRectF>
#include <QtSql>
#include <mutex>
#include <thread>
#include <QObject>
class SQLConnectorException : public std::exception{
public:
    const char* what() const noexcept override;
};
class SQLGetConnectionException : public SQLConnectorException{
public:
    const char* what() const noexcept override;
};
class SQLRequestException : public SQLConnectorException{
public:
    const char* what() const noexcept override;
};

class SQLConnector : QObject{
    Q_OBJECT
private:
    static bool& IsAdded(){
        static bool isAdded = false;
        return isAdded;
    }
    bool TryToCreateConnection();
public:
    explicit SQLConnector(QObject* parent = nullptr) noexcept(false);
    ~SQLConnector();
    QRect GetRectFromCoord(const QPoint& p);
    SQLConnector(const SQLConnector& _) = delete;
    SQLConnector(const SQLConnector&& _) = delete;
    SQLConnector& operator=(const SQLConnector& _) = delete;
    SQLConnector& operator=(const SQLConnector&& _) = delete;
public slots:
    QPointF GetHousePositionFromDB(const QString& adress); //позиция в глобальных координатах
    std::list<QRectF> GetObstaclesFromDB(const QRectF curPos);//позиция в глобальных координатах
};

#endif // CONNECTOR_TO_SQL_DECLARATION_H
