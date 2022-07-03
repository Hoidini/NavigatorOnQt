#ifndef MYPICTUREBOX_DECLARATION_H
#define MYPICTUREBOX_DECLARATION_H
#include <QLabel>
#include <QImage>
#include <QString>
#include <QRect>
#include <vector>
#include <string>
#include "navigatorhandler_declaration.h"
class QPictureBox : public QWidget{
    Q_OBJECT
private:
    std::unique_ptr<QImage> image;
    std::unique_ptr<QImage> scaledImage;
    QSize curWidgetWindowSize;
    QPoint globalPos; //верхняя левая точка рассматриваемого куска изображения
    qreal scaleKoef;
    QString pathToImage;
    std::optional<std::vector<QRect>> rectsToDraw;
    std::unique_ptr<QPoint> lastPos;
    void SetImage(QImage&& imageToSet) noexcept;
    void GlobalPosFilterX() noexcept;
    void GlobalPosFilterY() noexcept;
    std::shared_ptr<HandlerSyncPackage> curProm;
    std::string ErrStr;
public:
    QPictureBox(const QSize& size, QWidget* parent = nullptr);
    ~QPictureBox();
    template<class I>
    bool tryToSetImageUsingPath(I&& pathToIm) noexcept{
        constexpr bool isOk = std::is_same_v<QString, std::decay_t<I>>;
        if constexpr(isOk){
            this->pathToImage = std::forward<I>(pathToIm);
            this->SetImage(QImage(this->pathToImage));
        }
        return isOk;
    }
    void SetProm(std::shared_ptr<HandlerSyncPackage> prom);
protected:
    void timerEvent(QTimerEvent* tEvent) override;
    void paintEvent(QPaintEvent* pEvent) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *pEvent) override;
    QSize sizeHint() const override;
    void wheelEvent(QWheelEvent* wEvent) override;
public slots:
    void ShowMessageBox(QString str1, QString str2);
};
#endif // MYPICTUREBOX_DECLARATION_H
