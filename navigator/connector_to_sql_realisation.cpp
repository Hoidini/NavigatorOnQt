#include "connector_to_sql_declaration.h"
#include <exception>
SQLConnector::SQLConnector(QObject* parent) noexcept(false) : QObject(parent){
    if(!SQLConnector::IsAdded()){
        this->TryToCreateConnection();
        SQLConnector::IsAdded() = true;
    }
}
SQLConnector::~SQLConnector(){
}
bool SQLConnector::TryToCreateConnection(){
    auto db = QSqlDatabase::addDatabase("QMYSQL", "myDB") ;
    db.setHostName("127.0.0.1");
    db.setDatabaseName("navigator");
    db.setUserName("root");
    db.setPassword("12345434");
    return db.open();
}
const char* SQLConnectorException::what() const noexcept{
    return "Troubles with DB\0";
}
const char* SQLGetConnectionException::what() const noexcept{
    return "Failure connection to DB\0";
}
const char* SQLRequestException::what() const noexcept{
    return "Uncorrect request\0";
}
std::list<QRectF> SQLConnector::GetObstaclesFromDB(const QRectF curPos){

    std::list<QRectF> result;
    QSqlQuery que(QSqlDatabase::database("myDB"));
    QString xPos1Str = QString::number(curPos.x() - 0.2);
    QString xPos2Str = QString::number(curPos.x() + curPos.width() + 0.2);
    QString yPos1Str = QString::number(curPos.y() - 0.2);
    QString yPos2Str = QString::number(curPos.y() + curPos.height() + 0.2);
    QString notInersCond = "(coordinate_y + size_y < " + yPos1Str + " OR coordinate_y > " +
            yPos2Str + " OR coordinate_x + size_x < " + xPos1Str + " OR coordinate_x > " + xPos2Str
            + " )";
    QString finalCondition = " NOT " + notInersCond;
    QString request = "SELECT * FROM"
                      " houses WHERE " + finalCondition +";" ;
    if(!que.exec(request)){
        throw SQLRequestException();
    }
    QSqlRecord record = que.record();
    while(que.next()){
        qreal xPos, yPos, szX, szY;
        xPos = que.value(record.indexOf("coordinate_x")).toFloat();
        yPos = que.value(record.indexOf("coordinate_y")).toFloat();
        szX = que.value(record.indexOf("size_x")).toFloat();
        szY = que.value(record.indexOf("size_y")).toFloat();
        result.push_back(QRectF(xPos, yPos, szX, szY));
    }
    return result;
}
QRect SQLConnector::GetRectFromCoord(const QPoint& p){
    QPoint np(p.x() - (p.x() % 2), p.y() - (p.y() % 2));
    QRect result(np, QSize(2, 2));
    return result;
}
QPointF SQLConnector::GetHousePositionFromDB(const QString& adress){
    QPointF result;
    QSqlQuery que(QSqlDatabase::database("myDB"));
    QString homeNumber;
    QString entranceNumber;
    auto parser = [&homeNumber, &entranceNumber](QString adressStr){
        auto iter = std::find(adressStr.begin(), adressStr.end(), '/');
        if(iter == adressStr.end()){
            throw std::invalid_argument("Uncorrect adress");
        }
        std::copy(adressStr.begin(), iter, std::back_inserter(homeNumber));
        iter = std::next(iter,1);
        std::copy(iter, adressStr.end(), std::back_inserter(entranceNumber));

    };
    parser(adress);
    QString requestStr = "SELECT e.coordinate_x, e.coordinate_y "
                         "FROM houses h JOIN entrances e"
                         " ON e.house_id = h.house_id "
                         "WHERE h.home_number = "+ homeNumber + " AND e.entrance_number = " + entranceNumber +";";
    if(!que.exec(requestStr)){
        throw SQLRequestException();
    }
    QSqlRecord record = que.record();
    qreal xPos, yPos;
    if(que.size() == 0){
        throw std::invalid_argument("Uncorrect input data");
    }
    que.next();
    xPos = que.value(record.indexOf("coordinate_x")).toFloat();
    yPos = que.value(record.indexOf("coordinate_y")).toFloat();
    result.setX(xPos);
    result.setY(yPos);
    return result;
}
