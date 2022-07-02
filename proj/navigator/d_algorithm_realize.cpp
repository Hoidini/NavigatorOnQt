#include "d_algorithm_h.h"
#include "route_Calculator.h"
#include "connector_to_sql_declaration.h"
#include "HelpFuncs.h"
using rNode = d_algorithm::myNode;
using rKey = d_algorithm::myKey;
using rFloat = d_algorithm::myFloat;
d_algorithm::QKey::QKey() = default;
d_algorithm::QKey::~QKey() = default;
d_algorithm::QKey::QKey(const QKey& data){
    this->key = data.key;
    this->node = data.node;
}
d_algorithm::QKey::QKey(QKey&& data){
    this->key = data.key;
    this->node = data.node;
}
d_algorithm::QKey& d_algorithm::QKey::operator=(const QKey& data) {
    this->key = data.key;
    this->node = data.node;
    return *this;
}
d_algorithm::QKey& d_algorithm::QKey::operator=(QKey&& data){
    this->key = data.key;
    this->node = data.node;
    return *this;
}
d_algorithm::QKey::QKey(const std::pair<myKey, myNode>& data){
    this->key = data.first;
    this->node = data.second;
}
d_algorithm::QKey::QKey(const std::pair<myKey, myNode>&& data){
    this->key = data.first;
    this->node = data.second;
}
d_algorithm::QKey& d_algorithm::QKey::operator=(const std::pair<myKey, myNode>& data){
    this->key = data.first;
    this->node = data.second;
    return *this;
}
d_algorithm::QKey& d_algorithm::QKey::operator=(const std::pair<myKey, myNode>&& data){
    this->key = data.first;
    this->node = data.second;
    return *this;
}

bool d_algorithm::QKey::operator<(const QKey& p2) const{
    if(this->key == p2.key){
        return this->node < p2.node;
    }
    else{
        return this->key < p2.key;
    }
}

