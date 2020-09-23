/* Simple demo showing how to communicate with Net F/T using C language. */

#include "Fsensor.h"

int socketHandle;			/* Handle to UDP socket used to communicate with Net F/T. */
int nsocketHandle;	
struct sockaddr_in addr;	/* Address of Net F/T. */
struct hostent *he;			/* Host entry for Net F/T. */
Fbyte request[8];			/* The request data sent to the Net F/T. */
RESPONSE resp;				/* The structured response received from the Net F/T. */
Fbyte response[36];			/* The raw response data received from the Net F/T. */
int i;						/* Generic loop/array index. */
int err;					/* Error status of operations. */
char * AXES[] = { "Fx", "Fy", "Fz", "Tx", "Ty", "Tz" };	/* The names of the force and torque axes. */

// int main ( int argc, char ** argv ) {
int Fsensor_init ( ) {

	socketHandle = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketHandle < 0) {
		printf("create socket error\n");  
        return -1;
	}


	*(Fuint16*)&request[0] = htons(0x1234); /* standard header. */
	*(Fuint16*)&request[2] = htons(FCOMMAND); /* per table 9.1 in Net F/T user manual. */
	*(Fuint32*)&request[4] = htonl(FNUM_SAMPLES); /* see section 9.1 in Net F/T user manual. */
	

	addr.sin_addr.s_addr = inet_addr(FSERVER_ADDR);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(FPORT);
	
	err = connect( socketHandle, (struct sockaddr *)&addr, sizeof(addr) );
	if (err <0) {
		printf ("ERROR: Failed to connect to the host:%d!\n",err);
		return -2;
	}
	send( socketHandle, request, 8, 0 );


    // memset(&addr, 0, sizeof(addr));           // server address
    // addr.sin_family = AF_INET;                       // Protocol Family 
    // addr.sin_port = htons(FPORT);                     // Port number 
    // if( inet_pton(AF_INET, FSERVER_ADDR, &addr.sin_addr) <= 0)
    // {
    //     printf("inet_pton error for %s\n",FSERVER_ADDR);
    //     return -2;
    // }
    // // if ( nsocketHandle=connect(socketHandle, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0)
	// if (           err=connect(socketHandle, (struct sockaddr *)&addr, sizeof(addr))<0 );
    // {
    //     // printf ("ERROR: Failed to connect to the host!\n");
    //     printf("connect error: %s(errno: %d)\n",strerror(errno),errno); 
	// 	printf("connect error\n"); 
    //     return -3;
    // }




	send( socketHandle, request, 8, 0 );
    return 0;
}
int Fdata_get()
{
    while(1){
	recv( socketHandle, response, 36, 0 );
	resp.rdt_sequence = ntohl(*(Fuint32*)&response[0]);
	resp.ft_sequence = ntohl(*(Fuint32*)&response[4]);
	resp.status = ntohl(*(Fuint32*)&response[8]);
	for( i = 0; i < 6; i++ ) {
		resp.FTData[i] = ntohl(*(Fint32*)&response[12 + i * 4]);
	}

	/* Output the response data. */
	// printf( "Status: 0x%08x\n", resp.status );
	for (i =0;i < 6;i++) {
		printf("%s: %8.4lf ", AXES[i], (double)resp.FTData[i]/1000000);
	}
	printf("\n");}
	return 1;
}
