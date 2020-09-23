# include"koll_unit_conver.h"
#include "TestMaster.h"

/**
 * 注意有三种工作模式
 * 1-将"目标物理值"转化为"目标驱动值":位置\速度\电流 p-m
 * 2-将"实际驱动值"转化为"实际物理值":位置\速度\电流 m-p
*/
int koll_unit_conver(int direction ,int joint)
{
	int ap[7]={0, 0,0   , 262144,0    ,262144,0};//位置-初始偏差(输入单位)
	int ep[7]={0, -27482 , 9105, -5745  ,10763,135,-11998}; //位置-调整偏差(输入单位),

	if (direction==Physical_to_Nominal)/*用于将物理值，转化为标称值，但是并不完成赋值任务*/
	{
        if(joint==0){
            pthread_mutex_lock(&mut_run); 
            TargetPosition[1]=TargetPosition_ph[1]* (65536)/(45)+ap[1]-ep[1];
            TargetPosition[2]=TargetPosition_ph[2]* (65536)/(45)+ap[2]-ep[2];
            TargetPosition[3]=-TargetPosition_ph[3]*(65536)/(45)+ap[3]-ep[3];
            TargetPosition[4]=TargetPosition_ph[4]* (65536)/(45)+ap[4]-ep[4];
            TargetPosition[5]=TargetPosition_ph[5]* (65536)/(45)+ap[5]-ep[5];
            TargetPosition[6]=TargetPosition_ph[6]* (65536)/(45)+ap[6]-ep[6];
    
            TargetVelocity[1]=TargetVelocity_ph[1]* (1847221816/9375);
            TargetVelocity[2]=TargetVelocity_ph[2]* (1847221816/9375);
            TargetVelocity[3]=-TargetVelocity_ph[3]*(1847221816/9375);
            TargetVelocity[4]=TargetVelocity_ph[4]* (1847221816/9375);
            TargetVelocity[5]=TargetVelocity_ph[5]* (1847221816/9375);
            TargetVelocity[6]=TargetVelocity_ph[6]* (1847221816/9375);
            
		    TargetTorque[1]=4*TargetTorque_ph[1]* (819200.0/24846);
		    TargetTorque[2]=4*TargetTorque_ph[2]* (819200.0/24846);
		    TargetTorque[3]=-4*TargetTorque_ph[3]*(819200.0/24846);
		    TargetTorque[4]=4*TargetTorque_ph[4]* (81920.0/1111  );
		    TargetTorque[5]=4*TargetTorque_ph[5]* (81920.0/1111  );
		    TargetTorque[6]=4*TargetTorque_ph[6]* (81920.0/1111  );
            pthread_mutex_unlock(&mut_run);
            }
        else{
            pthread_mutex_lock(&mut_run); 
            switch (joint){
                case 1: TargetPosition[1]= TargetPosition_ph[1]*(65536/45)+ap[1]-ep[1];  TargetVelocity[1]= TargetVelocity_ph[1]*(1847221816/9375);  TargetTorque[1]= 4*TargetTorque_ph[1]*(819200.0/24846);  break;
                case 2: TargetPosition[2]= TargetPosition_ph[2]*(65536/45)+ap[2]-ep[2];  TargetVelocity[2]= TargetVelocity_ph[2]*(1847221816/9375);  TargetTorque[2]= 4*TargetTorque_ph[2]*(819200.0/24846);  break;
                case 3: TargetPosition[3]=-TargetPosition_ph[3]*(65536/45)+ap[3]-ep[3];  TargetVelocity[3]=-TargetVelocity_ph[3]*(1847221816/9375);  TargetTorque[3]=-4*TargetTorque_ph[3]*(819200.0/24846);  break;
                case 4: TargetPosition[4]= TargetPosition_ph[4]*(65536/45)+ap[4]-ep[4];  TargetVelocity[4]= TargetVelocity_ph[4]*(1847221816/9375);  TargetTorque[4]= 4*TargetTorque_ph[4]*(81920.0/1111  );  break;
                case 5: TargetPosition[5]= TargetPosition_ph[5]*(65536/45)+ap[5]-ep[5];  TargetVelocity[5]= TargetVelocity_ph[5]*(1847221816/9375);  TargetTorque[5]= 4*TargetTorque_ph[5]*(81920.0/1111  );  break;
                case 6: TargetPosition[6]= TargetPosition_ph[6]*(65536/45)+ap[6]-ep[6];  TargetVelocity[6]= TargetVelocity_ph[6]*(1847221816/9375);  TargetTorque[6]= 4*TargetTorque_ph[6]*(81920.0/1111  );  break;
                default:break;
                }
            pthread_mutex_unlock(&mut_run);
        }
	}
    else if (direction==Nominal_to_Physical)/*用于提取实际标称值，并将其转化为物理值*/
	{
        if(joint==0){
            EnterMutex();
            pthread_mutex_lock(&mut_run); 

		    ActualPosition_ph[1]=(ActualPosition6+ep[1]-ap[1]) *(45.0)/(65536.0);
		    ActualPosition_ph[2]=(ActualPosition5+ep[2]-ap[2]) *(45.0)/(65536.0);
		    ActualPosition_ph[3]=-(ActualPosition4+ep[3]-ap[3])*(45.0)/(65536.0);
		    ActualPosition_ph[4]=(ActualPosition3+ep[4]-ap[4]) *(45.0)/(65536.0);
		    ActualPosition_ph[5]=(ActualPosition2+ep[5]-ap[5]) *(45.0)/(65536.0);
		    ActualPosition_ph[6]=(ActualPosition1+ep[6]-ap[6]) *(45.0)/(65536.0);

		    ActualVelocity_ph[1]=ActualVelocity6* (9375/1847221816.0);
		    ActualVelocity_ph[2]=ActualVelocity5* (9375/1847221816.0);
		    ActualVelocity_ph[3]=-ActualVelocity4*(9375/1847221816.0);
		    ActualVelocity_ph[4]=ActualVelocity3* (9375/1847221816.0);
		    ActualVelocity_ph[5]=ActualVelocity2* (9375/1847221816.0);
		    ActualVelocity_ph[6]=ActualVelocity1* (9375/1847221816.0);

		    ActualTorque_ph[1]=ActualTorque6* (24846/819200.0);
		    ActualTorque_ph[2]=ActualTorque5* (24846/819200.0);
		    ActualTorque_ph[3]=-ActualTorque4*(24846/819200.0);
		    ActualTorque_ph[4]=ActualTorque3* (1111/81920.0  );
		    ActualTorque_ph[5]=ActualTorque2* (1111/81920.0  );
		    ActualTorque_ph[6]=ActualTorque1* (1111/81920.0  );

            pthread_mutex_unlock(&mut_run);
            LeaveMutex();
        }
        else{
            EnterMutex();
            pthread_mutex_lock(&mut_run); 
            switch (joint){
                case 1: ActualPosition_ph[1]= (ActualPosition6+ep[1]-ap[1])*(45.0/65536.0); ActualVelocity_ph[1]=ActualVelocity6* (9375/1847221816.0); ActualTorque_ph[1]= ActualTorque6*(24846/819200.0);break;
                case 2: ActualPosition_ph[2]= (ActualPosition5+ep[2]-ap[2])*(45.0/65536.0); ActualVelocity_ph[2]=ActualVelocity5* (9375/1847221816.0); ActualTorque_ph[2]= ActualTorque5*(24846/819200.0);break;
                case 3: ActualPosition_ph[3]=-(ActualPosition4+ep[3]-ap[3])*(45.0/65536.0); ActualVelocity_ph[3]=-ActualVelocity4*(9375/1847221816.0); ActualTorque_ph[3]=-ActualTorque4* (24846/819200.0);break;
                case 4: ActualPosition_ph[4]= (ActualPosition3+ep[4]-ap[4])*(45.0/65536.0); ActualVelocity_ph[4]=ActualVelocity3* (9375/1847221816.0); ActualTorque_ph[4]= ActualTorque3*(1111/81920.0  );break;
                case 5: ActualPosition_ph[5]= (ActualPosition2+ep[5]-ap[5])*(45.0/65536.0); ActualVelocity_ph[5]=ActualVelocity2* (9375/1847221816.0); ActualTorque_ph[5]= ActualTorque2*(1111/81920.0  );break;
                case 6: ActualPosition_ph[6]= (ActualPosition1+ep[6]-ap[6])*(45.0/65536.0); ActualVelocity_ph[6]=ActualVelocity1* (9375/1847221816.0); ActualTorque_ph[6]= ActualTorque1*(1111/81920.0  );break;
                default:break;
                }
            pthread_mutex_unlock(&mut_run);
            LeaveMutex();
        }
	}   
}