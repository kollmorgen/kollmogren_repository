#ifndef _FSENSOR_H
#define _FSENSOR_H


#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <istream>


#include <QMainWindow>

#include <QDialog>
#include <QString>
#include <QDebug>//调试
#include <QMessageBox> //消息对话框
#include <QWidget>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>


#define FPORT 49152 /* Port the Net F/T always uses */
#define FCOMMAND 2 /* Command code 2 starts streaming */
#define FNUM_SAMPLES 1 /* Will send 1 sample before stopping */
#define FSERVER_ADDR "192.168.1.120"

/* Typedefs used so integer sizes are more explicit */


typedef unsigned int Fuint32;
typedef int Fint32;
typedef unsigned short Fuint16;
typedef short Fint16;
typedef unsigned char Fbyte;
typedef struct response_struct {
	Fuint32 rdt_sequence;
	Fuint32 ft_sequence;
	Fuint32 status;
	Fint32 FTData[6];
} RESPONSE;

int Fsensor_init () ;
float *Fdata_get();
void Fsensor_colse();
int Fsensor_ping ();

extern	char * AXES[];	/* The names of the force and torque axes. */
extern float Fsensor[];


#endif
