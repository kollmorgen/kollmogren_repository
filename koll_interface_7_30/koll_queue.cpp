#include "koll_queue.h"

void init_s_command(S_Command * kollSstruct)
{
    kollSstruct->c_index = '0';
    strcpy(kollSstruct->k_command, "");
    kollSstruct->motor_PID[0] = 0.0;
    kollSstruct->motor_PID[1] = 0.0;
    kollSstruct->motor_PID[2] = 0.0;
}


void init_r_command(R_Command * kollstruct)
{
    int i=0;
    kollstruct->c_index = '0';
    strcpy(kollstruct->k_command, "");
    strcpy(kollstruct->data_status, "");
    for(i=0; i<6; i++)
    {
        kollstruct->data_cur[i]=0.0;
        kollstruct->data_vel[i]=0.0;
        kollstruct->data_pos[i]=0.0;
        kollstruct->data_fsensor[i]=0.0;
    }
}

int is_s_empty(S_Command *kollstruct)
{
    return kollstruct->c_index - 48;
}

int is_r_empty(R_Command *kollstruct)
{
    return kollstruct->c_index - 48;
}


void printf_s_command(S_Command *com)
{
    //***************************Maybe have problems*********//
    QString k_com = "";
    for(int i=0; com->k_command[i] != '\0'; i++)
    {
         k_com = k_com + com->k_command[i];
    }

    qDebug()<<"\n";
    if(com->c_index == '0')
        qDebug()<<"Empty Queue!!\nNo send command!\n";
    else
    {
        qDebug()<<com->c_index;
        qDebug()<<k_com;
        qDebug()<<com->motor_PID[0]<<", "<<com->motor_PID[1]<<", "<<com->motor_PID[2];
    }
}

void printf_r_command(R_Command *com)
{
    //***************************Maybe have problems*********//
    QString k_com = "";
    for(int i=0; com->k_command[i] != '\0'; i++)
    {
         k_com = k_com + com->k_command[i];
    }

    QString k_datacom = "";
    for(int i=0; com->data_status[i] != '\0'; i++)
    {
         k_com = k_com + com->k_command[i];
    }

    qDebug()<<"\n";
    if(com->c_index == '0')
        qDebug()<<"Empty Queue!!\nNo receive command!\n";
    else
    {
        qDebug()<<com->c_index;
        qDebug()<<k_com;
        qDebug()<<k_datacom;
        qDebug()<<"Joint\tPosition\tVelocity\tCurrent";
        qDebug()<<"Joint1:"<<"\t"<<com->data_pos[0]<<"\t"<<com->data_vel[0]<<"\t"<<com->data_cur[0];
        qDebug()<<"Joint2:"<<"\t"<<com->data_pos[1]<<"\t"<<com->data_vel[1]<<"\t"<<com->data_cur[1];
        qDebug()<<"Joint3:"<<"\t"<<com->data_pos[2]<<"\t"<<com->data_vel[2]<<"\t"<<com->data_cur[2];
        qDebug()<<"Joint4:"<<"\t"<<com->data_pos[3]<<"\t"<<com->data_vel[3]<<"\t"<<com->data_cur[3];
        qDebug()<<"Joint5:"<<"\t"<<com->data_pos[4]<<"\t"<<com->data_vel[4]<<"\t"<<com->data_cur[4];
        qDebug()<<"Joint6:"<<"\t"<<com->data_pos[5]<<"\t"<<com->data_vel[5]<<"\t"<<com->data_cur[5];
    }
}

void copy_r_command(R_Command *new_cmd, R_Command *old_cmd)
{
    new_cmd->c_index = old_cmd->c_index;
    strcpy(new_cmd->k_command, old_cmd->k_command);
    strcpy(new_cmd->data_status, old_cmd->data_status);
    for(int i=0; i<6; i++)
    {
        new_cmd->data_pos[i] = old_cmd->data_pos[i];
        new_cmd->data_vel[i] = old_cmd->data_vel[i];
        new_cmd->data_cur[i] = old_cmd->data_cur[i];
        new_cmd->data_fsensor[i] = old_cmd->data_fsensor[i];
    }

}

void copy_s_command(S_Command *new_cmd, S_Command *old_cmd)
{
    new_cmd->c_index = old_cmd->c_index;
    strcpy(new_cmd->k_command, old_cmd->k_command);
    new_cmd->motor_PID[0] = old_cmd->motor_PID[0];
    new_cmd->motor_PID[1] = old_cmd->motor_PID[1];
    new_cmd->motor_PID[2] = old_cmd->motor_PID[2];
}

