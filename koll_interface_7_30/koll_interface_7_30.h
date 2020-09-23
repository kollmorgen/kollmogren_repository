#ifndef koll_interface_7_30_H
#define koll_interface_7_30_H
#include "Fsensor.h"
#include "koll_tcp.h"
#include "koll_queue.h"
#include "koll_plot.h"

#include <QMainWindow>
#include <QTimer>
#include <qdatetime.h>
#include <QDialog>
#include <QString>
#include <QDebug>//调试
#include <QMessageBox> //消息对话框
#include <QWidget>
#include <QAction>
#include <QShortcut>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextCodec>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <string>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QtCharts>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextCodec>


#define DISPLAY_MOTOR1 0
#define DISPLAY_MOTOR2 1
#define DISPLAY_MOTOR3 2
#define DISPLAY_MOTOR4 3
#define DISPLAY_MOTOR5 4
#define DISPLAY_MOTOR6 5

#define DISPLAY_MODE_POSTION 0
#define DISPLAY_MODE_VELOCITY 1
#define DISPLAY_MODE_CURRENT 2

using namespace std;
QT_CHARTS_USE_NAMESPACE


#define DISPLAY 1
#define CLEAR 0

//message define

//motor control
#define Motor_start "1Start"
#define Shutdown "1Shutdown"
#define Stop "1Stop"

//motor test
#define M1_q "2q1"
#define M2_q "2q2"
#define M3_q "2q3"
#define M4_q "2q4"
#define M5_q "2q5"
#define M6_q "2q6"

#define M1_s "2s1"
#define M2_s "2s2"
#define M3_s "2s3"
#define M4_s "2s4"
#define M5_s "2s5"
#define M6_s "2s6"

#define M1_q_inv "2qq1"
#define M2_q_inv "2qq2"
#define M3_q_inv "2qq3"
#define M4_q_inv "2qq4"
#define M5_q_inv "2qq5"
#define M6_q_inv "2qq6"

//Excitation trajectory
#define Tra1 "4run1"
#define Tra2 "4run2"
#define Tra3 "4run3"
#define Tra4 "4run4"

//zero force
#define No_compasenta "5No"
#define Gravity_compasenta "5G"
#define Fri_gra_compasenta "5FG"

//drag and teach
#define Pos_init "6Pinit"
#define Pos_identif "6Pdentif"
#define Mark1 "6Mark1"
#define Mark2 "6Mark2"
#define Mark3 "6Mark3"
#define Run_to_Mark1 "6RunMark1"
#define Run_to_Mark2 "6RunMark2"
#define Run_to_Mark3 "6RunMark3"
//PID
#define motorPID "P_"

extern char SendMessage[1024];
extern char ReciveMessage[1024];

//S_Command *send_cmd = (S_Command *)malloc(sizeof (S_Command));
//R_Command *rece_cmd = (R_Command *)malloc(sizeof (R_Command));

namespace Ui {
class koll_interface_7_30;
}

class koll_interface_7_30 : public QMainWindow
{
    Q_OBJECT

public:
    explicit koll_interface_7_30(QWidget *parent = 0);
    ~koll_interface_7_30();
    RESPONSE resp;
    QString motor_state;
    QString titlebutton_state;
    QString titlebutton_state_old;
    QTimer *timer_force;
    QTimer *timer_data;
    QShortcut *shutdown_shortcut;
    float Fsensor[6];

    void closeEvent(QCloseEvent *event);



private slots:


    void timerUpdate();

    void timerUpdate_data();

    void Force_show();

    void timerUpdate_title();

    void on_pushButton_init_clicked();

    void on_pushButton_home_clicked();

    void on_pushButton_diagnosis_clicked();

    void on_pushButton_prepare_clicked();

    void on_pushButton_operation_clicked();

    void on_pushButton_analysis_clicked();

    void on_pushButton_monitor_clicked();

    void monitor_lamp(QString mlampcolor);

    void build_communication(QString build);

    void return_title_button(QString old);

    
    void on_motorcontrol4_14_clicked();

    void on_motorcontrol4_13_clicked();

    void on_groupBox_17_clicked();

    void on_motorcontrol4_28_clicked();

