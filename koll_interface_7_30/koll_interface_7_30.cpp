#include "koll_interface_7_30.h"
#include "ui_koll_interface_7_30.h"


QT_CHARTS_USE_NAMESPACE

char ReciveMessage[1024];
char SendMessage[1024];

S_Command Send_cmd, *s_cmd=&Send_cmd;
S_Command Send_cmd2, *s_cmd_main=&Send_cmd2;
R_Command Rece_cmd, *r_cmd=&Rece_cmd;
R_Command Rece_cmd2, *r_cmd_tcp_swap=&Rece_cmd2;
R_Command Rece_cmd3, *r_cmd_main=&Rece_cmd3;

koll_interface_7_30::koll_interface_7_30(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::koll_interface_7_30)
{
    ui->setupUi(this);

    /*时间设置*/
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);

    /*电机状态设置*/
    motor_state="disconnect";
    /*状态设置*/
    build_communication(motor_state);
    /*标题按钮设置*/
    titlebutton_state="home";
    titlebutton_state_old="0";
    /*初始化到首页*/
    ui->middle_interface->setCurrentIndex(1);
    /*标题栏按键刷新*//*时间设置
     init_s_command(send_cmd);
     init_r_command(rece_cmd);

     printf_s_command(send_cmd);*/
    QTimer *timer_title = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate_title()));
    timer->start(10);


    ui->pushButton_home->setStyleSheet("QPushButton        {border-image: url(:/image/2-1.1-首页.png)}"
                                       "QPushButton:hover  {border-image: url(:/image/2-1.2-首页.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/2-1.3-首页.png);}");
    ui->pushButton_init->setStyleSheet ("QPushButton       {border-image: url(:/image/2-2.1-初始.png)}"
                                       "QPushButton:hover  {border-image: url(:/image/2-2.2-初始.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/2-2.3-初始.png);}");
    ui->pushButton_diagnosis->setStyleSheet("QPushButton   {border-image: url(:/image/2-3.1-诊断.png)}"
                                       "QPushButton:hover  {border-image: url(:/image/2-3.2-诊断.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/2-3.3-诊断.png);}");
    ui->pushButton_prepare->setStyleSheet("QPushButton     {border-image: url(:/image/2-4.1-准备.png)}"
                                       "QPushButton:hover  {border-image: url(:/image/2-4.2-准备.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/2-4.3-准备.png);}");
    ui->pushButton_operation->setStyleSheet("QPushButton   {border-image: url(:/image/2-5.1-手术.png)}"
                                       "QPushButton:hover  {border-image: url(:/image/2-5.2-手术.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/2-5.3-手术.png);}");
    ui->pushButton_analysis->setStyleSheet("QPushButton    {border-image: url(:/image/2-6.1-分析.png)}"
                                       "QPushButton:hover  {border-image: url(:/image/2-6.2-分析.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/2-6.3-分析.png);}");
    ui->pushButton_monitor->setStyleSheet("QPushButton     {border-image: url(:/image/2-7.1-监控.png)}"
                                       "QPushButton:hover  {border-image: url(:/image/2-7.2-监控.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/2-7.3-监控.png);}");
    monitor_lamp("red");

    /*监控界面按键初始化*/
//    ui->pushButton_monitor_start->setStyleSheet("QPushButton{font:15pt ;border-radius:5px;color:white;background-color: rgb(68, 144, 217);border:2px solid #3878b4;}""QPushButton:hover{background-color: rgb(56, 120, 180);border:2px solid #000000;}""QPushButton:pressed{font:16pt;}" "QPushButton:disabled{background-color: rgb(148, 148, 148);color:black;border:none;}");
//    ui->pushButton_monitor_end->setStyleSheet("QPushButton{font:15pt ;border-radius:5px;color:white;background-color: rgb(68, 144, 217);border:2px solid #3878b4;}""QPushButton:hover{background-color: rgb(56, 120, 180);border:2px solid #000000;}""QPushButton:pressed{font:16pt;}" "QPushButton:disabled{background-color: rgb(148, 148, 148);color:black;border:none;}");
//    ui->pushButton_monitor_return0->setStyleSheet("QPushButton{font:15pt ;border-radius:5px;color:white;background-color: rgb(68, 144, 217);border:2px solid #3878b4;}""QPushButton:hover{background-color: rgb(56, 120, 180);border:2px solid #000000;}""QPushButton:pressed{font:16pt;}" "QPushButton:disabled{background-color: rgb(148, 148, 148);color:black;border:none;}");
//    ui->pushButton_monitor_start_pos->setStyleSheet("QPushButton{font:15pt ;border-radius:5px;color:white;background-color: rgb(68, 144, 217);border:2px solid #3878b4;}""QPushButton:hover{background-color: rgb(56, 120, 180);border:2px solid #000000;}""QPushButton:pressed{font:16pt;}" "QPushButton:disabled{background-color: rgb(148, 148, 148);color:black;border:none;}");
//    ui->pushButton_t->setStyleSheet("QPushButton{font:15pt ;border-radius:5px;color:white;background-color: rgb(68, 144, 217);border:2px solid #3878b4;}""QPushButton:hover{background-color: rgb(56, 120, 180);border:2px solid #000000;}""QPushButton:pressed{font:16pt;}" "QPushButton:disabled{background-color: rgb(148, 148, 148);color:black;border:none;}");
//    //ui->pushButton_monitor_upload->setStyleSheet("QPushButton{font:15pt ;border-radius:5px;color:white;background-color: rgb(68, 144, 217);border:2px solid #3878b4;}""QPushButton:hover{background-color: rgb(56, 120, 180);border:2px solid #000000;}""QPushButton:pressed{font:16pt;}" "QPushButton:disabled{background-color: rgb(148, 148, 148);color:black;border:none;}");


    /*Force Sensor Init*/
    timer_force = new QTimer(this);
    connect(timer_force,SIGNAL(timeout()),this,SLOT(Force_show()));


    timer_data = new QTimer(this);
    connect(timer_data,SIGNAL(timeout()),this,SLOT(timerUpdate_data()));

    memset(SendMessage,'\0',sizeof(1024*sizeof(char)));

    init_s_command(s_cmd_main);
    init_r_command(r_cmd_main);
    init_s_command(s_cmd);

    shutdown_shortcut = new QShortcut(this);
    shutdown_shortcut->setKey(tr("shift+return"));
    shutdown_shortcut->setAutoRepeat(false);
    connect(shutdown_shortcut,SIGNAL(activated()),this, SLOT(shutdown_now()));
}

koll_interface_7_30::~koll_interface_7_30()
{
    delete ui;
}
//同时完成时间更新与标题栏状态更新
void koll_interface_7_30::timerUpdate(void)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->label_time->setText(str);
    //标题栏状态更新
}


