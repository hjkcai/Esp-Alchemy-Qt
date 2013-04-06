SOURCES += \
    src/main.cpp \
    src/element.cpp \
    src/combination.cpp \
    src/textItem.cpp \
    src/rectangleItem.cpp \
    src/imageItem.cpp \
    src/elementItem.cpp \
    src/graphics.cpp \
    src/gamewidget.cpp \
    src/scrollbar.cpp \
    src/elementdetaildialog.cpp \
    src/global.cpp \
    src/introdialog.cpp \
    src/achievement.cpp \
    src/configs.cpp \
    src/notification.cpp

RESOURCES +=

HEADERS += \
    src/element.h \
    src/combination.h \
    src/textItem.h \
    src/rectangleItem.h \
    src/imageItem.h \
    src/elementItem.h \
    src/graphics.h \
    src/gamewidget.h \
    src/scrollbar.h \
    src/elementdetaildialog.h \
    src/global.h \
    src/introdialog.h \
    src/achievement.h \
    src/configs.h \
    src/notification.h

QT += opengl

TRANSLATIONS += sres/Alchemy.ts

win32 {
    RC_FILE = sres/Alchemy.rc
}

mac {
    ICON = sres/icon.icns
    QMAKE_INFO_PLIST = sres/Info.plist
}
