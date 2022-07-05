#include "navigatorhandler_declaration.h"
#include "connector_to_sql_declaration.h"
#include <QMessageBox>
NavigatorHandler::NavigatorHandler()
{
    this->curPromise = nullptr;
    this->curNavigatorTask = nullptr;
    this->isEnd = false;
    std::thread t(&NavigatorHandler::ThreadProcess, this);
    this->workThread = std::make_unique<std::thread>(std::move(t));
}
NavigatorHandler& NavigatorHandler::GetInstance(){
    static NavigatorHandler handler;
    return handler;
}
void NavigatorHandler::ThreadProcess(){
    while(true){
        std::unique_lock dc(this->taskReadingWritingMutex);
        if(!(this->adresses != nullptr || this->isEnd)){
            this->isTaskAddedOrEdited.wait(dc, [this](){
                return this->adresses != nullptr || this->isEnd;
            });
        }
        if(this->isEnd){
            return;
        }
        dc.unlock();
        try{
            this->addNewTask(d_algorithm::NodeBuilder::GetNodeFromDBEntIndex(this->adresses->first,
                                                                             this->adresses->second));
        } catch(std::invalid_argument I){
            this->adresses = nullptr;
            this->curPromise->SetExStr(I.what());
            this->curPromise->SetIsReady(true);
            continue;
        }
        catch(SQLRequestException sq){
            this->adresses = nullptr;
            this->curPromise->SetExStr(sq.what());
            this->curPromise->SetIsReady(true);
            continue;
        }
        this->adresses = nullptr;
         this->curNavigatorTask->ComputeShortestPath();
        if(!this->curNavigatorTask->CurMinPath(this->curNavigatorTask->GetCur()).has_value()){//если пути нет
            std::unique_lock rLock(this->taskReadingWritingMutex);
            this->curPath = RectsPathCont();
            this->curPromise->SetRectsPathCont(this->curPath);
            this->curPromise->SetIsReady(true);
            this->curPromise = nullptr;//отбрасываем выполненное обещание
            this->curNavigatorTask = nullptr;
            continue;
        }
        std::vector<QRect> preRes;//инициализируем решение
        preRes.push_back(this->curNavigatorTask->GetCur().getRect());
        while(this->curNavigatorTask->GetCur() != this->curNavigatorTask->GetStart()){
            auto mindNodeAndRect = this->curNavigatorTask->getNeighNodeWithMinPotPath(this->curNavigatorTask->GetCur());
            this->curNavigatorTask->SetCur(mindNodeAndRect.first);
            preRes.push_back(this->curNavigatorTask->GetCur().getRect());
            //если будет дописан механизм изменения графа, то дописать алгоритм
        }
                this->curPath = preRes;
        std::unique_lock rLock(this->taskReadingWritingMutex);
        this->curPromise->SetRectsPathCont(this->curPath);
        this->curPromise->SetIsReady(true);
        this->curPromise = nullptr;//отбрасываем выполненное обещание
        rLock.unlock();
        this->curNavigatorTask = nullptr;
    }
}
void NavigatorHandler::addNewTask(QString adress1, QString adress2){
    {
        std::lock_guard lc(this->taskReadingWritingMutex);
        this->adresses = std::make_shared<std::pair<QString, QString>>(std::make_pair(adress1, adress2));
        this->curPromise = std::make_shared<HandlerSyncPackage>();
    }
    this->isTaskAddedOrEdited.notify_all();
}
void NavigatorHandler::addNewTask(d_algorithm::NavigatorTask curTask){//вызывется из потока обработки
    std::lock_guard lc(this->taskReadingWritingMutex);
    try{
        if(this->isEnd){
            return;
        }
        this->curPath = RectsPathCont{};
        this->curNavigatorTask = std::make_unique<d_algorithm::NavigatorTask>(curTask);
    } catch(std::invalid_argument I){
        throw I;
    } catch(SQLRequestException ex){
        throw ex;
    }
}
std::shared_ptr<HandlerSyncPackage> NavigatorHandler::getRectsPath() const noexcept{
    std::lock_guard lc(this->taskReadingWritingMutex);
    return this->curPromise;
}
NavigatorHandler::~NavigatorHandler(){
    {
        std::lock_guard lc(this->taskReadingWritingMutex);
        this->isEnd = true;//уведомляем поток о завершении работы
    }
    this->isTaskAddedOrEdited.notify_one();
    this->workThread->join();//дожидаемся его окончания
}
