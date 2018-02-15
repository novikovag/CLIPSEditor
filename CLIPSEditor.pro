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
