SOURCES += \
    main.cpp \
    element.cpp \
    combination.cpp \
    textItem.cpp \
    rectangleItem.cpp \
    imageItem.cpp \
    elementItem.cpp \
    graphics.cpp \
    des.cpp \
    gamewidget.cpp \
    scrollbar.cpp \
    elementdetaildialog.cpp \
    global.cpp \
    introdialog.cpp

RESOURCES += \
    res.qrc

HEADERS += \
    element.h \
    combination.h \
    textItem.h \
    rectangleItem.h \
    imageItem.h \
    elementItem.h \
    graphics.h \
    des.h \
    gamewidget.h \
    scrollbar.h \
    elementdetaildialog.h \
    global.h \
    introdialog.h

QT += opengl

TRANSLATIONS += Alchemy.ts

win32 {
    RC_FILE = Alchemy.rc
}

mac {
    ICON = icon.icns
    QMAKE_INFO_PLIST = Info.plist
}
