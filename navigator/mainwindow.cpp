#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QPushButton>
#include <QTextLine>
#include <QDialog>
#include <QMessageBox>
#include "navigatorhandler_declaration.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    std::cout.rdbuf(NULL);
    std::cerr.rdbuf(NULL);
    std::clog.rdbuf(NULL);
    ui->setupUi(this);
    this->pBox = new QPictureBox(QSize(600, 400), this);
    this->pBox->tryToSetImageUsingPath(QString(":res/images/map.png"));
    QGridLayout* l = new QGridLayout(this);
    l->addWidget(this->pBox, 1, 2, 5, 3);
    l->addWidget(this->ui->lineEdit, 0, 0);
    l->addWidget(this->ui->pushButton, 0, 3);
    l->addWidget(this->ui->lineEdit_2, 0, 5);
    this->centralWidget()->setLayout(l);
    QString pathToWaitAnim(QCoreApplication::applicationDirPath() + "/imageformats/WaitingAnim.gif");
    this->waitAnim = std::make_unique<QMovie>(pathToWaitAnim);
    bool check = this->waitAnim->isValid();
    this->ui->animLabel->setMovie(this->waitAnim.get());
    QObject::connect(this->pBox, &QPictureBox::SendWaitAnimState, this, &MainWindow::ChangeAnimState);
}

MainWindow::~MainWindow()
{
    this->waitAnim->stop(); //по идее не надо, но пусть будет
    delete ui;
}
void MainWindow::StopWaitAnim(){
    this->waitAnim->stop();
    this->ui->animLabel->hide();
}
void MainWindow::StartWaitAnim(){
    this->waitAnim->setScaledSize(this->ui->animLabel->size());
    this->waitAnim->start();
    this->ui->animLabel->show();
}
void MainWindow::ChangeAnimState(bool newAnimState){
    if(newAnimState){
        this->StartWaitAnim();
    }
    else{
        this->StopWaitAnim();
    }
}
void MainWindow::on_pushButton_released()
{
    NavigatorHandler::GetInstance().addNewTask(this->ui->lineEdit->text(), this->ui->lineEdit_2->text());
    this->pBox->SetProm(NavigatorHandler::GetInstance().getRectsPath());
}

