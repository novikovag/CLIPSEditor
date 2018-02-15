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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QtGui>

#include "ui_configdialog.h"

#include "config.h"
#include "codeeditor.h"

#define STYLE(s) QString("border: 1px solid black; background: %1").arg(s.name())

class ConfigDialog : public QDialog, private Ui::ConfigDialog
{
    Q_OBJECT

public:
    ConfigDialog(Config *);

private slots:
    void setFont();
    void setScheme(QListWidgetItem *);
    void setColor();
    void applay();

private:
     Config* config;
};

#endif // CONFIGDIALOG_H
