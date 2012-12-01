#include <QtGui>
#include "MainWindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    initializeAllElements();
    initializeAllCombinations();

    MainWindow *mw = new MainWindow();
    mw->show();

    return app.exec();
}
