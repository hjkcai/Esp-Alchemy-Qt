#include <QtGui>
#include "gamewidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    initializeAllElements();
    initializeAllCombinations();

    GameWidget *mw = new GameWidget();
    mw->show();

    return app.exec();
}
