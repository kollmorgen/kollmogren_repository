#include "koll_interface_7_30.h"
#include <QApplication>
#include <Fsensor.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec=QTextCodec::codecForName("utf-8");
        QTextCodec::setCodecForLocale(codec);
//        QTextCodec::setCodecForCStrings(codec);
//        QTextCodec::setCodecForTr(codec);

    koll_interface_7_30 w;
    w.showMaximized();
    return a.exec();
}
