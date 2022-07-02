#include "HelpFuncs.h"
bool IsIntersectViaY(const QRectF& r1, const QRectF& r2){
    return (r1.y() >= r2.y() && r1.y() <= r2.y() + r2.height())
            || (r1.y() + r1.height() >= r2.y() && r1.y() + r1.height() <= r2.y() + r2.height()) ||
            (r2.y() >= r1.y() && r2.y() <= r1.y() + r1.height())
            || (r2.y()+ r2.height() >= r1.y() && r2.y()+ r2.height() <= r1.y() + r1.height());
}
bool IsIntersectViaX(const QRectF& r1, const QRectF& r2){
    return (r1.x() >= r2.x() && r1.x() <= r2.x() + r2.width())
            || (r1.x() + r1.width() >= r2.x() && r1.x() + r1.width() <= r2.x() + r2.width()) ||
            (r2.x() >= r1.x() && r2.x() <= r1.x() + r1.width())
            || (r2.x()+ r2.width() >= r1.x() && r2.x()+ r2.width() <= r1.x() + r1.width());
}
HandlerSyncPackage::HandlerSyncPackage(){
    this->SetIsReady(false);
}
void HandlerSyncPackage::SetExStr(std::string exStr){
    std::lock_guard lc(this->m);
    this->ex = exStr;
}
HandlerSyncPackage::~HandlerSyncPackage() = default;
std::string HandlerSyncPackage::GetExStr() const{
    std::lock_guard lc(this->m);
    return this->ex;
}
bool HandlerSyncPackage::IsReady() const noexcept{
    return this->isReady.load(std::memory_order_acquire);
}
void HandlerSyncPackage::SetIsReady(bool state){
    this->isReady.store(state, std::memory_order_release);
}
RectsPathCont HandlerSyncPackage::GetCont() const noexcept{
    std::lock_guard lc(this->m);
    return this->cont;
}
void HandlerSyncPackage::SetRectsPathCont(RectsPathCont cont){
    std::lock_guard lc(this->m);
    this->cont = cont;
}
