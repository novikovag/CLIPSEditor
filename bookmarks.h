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

#define DATAMARK item->data(Qt::UserRole).value<CodeEditor::Bookmark*>()

class Bookmarks : public QWidget, private Ui::Bookmarks
{
    Q_OBJECT

public:
    Bookmarks(QTabWidget *);

signals:
    void toBookmark(CodeEditor::Bookmark *);

public slots:
    void addBookmark(CodeEditor::Bookmark *);

    void moveBookmark(CodeEditor::Bookmark*  mark, int)
    {
        // при переименование может теряться маркер '^\\d+'
        map[mark]->setText(map[mark]->text().replace(QRegExp("^\\d+:"), QString("%1:").arg(mark->block.blockNumber() + 1)));
    }

    void removeBookmark(CodeEditor::Bookmark *);

    void clicked(QListWidgetItem *);

    void clicked(const QPoint &point)
    {
        if (lstBookmarks->itemAt(point))
            menu->exec(mapToGlobal(point));
    }

    void rename() { lstBookmarks->editItem(lstBookmarks->currentItem()); }

    void remove() { lstBookmarks->currentItem()->data(Qt::UserRole).value<CodeEditor::Bookmark*>()->setActive(false); }

private:
    QMenu      *menu;
    QTabWidget *tabWidget;
    QMap<CodeEditor::Bookmark *, QListWidgetItem *> map;
};

#endif // BOOKMARKS_H