void koll_interface_7_30::timerUpdate_title()
{
    if(titlebutton_state=="home" &&  titlebutton_state_old!="home")
    {
        ui->pushButton_home->setStyleSheet("QPushButton        {border-image: url(:/image/2-1.2-首页.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-1.2-首页.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-1.3-首页.png);}");
        return_title_button(titlebutton_state_old);
        titlebutton_state_old="home";
    }
    if(titlebutton_state=="init" &&  titlebutton_state_old!="init")
    {
        ui->pushButton_init->setStyleSheet("QPushButton        {border-image: url(:/image/2-2.2-初始.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-2.2-初始.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-2.3-初始.png);}");
         return_title_button(titlebutton_state_old);
        titlebutton_state_old="init";
    }
    if(titlebutton_state=="diagnosis" &&  titlebutton_state_old!="diagnosis")
    {
        ui->pushButton_diagnosis->setStyleSheet("QPushButton        {border-image: url(:/image/2-3.2-诊断.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-3.2-诊断.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-3.3-诊断.png);}");
         return_title_button(titlebutton_state_old);
        titlebutton_state_old="diagnosis";
    }
    if(titlebutton_state=="prepare" &&  titlebutton_state_old!="prepare")
    {
        ui->pushButton_prepare->setStyleSheet("QPushButton        {border-image: url(:/image/2-4.2-准备.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-4.2-准备.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-4.3-准备.png);}");
         return_title_button(titlebutton_state_old);
        titlebutton_state_old="prepare";
    }
    if(titlebutton_state=="operation" &&  titlebutton_state_old!="operation")
    {
        ui->pushButton_operation->setStyleSheet("QPushButton        {border-image: url(:/image/2-5.2-手术.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-5.2-手术.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-5.3-手术.png);}");
         return_title_button(titlebutton_state_old);
        titlebutton_state_old="operation";
    }
    if(titlebutton_state=="analysis" &&  titlebutton_state_old!="analysis")
    {
        ui->pushButton_analysis->setStyleSheet("QPushButton        {border-image: url(:/image/2-6.2-分析.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-6.2-分析.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-6.3-分析.png);}");
         return_title_button(titlebutton_state_old);
        titlebutton_state_old="analysis";
    }
    if(titlebutton_state=="monitor" &&  titlebutton_state_old!="monitor")
    {
        ui->pushButton_monitor->setStyleSheet("QPushButton        {border-image: url(:/image/2-7.2-监控.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-7.2-监控.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-7.3-监控.png);}");
         return_title_button(titlebutton_state_old);
        titlebutton_state_old="monitor";
    }

}

void koll_interface_7_30::return_title_button(QString old)
{
    if(old=="home")
    {
        ui->pushButton_home->setStyleSheet("QPushButton        {border-image: url(:/image/2-1.1-首页.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-1.2-首页.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-1.3-首页.png);}");
    }
    if(old=="init")
    {
        ui->pushButton_init->setStyleSheet("QPushButton        {border-image: url(:/image/2-2.1-初始.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-2.2-初始.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-2.3-初始.png);}");
    }
    if(old=="diagnosis")
    {
        ui->pushButton_diagnosis->setStyleSheet("QPushButton        {border-image: url(:/image/2-3.1-诊断.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-3.2-诊断.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-3.3-诊断.png);}");
    }
    if(old=="prepare")
    {
        ui->pushButton_prepare->setStyleSheet("QPushButton        {border-image: url(:/image/2-4.1-准备.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-4.2-准备.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-4.3-准备.png);}");
    }
    if(old=="operation")
    {
        ui->pushButton_operation->setStyleSheet("QPushButton        {border-image: url(:/image/2-5.1-手术.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-5.2-手术.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-5.3-手术.png);}");
    }
    if(old=="analysis")
    {
        ui->pushButton_analysis->setStyleSheet("QPushButton        {border-image: url(:/image/2-6.1-分析.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-6.2-分析.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-6.3-分析.png);}");
    }
    if(old=="monitor")
    {
        ui->pushButton_monitor->setStyleSheet("QPushButton        {border-image: url(:/image/2-7.1-监控.png)}"
                                           "QPushButton:hover  {border-image: url(:/image/2-7.2-监控.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/2-7.3-监控.png);}");
    }
}

void koll_interface_7_30::on_pushButton_init_clicked()
{

     ui->middle_interface->setCurrentIndex(1);
     titlebutton_state="init";

}

void koll_interface_7_30::on_pushButton_home_clicked()
{
     ui->middle_interface->setCurrentIndex(0);
     titlebutton_state="home";
}

void koll_interface_7_30::on_pushButton_diagnosis_clicked()
{
     ui->middle_interface->setCurrentIndex(2);
     titlebutton_state="diagnosis";
}

void koll_interface_7_30::on_pushButton_prepare_clicked()
{
     ui->middle_interface->setCurrentIndex(3);
     titlebutton_state="prepare";
}

void koll_interface_7_30::on_pushButton_operation_clicked()
{
     ui->middle_interface->setCurrentIndex(4);
     titlebutton_state="operation";
}

void koll_interface_7_30::on_pushButton_analysis_clicked()
{
     ui->middle_interface->setCurrentIndex(5);
     titlebutton_state="analysis";
}

void koll_interface_7_30::on_pushButton_monitor_clicked()
{
     ui->middle_interface->setCurrentIndex(6);
     titlebutton_state="monitor";
}

void koll_interface_7_30::monitor_lamp(QString mlampcolor)
{
    if (mlampcolor=="green")
    {
        ui->frame_motor1_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.1-绿灯.png)}");
        ui->frame_motor2_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.1-绿灯.png)}");
        ui->frame_motor3_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.1-绿灯.png)}");
        ui->frame_motor4_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.1-绿灯.png)}");
        ui->frame_motor5_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.1-绿灯.png)}");
        ui->frame_motor6_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.1-绿灯.png)}");
        ui->frame_force1_lamp ->setStyleSheet("QFrame{border-image: url(:/image/3-1.1-绿灯.png)}");
        ui->frame_force2_lamp ->setStyleSheet("QFrame{border-image: url(:/image/3-1.1-绿灯.png)}");
    }
    if (mlampcolor=="yellow")
    {
        ui->frame_motor1_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.2-黄灯.png)}");
        ui->frame_motor2_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.2-黄灯.png)}");
        ui->frame_motor3_lamp->setStyleSheet("QFrameQFrame{border-image: url(:/image/3-1.2-黄灯.png)}");
        ui->frame_motor4_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.2-黄灯.png)}");
        ui->frame_motor5_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.2-黄灯.png)}");
        ui->frame_motor6_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.2-黄灯.png)}");
        ui->frame_force1_lamp ->setStyleSheet("QFrame{border-image: url(:/image/3-1.2-黄灯.png)}");
        ui->frame_force2_lamp ->setStyleSheet("QFrame{border-image: url(:/image/3-1.2-黄灯.png)}");
    }
    if (mlampcolor=="red")
    {
        ui->frame_motor1_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.3-红灯.png)}");
        ui->frame_motor3_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.3-红灯.png)}");
        ui->frame_motor4_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.3-红灯.png)}");
        ui->frame_motor5_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.3-红灯.png)}");
        ui->frame_motor2_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.3-红灯.png)}");
        ui->frame_motor6_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.3-红灯.png)}");
        ui->frame_force1_lamp ->setStyleSheet("QFrame{border-image: url(:/image/3-1.3-红灯.png)}");
        ui->frame_force2_lamp ->setStyleSheet("QFrame{border-image: url(:/image/3-1.3-红灯.png)}");
    }

}



