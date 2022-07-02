#ifndef D_ALGORITHM_H_H
#define D_ALGORITHM_H_H
#include <iostream>
#include <QRect>
#include <optional>
#include <list>
#include <map>
#include<queue>
#include <set>
#include <functional>
namespace d_algorithm{
struct myFloat{
    std::optional<float> data;
    myFloat();
    myFloat(const std::optional<float>& data);
    myFloat(const float& data);
    myFloat(std::optional<float>&& data);
    myFloat(float&& data);
    myFloat& operator=(const std::optional<float>& data);
    myFloat& operator=(const float& data);
    myFloat& operator=(std::optional<float>&& data);
    myFloat& operator=(float&& data);
    operator bool() const;
    float value() const;
    bool has_value() const;
    bool operator==(const myFloat& k2) const noexcept;
    bool operator <(const myFloat& k2) const noexcept;
    bool operator >(const myFloat& k2) const noexcept;
    bool operator <=(const myFloat& k2) const noexcept;
    bool operator >=(const myFloat& k2) const noexcept;
    bool operator !=(const myFloat& k2) const noexcept;
};

class myKey{
private:
    std::pair<myFloat, myFloat> key;
public:
    myKey();
    myKey(const std::pair<myFloat, myFloat>& data);
    myKey(const myKey& key);
    myKey(myKey&& key);
    myKey& operator=(const myKey& key);
    myKey& operator=(myKey&& key);
    bool operator==(const myKey& k2) const noexcept;
    bool operator <(const myKey& k2) const noexcept;
    bool operator >(const myKey& k2) const noexcept;
    bool operator <=(const myKey& k2) const noexcept;
    bool operator >=(const myKey& k2) const noexcept;
    bool operator !=(const myKey& k2) const noexcept;
    std::pair<myFloat, myFloat> getKey() const noexcept;
    void setKey(const std::pair<myFloat, myFloat>& data);
};
//переписать так, чтобы можно было однозначно хранить ноды
//скорее всего, нужно хранить в целочисленном виде, а при считывании с базы данных осуществлять
//преобразования
class myNode{
private:
    QRect nodeInfo;
public:
    myNode();
    myNode(QRect rect);
    myNode(const myNode& node);
    myNode& operator=(const myNode& node);
    myNode(myNode&& node);
    myNode& operator=(myNode&& node);
    QRect getRect() const noexcept;
    void setRect(const QRect& rect);
    bool operator==(const myNode& k2) const noexcept;
    bool operator <(const myNode& k2) const noexcept;
    bool operator >(const myNode& k2) const noexcept;
    bool operator <=(const myNode& k2) const noexcept;
    bool operator >=(const myNode& k2) const noexcept;
    bool operator !=(const myNode& k2) const noexcept;
};
class QKey{
private:
    myKey key;
    myNode node;
public:
    QKey();
    ~QKey();
    QKey(const QKey& data);
    QKey(QKey&& data);
    QKey& operator=(const QKey& data);
    QKey& operator=(QKey&& data);
    QKey(const std::pair<myKey, myNode>& data);
    QKey(const std::pair<myKey, myNode>&& data);
    QKey& operator=(const std::pair<myKey, myNode>& data);
    QKey& operator=(const std::pair<myKey, myNode>&& data);
    bool operator<(const QKey& p2) const;
    bool operator==(const QKey& p2) const;
    bool operator!=(const QKey& p2) const;
    bool operator>(const QKey& p2) const;
    bool operator>=(const QKey& p2) const;
    bool operator<=(const QKey& p2) const;
    operator std::pair<myKey, myNode>() const;
    myKey GetKey() const;
    myNode GetNode() const;
};

class NavigatorTask;
struct NodeBuilder{
    static NavigatorTask GetNodeFromDBEntIndex(QString index1, QString index2);
};
struct NodesComparatorViaCoord{
    bool operator()(const myNode& a, const myNode& b) const;
};
struct QKeyCompare{
    bool operator()(const QKey& p1, const QKey& p2) const{
        return p1 < p2;
    }
};


class NavigatorTask{
public:
    struct DataCont{
        std::map<myNode, std::pair<myFloat, myFloat> , NodesComparatorViaCoord> myNodesContainer;
        std::set<QKey, QKeyCompare> keysHeapForQueue;
        void SetPotentialMinPath(myNode& cur, const myFloat data);
        void SetCurMinPath(myNode& cur, const myFloat data);
        myFloat CurMinPath(myNode cur) const;
        myKey GetKey(myNode cur) const;
        void SetKey(myNode cur, myKey key);
        myFloat PotentialMinPath(myNode cur) const;
    };
private:
    bool isGraphChanged;
    myNode start;
    myNode curNode;
    std::shared_ptr<DataCont> dataCont;
    //первый параметр множества хранит минимальный ключ, второй - потенциальный минимальный ключ
    //компаратор на основе координат квадратов
    //сортировка на основе пути и координат квадратов
    myFloat Km;
public:
    bool GetIsGraphChanged() const;
    void SetGraphChanged(bool state);
    NavigatorTask();
    ~NavigatorTask();
    NavigatorTask(const NavigatorTask& task);
    NavigatorTask(const NavigatorTask&& task);
    NavigatorTask& operator=(const NavigatorTask& task);
    NavigatorTask& operator=(const NavigatorTask&& task);
    NavigatorTask(myNode start, myNode end);
    NavigatorTask(QRect start, QRect end);
    myNode GetCur() const;
    myNode GetStart() const;
    void SetCur(myNode node);
    myKey CalcKey(const myNode& s); //рассчитывает ключ
    void UpdateNode(myNode cur);
    void ComputeShortestPath();
    std::list<myNode> neighboringNodes(const myNode& node);
    std::list<myNode> neighboringLifeNodes(const myNode& node);
    myFloat EvaluationFunc(myNode cur) const;
    myFloat CurMinPath(myNode cur) const;
    myKey GetKey(myNode cur) const;
    void SetKey(myNode cur, myKey key);
    myFloat PotentialMinPath(myNode cur) const;
    std::pair<myNode, myFloat> getNeighNodeWithMinPotPath(const myNode& cur);
    void SetPotentialMinPath(myNode& cur, const myFloat data);
    void SetCurMinPath(myNode& cur, const myFloat data);
};
}
#endif // D_ALGORITHM_H_H
