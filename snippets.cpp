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

#include "snippets.h"

Snippets::Snippets(Config *config)
    : config(config)
{
    setupUi(this);

    readPath(QDir().exists(config->snippetPath) ? config->snippetPath : QDir::currentPath());

    connect(btnPath,    SIGNAL(clicked()),                            SLOT(setPath()));
    connect(lstSnippet, SIGNAL(itemDoubleClicked(QListWidgetItem *)), SLOT(insert(QListWidgetItem *)));
    connect(lstSnippet, SIGNAL(itemClicked(QListWidgetItem *)),       SLOT(readFile(QListWidgetItem *)));
}

void Snippets::setPath()
{
    QString name = QFileDialog::getExistingDirectory(this, tr("Snippets Directory"), lblPath->text(), QFileDialog::ShowDirsOnly);

    if (!name.isEmpty())
        readPath(name);
}

void Snippets::readPath(QString name)
{
    lblPath->setText(name);

    lstSnippet->clear();
    tedSnippet->clear();

    QFileInfoList list = QDir(name).entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName(), lstSnippet);
        item->setData(Qt::UserRole, fileInfo.absoluteFilePath());
    }
}

void Snippets::readFile(QListWidgetItem *item)
{
    setCursor(Qt::WaitCursor);

    QFile file(item->data(Qt::UserRole).toString());

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr(PROGNAME), tr("Cannot read file %1: %2").arg(item->data(Qt::UserRole).toString(),file.errorString()));
        snippet.clear();
    } else {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        snippet = in.readAll();
    }

    tedSnippet->setPlainText(snippet);
    setCursor(Qt::ArrowCursor);
}

Snippets::~Snippets()
{
    config->snippetSplitter = splitter->saveState();
    config->snippetPath = lblPath->text();
}