    void on_motorcontrol4_26_clicked();


    void on_motorcontrol4_6_clicked();

    void on_pushButton_analysis_1_28_clicked();

    void on_pushButton_analysis_1_25_clicked();

    void on_pushButton_analysis_1_18_clicked();


    void on_motorcontrol4_6_clicked(bool checked);

    void on_pushButton_analysis_1_15_pressed();

    void on_motorcontrol4_15_clicked();

    void on_motorcontrol4_16_clicked();

    void Send_message_processing(char * motor_cmd);

    void Send_control_message(void);

    void Receive_message_processing(void);

    void Receive_message_display(void);

    void Dispalay_motor_data(bool isdisplay);

    void on_pushButton_analysis_1_14_clicked();

    char* motor_mode_control_string(int mode, int motor, string control_num);

    void wait_for_receiving(void);

    void shutdown_now(void);

    void Motor_Data_Process_and_Display(const int Mode, QString File1, QString File_d);

    void Display_Data(QChart * chart, int Motor, int Mode, QSplineSeries *series, QSplineSeries *series2 = nullptr);

    void on_pushButton_analysis_1_15_released();

    void on_pushButton_analysis_1_17_pressed();

    void on_pushButton_analysis_1_17_released();

    void on_pushButton_analysis_1_19_pressed();

    void on_pushButton_analysis_1_19_released();

    void on_pushButton_analysis_1_24_released();

    void on_pushButton_analysis_1_24_pressed();

    void on_pushButton_analysis_1_23_pressed();

    void on_pushButton_analysis_1_23_released();

    void on_pushButton_analysis_1_26_pressed();

    void on_pushButton_analysis_1_26_released();

    void on_pushButton_analysis_1_20_pressed();

    void on_pushButton_analysis_1_20_released();

    void on_pushButton_analysis_1_51_pressed();

    void on_pushButton_analysis_1_51_released();

    void on_pushButton_analysis_1_21_pressed();

    void on_pushButton_analysis_1_21_released();

    void on_pushButton_analysis_1_50_pressed();

    void on_pushButton_analysis_1_50_released();

    void on_pushButton_analysis_1_22_pressed();

    void on_pushButton_analysis_1_22_released();

    void on_pushButton_analysis_1_27_pressed();

    void on_pushButton_analysis_1_27_released();

    void on_motorcontrol2_clicked();

    void on_motorcontrol3_clicked();

    void on_motorcontrol4_2_clicked();

    void on_motorcontrol5_clicked();

    void on_motorcontrol6_2_clicked();

    void on_motorcontrol6_4_clicked();

    void on_motorcontrol6_5_clicked();

    void on_motorcontrol6_17_clicked();

    void on_motorcontrol6_21_clicked();

    void on_motorcontrol6_32_clicked();

    void on_motorcontrol6_33_clicked();

    void on_motorcontrol6_34_clicked();

    void on_motorcontrol4_4_clicked();

    void on_motorcontrol4_7_clicked();

    void on_pushButton_pid_clicked();





    void on_motorcontrol6_6_clicked();

    void on_motorcontrol6_9_clicked();

    void on_motorcontrol6_7_clicked();

    void on_motorcontrol6_8_clicked();

    void on_motorcontrol6_11_clicked();

    void on_motorcontrol6_12_clicked();

    void on_motorcontrol6_14_clicked();

    void on_motorcontrol6_16_clicked();

    void on_motorcontrol6_10_clicked();

    void on_motorcontrol6_13_clicked();

    void on_motorcontrol6_15_clicked();

    void on_motorcontrol6_18_clicked();

    void on_pushButton_analysis_1_32_clicked();

    void on_pushButton_analysis_1_33_clicked();

    void on_pushButton_analysis_1_16_clicked();

    void on_pushButton_analysis_1_29_clicked();

    void on_pushButton_analysis_1_31_clicked();

    void on_pushButton_analysis_1_30_clicked();

    void on_pushButton_analysis_1_35_clicked();

    void on_pushButton_analysis_1_34_clicked();

private:
    Ui::koll_interface_7_30 *ui;

};

#endif // koll_interface_7_30_H
