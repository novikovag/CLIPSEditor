/*===========================================================================
    CLIPSEditor, editor for CLIPS (C Language Integrated Production System)
    Copyright (C) 2012-2018 Artem G. Novikov

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
    language = value("language", "English").toString();

    style = value("style").toString();

    if (style.isEmpty()) {
        QFile file(":/rc/style.css");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        style = QLatin1String(file.readAll());
    }

    fKey     = value("fKey", "F5").toString();

    int size = beginReadArray("fKeys");

    for (int i = 0; i < size; i++) {
        setArrayIndex(i);

        fKeys.insert(value("key").toString(), value("command").toString());
    }

    endArray();

    if (!size) {
        fKeys.insert("F5", "");
        fKeys.insert("F6", "");
        fKeys.insert("F7", "");
        fKeys.insert("F8", "");
        fKeys.insert("F9", "");
    }

    recentFiles = value("recentFiles").toStringList();
    openFiles   = value("openFiles").toStringList();

    QMutableStringListIterator itOpenFiles(openFiles);

    while (itOpenFiles.hasNext())
        if (!QFileInfo(itOpenFiles.next().split('#')[0]).exists())
            itOpenFiles.remove();

    lastFile  		   = value("lastFile").toString();

    mainWindowGeometry = value("mainWindowGeometry").toByteArray();
    mainWindowState    = value("mainWindowState").toByteArray();
    helpWindowGeometry = value("helpWindowGeometry").toByteArray();
    endGroup();

    beginGroup("Editor");
    fontFamily   = value("fontFamily", "Courier New").toString();
    fontSize     = value("fontSize", 12).toInt();
    autoIndent   = value("autoIndent").toBool();
    backUnindent = value("backUnindent").toBool();
    spaceTabs    = value("spaceTabs", true).toBool();
    indentSize   = value("indentSize", 4).toInt();
    tabSize      = value("tabSize", 4).toInt();
    whitespaces  = value("whitespaces").toBool();
    verticalEdge = value("verticalEdge", 80).toInt();

    QTextCharFormat format; // формат по умолчанию, не изменяется
    format.setBackground(QColor(255, 255, 255, 0)) ; // прозрачный белый

    size = beginReadArray("ColorScheme");

    for (int i = 0; i < size; i++) {
        setArrayIndex(i);

        QString type = value("type").toString();

        colorScheme.insert(type, format);
        colorScheme[type].setFontItalic(value("italic").toBool());
        colorScheme[type].setFontWeight(value("weihgt").toInt());
        colorScheme[type].setForeground(value("foreground").value<QColor>());
        colorScheme[type].setBackground(value("background").value<QColor>());
    }

    endArray();

    QHashIterator<QString, QString> itKeywords(keywords.keywords);

    while (itKeywords.hasNext()) {
        itKeywords.next();

        QString keyword = itKeywords.value();

        keyword.replace("*", "\\*").replace("$", "\\$");
        patterns.insert(itKeywords.key(), QRegExp(QString("\\b%1\\b").arg(keyword)));

        if (!colorScheme.contains(itKeywords.key())) // для всех слов пустая схема при size == 0
            colorScheme.insert(itKeywords.key(), format);
    }
    // 1. для неключевых слов добавить шаблон
    patterns.insert("Parentheses", 	    QRegExp("[\\(\\)\\[\\]]"));
    patterns.insert("Numbers", 			QRegExp("(^|\\s)[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?"));
    patterns.insert("Local Variables",  QRegExp("\\$?\\?[\\w-]+"));
    patterns.insert("Global Variables", QRegExp("\\?\\*[\\w-]+\\*"));
    // 2. настроить схему по умолчанию
    if (!size) {
        colorScheme.insert("Constructs", format);
        colorScheme["Constructs"].setFontWeight(QFont::Bold);
        colorScheme["Constructs"].setForeground(Qt::darkBlue);

        colorScheme.insert("Strings", format);
        colorScheme["Strings"].setForeground(Qt::darkRed);

        colorScheme.insert("Comments", format);
        colorScheme["Comments"].setFontItalic(true);
        colorScheme["Comments"].setForeground(Qt::darkGreen);

        colorScheme.insert("Parentheses", format);
        colorScheme["Parentheses"].setFontWeight(QFont::Bold);

        colorScheme.insert("Numbers", format);
        colorScheme["Numbers"].setForeground(Qt::darkYellow);

        colorScheme.insert("Local Variables", format);
        colorScheme["Local Variables"].setForeground(Qt::darkYellow);

        colorScheme.insert("Global Variables", format);
        colorScheme["Global Variables"].setForeground(Qt::darkYellow);

        colorScheme.insert("Text", format);
        colorScheme["Text"].setForeground(Qt::black);
        colorScheme["Text"].setBackground(Qt::white);

        colorScheme.insert("Line Numbers", format);
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
    setValue("language", language);

    setValue("style", style);

    setValue("fKey",  fKey);

    QMapIterator<QString, QString> itFKeys(fKeys);

    beginWriteArray("fKeys");

    int i = 0;

    while (itFKeys.hasNext()) {
        itFKeys.next();

        setArrayIndex(i++);
        setValue("key",     itFKeys.key());
        setValue("command", itFKeys.value());
    }

    endArray();

    setValue("recentFiles",        recentFiles);
    setValue("openFiles",          openFiles);
    setValue("lastFile",           lastFile);
    setValue("mainWindowGeometry", mainWindowGeometry);
    setValue("mainWindowState",    mainWindowState);
    setValue("helpWindowGeometry", helpWindowGeometry);

    endGroup(); // General

    beginGroup("Editor");
    setValue("fontFamily",   fontFamily);
    setValue("fontSize",     fontSize);
    setValue("autoIndent",   autoIndent);
    setValue("backUnindent", backUnindent);
    setValue("spaceTabs",    spaceTabs);
    setValue("indentSize",   indentSize);
    setValue("tabSize",      tabSize);
    setValue("whitespaces",  whitespaces);
    setValue("verticalEdge", verticalEdge);

    QMapIterator<QString, QTextCharFormat> itColorScheme(colorScheme);

    beginWriteArray("ColorScheme");

    i = 0;

    while (itColorScheme.hasNext()) {
        itColorScheme.next();

        setArrayIndex(i++);
        setValue("type",       itColorScheme.key());
        setValue("italic",     itColorScheme.value().font().italic());
        setValue("weihgt",     itColorScheme.value().font().weight());
        setValue("foreground", itColorScheme.value().foreground());
        setValue("background", itColorScheme.value().background());
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
