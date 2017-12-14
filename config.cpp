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

#include "config.h"

Config::Config()
    : QSettings(ORGNAME, PROGNAME)
{
    keywordsSorted = keywords.keywords.values();
    keywordsSorted.sort();

    languages.insert("English", "en_EN");
    languages.insert("Russian", "ru_RU");

    maxRecentFiles = 9;
    maxHistory 	   = 10;

    beginGroup("General");
    language    	   = value("language", "English").toString();
    recentFiles 	   = value("recentFiles").toStringList();

    openFiles   	   = value("openFiles").toStringList();

    QMutableStringListIterator si(openFiles);

    while (si.hasNext())
        if (!QFileInfo(si.next().split('#')[0]).exists())
            si.remove();

    lastFile  		   = value("lastFile").toString();

    mainWindowGeometry = value("mainWindowGeometry").toByteArray();
    mainWindowState    = value("mainWindowState").toByteArray();
    helpWindowGeometry = value("helpWindowGeometry").toByteArray();
    endGroup();

    beginGroup("Editor");
    fontFamily   = value("fontFamily", "Courier New").toString();
    fontSize     = value("fontSize", 12).toInt();
    tabIndents   = value("tabIndents").toBool();
    autoIndent   = value("autoIndent").toBool();
    backUnindent = value("backUnindent").toBool();
    spaceTabs    = value("spaceTabs", true).toBool();
    indentSize   = value("indentSize", 4).toInt();
    tabSize      = value("tabSize", 4).toInt();
    whitespaces  = value("whitespaces").toBool();
    verticalEdge = value("verticalEdge", 80).toInt();

    QTextCharFormat fmt; // формат по умолчанию, не изменяется
    fmt.setBackground(QColor(255, 255, 255, 0)) ; // прозрачный белый

    int size = beginReadArray("ColorScheme");

    for (int i = 0; i < size; i++) {
        setArrayIndex(i);

        QString type = value("type").toString();

        colorScheme.insert(type, fmt);
        colorScheme[type].setFontItalic(value("italic").toBool());
        colorScheme[type].setFontWeight(value("weihgt").toInt());
        colorScheme[type].setForeground(value("foreground").value<QColor>());
        colorScheme[type].setBackground(value("background").value<QColor>());
    }

    endArray();

    QHashIterator<QString, QString> hi(keywords.keywords);

    while (hi.hasNext()) {
        hi.next();

        QString keyword = hi.value();

        keyword.replace("*", "\\*").replace("$", "\\$");
        patterns.insert(hi.key(), QRegExp(QString("\\b%1\\b").arg(keyword)));

        if (!colorScheme.contains(hi.key())) // для всех слов пустая схема при size == 0
            colorScheme.insert(hi.key(), fmt);
    }
    // 1. для неключевых слов добавить шаблон
    patterns.insert("Parentheses", 	    QRegExp("[\\(\\)\\[\\]]"));
    patterns.insert("Numbers", 			QRegExp("(^|\\s)[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?"));
    patterns.insert("Local Variables",  QRegExp("\\$?\\?[\\w-]+"));
    patterns.insert("Global Variables", QRegExp("\\?\\*[\\w-]+\\*"));
    // 2. настроить схему по умолчанию
    if (!size) {
        colorScheme.insert("Constructs", fmt);
        colorScheme["Constructs"].setFontWeight(QFont::Bold);
        colorScheme["Constructs"].setForeground(Qt::darkBlue);

        colorScheme.insert("Strings", fmt);
        colorScheme["Strings"].setForeground(Qt::darkRed);

        colorScheme.insert("Comments", fmt);
        colorScheme["Comments"].setFontItalic(true);
        colorScheme["Comments"].setForeground(Qt::darkGreen);

        colorScheme.insert("Parentheses", fmt);
        colorScheme["Parentheses"].setFontWeight(QFont::Bold);

        colorScheme.insert("Numbers", fmt);
        colorScheme["Numbers"].setForeground(Qt::darkYellow);

        colorScheme.insert("Local Variables", fmt);
        colorScheme["Local Variables"].setForeground(Qt::darkYellow);

        colorScheme.insert("Global Variables", fmt);
        colorScheme["Global Variables"].setForeground(Qt::darkYellow);

        colorScheme.insert("Text", fmt);
        colorScheme["Text"].setForeground(Qt::black);
        colorScheme["Text"].setBackground(Qt::white);

        colorScheme.insert("Line Numbers", fmt);
        colorScheme["Line Numbers"].setForeground(QColor(170, 170, 255));
        colorScheme["Line Numbers"].setBackground(QColor( 75,  75, 112));
    }

    endGroup(); // Editor

    beginGroup("Sessions");
    leaveOpen       = value("leaveOpen").toBool();
    sessions 	    = value("sessions").toMap();
    sessionSplitter	= value("sessionSplitter").toByteArray();
    endGroup();

    beginGroup("Snippets");
    snippetPath		= value("snippetPath").toString();
    snippetSplitter = value("snippetSplitter").toByteArray();
    endGroup();

    beginGroup("SearchReplace");
    findHistory     = value("findHistory").toStringList();
    replaceHistory  = value("replaceHistory").toStringList();
    matchCase       = value("matchCase").toBool();
    regExp          = value("regExp").toBool();
    allFiles        = value("allFiles").toBool();
    endGroup();
}

Config::~Config()
{
    beginGroup("General");
    setValue("language",           language);
    setValue("recentFiles",        recentFiles);
    setValue("openFiles",          openFiles);
    setValue("lastFile",           lastFile);
    setValue("mainWindowGeometry", mainWindowGeometry);
    setValue("mainWindowState",    mainWindowState);
    setValue("helpWindowGeometry", helpWindowGeometry);
    endGroup();

    beginGroup("Editor");
    setValue("fontFamily",   fontFamily);
    setValue("fontSize",     fontSize);
    setValue("tabIndents",   tabIndents);
    setValue("autoIndent",   autoIndent);
    setValue("backUnindent", backUnindent);
    setValue("spaceTabs",    spaceTabs);
    setValue("indentSize",   indentSize);
    setValue("tabSize",      tabSize);
    setValue("whitespaces",  whitespaces);
    setValue("verticalEdge", verticalEdge);

    QMapIterator<QString, QTextCharFormat> it(colorScheme);

    beginWriteArray("ColorScheme");

    int i = 0;

    while (it.hasNext()) {
        it.next();

        setArrayIndex(i++);
        setValue("type",       it.key());
        setValue("italic",     it.value().font().italic());
        setValue("weihgt",     it.value().font().weight());
        setValue("foreground", it.value().foreground());
        setValue("background", it.value().background());
    }

    endArray();

    endGroup(); // Editor

    beginGroup("Sessions");
    setValue("leaveOpen", 	    leaveOpen);
    setValue("sessions", 		sessions);
    setValue("sessionSplitter", sessionSplitter);
    endGroup();

    beginGroup("Snippets");
    setValue("snippetPath",     snippetPath);
    setValue("snippetSplitter", snippetSplitter);
    endGroup();

    beginGroup("SearchReplace");
    findHistory.sort();
    replaceHistory.sort();
    setValue("findHistory",     findHistory);
    setValue("replaceHistory",  replaceHistory);
    setValue("matchCase", 	    matchCase);
    setValue("regExp",    	    regExp);
    setValue("allFiles",  	    allFiles);
    endGroup();
}
