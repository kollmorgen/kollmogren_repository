#ifndef _FSENSOR_H
#define _FSENSOR_H


#ifdef __cpluscplus
extern "C"{
#endif

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "data.h"

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
int Fdata_get();

extern	int socketHandle;			/* Handle to UDP socket used to communicate with Net F/T. */
extern	int nsocketHandle;	
extern	struct sockaddr_in addr;	/* Address of Net F/T. */
extern	struct hostent *he;			/* Host entry for Net F/T. */
extern	Fbyte request[8];			/* The request data sent to the Net F/T. */
extern	RESPONSE resp;				/* The structured response received from the Net F/T. */
extern	Fbyte response[36];			/* The raw response data received from the Net F/T. */
extern	int i;						/* Generic loop/array index. */
extern	int err;					/* Error status of operations. */
extern	char * AXES[];	/* The names of the force and torque axes. */






#ifdef __cpluscplus
}
#endif

#endif