void koll_interface_7_30::build_communication(QString build)
{
    if(build=="connect")
    {
        ui->label_state->setText("已经连接");
        ui->frame_state_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.1-绿灯.png)}");
    }
    if(build=="disconnect")
    {
        ui->label_state->setText("尚未连接");
        ui->frame_state_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.3-红灯.png)}");
    }
    if(build=="fault")
    {
        ui->label_state->setText("出现故障");
        ui->frame_state_lamp->setStyleSheet("QFrame{border-image: url(:/image/3-1.2-黄灯.png)}");
    }
}



void koll_interface_7_30::on_motorcontrol4_14_clicked()
{
   timer_force->start(100);
}

void koll_interface_7_30::on_motorcontrol4_13_clicked()
{
    timer_force->stop();
    ui->lineEdit_Fx->setText("");ui->progressBar_Fx->setValue(0);
    ui->lineEdit_Fy->setText("");ui->progressBar_Fy->setValue(0);
    ui->lineEdit_Fz->setText("");ui->progressBar_Fz->setValue(0);
    ui->lineEdit_Tx->setText("");ui->progressBar_Tx->setValue(0);
    ui->lineEdit_Ty->setText("");ui->progressBar_Ty->setValue(0);
    ui->lineEdit_Tz->setText("");ui->progressBar_Tz->setValue(0);
    ui->lineEdit_FSstate->setText("");
}
void koll_interface_7_30::Force_show()
{
    int FSinit=Fsensor_init ();
    if(FSinit==0)
        ui->lineEdit_FSstate->setText("Connecting");
    else if(FSinit==-1)
        ui->lineEdit_FSstate->setText("Socket Error");
    else if(FSinit==-2)
        ui->lineEdit_FSstate->setText("Connect Error");
    else
        ui->lineEdit_FSstate->setText("?Error");
    float *F;
    F=Fdata_get();
    QString str1 = QString::number(F[0],'f',4);ui->lineEdit_Fx->setText(str1);ui->progressBar_Fx->setValue(fabs(F[0]));
    QString str2 = QString::number(F[1],'f',4);ui->lineEdit_Fy->setText(str2);ui->progressBar_Fy->setValue(fabs(F[1]));
    QString str3 = QString::number(F[2],'f',4);ui->lineEdit_Fz->setText(str3);ui->progressBar_Fz->setValue(fabs(F[2]));
    QString str4 = QString::number(F[3],'f',4);ui->lineEdit_Tx->setText(str4);ui->progressBar_Tx->setValue(fabs(F[3]));
    QString str5 = QString::number(F[4],'f',4);ui->lineEdit_Ty->setText(str5);ui->progressBar_Ty->setValue(fabs(F[4]));
    QString str6 = QString::number(F[5],'f',4);ui->lineEdit_Tz->setText(str6);ui->progressBar_Tz->setValue(fabs(F[5]));
    Fsensor_colse();

}

void koll_interface_7_30::on_groupBox_17_clicked()
{

}

void koll_interface_7_30::on_motorcontrol4_28_clicked()
{
    int FSinit=Fsensor_ping();
    if(FSinit==0)
        ui->sensor_status->setText("Ping[192.168.1.120] OK");
    else if(FSinit==-1)
        ui->sensor_status->setText("Socket Error");
    else if(FSinit==-2)
        ui->manipulator_status->setText("Connect Error");
    else
        ui->sensor_status->setText("?Error");
}

void koll_interface_7_30::on_motorcontrol4_26_clicked()
{
    int KollSinit=kolltcp_ping();
    if(KollSinit==0)
        ui->manipulator_status->setText("Ping[192.168.1.100] OK");
    else if(KollSinit==-1)
        ui->manipulator_status->setText("Socket Error");
    else if(KollSinit==-2)
        ui->manipulator_status->setText("Connect Error");
    else
        ui->manipulator_status->setText("?Error");
}


//Start button
void koll_interface_7_30::on_motorcontrol4_6_clicked()
{

//    char index[31] = Motor_start;
//    Send_message_processing(index);
//    Send_control_message();

//    wait_for_receiving();

//    Receive_message_processing();
//    Receive_message_display();

}

void koll_interface_7_30::on_pushButton_analysis_1_28_clicked()
{
    ui->TCP_tset_recv->setText("Ping OK Wating Client...");
    int KollSinit=kolltcp_init();
    if(KollSinit==0)
    {
        build_communication("connect");
        ui->TCP_tset_recv->setText("Connect to Koll");
    }
    else if(KollSinit==-1)
        ui->TCP_tset_recv->setText("Socket Error");
    else if(KollSinit==-2)
        ui->TCP_tset_recv->setText("Connect Error");
    else
        ui->TCP_tset_recv->setText("?Error");
}

void koll_interface_7_30::on_pushButton_analysis_1_25_clicked()
{


    char index[31] = Shutdown;
    Send_message_processing(index);
    Send_control_message();
    QTime t;
    t.start();
    while(t.elapsed()<1);
    kolltcp_close();
    ui->TCP_tset_recv->setText("Shutdown the koll");
}

/*************************
 * TCP send demo
**************************/
void koll_interface_7_30::on_pushButton_analysis_1_18_clicked()
{
    string send = ui->TCP_tset_send->text().toStdString();

    pthread_mutex_lock(&mut);
    s_cmd->c_index = '1';
    strcpy(s_cmd->k_command, "Start");
    pthread_mutex_unlock(&mut);

    QTime t;
    t.start();
    while(t.elapsed()<2);

    pthread_mutex_lock(&mut_rec);

    if(r_cmd->c_index != '0')
    {
        qDebug()<<r_cmd->c_index;
        ui->TCP_tset_recv_2->setText(r_cmd->k_command);
    }

    pthread_mutex_unlock(&mut_rec);

}


void koll_interface_7_30::on_motorcontrol4_6_clicked(bool checked)
{
//     system("gnome-terminal");
   // gnome-terminal -x bash -c 'export RUN_AFTER_BASHRC="ls --help"; exec bash'
}



