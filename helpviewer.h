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

#ifndef HELPVIEWER_H
#define HELPVIEWER_H

#include <QtGui>
#include <QtHelp/QtHelp> // !W32STATIC <QtHelp>

#include "ui_helpviewer.h"

#include "config.h"

class HelpViewer : public QWidget, private Ui::HelpViewer
{
    Q_OBJECT

public:
    HelpViewer(Config *);
    void setIndex(const QString &);

private slots:
    void loadSource(const QUrl &);

private:
    void closeEvent(QCloseEvent *) { config->helpWindowGeometry = saveGeometry(); }

    QFile        helpFiles[2]; // qhc, qch

    QHelpEngine *helpEngine;
    Config      *config;
};

#endif // MAINWINDOW_H
