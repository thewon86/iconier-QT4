#include "icondialog.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QTextCodec *gcodec = QTextCodec::codecForName("UTF-8");
//    QTextCodec::setCodecForLocale(gcodec);
//    QTextCodec::setCodecForCStrings(gcodec);
//    QTextCodec::setCodecForTr(gcodec);

    IconDialog w;
    w.show();

    return a.exec();
}
