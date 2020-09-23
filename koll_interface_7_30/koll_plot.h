#ifndef KOLL_PLOT_H
#define KOLL_PLOT_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QtCharts>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextCodec>
#include <stdio.h>
#include <string>
#include <iostream>
#include <QString>

#include <QMainWindow>
#include <QTimer>
#include <qdatetime.h>
#include <QDialog>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>
#include <QHBoxLayout>


#define DISPLAY_MOTOR1 0
#define DISPLAY_MOTOR2 1
#define DISPLAY_MOTOR3 2
#define DISPLAY_MOTOR4 3
#define DISPLAY_MOTOR5 4
#define DISPLAY_MOTOR6 5

#define DISPLAY_MODE_POSTION 0
#define DISPLAY_MODE_VELOCITY 1
#define DISPLAY_MODE_CURRENT 2


void Display_Data(QChart * chart, int Motor, int Mode, QSplineSeries *series, QSplineSeries *series2 = nullptr);


#endif // MAINWINDOW_H
