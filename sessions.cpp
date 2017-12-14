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

#include "sessions.h"

Sessions::Sessions(Config *config, QTabWidget *tabWidget)
    : config(config), tabWidget(tabWidget)
{
    setupUi(this);

    QMapIterator<QString, QVariant> it(config->sessions);

    while (it.hasNext()) {
        it.next();

        QListWidgetItem *item = new QListWidgetItem(it.key(), lstSessions);
        item->setData(Qt::UserRole, it.value());
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }

    menu = new QMenu();
    menu->addAction(tr("Rename"), this, SLOT(rename()));
    menu->addAction(tr("Update"), this, SLOT(update()));
    menu->addAction(tr("Remove"), this, SLOT(remove()));

    connect(btnAdd,      SIGNAL(clicked()),                                  SLOT(add()));
    connect(lstSessions, SIGNAL(itemSelectionChanged()),                     SLOT(select()));
    connect(lstSessions, SIGNAL(itemDoubleClicked(QListWidgetItem *)),       SLOT(clicked(QListWidgetItem *)));
    connect(lstSessions, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(clicked(const QPoint &)));
}

void Sessions::add()
{
    if (!ledSession->text().isEmpty() && lstSessions->findItems(ledSession->text(), Qt::MatchExactly).empty()) {
        QString txt;

        for (int i = 0; i < tabWidget->count(); i++)
            if (!tabWidget->widget(i)->windowFilePath().isEmpty()) {
                if (i >= 1)
                    txt += "\n";

                txt += tabWidget->widget(i)->windowFilePath();
            }

        if (!txt.isEmpty()) {
            QListWidgetItem *item = new QListWidgetItem(ledSession->text(), lstSessions);
            item->setData(Qt::UserRole, txt);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            ledSession->clear();
        }
    }
}

void Sessions::remove()
{
    QListIterator<QListWidgetItem *> it(lstSessions->selectedItems());

    while (it.hasNext())
        delete it.next();
}

void Sessions::select()
{
    tedSession->clear();

    if (lstSessions->currentItem())
        tedSession->insertPlainText(lstSessions->currentItem()->data(Qt::UserRole).toString());
}

void Sessions::update()
{
    QString txt;

    for (int i = 0; i < tabWidget->count(); i++)
        if (!tabWidget->widget(i)->windowFilePath().isEmpty()) {
            if (i >= 1)
                txt += "\n";

            txt += tabWidget->widget(i)->windowFilePath();
        }

    if (!txt.isEmpty()) {
        lstSessions->currentItem()->setData(Qt::UserRole, txt);
        select();
    }
}

Sessions::~Sessions()
{
    config->sessions.clear();

    for (int i = 0; i < lstSessions->count(); i++)
        config->sessions[lstSessions->item(i)->text()] = lstSessions->item(i)->data(Qt::UserRole);

    config->sessionSplitter = splitter->saveState();
}
