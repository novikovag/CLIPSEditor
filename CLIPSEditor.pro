#-------------------------------------------------
#
# Project created by QtCreator 2011-06-29T14:17:21
#
#-------------------------------------------------

QT += core gui widgets help

TARGET = clipseditor

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    highlighter.cpp \
    helpviewer.cpp \
    codeeditor.cpp \
    configdialog.cpp \
    keywords.cpp \
    config.cpp \
    searchbar.cpp \
    sessions.cpp \
    snippets.cpp \
    bookmarks.cpp

HEADERS  += \
    mainwindow.h \
    highlighter.h \
    helpviewer.h \
    codeeditor.h \
    configdialog.h \
    config.h \
    keywords.h \
    searchbar.h \
    sessions.h \
    snippets.h \
    bookmarks.h

RESOURCES += \
    clipseditor.qrc

FORMS += \
    searchbar.ui \
    configdialog.ui \
    sessions.ui \
    snippets.ui \
    bookmarks.ui \
    helpviewer.ui



# #Set Style
# DEFINES += SETSTYLE

# смотреть static.txt
# CONFIG += static upx

win32: {
    RC_ICONS += rc/icon.ico

    static {
        message("*** w32 static")

        DEFINES += W32STATIC
        QT -= help
        INCLUDEPATH += C:/Qt/Qt5.4.1/5.4/Src/qttools/include/ \
                       C:/X-Files/CLIPSEditor/build-qttools-Desktop_Qt_5_4_1_MinGW_32bit_Static-Release/include
        LIBS += -LC:/X-Files/CLIPSEditor/build-qttools-Desktop_Qt_5_4_1_MinGW_32bit_Static-Release/lib -lQt5Help

        upx {
            message("+++ upx")

            PWD_WIN = $${PWD}
            PWD_WIN ~= s,/,\\,g

            OUT_PWD_WIN = $${OUT_PWD}
            OUT_PWD_WIN ~= s,/,\\,g

            QMAKE_POST_LINK = $${PWD_WIN}\\misc\\upx\\upx.exe $${OUT_PWD_WIN}\\release\\$${TARGET}.exe
        }
    } else {
        QT += help
    }
}
