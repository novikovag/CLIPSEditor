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

#include "searchbar.h"

SearchBar::SearchBar(Config *config)
    : config(config)
{
    setupUi(this);

    QRegExp rx("^[^\\n]+&"); // толкько одну строку
    QRegExpValidator *v = new QRegExpValidator(rx, this);

    cmbFind->setMaxCount(config->maxHistory);
    cmbFind->addItems(config->findHistory);
    cmbFind->setCurrentIndex(-1);
    cmbFind->setValidator(v);

    cmbReplace->setMaxCount(config->maxHistory);
    cmbReplace->addItems(config->replaceHistory);
    cmbReplace->setCurrentIndex(-1);
    cmbReplace->setValidator(v);

    chbCase->setChecked(config->matchCase);
    chbRegExp->setChecked(config->regExp);
    chbAllFiles->setChecked(config->allFiles);

    setFixedHeight(minimumHeight());

    connect(btnFindNext,    SIGNAL(clicked()), SLOT(clicked()));
    connect(btnFindPrev,    SIGNAL(clicked()), SLOT(clicked()));
    connect(btnReplaceNext, SIGNAL(clicked()), SLOT(clicked()));
    connect(btnReplacePrev, SIGNAL(clicked()), SLOT(clicked()));
    connect(btnReplaceAll,  SIGNAL(clicked()), SLOT(clicked()));
}

void SearchBar::clicked()
{
    if (cmbFind->currentText().isEmpty())
        return;

    update(cmbFind);

    int flags = 0;

    if (chbCase->isChecked())
        flags |= MatchCase;

    if (chbRegExp->isChecked())
        flags |= RegExp;

    if (chbAllFiles->isChecked())
        flags |= AllFiles;

    if (sender() == btnReplaceAll)
        flags |= ReplaceAll;

    if (sender() == btnFindPrev || sender() == btnReplacePrev)
        flags |= Backward;

    if (sender() == btnFindNext || sender() == btnFindPrev) {
        emit searchReplace(cmbFind->currentText(), 0, flags);
    } else if (cmbFind->currentText() != cmbReplace->currentText()) {
        update(cmbReplace);
        emit searchReplace(cmbFind->currentText(), cmbReplace->currentText(), flags);
    }
}

void SearchBar::update(QComboBox *combo)
{
    int index = combo->findText(combo->currentText());

    if (index == -1)
        combo->insertItem(0, combo->currentText());
    else
        combo->setCurrentIndex(index);
}

SearchBar::~SearchBar()
{
    config->matchCase = chbCase->isChecked();
    config->regExp    = chbRegExp->isChecked();
    config->allFiles  = chbAllFiles->isChecked();

    config->findHistory.clear();

    for (int i = 0; i < cmbFind->count(); i++)
        config->findHistory << cmbFind->itemText(i);

    config->replaceHistory.clear();

    for (int i = 0; i < cmbReplace->count(); i++)
        config->replaceHistory << cmbReplace->itemText(i);
}
