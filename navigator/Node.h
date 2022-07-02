#ifndef NODE_H
#define NODE_H
#include <QPoint>
#include <QSize>
namespace d_Node{
class Node{
private:
QPoint pos;
QSize size;
public:
Node(QPoint pos, QSize size);
Node();
QPoint getPos() const noexcept;
QSize getSize() const noexcept;

};
}
#endif // NODE_H
