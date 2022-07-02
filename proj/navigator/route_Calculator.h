#ifndef ROUTE_CALCULATOR_H
#define ROUTE_CALCULATOR_H
#include <QPoint>
#include <list>
#include <QRect>
#include <variant>
#include "connector_to_sql_declaration.h"
class RouteCalculatorFromDB{
public:
    float GetRouteLen(QPoint p1, QPoint p2);
    std::vector<QRect> FindNeighborhood(QRect rect);
};
using RouteCalculatorBase = std::variant<RouteCalculatorFromDB>;

#endif // ROUTE_CALCULATOR_H
