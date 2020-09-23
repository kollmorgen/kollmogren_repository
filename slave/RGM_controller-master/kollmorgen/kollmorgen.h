#ifndef KOLLMORGEN_H
#define KOLLMORGEN_H

#define SHUTDOWN_RGM 7
#define STOP_RGM 0
#define START_RGM 1

extern int CANopen_status;



int tcp_connect();

int CANopen_connect();

int run_connect();


#endif