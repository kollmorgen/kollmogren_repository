#include "koll.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QTextCodec *codec=QTextCodec::codecForName("utf-8");
//    QTextCodec::setCodecForLocale(codec);
    koll w;

    w.showMaximized();
    return a.exec();
}
