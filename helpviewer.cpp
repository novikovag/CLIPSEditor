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

#include "helpviewer.h"

HelpViewer::HelpViewer(Config *config) : config(config)
{
    setupUi(this);

    helpFiles[0].setFileName(QString("%1/clipshelp.qhc").arg(QDir::tempPath()));
    helpFiles[1].setFileName(QString("%1/clipshelp.qch").arg(QDir::tempPath()));
    // при обновлении справки, удалить предыдущие из временной директории,
    // имя файла учитывается в clipshelp.qhcp
    QFile::copy(":/help/clipshelp.qhc", helpFiles[0].fileName());
    QFile::copy(":/help/clipshelp.qch", helpFiles[1].fileName());

    helpEngine = new QHelpEngine(helpFiles[0].fileName());
    helpEngine->setupData();

    lytContent->addWidget(helpEngine->contentWidget());
    lytIndex->addWidget(helpEngine->indexWidget());

    tebHelp->setOpenLinks(false);     // клик обрабатываем сами
    splitter->setStretchFactor(1, 1); // окно просмотра по максимуму

    setWindowFlags(Qt::WindowCloseButtonHint);
    restoreGeometry(config->helpWindowGeometry);
    loadSource(QUrl("qthelp://clips/doc/release_notes.html"));

    connect(helpEngine->contentWidget(), SIGNAL(linkActivated(const QUrl &)),                  SLOT(loadSource(const QUrl &)));
    connect(helpEngine->indexWidget(),   SIGNAL(linkActivated(const QUrl &, const QString &)), SLOT(loadSource(const QUrl &)));
    connect(tebHelp,                     SIGNAL(anchorClicked(QUrl)),                          SLOT(loadSource(const QUrl &)));
}

void HelpViewer::setIndex(const QString &word)
{
    QHelpIndexModel *model = helpEngine->indexModel();

    for (int i = 0; i < model->rowCount() && !word.isEmpty(); i++)
        if (model->stringList().at(i) == word) {
            helpEngine->indexWidget()->scrollTo(model->index(i), QAbstractItemView::PositionAtTop);
            helpEngine->indexWidget()->selectionModel()->setCurrentIndex(model->index(i), QItemSelectionModel::SelectCurrent);
            helpEngine->indexWidget()->activateCurrentItem();
            tbwHelp->setCurrentIndex(1);
            break;
        }
}

void HelpViewer::loadSource(const QUrl &url)
{
    if (url.scheme() == "http") // переход по внешней ссылке
        QDesktopServices::openUrl(url);
    else                        // прямая или косвенная ссылка
        tebHelp->setHtml(helpEngine->fileData(url.scheme() == "qthelp" ? url : "qthelp://clips/doc/" + url.fileName()));
}

