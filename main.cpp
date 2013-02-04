#include <QtGui>
#include "gamewidget.h"
#include "mainwindow.h"

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

//    MainWindow *mw = new MainWindow();
//    InAppDialog *ad = new InAppDialog();
//    mw->resize(550, 400);
//    ad->resize(300,300);
//    mw->showDialog(ad);
//    mw->show();

    return app.exec();
}
