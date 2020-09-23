#!/bin/bash
echo "*******Kollmorgen Robot Init+Run*******"
canconfig can0 bitrate 1000000
canconfig can0 bitrate start
./kollmorgen
