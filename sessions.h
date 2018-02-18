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

#ifndef SESSIONS_H
#define SESSIONS_H

#include <QtWidgets>

#include "ui_sessions.h"

#include "config.h"

class Sessions : public QWidget, private Ui::Sessions
{
    Q_OBJECT

public:
    Sessions(Config *, QTabWidget *);
   ~Sessions();

signals:
    void load(QStringList, bool);

private slots:
    void add();
    void remove();
    void select();
    void update();
    void change(QString) { btnAdd->setEnabled(!ledSession->text().isEmpty()); }

    void rename() { lstSessions->editItem(lstSessions->currentItem()); }

    void clicked(QListWidgetItem* item) { emit load(item->data(Qt::UserRole).toString().split("\n"), chbLeaveTabs->isChecked()); }

    void clicked(const QPoint& point)
    {
        if (lstSessions->itemAt(point))
            menu->exec(mapToGlobal(point));
    }

private:
    QMenu      *menu;
    QTabWidget *tabWidget;
    Config 	   *config;
};

#endif // SESSIONS_H
