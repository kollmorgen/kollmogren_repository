#include "koll.h"
#include "ui_koll.h"

koll::koll(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::koll)
{
    ui->setupUi(this);
    //(1)初始化设计
    this->setWindowTitle("协作机器人实验软件");
    this->setWindowFlags(Qt::WindowCloseButtonHint);
//    this->setWindowFlags(Qt::WindowContextHelpButtonHint);
}



koll::~koll()
{
    delete ui;
}


