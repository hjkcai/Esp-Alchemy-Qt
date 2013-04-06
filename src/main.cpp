#include <QtGui>
#include "achievement.h"
#include "gamewidget.h"
#include "global.h"

void initializeGlobalStrings();

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    initializeGlobalStrings();

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    QTranslator *translator = new QTranslator();
    if (!translator->load(ResourcesDir.arg("zh-cn.qm")))
    {
        QMessageBox::critical(NULL, QObject::tr("警告"), QObject::tr("找不到翻译文件！请确定游戏文件完整！"));
        return -1;
    }
    app.installTranslator(translator);

    if (!initializeAllElements())
    {
        QMessageBox::critical(NULL, QObject::tr("Fatal Error!"), QObject::tr("Can not read source data file! Alchemy will now exit."));
        return -1;
    }

    initializeAllCombinations();

    GameWidget *mw = new GameWidget();
    mw->show();

    return app.exec();
}

void initializeGlobalStrings()
{
    ResourcesDir = QString(
#ifndef Q_OS_MAC
                           "%0/res/%1"
#else
                           "%0/../Resources/%1"
#endif
                           ).arg(QApplication::applicationDirPath());

    GameSaveDirParent = QString(
#ifndef Q_OS_MAC
                           "%0/"
#else
                           "%0/../Resources/"
#endif
                           ).arg(QApplication::applicationDirPath());

    GameSaveFileName = QString("%0saves/%1.sav").arg(GameSaveDirParent);
}
