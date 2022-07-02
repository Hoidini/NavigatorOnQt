#include "MyPictureBox_declaration.h"
#include <QPaintEngine>
#include<QWheelEvent>
#include <QMessageBox>
void QPictureBox::GlobalPosFilterX() noexcept{
    if(this->globalPos.x() < 0){
        this->globalPos.setX(0);
    }
    else if(this->globalPos.x() > this->scaledImage->size().width() - this->curWidgetWindowSize.width()){
        this->globalPos.setX(this->scaledImage->size().width() - this->curWidgetWindowSize.width());
    }
}
void QPictureBox::GlobalPosFilterY() noexcept{
    if(this->globalPos.y() < 0){
        this->globalPos.setY(0);
    }
    else if(this->globalPos.y() > this->scaledImage->size().height() - this->curWidgetWindowSize.height()){
        this->globalPos.setY(this->scaledImage->size().height() - this->curWidgetWindowSize.height());
    }
}
QPictureBox::QPictureBox(const QSize& size, QWidget* parent) : QWidget(parent){
    this->curWidgetWindowSize = size;
    this->setFixedWidth(size.width());
    this->setFixedHeight(size.height());
    this->curProm = nullptr;
    this->scaleKoef = 1;
    this->lastPos = nullptr;
    this->globalPos = QPoint(0,0);
    this->startTimer(100);
}
template <class T>
bool IsPromiseReady(std::shared_future<T> pr){
    return pr.valid() && pr.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
}
void QPictureBox::SetProm(std::shared_ptr<HandlerSyncPackage> prom){
    this->rectsToDraw.reset();
    this->curProm = prom;
}
QPictureBox::~QPictureBox() = default;
void QPictureBox::paintEvent(QPaintEvent* pEvent){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    if(this->image != nullptr){
        painter.drawImage(0, 0, *(this->scaledImage), this->globalPos.x(), this->globalPos.y());
    }
    if(this->curProm != nullptr && this->curProm->IsReady()){
        auto strEx = this->curProm->GetExStr();
        if(strEx != ""){
            this->curProm = nullptr;
            /*QMessageBox* mb = new QMessageBox(QMessageBox::Information, "WTF", "WTF");
            mb->exec();
            delete mb;*/

        }
        else{
            this->rectsToDraw = this->curProm->GetCont();
            this->curProm = nullptr;
        }
    }
    //43.18, 25.12
    if(this->rectsToDraw.has_value()){
       qreal ySizeGlobalScaleKoef = static_cast<qreal>(this->scaledImage->height()) / 251;
       qreal xSizeGlobalScaleKoef = static_cast<qreal>(this->scaledImage->width()) / 431;
       std::optional<QPoint> prev;
       for(auto rect: this->rectsToDraw.value()){
           int xGlobalPose = rect.center().x() * xSizeGlobalScaleKoef;
           int yGlobalPose = rect.center().y() * ySizeGlobalScaleKoef;
           int xPoseToDraw = xGlobalPose - this->globalPos.x();
           int yPoseToDraw = yGlobalPose - this->globalPos.y();
           //qreal rectWidth = rect.width() * xSizeGlobalScaleKoef;
           //qreal rectHeight = rect.height() * ySizeGlobalScaleKoef;
           QPoint cur(xPoseToDraw, yPoseToDraw);
           if(prev){
               painter.drawLine(prev.value(), cur);
           }
           prev = cur;
       }
    }
}
void QPictureBox::SetImage(QImage&& imageToSet) noexcept{
    this->image = std::make_unique<QImage>(std::move(imageToSet));
    this->scaleKoef = 1;
    this->scaledImage = std::make_unique<QImage>(this->image->scaled(this->image->width() * this->scaleKoef,
                                                                     this->image->height() * this->scaleKoef, Qt::IgnoreAspectRatio));
    this->update();

}
void QPictureBox::ShowMessageBox(QString str1, QString str2){
    QMessageBox::information(0, str1, str2);
}
QSize QPictureBox::sizeHint() const{
    return this->curWidgetWindowSize;
}
void QPictureBox::mousePressEvent(QMouseEvent *pEvent){
    this->lastPos = std::make_unique<QPoint>(pEvent->pos());
}
void QPictureBox::mouseMoveEvent(QMouseEvent*mEvent){
    int dx = (mEvent->pos().x() - this->lastPos->x());
    int dy = (mEvent->pos().y() - this->lastPos->y());
    if(this->curWidgetWindowSize.width() < this->scaledImage->width()){
        this->globalPos.setX(this->globalPos.x() - dx);
        this->GlobalPosFilterX();
    }
    if(this->curWidgetWindowSize.height() < this->scaledImage->height()){
        this->globalPos.setY(this->globalPos.y() - dy);
        this->GlobalPosFilterY();
    }
    this->lastPos = std::make_unique<QPoint>(mEvent->pos());
    this->update();
}
void QPictureBox::timerEvent(QTimerEvent* tEvent) {
    this->update();
}
void QPictureBox::wheelEvent(QWheelEvent* wEvent){
    int angle = wEvent->angleDelta().y();
    this->scaleKoef+= 0.0005 * angle;
    this->scaleKoef = (this->scaleKoef > 1.3) ? 1.3: ((this->scaleKoef < 0.7) ? 0.7 : this->scaleKoef);
    this->scaledImage = std::make_unique<QImage>(this->image->scaled(this->image->width() * this->scaleKoef,
                                                                     this->image->height() * this->scaleKoef, Qt::KeepAspectRatio));
    this->GlobalPosFilterX();
    this->GlobalPosFilterY();
    this->update();
}
