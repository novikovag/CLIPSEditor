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
    menu->addAction(tr("Remove"), this, SLOT(removeBookmark()));

    QStringList labels;
    labels << tr("File") << tr("Line") << tr("Comment");

    tblBookmarks->setHorizontalHeaderLabels(labels);
    tblBookmarks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(tblBookmarks, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(contextMenu(const QPoint &)));
}

void Bookmarks::addBookmark(CodeEditor::Bookmark *mark)
{
    tblBookmarks->insertRow(tblBookmarks->rowCount());

    QTableWidgetItem *item = new QTableWidgetItem(tabWidget->currentWidget()->windowTitle().remove("[*]"));
    item->setFlags(0);
    tblBookmarks->setItem(tblBookmarks->rowCount() - 1, 0, item);

    item = new QTableWidgetItem(QString("%1").arg(mark->block.blockNumber() + 1));
    item->setFlags(0);
    item->setData(Qt::UserRole, QVariant::fromValue(mark));
    // сохраняем только элемент изменяемого номера строки
    map[mark] = item;
    tblBookmarks->setItem(tblBookmarks->rowCount() - 1, 1, item);

    item = new QTableWidgetItem(mark->block.text());
    tblBookmarks->setItem(tblBookmarks->rowCount() - 1, 2, item);
}

void Bookmarks::removeBookmark() {
    int i = tblBookmarks->selectedRanges().at(0).topRow();

    tblBookmarks->item(i, 1)->data(Qt::UserRole).value<CodeEditor::Bookmark*>()->setActive(false);
}

void Bookmarks::removeBookmark(CodeEditor::Bookmark *mark)
{
   tblBookmarks->removeRow(tblBookmarks->row(map[mark]));
   map.remove(mark);
}

void Bookmarks::contextMenu(const QPoint &point)
{
    int i = tblBookmarks->rowAt(point.y());

    if (i != -1) {
        tblBookmarks->selectRow(i);
        menu->exec(tblBookmarks->mapToGlobal(point));
    }
}



