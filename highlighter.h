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

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QtGui>

#include "config.h"
#include "codeeditor.h"

class CodeEditor;

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(Config *, CodeEditor *);

    void highlightBlock(const QString &);

private slots:
    void matchParentheses();

private:
    bool goodPosition(int); // позиция вне строки или комментария

    QTextCharFormat errorFmt;
    QTextCharFormat matchFmt;
    QTextCharFormat mismatchFmt;

    Config         *config;
    CodeEditor     *codeEditor;
};

#endif // HIGHLIGHTER_H
