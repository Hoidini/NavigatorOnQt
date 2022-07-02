#ifndef NAVIGATORHANDLER_H
#define NAVIGATORHANDLER_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "d_algorithm_h.h"
#include <vector>
#include <chrono>
#include <future>
#include "HelpFuncs.h"
//заменить механизм синхронизации
//на shared_ptr, содержащий в себя атомарный статус готовности, мьютекс(на всякий случай), а также
//сами данные
class NavigatorHandler
{
private:
    std::shared_ptr<std::pair<QString, QString>> adresses;
    std::unique_ptr<std::thread> workThread;
    mutable std::mutex taskHandlingMutex;//вызывается для безопасного изменения пути и работы с задачей
    mutable std::mutex taskReadingWritingMutex;//вызывается для безопасного чтения пути и обещаний
    std::atomic_bool isEnd;
    RectsPathCont curPath; //изменение требует захвата мьютекса
    std::condition_variable isTaskAddedOrEdited;
    std::unique_ptr<d_algorithm::NavigatorTask> curNavigatorTask; //изменение требует захвата мьютекса
    //данный вектор хранит обещания для доступа сторонних потоков к просчитанному маршруту к тому
    //моменту, когда тот будет рассчитан
    std::shared_ptr<HandlerSyncPackage> curPromise; //изменение требует захвата мьютекса
    NavigatorHandler();
    ~NavigatorHandler();
    void ThreadProcess();
    void addNewTask(d_algorithm::NavigatorTask curTask);
public:
    NavigatorHandler(const NavigatorHandler& ) = delete;
    NavigatorHandler& operator= (const NavigatorHandler&) = delete;
    void addNewTask(QString adress1, QString adress2);
    static NavigatorHandler& GetInstance();
    std::shared_ptr<HandlerSyncPackage> getRectsPath() const noexcept;
};

#endif // NAVIGATORHANDLER_H
