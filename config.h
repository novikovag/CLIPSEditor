/*===========================================================================
    CLIPSEditor, editor for CLIPS (C Language Integrated Production System)
    Copyright (C) 2012-2017 Novikov Artem Gennadievich

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
===========================================================================*/

#ifndef CONFIG_H
#define CONFIG_H

#include <QtGui>

#include "keywords.h"

#define ORGNAME  "NovikovAG"

#define PROGNAME "CLIPS Editor"
#define VERSION  "1.2"

class Config : public QSettings
{
    Q_OBJECT

public:
    enum Receiver {
        Init        = 1, // 0000001
        Editor      = 2, // 0000010
        Highlighter = 4  // 0000100
    };

signals:
    void reread(int);

public slots:
    void reconfig(int receiver) { emit reread(receiver); }

public:
    Config();
   ~Config();
    // General
    QString     language;
    QMap<QString, QString> languages;

    QByteArray  mainWindowGeometry;
    QByteArray  mainWindowState;
    QByteArray  helpWindowGeometry;
    QByteArray	sessionSplitter;
    QByteArray  snippetSplitter;

    int         maxRecentFiles;
    QStringList recentFiles;

    QStringList openFiles;
    QString		lastFile;

    QString     snippetPath;

    QMap<QString, QVariant>	sessions;
    bool		leaveOpen;
    // Editor
    QString     fontFamily;
    int         fontSize;

    bool        tabIndents;
    bool        autoIndent;
    bool        backUnindent;
    bool        spaceTabs;

    int         indentSize;
    int         tabSize;

    bool        whitespaces;

    int         verticalEdge;
    // Search
    int         maxHistory;
    QStringList findHistory;
    QStringList replaceHistory;

    bool        matchCase;
    bool        regExp;
    bool        allFiles;
    // Highlighter
    QMap<QString, QTextCharFormat> colorScheme;
    QMultiHash<QString, QRegExp>   patterns;
    // Highlighter, Completer
    Keywords    keywords;
    QStringList keywordsSorted;
};

#endif // CONFIG_H
