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

#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QtGui>

#include "ui_searchbar.h"

#include "config.h"

class SearchBar : public QWidget, private Ui::SearchBar
{
    Q_OBJECT

public:
    enum SearchFlag {
        Backward   = 1,
        MatchCase  = 2,
        WholeWords = 4,
        RegExp     = 8,
        AllFiles   = 16,
        ReplaceAll = 32
    };

public:
    SearchBar(Config*);
   ~SearchBar();

    void searchText(QString txt) { cmbFind->setCurrentText(txt); }

signals:
    void searchReplace(QString, QString, int);

private slots:
    void clicked();

private:
    void update(QComboBox *);

    Config *config;
};

#endif // SEARCHBAR_H
