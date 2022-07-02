#include "route_Calculator.h"
#include <cmath>
#include <vector>
#include <optional>
#include "HelpFuncs.h"
float RouteCalculatorFromDB::GetRouteLen(QPoint p1, QPoint p2){
    float dx = p2.x() - p1.x();
    float dy = p2.y() - p1.y();
    return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}
std::vector<QRect> RouteCalculatorFromDB::FindNeighborhood(QRect rect){
    QRectF rectSendToDb;
    //преобразуем из формата обработки в формат хранения
    {
        qreal x = static_cast<qreal>(rect.x()) / 10;
        qreal y = static_cast<qreal>(rect.y()) / 10;
        qreal szX = static_cast<qreal>(rect.width()) / 10;
        qreal szY = static_cast<qreal>(rect.height()) / 10;
        rectSendToDb = QRectF(x, y, szX, szY);
    }
    auto obstacles = SQLConnector{}.GetObstaclesFromDB(rectSendToDb);//делаем запрос
    std::vector<QRect> transofmedObstacles;
    //преоразуем обратно
    std::transform(obstacles.begin(), obstacles.end(), std::back_inserter(transofmedObstacles), [](QRectF rectFromDb){
        auto x = rectFromDb.x();
        auto y = rectFromDb.y();
        auto szX = rectFromDb.width();
        auto szY = rectFromDb.height();
        int iX = static_cast<int>(x * 10);
        int iY = static_cast<int>(y * 10);
        int iSzX = static_cast<int>(szX * 10);
        int iSzY = static_cast<int>(szY * 10);
        return QRect(iX, iY, iSzX, iSzY);
    });
    std::vector<QRect> result;
    std::vector<std::optional<QRect>> help(8);//слева, справа, сверху, снизу, nw, ne, se, sw
    //стартовый инциализатор значений
    auto initHelp = [&help, &rect](){
        bool isUncorX = (rect.x() - 2 < 0);
        bool isUncorY = (rect.x() - 2 < 0);
        help[0] = isUncorX ? help[0] : QRect(rect.x() - 2, rect.y(), 2, 2);
        help[1] = QRect(rect.x() + 2, rect.y(), 2, 2);
        help[2] = isUncorY ? help[2] : QRect(rect.x(), rect.y() - 2, 2, 2);
        help[3] = QRect(rect.x(), rect.y()+ 2, 2, 2);
        help[4] = isUncorX || isUncorY ? help[4] : QRect(rect.x() - 2, rect.y() - 2, 2, 2);
        help[5] = isUncorY ? help[5] : QRect(rect.x() + 2, rect.y() - 2, 2, 2);
        help[6] = QRect(rect.x() + 2, rect.y() + 2, 2, 2);
        help[7] = isUncorY ? help[7] : QRect(rect.x() - 2, rect.y() + 2, 2, 2);
    };
    initHelp();
    //проверяем заранее инициализированные сектора на пересечение с препятствием
    for(auto h: help){
        if(h.has_value()){
            bool isInters = false;
            for(auto obs: transofmedObstacles){
                if(obs.intersects(h.value())){
                    isInters = true;
                    break;
                }
            }
            if(!isInters){
                result.push_back(h.value());
            }
        }
    }
    return result;
}
