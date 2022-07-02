#ifndef HELPFUNCS_H
#define HELPFUNCS_H
#include <vector>
#include <QRectF>
#include <optional>
#include <QRect>
#include <mutex>
#include <atomic>
using RectsPathCont = std::optional<std::vector<QRect>>;
bool IsIntersectViaY(const QRectF& r1, const QRectF& r2);
bool IsIntersectViaX(const QRectF& r1, const QRectF& r2);
class HandlerSyncPackage{
private:
    std::atomic<bool> isReady;
    mutable std::mutex m;
    std::string ex;
    RectsPathCont cont;
public:
    void SetExStr(std::string exStr);
    std::string GetExStr() const;
    HandlerSyncPackage();
    ~HandlerSyncPackage();
    HandlerSyncPackage(const HandlerSyncPackage& an) = delete;
    HandlerSyncPackage(HandlerSyncPackage&& an) = delete;
    HandlerSyncPackage& operator=(const HandlerSyncPackage& an) = delete;
    HandlerSyncPackage& operator=(HandlerSyncPackage&& an) = delete;
    bool IsReady() const noexcept;
    void SetIsReady(bool state);
    RectsPathCont GetCont() const noexcept;
    void SetRectsPathCont(RectsPathCont cont);
};

#endif // HELPFUNCS_H
