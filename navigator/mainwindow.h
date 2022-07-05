#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "MyPictureBox_declaration.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_released();
    void StopWaitAnim();
    void StartWaitAnim();
    void ChangeAnimState(bool newAnimState);

private:
    Ui::MainWindow *ui;
    QPictureBox* pBox;
    std::unique_ptr<QMovie> waitAnim;
};
#endif // MAINWINDOW_H