//**********************************load motor data and Force sensor data Start*************************//
void koll_interface_7_30::on_motorcontrol4_15_clicked()
{
    ui->lineEdit_55->setText("Loading data");
    monitor_lamp("green");
    timer_data->start(50);
}

void koll_interface_7_30::timerUpdate_data(void)
{
    char index = 'D';

    Send_message_processing(&index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

void koll_interface_7_30::on_motorcontrol4_16_clicked()
{
    //Stop the timer for stopping
    timer_data->stop();
    monitor_lamp("red");
    Dispalay_motor_data(CLEAR);

}
//**********************************load motor data and Force sensor data Stop*************************//








/*************************************Test Mode Start***********************************************/
/*************************************Test Mode Start***********************************************/
/*************************************Test Mode Start***********************************************/

//Joint 1 Test Positive
void koll_interface_7_30::on_pushButton_analysis_1_15_pressed()
{
    char index[31] = M1_q;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();

}
//Joint 1 stop
void koll_interface_7_30::on_pushButton_analysis_1_15_released()
{
    char index[31] = M1_s;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 1 Test Negative
void koll_interface_7_30::on_pushButton_analysis_1_17_pressed()
{
    char index[31] = M1_q_inv;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();

}
//Joint 1 Stop
void koll_interface_7_30::on_pushButton_analysis_1_17_released()
{
    char index[31] = M1_s;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

//Joint 2 Test Positive
void koll_interface_7_30::on_pushButton_analysis_1_19_pressed()
{
    char index[31] = M2_q;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 2 Stop
void koll_interface_7_30::on_pushButton_analysis_1_19_released()
{
    char index[31] = M2_s;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 2 Test Negative
void koll_interface_7_30::on_pushButton_analysis_1_24_pressed()
{
    char index[31] = M2_q_inv;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 2 Stop
void koll_interface_7_30::on_pushButton_analysis_1_24_released()
{
    char index[31] = M2_s;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

//Joint 3 Test Positive
void koll_interface_7_30::on_pushButton_analysis_1_23_pressed()
{
    char index[31] = M3_q;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 3 Stop
void koll_interface_7_30::on_pushButton_analysis_1_23_released()
{
    char index[31] = M3_s;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 3 Test Negative
void koll_interface_7_30::on_pushButton_analysis_1_26_pressed()
{
    char index[31] = M3_q_inv;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 3 Stop
void koll_interface_7_30::on_pushButton_analysis_1_26_released()
{
    char index[31] = M3_s;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

//Joint 4 Test Positive
void koll_interface_7_30::on_pushButton_analysis_1_20_pressed()
{
    char index[31] = M4_q;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 4 Stop
void koll_interface_7_30::on_pushButton_analysis_1_20_released()
{
    char index[31] = M4_s;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 4 Test Negative
void koll_interface_7_30::on_pushButton_analysis_1_51_pressed()
{
    char index[31] = M4_q_inv;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 4 Stop
void koll_interface_7_30::on_pushButton_analysis_1_51_released()
{
    char index[31] = M4_s;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

//Joint 5 Test Positive
void koll_interface_7_30::on_pushButton_analysis_1_21_pressed()
{
    char index[31] = M5_q;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 5 Stop
void koll_interface_7_30::on_pushButton_analysis_1_21_released()
{
    char index[31] = M5_s;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 5 Test Negative
void koll_interface_7_30::on_pushButton_analysis_1_50_pressed()
{
    char index[31] = M5_q_inv;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 5 Stop
void koll_interface_7_30::on_pushButton_analysis_1_50_released()
{
    char index[31] = M5_s;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

//Joint 6 Test Positive
void koll_interface_7_30::on_pushButton_analysis_1_22_pressed()
{
    char index[31] = M6_q;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 6 Stop
void koll_interface_7_30::on_pushButton_analysis_1_22_released()
{
    char index[31] = M6_s;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 6 Test Negative
void koll_interface_7_30::on_pushButton_analysis_1_27_pressed()
{
    char index[31] = M6_q_inv;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
//Joint 6 Stop
void koll_interface_7_30::on_pushButton_analysis_1_27_released()
{
    char index[31] = M6_s;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

/*************************************Test Mode Stop***********************************************/
/*************************************Test Mode Stop***********************************************/
/*************************************Test Mode Stop***********************************************/




/***********************************Mode control Start***********************************************/
/***********************************Mode control Start***********************************************/
/***********************************Mode control Start***********************************************/

//motor1 pos/vel/cur control
void koll_interface_7_30::on_pushButton_analysis_1_14_clicked()
{
    //control command
    int mode = ui->comboBox_3->currentIndex()-1;  //control mode
    int motor = 1;//motor
    string control_num = ui->motor1_control_signal->text().toStdString(); //control quantity

    //generate command string
    char *index_;
    index_ = motor_mode_control_string(mode, motor, control_num);

    //processing information and give the command to s_cmd_main
    Send_message_processing(index_);
    //send message
    Send_control_message();
    //as it
    wait_for_receiving();
    //swap receive message
    Receive_message_processing();
    //display receive message
    Receive_message_display();
}

void koll_interface_7_30::on_motorcontrol2_clicked()
{
    //control command
    int mode = ui->comboBox_8->currentIndex()-1;  //control mode
    int motor = 2;//motor
    string control_num = ui->motor1_control_signal_2->text().toStdString(); //control quantity

    //generate command string
    char *index_;
    index_ = motor_mode_control_string(mode, motor, control_num);

    //processing information and give the command to s_cmd_main
    Send_message_processing(index_);
    //send message
    Send_control_message();
    //as it
    wait_for_receiving();
    //swap receive message
    Receive_message_processing();
    //display receive message
    Receive_message_display();
}

void koll_interface_7_30::on_motorcontrol3_clicked()
{
    //control command
    int mode = ui->comboBox_7->currentIndex()-1;  //control mode
    int motor = 3;//motor
    string control_num = ui->motor1_control_signal_3->text().toStdString(); //control quantity

    //generate command string
    char *index_;
    index_ = motor_mode_control_string(mode, motor, control_num);

    //processing information and give the command to s_cmd_main
    Send_message_processing(index_);
    //send message
    Send_control_message();
    //as it
    wait_for_receiving();
    //swap receive message
    Receive_message_processing();
    //display receive message
    Receive_message_display();
}

void koll_interface_7_30::on_motorcontrol4_2_clicked()
{
    //control command
    int mode = ui->comboBox_5->currentIndex()-1;  //control mode
    int motor = 4;//motor
    string control_num = ui->motor1_control_signal_7->text().toStdString(); //control quantity

    //generate command string
    char *index_;
    index_ = motor_mode_control_string(mode, motor, control_num);

    //processing information and give the command to s_cmd_main
    Send_message_processing(index_);
    //send message
    Send_control_message();
    //as it
    wait_for_receiving();
    //swap receive message
    Receive_message_processing();
    //display receive message
    Receive_message_display();
}

void koll_interface_7_30::on_motorcontrol5_clicked()
{
    //control command
    int mode = ui->comboBox_6->currentIndex()-1;  //control mode
    int motor = 5;//motor
    string control_num = ui->motor1_control_signal_5->text().toStdString(); //control quantity

    //generate command string
    char *index_;
    index_ = motor_mode_control_string(mode, motor, control_num);

    //processing information and give the command to s_cmd_main
    Send_message_processing(index_);
    //send message
    Send_control_message();
    //as it
    wait_for_receiving();
    //swap receive message
    Receive_message_processing();
    //display receive message
    Receive_message_display();
}

void koll_interface_7_30::on_motorcontrol6_2_clicked()
{
    //control command
    int mode = ui->comboBox_4->currentIndex()-1;  //control mode
    int motor = 6;//motor
    string control_num = ui->motor1_control_signal_8->text().toStdString(); //control quantity

    //generate command string
    char *index_;
    index_ = motor_mode_control_string(mode, motor, control_num);

    //processing information and give the command to s_cmd_main
    Send_message_processing(index_);
    //send message
    Send_control_message();
    //as it
    wait_for_receiving();
    //swap receive message
    Receive_message_processing();
    //display receive message
    Receive_message_display();
}
/***********************************Mode control Stop***********************************************/
/***********************************Mode control Stop***********************************************/
/***********************************Mode control Stop***********************************************/



/***********************************Excitation Trajectory Start***********************************************/
/***********************************Excitation Trajectory Start***********************************************/
/***********************************Excitation Trajectory Start***********************************************/

//Excitation Trajectory 1
void koll_interface_7_30::on_motorcontrol6_4_clicked()
{
    char index[31] = Tra1;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

//Excitation Trajectory 2
void koll_interface_7_30::on_motorcontrol6_5_clicked()
{
    char index[31] = Tra2;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

//Excitation Trajectory 3
void koll_interface_7_30::on_motorcontrol6_17_clicked()
{
    char index[31] = Tra3;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

//Excitation Trajectory 4
void koll_interface_7_30::on_motorcontrol6_21_clicked()
{
    char index[31] = Tra4;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

/***********************************Excitation Trajectory Stop***********************************************/
/***********************************Excitation Trajectory Stop***********************************************/
/***********************************Excitation Trajectory Stop***********************************************/



/***********************************Zero Force control Start***********************************************/
/***********************************Zero Force control Start***********************************************/
/***********************************Zero Force control Start***********************************************/

//No compasenta
void koll_interface_7_30::on_motorcontrol6_32_clicked()
{
    char index[31] = No_compasenta;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

//Gravity compasenta
void koll_interface_7_30::on_motorcontrol6_33_clicked()
{
    char index[31] = Gravity_compasenta;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

//Friction and Gravity compasenta
void koll_interface_7_30::on_motorcontrol6_34_clicked()
{
    char index[31] = Fri_gra_compasenta;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

/***********************************Zero Force control Stop***********************************************/
/***********************************Zero Force control Stop***********************************************/
/***********************************Zero Force control Stop***********************************************/


//Disable Motor
void koll_interface_7_30::on_motorcontrol4_4_clicked()
{

}

//Send PID parameters
void koll_interface_7_30::on_pushButton_pid_clicked()
{
    char index[10] = motorPID;

    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

//Stop Motor
void koll_interface_7_30::on_motorcontrol4_7_clicked()
{
    char index[31] = Stop;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

//Stop Motor using keyboard shortcut
void koll_interface_7_30::shutdown_now(void)
{
  kolltcp_close();
  qDebug()<<"Shutdown the robot!!NOW!!";

}





/****************************************Tool Function Start****************************************************/

/*****************************************************************
 * Function: Processing send message according to parameters
 * Parameters: motor_cmd(For controlling motor, it is equal to s_cmd->c_index+s_cmd->k_command )
 * Return: void
******************************************************************/
void koll_interface_7_30::Send_message_processing(char * motor_cmd)
{
    switch (motor_cmd[0])
    {
        case '0': {qDebug()<<"No commands\nPlease check your command index!\n";}break;
        case 'D':
        {
            qDebug()<<"Send motor data\n";
            s_cmd_main->c_index = motor_cmd[0];
        }break;
        case 'P':
        {
            s_cmd_main->c_index = motor_cmd[0];
            s_cmd_main->motor_PID[0] = ui->Kp_signal->text().toFloat();
            s_cmd_main->motor_PID[1] = ui->Ki_signal->text().toFloat();
            s_cmd_main->motor_PID[2] = ui->Kd_signal->text().toFloat();
        }break;
        default:
        {
            s_cmd_main->c_index = motor_cmd[0];
            strcpy(s_cmd_main->k_command, &motor_cmd[1]);
            //qDebug()<<"Send_message_processing function::s_cmd_main->k_command: "<<s_cmd_main->k_command;
        }break;
    }
}

/*****************************************************************
 * Function: send motor control message
 * Parameters: void
 * Return: void
******************************************************************/
void koll_interface_7_30::Send_control_message(void)
{
    pthread_mutex_lock(&mut);

    memcpy(s_cmd, s_cmd_main, sizeof(S_Command));    //swap the send message
    init_s_command(s_cmd_main);

    pthread_mutex_unlock(&mut);


}


/*****************************************************************
 * Function: Swap the receive message
 * Parameters: void
 * Return: void
******************************************************************/
void koll_interface_7_30::Receive_message_processing(void)
{

    pthread_mutex_lock(&mut);

    memcpy(r_cmd_main, r_cmd_tcp_swap, sizeof(R_Command));    //swap the receive message
    init_r_command(r_cmd_tcp_swap);

    pthread_mutex_unlock(&mut);

}

/*****************************************************************
 * Function: Display receive message on the interface
 * Parameters: motor_cmd(For controlling motor, 20 char)
 * Return: void
******************************************************************/
void koll_interface_7_30::Receive_message_display(void)
{
    //Lower machine need to give r_cmd->c_index to s_cmd->index
    switch (r_cmd_main->c_index) {
        case '0': {qDebug()<<"No commands\nPlease check your command index!\n";}break;
        case 'D':
        {
            Dispalay_motor_data(DISPLAY);
        }
        default:
        {
            qDebug()<<"In Display";
            ui->motor1_control_signal_13->setText(r_cmd_main->k_command);
        }

    }

}

/*****************************************************************
 * Function: Display or Clear motor data
 * Parameters: isdisplay
 * Return: void
******************************************************************/
void koll_interface_7_30::Dispalay_motor_data(bool isdisplay)
{
    if(isdisplay)
    {

//            QString str1 = QString::number(F[0],'f',4);ui->lineEdit_Fx->setText(str1);ui->progressBar_Fx->setValue(fabs(F[0]));


        //Motor and Force Data Status
        //QString status_1(r_cmd_main->data_status);
        ui->lineEdit_55->setText("Ok");

        //Force Sensor Data
        qDebug()<<"TimerUpadte_data MotorData: "<<r_cmd_main->c_index;
        QString data_;
        data_ =QString::number(r_cmd_main->data_fsensor[0],'f',4);
        ui->lineEdit_Fx_2->setText(data_);ui->progressBar_Fx_2->setValue(fabs(r_cmd_main->data_fsensor[0]));
        data_ =QString::number(r_cmd_main->data_fsensor[1],'f',4);
        ui->lineEdit_Fy_2->setText(data_);ui->progressBar_Fy_2->setValue(fabs(r_cmd_main->data_fsensor[1]));
        data_ =QString::number(r_cmd_main->data_fsensor[2],'f',4);
        ui->lineEdit_Fz_2->setText(data_);ui->progressBar_Fz_2->setValue(fabs(r_cmd_main->data_fsensor[2]));
        data_ =QString::number(r_cmd_main->data_fsensor[3],'f',4);
        ui->lineEdit_Tx_2->setText(data_);ui->progressBar_Nx_2->setValue(fabs(r_cmd_main->data_fsensor[3]));
        data_ =QString::number(r_cmd_main->data_fsensor[4],'f',4);
        ui->lineEdit_Ty_2->setText(data_);ui->progressBar_Ny_2->setValue(fabs(r_cmd_main->data_fsensor[4]));
        data_ =QString::number(r_cmd_main->data_fsensor[5],'f',4);
        ui->lineEdit_Tz_2->setText(data_);ui->progressBar_Nz_2->setValue(fabs(r_cmd_main->data_fsensor[5]));

        //Motor Position Data
        data_ =QString::number(r_cmd_main->data_pos[0],'f',4);
        ui->p1->setText(data_);ui->progressBar_p1->setValue(fabs(r_cmd_main->data_pos[0]));
        data_ =QString::number(r_cmd_main->data_pos[1],'f',4);
        ui->p1_5->setText(data_);ui->progressBar_p2->setValue(fabs(r_cmd_main->data_pos[1]));
        data_ =QString::number(r_cmd_main->data_pos[2],'f',4);
        ui->p1_6->setText(data_);ui->progressBar_p3->setValue(fabs(r_cmd_main->data_pos[2]));
        data_ =QString::number(r_cmd_main->data_pos[3],'f',4);
        ui->p1_7->setText(data_);ui->progressBar_p4->setValue(fabs(r_cmd_main->data_pos[3]));
        data_ =QString::number(r_cmd_main->data_pos[4],'f',4);
        ui->p1_8->setText(data_);ui->progressBar_p5->setValue(fabs(r_cmd_main->data_pos[4]));
        data_ =QString::number(r_cmd_main->data_pos[5],'f',4);
        ui->p1_9->setText(data_);ui->progressBar_p6->setValue(fabs(r_cmd_main->data_pos[5]));

        //Motor Velocity Data
        data_ =QString::number(r_cmd_main->data_vel[0],'f',4);
        ui->v1->setText(data_);ui->progressBar_v1->setValue(fabs(r_cmd_main->data_vel[0]));
        data_ =QString::number(r_cmd_main->data_vel[1],'f',4);
        ui->v1_5->setText(data_);ui->progressBar_v2->setValue(fabs(r_cmd_main->data_vel[1]));
        data_ =QString::number(r_cmd_main->data_vel[2],'f',4);
        ui->v1_6->setText(data_);ui->progressBar_v3->setValue(fabs(r_cmd_main->data_vel[2]));
        data_ =QString::number(r_cmd_main->data_vel[3],'f',4);
        ui->v1_7->setText(data_);ui->progressBar_v4->setValue(fabs(r_cmd_main->data_vel[3]));
        data_ =QString::number(r_cmd_main->data_vel[4],'f',4);
        ui->v1_8->setText(data_);ui->progressBar_v5->setValue(fabs(r_cmd_main->data_vel[4]));
        data_ =QString::number(r_cmd_main->data_vel[5],'f',4);
        ui->v1_9->setText(data_);ui->progressBar_v6->setValue(fabs(r_cmd_main->data_vel[5]));

        //Motor Current Data
        data_ =QString::number(r_cmd_main->data_cur[0],'f',4);
        ui->c1->setText(data_);ui->progressBar_t1->setValue(fabs(r_cmd_main->data_cur[0]));
        data_ =QString::number(r_cmd_main->data_cur[1],'f',4);
        ui->c1_5->setText(data_);ui->progressBar_t2->setValue(fabs(r_cmd_main->data_cur[1]));
        data_ =QString::number(r_cmd_main->data_cur[2],'f',4);
        ui->c1_6->setText(data_);ui->progressBar_t3->setValue(fabs(r_cmd_main->data_cur[2]));
        data_ =QString::number(r_cmd_main->data_cur[3],'f',4);
        ui->c1_7->setText(data_);ui->progressBar_t4->setValue(fabs(r_cmd_main->data_cur[3]));
        data_ =QString::number(r_cmd_main->data_cur[4],'f',4);
        ui->c1_8->setText(data_);ui->progressBar_t5->setValue(fabs(r_cmd_main->data_cur[4]));
        data_ =QString::number(r_cmd_main->data_cur[5],'f',4);
        ui->c1_9->setText(data_);ui->progressBar_t6->setValue(fabs(r_cmd_main->data_cur[5]));

        init_r_command(r_cmd_main);
    }
    else
    {
        //Motor and Force Data Status
        ui->lineEdit_55->setText("No Data!");

        //Force Sensor Data
        qDebug()<<"MotorData: "<<r_cmd_main->c_index;
        ui->lineEdit_Fx_2->clear();
        ui->lineEdit_Fy_2->clear();
        ui->lineEdit_Fz_2->clear();
        ui->lineEdit_Tx_2->clear();
        ui->lineEdit_Ty_2->clear();
        ui->lineEdit_Tz_2->clear();

        //Motor Position Data
        ui->p1->clear();
        ui->p1_5->clear();
        ui->p1_6->clear();
        ui->p1_7->clear();
        ui->p1_8->clear();
        ui->p1_9->clear();

        //Motor Velocity Data
        ui->v1->clear();
        ui->v1_5->clear();
        ui->v1_6->clear();
        ui->v1_7->clear();
        ui->v1_8->clear();
        ui->v1_9->clear();

        //Motor Current Data
        ui->c1->clear();
        ui->c1_5->clear();
        ui->c1_6->clear();
        ui->c1_7->clear();
        ui->c1_8->clear();
        ui->c1_9->clear();
    }
}



/*****************************************************************
 * Function: Generate control command of motor mode
 *
 * Parameters: model: control mode(pvf)
 * *********** motor: which one motor
 * *********** control_num: control quantity of motor
 *
 * Return: char of control command
******************************************************************/
char* koll_interface_7_30::motor_mode_control_string(int mode, int motor, string control_num)
{
    char *index_ = (char *)malloc(sizeof(r_cmd_main->k_command));
    if (mode == -1)
    {
        qDebug()<<"Please Input the control signal of motor1!\n";
        QMessageBox::StandardButton warn1 = QMessageBox::information(NULL, "警告", "请选择控制模式！", QMessageBox::Yes);
        index_[0] = '0';
        return index_;
    }
    char control_mode[5] = "pvf-";
    char motor_index[7] = "123456"; //choose control mode and motor
    string control_mode_ = "";
    control_mode_.push_back(control_mode[mode]);   //char->string
    control_mode_.push_back(control_mode[3]);      //p- or v- or f-

    string index = "3M";
    index.push_back(motor_index[motor-1]);
    index.push_back(control_mode[3]);    //"3M1-"

    string Mode_control_command = index+control_mode_+control_num;  //add string to one char
    Mode_control_command.copy(index_, Mode_control_command.length());  //change it into char*

    return index_;
}

/*****************************************************************
 * Function: Waiting for several millisecond before reveiving message
 * Parameters: void
 * Return: void
******************************************************************/
void koll_interface_7_30::wait_for_receiving(void)
{
    QTime t;
    t.start();
    while(t.elapsed()<10);
}


//slot function of close the wdget!
void koll_interface_7_30::closeEvent(QCloseEvent *event) //系统自带退出确定程序
{
//    int choose;
//    choose= QMessageBox::question(this, tr("退出程序d"),
//                                   QString(tr("Please confirm you have STOP the TCP!!\n\nDid you?????\n\n确认退出程序?")),
//                                   QMessageBox::Yes | QMessageBox::No);

//    if (choose== QMessageBox::No)
//     {
//          event->ignore();  //忽略//程序继续运行
//    }
//    else if (choose== QMessageBox::Yes)//    {
//          event->accept();  //介绍//程序退出
//    }
}

/****************************************Tool Function End****************************************************/



//Plot the motor data
void koll_interface_7_30::on_motorcontrol6_6_clicked()
{
    QString File_path = "/home/mfs/1/tra1.txt";
    QString Filed_path = "/home/mfs/1/tra1_d.txt";
    Motor_Data_Process_and_Display(DISPLAY_MODE_POSTION, File_path, Filed_path);
}

void koll_interface_7_30::on_motorcontrol6_9_clicked()
{
    QString File_path = "/home/mfs/1/tra2.txt";
    QString Filed_path = "/home/mfs/1/tra2_d.txt";
    Motor_Data_Process_and_Display(DISPLAY_MODE_POSTION, File_path, Filed_path);
}


void koll_interface_7_30::on_motorcontrol6_7_clicked()
{
    QString File_path = "/home/mfs/1/tra3.txt";
    QString Filed_path = "/home/mfs/1/tra3_d.txt";
    Motor_Data_Process_and_Display(DISPLAY_MODE_POSTION, File_path, Filed_path);
}

void koll_interface_7_30::on_motorcontrol6_8_clicked()
{
    QString File_path = "/home/mfs/1/tra4.txt";
    QString Filed_path = "/home/mfs/1/tra4_d.txt";
    Motor_Data_Process_and_Display(DISPLAY_MODE_POSTION, File_path, Filed_path);
}

void koll_interface_7_30::Motor_Data_Process_and_Display(const int Mode, QString File1, QString File_d)
{
    int i_txt = 0;
    QStringList strlist;
    QStringList strlistd;

    //Data file
    QFile file_data(File1);
    QFile file2_data(File_d);

    if((! file_data.open(QIODevice::ReadOnly|QIODevice::Text)) || (!file2_data.open(QIODevice::ReadOnly|QIODevice::Text)))
        qDebug()<<file_data.errorString()<<"&&&&&"<<file2_data.errorString();
    else
        qDebug()<<"Open the file!";

    file_data.seek(0);
    file2_data.seek(0);

    QTextStream motor_Data(&file_data);
    QTextStream expected_Data(&file2_data);

    QSplineSeries *series1 = new QSplineSeries();
    QSplineSeries *series2 = new QSplineSeries();
    QSplineSeries *series3 = new QSplineSeries();
    QSplineSeries *series4 = new QSplineSeries();
    QSplineSeries *series5 = new QSplineSeries();
    QSplineSeries *series6 = new QSplineSeries();
    series1->setName("Motor1 data");
    series2->setName("Motor2 data");
    series3->setName("Motor3 data");
    series4->setName("Motor4 data");
    series5->setName("Motor5 data");
    series6->setName("Motor6 data");

    QSplineSeries *series1_d = new QSplineSeries();
    QSplineSeries *series2_d = new QSplineSeries();
    QSplineSeries *series3_d = new QSplineSeries();
    QSplineSeries *series4_d = new QSplineSeries();
    QSplineSeries *series5_d = new QSplineSeries();
    QSplineSeries *series6_d = new QSplineSeries();
    series1_d->setName("expected data 1");
    series2_d->setName("expected data 2");
    series3_d->setName("expected data 3");
    series4_d->setName("expected data 4");
    series5_d->setName("expected data 5");
    series6_d->setName("expected data 6");

    QChart *chart1 = new QChart();
    QChart *chart2 = new QChart();
    QChart *chart3 = new QChart();
    QChart *chart4 = new QChart();
    QChart *chart5 = new QChart();
    QChart *chart6 = new QChart();

    //Read file
    while(!motor_Data.atEnd())
    {
        QString line=motor_Data.readLine();

        strlist=line.split(QLatin1Char(','), QString::SkipEmptyParts);
//        qDebug()<<strlist;

        //data mode
        double timee = strlist[0].toDouble();
        double val1 = strlist[1+6*Mode].toDouble();
        double val2 = strlist[2+6*Mode].toDouble();
        double val3 = strlist[3+6*Mode].toDouble();
        double val4 = strlist[4+6*Mode].toDouble();
        double val5 = strlist[5+6*Mode].toDouble();
        double val6 = strlist[6+6*Mode].toDouble();

        series1->append(timee, val1);
        series2->append(timee, val2);
        series3->append(timee, val3);
        series4->append(timee, val4);
        series5->append(timee, val5);
        series6->append(timee, val6);
        i_txt++;
    }
    file_data.close();

    if(!Mode)
    {
        while(!expected_Data.atEnd())
        {
            QString line=expected_Data.readLine();
            //要改逗号
            strlistd=line.split(QLatin1Char(','), QString::SkipEmptyParts);
    //        qDebug()<<strlist;
            double timee = strlistd[0].toDouble();
            double val1 = strlistd[1].toDouble();
            double val2 = strlistd[2].toDouble();
            double val3 = strlistd[3].toDouble();
            double val4 = strlistd[4].toDouble();
            double val5 = strlistd[5].toDouble();
            double val6 = strlistd[6].toDouble();
            series1_d->append(timee, val1);
            series2_d->append(timee, val2);
            series3_d->append(timee, val3);
            series4_d->append(timee, val4);
            series5_d->append(timee, val5);
            series6_d->append(timee, val6);
            i_txt++;
        }
        file2_data.close();

        Display_Data(chart1, DISPLAY_MOTOR1, DISPLAY_MODE_POSTION, series1, series1_d);
        Display_Data(chart2, DISPLAY_MOTOR2, DISPLAY_MODE_POSTION, series2, series2_d);
        Display_Data(chart3, DISPLAY_MOTOR3, DISPLAY_MODE_POSTION, series3, series3_d);
        Display_Data(chart4, DISPLAY_MOTOR4, DISPLAY_MODE_POSTION, series4, series4_d);
        Display_Data(chart5, DISPLAY_MOTOR5, DISPLAY_MODE_POSTION, series5, series5_d);
        Display_Data(chart6, DISPLAY_MOTOR6, DISPLAY_MODE_POSTION, series6, series6_d);
    }
    else
    {
        Display_Data(chart1, DISPLAY_MOTOR1, DISPLAY_MODE_VELOCITY, series1);
        Display_Data(chart2, DISPLAY_MOTOR2, DISPLAY_MODE_VELOCITY, series2);
        Display_Data(chart3, DISPLAY_MOTOR3, DISPLAY_MODE_VELOCITY, series3);
        Display_Data(chart4, DISPLAY_MOTOR4, DISPLAY_MODE_VELOCITY, series4);
        Display_Data(chart5, DISPLAY_MOTOR5, DISPLAY_MODE_VELOCITY, series5);
        Display_Data(chart6, DISPLAY_MOTOR6, DISPLAY_MODE_VELOCITY, series6);
    }

}

void koll_interface_7_30::Display_Data(QChart * chart, int Motor, int Mode, QSplineSeries *series, QSplineSeries *series2)
{
    int monitor_x = 1920;
    int monitor_y = 900;

    int dispaly_x[6] = {0, 1, 2, 0, 1, 2};
    int dispaly_y[6] = {0, 0, 0, 1, 1, 1};
    Qt::WindowFlags m_flags = windowFlags();
    string control_model[3] = {"Angle/°", "angular velocity/°/s", "Torque/N"};
    QString axisX_title = QString::fromStdString(control_model[Mode]);
    if(series2 == NULL)
        chart->addSeries(series);
    else
    {
        chart->addSeries(series);
        chart->addSeries(series2);
    }
    string motor = "Motor "+to_string(Motor+1);
    QString serise_title = QString::fromStdString(motor);
    chart->setTitle(serise_title);
    chart->createDefaultAxes();
    chart->axisY()->setTitleText(axisX_title);
    chart->axisX()->setTitleText("Time/s");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setGeometry(monitor_x/3*dispaly_x[Motor], monitor_x/3*dispaly_y[Motor], monitor_x/3, monitor_y/2);

    //set front
    chartView->setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);

    chartView->show();
}



void koll_interface_7_30::on_motorcontrol6_11_clicked()
{
    QString File_path = "/home/mfs/1/tra1.txt";
    QString Filed_path = "/home/mfs/1/tra1_d.txt";
    Motor_Data_Process_and_Display(DISPLAY_MODE_VELOCITY, File_path, Filed_path);
}

void koll_interface_7_30::on_motorcontrol6_12_clicked()
{
    QString File_path = "/home/mfs/1/tra2.txt";
    QString Filed_path = "/home/mfs/1/tra2_d.txt";
    Motor_Data_Process_and_Display(DISPLAY_MODE_VELOCITY, File_path, Filed_path);
}

void koll_interface_7_30::on_motorcontrol6_14_clicked()
{
    QString File_path = "/home/mfs/1/tra3.txt";
    QString Filed_path = "/home/mfs/1/tra3_d.txt";
    Motor_Data_Process_and_Display(DISPLAY_MODE_VELOCITY, File_path, Filed_path);
}

void koll_interface_7_30::on_motorcontrol6_16_clicked()
{
    QString File_path = "/home/mfs/1/tra4.txt";
    QString Filed_path = "/home/mfs/1/tra4_d.txt";
    Motor_Data_Process_and_Display(DISPLAY_MODE_VELOCITY, File_path, Filed_path);
}

void koll_interface_7_30::on_motorcontrol6_10_clicked()
{
    QString File_path = "/home/mfs/1/tra1.txt";
    QString Filed_path = "/home/mfs/1/tra1_d.txt";
    Motor_Data_Process_and_Display(DISPLAY_MODE_CURRENT, File_path, Filed_path);
}

void koll_interface_7_30::on_motorcontrol6_13_clicked()
{
    QString File_path = "/home/mfs/1/tra2.txt";
    QString Filed_path = "/home/mfs/1/tra2_d.txt";
    Motor_Data_Process_and_Display(DISPLAY_MODE_CURRENT, File_path, Filed_path);
}

void koll_interface_7_30::on_motorcontrol6_15_clicked()
{
    QString File_path = "/home/mfs/1/tra3.txt";
    QString Filed_path = "/home/mfs/1/tra3_d.txt";
    Motor_Data_Process_and_Display(DISPLAY_MODE_CURRENT, File_path, Filed_path);
}

void koll_interface_7_30::on_motorcontrol6_18_clicked()
{
    QString File_path = "/home/mfs/1/tra4.txt";
    QString Filed_path = "/home/mfs/1/tra4_d.txt";
    Motor_Data_Process_and_Display(DISPLAY_MODE_CURRENT, File_path, Filed_path);
}

void koll_interface_7_30::on_pushButton_analysis_1_32_clicked()
{
    char index[31] = Pos_init;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

void koll_interface_7_30::on_pushButton_analysis_1_33_clicked()
{
    char index[31] = Pos_identif;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();

}

void koll_interface_7_30::on_pushButton_analysis_1_16_clicked()
{
    char index[31] = Mark1;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

void koll_interface_7_30::on_pushButton_analysis_1_29_clicked()
{
    char index[31] = Mark2;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

void koll_interface_7_30::on_pushButton_analysis_1_31_clicked()
{
    char index[31] = Mark3;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

void koll_interface_7_30::on_pushButton_analysis_1_30_clicked()
{
    char index[31] = Run_to_Mark1;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

void koll_interface_7_30::on_pushButton_analysis_1_35_clicked()
{
    char index[31] = Run_to_Mark2;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}

void koll_interface_7_30::on_pushButton_analysis_1_34_clicked()
{
    char index[31] = Run_to_Mark3;
    Send_message_processing(index);
    Send_control_message();

    wait_for_receiving();

    Receive_message_processing();
    Receive_message_display();
}
