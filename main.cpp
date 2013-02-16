#include <QtGui>
#include "gamewidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    QTranslator *translator = new QTranslator();
    translator->load(QString(
                #ifndef Q_OS_MAC
                     "%0/res/zh-cn.qm"
                #else
                     "%0/../Resources/zh-cn.qm"
                #endif
                    ).arg(QApplication::applicationDirPath()));
    app.installTranslator(translator);

    initializeAllElements();
    initializeAllCombinations();

    GameWidget *mw = new GameWidget();
    mw->show();

    return app.exec();
}
