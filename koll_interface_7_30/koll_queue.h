#ifndef KOLL_QUEUE_H
#define KOLL_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>

#include <QMainWindow>
#include <QDialog>
#include <QString>
#include <QDebug>//调试
#include <QMessageBox> //消息对话框
#include <QWidget>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#define send_struct 0;
#define receive_struct 1;

typedef struct Koll_Receive_Command_struct {
    char c_index;                      //Command index
    char k_command[30];
    char data_status[20];
    float data_pos[6];
    float data_vel[6];
    float data_cur[6];
    float data_fsensor[6];
} R_Command ;


typedef struct Koll_Send_Command_struct {
    char c_index;                      //Command index
    char k_command[30];
    float motor_PID[3];
} S_Command;



void init_s_command(S_Command * kollstruct);
void init_r_command(R_Command * kollstruct);
int is_s_empty(S_Command *kollstruct);
int is_r_empty(R_Command *kollstruct);
void printf_s_command(S_Command *com);
void printf_r_command(R_Command *com);
void copy_s_command(S_Command *new_cmd, S_Command *old_cmd);
void copy_r_command(R_Command *new_cmd, R_Command *old_cmd);
#endif // KOLL_QUEUE_H
