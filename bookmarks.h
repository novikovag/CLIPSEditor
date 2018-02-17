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

#ifndef BOOKMARKS_H
#define BOOKMARKS_H

#include <QtGui>

#include "ui_bookmarks.h"

#include "codeeditor.h"

class Bookmarks : public QWidget, private Ui::Bookmarks
{
    Q_OBJECT

public:
    Bookmarks(QTabWidget *);

signals:
    void toBookmark(CodeEditor::Bookmark *);

public slots:
    void addBookmark(CodeEditor::Bookmark *);

    void moveBookmark(CodeEditor::Bookmark*  mark, int) { map[mark]->setText(QString("%1:").arg(mark->block.blockNumber() + 1)); }

    void removeBookmark();
    void removeBookmark(CodeEditor::Bookmark *);

    void contextMenu(const QPoint &);
private:
    QMenu      *menu;
    QTabWidget *tabWidget;
    QMap<CodeEditor::Bookmark *, QTableWidgetItem *> map;
};

#endif // BOOKMARKS_H