bool d_algorithm::QKey::operator==(const QKey& p2) const{
    return this->key == p2.key && this->node == p2.node;
}
bool d_algorithm::QKey::operator!=(const QKey& p2) const{
    return !operator==(p2);
}
bool d_algorithm::QKey::operator>(const QKey& p2) const{
    return operator!=(p2) && !operator<(p2);
}
bool d_algorithm::QKey::operator>=(const QKey& p2) const{
    return operator==(p2) || operator>(p2);
}
d_algorithm::QKey::operator std::pair<myKey, myNode>() const{
    return std::make_pair(this->key, this->node);
}
bool d_algorithm::QKey::operator<=(const QKey& p2) const{
    return operator==(p2) || operator<(p2);
}
rKey d_algorithm::QKey::GetKey() const{
    return this->key;
}
rNode d_algorithm::QKey::GetNode() const{
    return this->node;
}
void d_algorithm::NavigatorTask::DataCont::SetPotentialMinPath(myNode& cur, const myFloat data){
    auto& cont = this->myNodesContainer;
    std::pair<rFloat, rFloat> dataToPush;
    if(cont.find(cur) == cont.end()){
        dataToPush = std::make_pair(rFloat(), data);
    }
    else{
        dataToPush = cont.at(cur);
        dataToPush.second = data;
    }
    cont[cur] = dataToPush;
}
void d_algorithm::NavigatorTask::DataCont::SetCurMinPath(myNode& cur, const myFloat data){
    auto& cont = this->myNodesContainer;
    std::pair<rFloat, rFloat> dataToPush;
    if(cont.find(cur) == cont.end()){
        dataToPush = std::make_pair(data, rFloat{});
    }
    else{
        dataToPush = cont.at(cur);
        dataToPush.first = data;
    }
    cont[cur] = dataToPush;
}
rFloat d_algorithm::NavigatorTask::DataCont::CurMinPath(myNode cur) const{
    auto& cont = this->myNodesContainer;
    rFloat result;
    if(cont.find(cur) != cont.end()){ //если есть, возвращаем значение, если его не будет
        //вернется бесконечность
        result = cont.at(cur).first;
    }
    return result;
}
rKey d_algorithm::NavigatorTask::DataCont::GetKey(myNode cur) const{
    rKey result;
    std::pair<rFloat, rFloat> data;
    data.first = this->CurMinPath(cur);
    data.second = this->PotentialMinPath(cur);
    result.setKey(data);
    return result;
}
void d_algorithm::NavigatorTask::DataCont::SetKey(myNode cur, myKey key){
    this->SetCurMinPath(cur, key.getKey().first);
    this->SetPotentialMinPath(cur, key.getKey().second);
}
rFloat d_algorithm::NavigatorTask::DataCont::PotentialMinPath(myNode cur) const{
    auto& cont = this->myNodesContainer;
    rFloat result;
    if(cont.find(cur) != cont.end()){
        result = cont.at(cur).second;
    }
    return result;
}
bool rNode::operator==(const myNode& k2) const noexcept{
    return this->getRect().topLeft() == k2.getRect().topLeft();
}
bool rNode::operator <(const myNode& k2) const noexcept{
    return !this->operator==(k2) && !this->operator>(k2);
}
bool rNode::operator >(const myNode& k2) const noexcept{
    if(this->operator==(k2)){
        return false;
    }
    if(this->getRect().x() == k2.getRect().x()){
        return this->getRect().y() > k2.getRect().y();
    }
    else{
        return this->getRect().x() > k2.getRect().x();
    }
}
bool rNode::operator <=(const myNode& k2) const noexcept{
    return this->operator==(k2) || this->operator<(k2);
}
bool rNode::operator >=(const myNode& k2) const noexcept{
    return this->operator==(k2) || this->operator>(k2);
}
bool rNode::operator !=(const myNode& k2) const noexcept{
    return !(this->operator==(k2));
}
bool rFloat::operator==(const myFloat& k2) const noexcept{
    if(this->data.has_value() && k2.data.has_value()){
        return std::abs(this->data.value() - k2.data.value()) < 0.01;
    }
    else if(!this->data.has_value() && !k2.data.has_value()){
        return true;
    }else
    {
        return false;
    }

}
float rFloat::value() const{
    return this->data.value();
}
bool rFloat::has_value() const{
    return this->data.has_value();
}
bool rFloat::operator <(const myFloat& k2) const noexcept{
    return !this->operator>(k2) && !this->operator==(k2);
}
rFloat::operator bool() const{
    return this->data.has_value();
}
bool rFloat::operator >(const myFloat& k2) const noexcept{
    if(this->data == k2.data){//если оба бесконечны или одинаково конечны
        return false;
    }
    else if(this->data.has_value() && k2.data.has_value()){//оба конечны
        return this->data.value() > k2.data.value();
    }
    else if(!this->data.has_value()){//если первое бесконечно
        return true;
    }
    else{//второе бесконечно
        return false;
    }
}
bool rFloat::operator <=(const myFloat& k2) const noexcept{
    return this->operator<(k2) || this->operator==(k2);
}
bool rFloat::operator >=(const myFloat& k2) const noexcept{
    return this->operator>(k2) || this->operator==(k2);
}
bool rFloat::operator !=(const myFloat& k2) const noexcept{
    return !(this->data == k2.data);
}
rFloat::myFloat() = default;
rFloat::myFloat(const std::optional<float>& data){
    this->data = data;
}
rFloat::myFloat(const float& data){
    this->data = data;
}
rFloat::myFloat(std::optional<float>&& data){
    this->data = std::move(data);
}
rFloat::myFloat(float&& data){
    this->data = std::move(data);
}
rFloat& rFloat::operator=(const std::optional<float>& data){
    this->data = data;
    return *this;
}
rFloat& rFloat::operator=(const float& data){
    this->data = data;
    return *this;
}
rFloat& rFloat::operator=(std::optional<float>&& data){
    this->data = std::move(data);
    return *this;
}
rFloat& rFloat::operator=(float&& data){
    this->data = std::move(data);
    return *this;
}
rKey::myKey(const myKey& key){
    this->key = key.key;
}
rKey::myKey(myKey&& key){
    this->key = key.key;
}
rKey& rKey::operator=(const myKey& key){
    this->key = key.key;
    return *this;
}
rKey& rKey::operator=(myKey&& key){
    this->key = std::move(key.key);
    return *this;
}
rKey::myKey() : myKey(std::make_pair<rFloat, rFloat>(rFloat{}, rFloat{})){}
rKey::myKey(const std::pair<myFloat, myFloat>& data){
    this->setKey(data);
}
bool d_algorithm::NodesComparatorViaCoord::operator()(const myNode &a, const myNode &b) const{
    return a < b;
}
std::pair<rFloat, rFloat> rKey::getKey() const noexcept{
    return this->key;
}
bool rKey::operator==(const myKey& k2) const noexcept{
    return this->key.first == k2.key.first && this->key.second == k2.key.second;
}
bool rKey::operator!=(const myKey& k2) const noexcept{
    return !rKey::operator==(k2);
}
bool rKey::operator<(const myKey& k2) const noexcept{
    if(this->operator==(k2)){
        return false;
    }
    auto comp_sec_key =  [this, k2](){
        if(!k2.key.second && !this->key.second){
            return false;
        }
        else if(!k2.key.second){
            return true;
        }
        else if(!this->key.second){
            return false;
        }
        else{
            return k2.key.second > this->key.second;
        }
    };
    if(!k2.key.first && !this->key.first){
        return comp_sec_key();
    }
    else if(!k2.key.first){
        return true;
    }
    else if(!this->key.first){
        return false;
    }
    else{
        if(this->key.first == k2.key.second){
            return comp_sec_key();
        }
        else{
            return k2.key.first > this->key.first;
        }
    }
}
bool rKey::operator>(const myKey& k2) const noexcept{
    return !rKey::operator<(k2) && rKey::operator!=(k2);
}
bool rKey::operator<=(const myKey& k2) const noexcept{
    return rKey::operator<(k2) || rKey::operator==(k2);
}
bool rKey::operator>=(const myKey& k2) const noexcept{
    return rKey::operator>(k2) || rKey::operator==(k2);
}
void rKey::setKey(const std::pair<myFloat, myFloat>& data){
    this->key = data;
}
rNode::myNode() : myNode(QRect()){}
rNode::myNode(QRect rect) {
    this->nodeInfo = rect;
}
rNode::myNode(const rNode& node){
    this->setRect(node.nodeInfo);
}
rNode& rNode::operator=(const rNode& node){
    this->setRect(node.nodeInfo);
    return *this;
}
rNode::myNode(myNode&& node){
    this->setRect(std::move(node.nodeInfo));
}
rNode& rNode::operator=(myNode&& node){
    this->setRect(std::move(node.nodeInfo));
    return *this;
}
QRect rNode::getRect() const noexcept{
    return this->nodeInfo;
}
void rNode::setRect(const QRect& rect){
    this->nodeInfo = rect;
}
bool d_algorithm::NavigatorTask::GetIsGraphChanged() const{
    return this->isGraphChanged;
}
void d_algorithm::NavigatorTask::SetGraphChanged(bool state){
    this->isGraphChanged = state;
}
d_algorithm::NavigatorTask::NavigatorTask() = default;
d_algorithm::NavigatorTask::~NavigatorTask() = default;
d_algorithm::NavigatorTask::NavigatorTask(const NavigatorTask& task){
    this->Km = task.Km;
    this->curNode = task.curNode;
    this->isGraphChanged = task.isGraphChanged;
    this->dataCont = task.dataCont;
    this->start = task.start;
}
d_algorithm::NavigatorTask::NavigatorTask(const NavigatorTask&& task){
    this->Km = task.Km;
    this->curNode = task.curNode;
    this->isGraphChanged = task.isGraphChanged;
    this->dataCont = task.dataCont;
    this->start = task.start;
}
d_algorithm::NavigatorTask& d_algorithm::NavigatorTask::operator=(const NavigatorTask& task){
    this->Km = task.Km;
    this->curNode = task.curNode;
    this->isGraphChanged = task.isGraphChanged;
    this->dataCont = task.dataCont;
    this->start = task.start;
    return *this;
}
d_algorithm::NavigatorTask& d_algorithm::NavigatorTask::operator=(const NavigatorTask&& task){
    this->Km = task.Km;
    this->curNode = task.curNode;
    this->isGraphChanged = task.isGraphChanged;
    this->dataCont = task.dataCont;
    this->start = task.start;
    return *this;
}
d_algorithm::NavigatorTask d_algorithm::NodeBuilder::GetNodeFromDBEntIndex(QString index1, QString index2){
    SQLConnector conn{};
    try{
    auto c1 = conn.GetHousePositionFromDB(index1);
    auto c2 = conn.GetHousePositionFromDB(index2);
    QPoint handP1(static_cast<int>(c1.x() * 10), static_cast<int>(c1.y() * 10));
    QPoint handP2(static_cast<int>(c2.x() * 10), static_cast<int>(c2.y() * 10));
    QRect r1 = conn.GetRectFromCoord(handP1);
    QRect r2 = conn.GetRectFromCoord(handP2);
    return d_algorithm::NavigatorTask(r1, r2);
    } catch (std::invalid_argument _){
        throw _;
    } catch(SQLRequestException _){
        throw _;
    }
}
rKey d_algorithm::NavigatorTask::GetKey(rNode cur) const{
    return this->dataCont->GetKey(cur);
}
d_algorithm::NavigatorTask::NavigatorTask(rNode start, rNode end): start(start){
    this->dataCont = std::make_shared<DataCont>();
    this->SetGraphChanged(true);
    this->Km = 0;
    this->curNode = end;
    this->SetPotentialMinPath(this->curNode, rFloat{});
    this->SetPotentialMinPath(this->start, 0);
    this->dataCont->keysHeapForQueue.insert(std::make_pair(this->CalcKey(this->start), this->start));
}
d_algorithm::NavigatorTask::NavigatorTask(QRect start, QRect end) : d_algorithm::NavigatorTask::NavigatorTask(rNode(start), rNode(end)){}
rKey d_algorithm::NavigatorTask::CalcKey(const rNode& cur){
    rKey result;
    auto comp = [](rFloat p1, rFloat p2){
        rFloat result{};
        if(p1 && p2){
            result = std::min(p1.value(), p2.value());
        }
        else if(p1){
            result = p1;
        }
        else if(p2){
            result = p2;
        }
        return result;
    };
    auto p1 = this->EvaluationFunc(cur);
    auto p2 = this->PotentialMinPath(cur);
    auto p3 = this->Km;
    auto p4 = this->CurMinPath(cur);
    auto minP = comp(p4, p2);
    rFloat res1Param = (!p1 ||!minP || !p3) ? rFloat{} : rFloat{p1.value()+ minP.value() + p3.value()};
    result = std::make_pair(res1Param, minP);
    return result;
}
d_algorithm::myNode d_algorithm::NavigatorTask::GetCur() const{
    return this->curNode;
}
void d_algorithm::NavigatorTask::SetCur(rNode node){

    this->curNode = node;
}
std::pair<rNode, rFloat> d_algorithm::NavigatorTask::getNeighNodeWithMinPotPath(const rNode& cur){
    auto calculator = [&cur](QRect s, rFloat& res){
        return [&cur, s, &res](RouteCalculatorFromDB calcP){
            res = calcP.GetRouteLen(cur.getRect().topLeft(), s.topLeft());
        };

    };
    RouteCalculatorBase calc = RouteCalculatorFromDB();
    rNode minRect{};
    rFloat minDist{};
    auto neigh = this->neighboringLifeNodes(cur);
    for(auto val: neigh){
        if(val == cur){
            continue;
        }
        rNode sNode(val);
        rFloat curMP = this->CurMinPath(sNode);
        if(!minDist){
            if(curMP){
                std::visit(calculator(val.getRect(), minDist), calc);
                minDist = minDist.value() + curMP.value();
                minRect = val;}
            else{
                minRect = val;
            }
        }
        else if(curMP){
            rFloat dist;
            std::visit(calculator(val.getRect(), dist), calc);
            dist = dist.value() + curMP.value();
            if(dist < minDist){
                minDist = dist;
                minRect = val;
            }
        }
    }
    return std::make_pair(minRect, minDist);
}
void d_algorithm::NavigatorTask::SetKey(rNode cur, rKey key){
    this->SetCurMinPath(cur, key.getKey().first);
    this->SetPotentialMinPath(cur, key.getKey().second);
}
void d_algorithm::NavigatorTask::SetPotentialMinPath(rNode& cur, const rFloat data){
    this->dataCont->SetPotentialMinPath(cur, data);
}
void d_algorithm::NavigatorTask::SetCurMinPath(rNode& cur, const rFloat data){
    this->dataCont->SetCurMinPath(cur, data);
}
void d_algorithm::NavigatorTask::UpdateNode(rNode curN){
    if(curN != this->start){
        auto minNodeAndPath = this->getNeighNodeWithMinPotPath(curN);
        auto minDist = minNodeAndPath.second;
        //if(minDist < this->PotentialMinPath(curN)){
             this->SetPotentialMinPath(curN, minDist);
        //}
    }
    auto& heap = this->dataCont->keysHeapForQueue;
    {
        while(true){
            auto it = std::find_if(std::begin(heap), std::end(heap), [curN](QKey k){
                    return k.GetNode() == curN;
        });
            if(it != heap.end()){
                heap.erase(it);
            }
            else{
                break;
            }
        }
    }
    if(this->CurMinPath(curN) != this->PotentialMinPath(curN)){
            this->dataCont->keysHeapForQueue.insert(std::make_pair(this->CalcKey(curN), curN));
    }
}
rFloat d_algorithm::NavigatorTask::CurMinPath(rNode cur) const{
    return this->dataCont->CurMinPath(cur);
}
rFloat d_algorithm::NavigatorTask::PotentialMinPath(rNode cur) const{
    return this->dataCont->PotentialMinPath(cur);
}
rFloat d_algorithm::NavigatorTask::EvaluationFunc(rNode cur) const{
     rFloat result;
     RouteCalculatorBase calc = RouteCalculatorFromDB();
     QPoint curP = cur.getRect().topLeft();
     QPoint startP = this->curNode.getRect().topLeft();
     auto calculator = [&curP, &startP, &result](RouteCalculatorFromDB c){
        result = c.GetRouteLen(curP, startP);
     };
     std::visit(calculator, calc);
     return result;
}
rNode d_algorithm::NavigatorTask::GetStart() const{
    return this->start;
}
std::list<rNode> d_algorithm::NavigatorTask::neighboringLifeNodes(const rNode& node){
    std::list<rNode> result;
    RouteCalculatorBase calc = RouteCalculatorFromDB();
    auto searchBuilder = [&node](auto& res){
        return [&node, &res](RouteCalculatorFromDB calcP){
            res = calcP.FindNeighborhood(node.getRect());
        };};
    std::vector<QRect> vect;
    std::visit(searchBuilder(vect), calc);
    for(auto valFromVect: vect){
        rNode findedNode(valFromVect);
        if(this->dataCont->myNodesContainer.find(findedNode) != this->dataCont->myNodesContainer.end()){
            result.push_back(findedNode);
        }
    }
    QRect cR = node.getRect();
    QRect bigSearchSector(QPoint(cR.x() - 2, cR.y() - 2), QPoint(cR.x() + 4, cR.y() + 4));
    if(std::find(result.begin(), result.end(), this->curNode) == result.end() &&
            bigSearchSector.intersects(this->curNode.getRect())){
        result.push_back(this->curNode);
    }
    if(std::find(result.begin(), result.end(), this->start) == result.end() &&
            bigSearchSector.intersects(this->start.getRect())){
        result.push_back(this->start);
    }
    return result;
}
std::list<rNode> d_algorithm::NavigatorTask::neighboringNodes(const rNode& node){ //извлекает значения из базы данных
    std::list<rNode> result;
    RouteCalculatorBase calc = RouteCalculatorFromDB();
    auto searchBuilder = [&node](auto& res){
        return [&node, &res](RouteCalculatorFromDB calcP){
            res = calcP.FindNeighborhood(node.getRect());
        };};
    std::vector<QRect> vect;
    std::visit(searchBuilder(vect), calc);
    for(auto valFromVect: vect){
            rNode findedNode(valFromVect);
            if(this->dataCont->myNodesContainer.find(findedNode) == this->dataCont->myNodesContainer.end()){
                this->dataCont->SetCurMinPath(findedNode, rFloat{});
            }
            result.push_back(findedNode);

    }
    QRect cR = node.getRect();
    QRect bigSearchSector(QPoint(cR.x() - 2, cR.y() - 2), QPoint(cR.x() + 4, cR.y() + 4));
    if(std::find(result.begin(), result.end(), this->curNode) == result.end() &&
            bigSearchSector.intersects(this->curNode.getRect())){
        result.push_back(this->curNode);
    }
    if(std::find(result.begin(), result.end(), this->start) == result.end() &&
            bigSearchSector.intersects(this->start.getRect())){
        result.push_back(this->start);
    }
    return result;
}
void d_algorithm::NavigatorTask::ComputeShortestPath(){//полностью просчитывает веса вершин по пути
    while((!this->dataCont->keysHeapForQueue.empty() &&
          (*this->dataCont->keysHeapForQueue.cbegin()).GetKey() < this->CalcKey(this->curNode)) ||
        this->CurMinPath(this->curNode) != this->PotentialMinPath(this->curNode)){
        auto iter = this->dataCont->keysHeapForQueue.cbegin();
        auto u = *iter; //связка вершина-ключ
        auto k = u.GetKey(); //ключ
        this->dataCont->keysHeapForQueue.erase(iter);
        auto node = u.GetNode();//вершина
        auto calced_key = this->CalcKey(node);
                auto potentialKey = this->PotentialMinPath(node);
        if(k < calced_key){
                this->dataCont->keysHeapForQueue.insert(std::make_pair(calced_key, node));
        }
        else if(this->CurMinPath(node) >= potentialKey){
            this->SetCurMinPath(node, potentialKey);
            for(auto val: this->neighboringNodes(node)){
                this->UpdateNode(rNode(val));
            }
        }
        else{
            this->SetCurMinPath(node, rFloat{});
            this->UpdateNode(node);
            for(auto val: this->neighboringNodes(node)){
                this->UpdateNode(rNode(val));
            }
        }
    }
}
