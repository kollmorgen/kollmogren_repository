#!/bin/bash
echo "*******Kollmorgen Robot Setup*******"
ssh root@192.168.1.100 "ls;\
canconfig can0 bitrate 1000000;\
canconfig can0 bitrate start;\
./kollmorgen" 
#exit
#canconfig can0 bitrate 1000000
#canconfig can0 bitrate start
#./kollmorgen
