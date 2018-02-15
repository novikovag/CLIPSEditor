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

#include "bookmarks.h"

Bookmarks::Bookmarks(QTabWidget *tabWidget)
    : tabWidget(tabWidget)
{
    setupUi(this);

    menu = new QMenu();
    menu->addAction(tr("Rename"), this, SLOT(rename())); // QKeySequence::?
    menu->addAction(tr("Remove"), this, SLOT(remove()));

    // флаги перетаскивания установлены в форме
    // setDropIndicatorShown(true)
    // setDefaultDropAction(Qt::TargetMoveAction)
    // setDragDropMode(QAbstractItemView::InternalMove)

    connect(lstBookmarks, SIGNAL(itemDoubleClicked(QListWidgetItem *)),       SLOT(clicked(QListWidgetItem *)));
    // если установлен флаг setContextMenuPolicy(Qt::CustomContextMenu)
    connect(lstBookmarks, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(clicked(const QPoint &)));
}

void Bookmarks::addBookmark(CodeEditor::Bookmark *mark)
{
    QListWidgetItem *item = new QListWidgetItem(QString("%1: %2 %3").arg(mark->block.blockNumber() + 1).
                                                                     arg(tabWidget->currentWidget()->windowTitle().remove("[*]")).
                                                                     arg(mark->block.text()), lstBookmarks);
    item->setData(Qt::UserRole, QVariant::fromValue(mark));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    map[mark] = item;
}

void Bookmarks::removeBookmark(CodeEditor::Bookmark *mark)
{
   delete map[mark];
   map.remove(mark);
}

void Bookmarks::clicked(QListWidgetItem *item)
{
    for (int i = 0; i < tabWidget->count(); i++) {
        if (tabWidget->widget(i) == DATAMARK->editor) {
            tabWidget->setCurrentIndex(i);

            QTextCursor cursor = DATAMARK->editor->textCursor();
            cursor.setPosition(DATAMARK->block.position());
            DATAMARK->editor->setTextCursor(cursor);
            DATAMARK->editor->centerCursor();
            break;
        }
    }
}
