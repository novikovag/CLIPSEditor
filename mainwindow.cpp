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

#include "mainwindow.h"

MainWindow::MainWindow(Config *config)
    : config(config)
{
    fileNum = 0;

    tabWidget = new QTabWidget(this);
    tabWidget->setMovable(true);
    tabWidget->setContextMenuPolicy(Qt::CustomContextMenu); // прехват правой кнопки

    sessions     = new Sessions(config, tabWidget);
    snippets     = new Snippets(config);
    searchBar    = new SearchBar(config);   
    helpViewer   = new HelpViewer(config);
    configDialog = new ConfigDialog(config);
    bookmarks    = new Bookmarks(tabWidget);

    status = new QLabel();
    status->setIndent(50);
    statusBar()->addWidget(status, 1);
    // индексы "Save"[2] и Separator[18] определены дефайнами
    fileMenu    = menuBar()->addMenu(tr("&File"));
    QMenu *menu = fileMenu;
    connect(menu, SIGNAL(aboutToShow()),    this, SLOT(updateRecentFiles()));
    menu->addAction(tr("New"),              this, SLOT(newFile()),         QKeySequence::New);
    menu->addAction(tr("Open..."),          this, SLOT(openFile()),        QKeySequence::Open);
    // [2]
    menu->addAction(tr("Save"),             this, SLOT(saveFile()), QKeySequence::Save);
    menu->addAction(tr("Close"),            this, SLOT(closeFile()), QKeySequence::Close);
    menu->addAction(tr("Save As..."),       this, SLOT(saveFileAs()),    Qt::CTRL + Qt::ALT + Qt::Key_S);
    menu->addAction(tr("Save All"),         this, SLOT(saveAllFiles()),  Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    menu->addAction(tr("Close All"),        this, SLOT(closeAllFiles()), Qt::CTRL + Qt::SHIFT + Qt::Key_C);
    menu->addSeparator();
    QAction *action = menu->addAction(tr("Sessions"), this, SLOT(showDock()), Qt::CTRL + Qt::Key_W);
    addDock(sessions, action, tr("Sessions"),   Qt::LeftDockWidgetArea, Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    action = menu->addAction(tr("Snippets"),    this, SLOT(showDock()), Qt::CTRL + Qt::Key_P);
    addDock(snippets, action, tr("Snippets"),   Qt::LeftDockWidgetArea, Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    action = menu->addAction(tr("Bookmarks"),   this, SLOT(showDock()), Qt::CTRL + Qt::Key_B);
    addDock(bookmarks, action, tr("Bookmarks"), Qt::LeftDockWidgetArea, Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    menu->addSeparator();
    action = menu->addAction(tr("Search && Replace"),  this, SLOT(showDock()), QKeySequence::Find);
    addDock(searchBar, action, tr("Search & Replace"), Qt::BottomDockWidgetArea, Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    menu->addSeparator();
    // [18]
    menu->addSeparator();

    for (int i = 0; i < config->maxRecentFiles; i++) {
        QAction *action = menu->addAction("", this, SLOT(openRecentFile()));
        action->setVisible(false);
        recentFileActs << action;
    }

    menu->addSeparator();
    menu->addAction(tr("Quit"),           this, SLOT(close()), Qt::CTRL + Qt::Key_Q);
    menu = menuBar()->addMenu(tr("&Settings"));
    menu->addAction(tr("Preferences..."), this, SLOT(preferences()));
    menu = menuBar()->addMenu(tr("&Help"));
    menu->addAction(tr("CLIPS Help"),     this, SLOT(help()),  QKeySequence::HelpContents);
    menu->addSeparator();
    menu->addAction(tr("About..."),       this, SLOT(about()));
#ifdef SETSTYLE
    menu->addAction(tr("#Set Style..."),    this, SLOT(setStyle()));
#endif
    // все индексы кроме "Close" и "Save As..." определены дефайнами
    tabMenu = new QMenu(this);
    tabMenu->addAction(tr("Close"),                  this, SLOT(closeFile()));
    tabMenu->addAction(tr("Close All BUT This"),     this, SLOT(closeOtherFiles()));
    tabMenu->addAction(tr("Close All to the Left"),  this, SLOT(closeLeftFiles()));
    tabMenu->addAction(tr("Close All to the Right"), this, SLOT(closeRightFiles()));
    tabMenu->addAction(tr("Save"),                   this, SLOT(saveFile()));
    tabMenu->addAction(tr("Save As..."),             this, SLOT(saveFileAs()));
    tabMenu->addAction(tr("Rename"),                 this, SLOT(renameFile()));

    setCentralWidget(tabWidget);
    setWindowTitle(PROGNAME);

    restoreGeometry(config->mainWindowGeometry);
    restoreState(config->mainWindowState);

    openFiles(config->openFiles);

    int i = config->openFiles.indexOf(config->lastFile);

    if (tabWidget->count() > 0) {
        if (i == -1)
            i = tabWidget->count() - 1;

        tabWidget->setCurrentIndex(i);
        CURRENT->setFocus();
    }

    currentChanged(i);

    connect(tabWidget, SIGNAL(currentChanged(int)),                        SLOT(currentChanged(int)));
    connect(tabWidget, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(tabContextMenu(const QPoint &)));
    connect(sessions,  SIGNAL(load(QStringList, bool)),                    SLOT(loadSession(QStringList, bool)));
    connect(snippets,  SIGNAL(insert(QString)),                            SLOT(insertSnippet(QString)));
    connect(searchBar, SIGNAL(searchReplace(QString, QString, int)),       SLOT(searchReplace(QString, QString, int)));

    tabWidget->tabBar()->setAccessibleName("one"); 
}

void MainWindow::newFile()
{
    CodeEditor *codeEditor = new CodeEditor(config);

    connect(codeEditor, SIGNAL(modificationChanged(bool)), SLOT(modificationChanged(bool)), Qt::UniqueConnection);
    connect(codeEditor, SIGNAL(cursorPositionChanged()),   SLOT(cursorPositionChanged()),   Qt::UniqueConnection);
    connect(codeEditor, SIGNAL(dropUrls(QList<QUrl>)),     SLOT(dropUrls(QList<QUrl>)), Qt::UniqueConnection);

    connect(codeEditor, SIGNAL(addBookmark(CodeEditor::Bookmark *)),       bookmarks, SLOT(addBookmark(CodeEditor::Bookmark *)),       Qt::UniqueConnection);
    connect(codeEditor, SIGNAL(moveBookmark(CodeEditor::Bookmark *, int)), bookmarks, SLOT(moveBookmark(CodeEditor::Bookmark *, int)), Qt::UniqueConnection);
    connect(codeEditor, SIGNAL(removeBookmark(CodeEditor::Bookmark *)),    bookmarks, SLOT(removeBookmark(CodeEditor::Bookmark *)),    Qt::UniqueConnection);

    QString name = tr("NEW %1").arg(++fileNum);
    //addTab(...) не устанавливает windowTitle
    tabWidget->setCurrentIndex(tabWidget->addTab(codeEditor, name));
    CURRENT->setWindowTitle(name);
}

void MainWindow::openFile(QString name)
{
    QStringList names(name);

    if (name.isEmpty())
        names = QFileDialog::getOpenFileNames(this, "", currentPath(), tr("CLIPS files (*.clp *.bat);;All types (*)"));

    openFiles(names);
}

bool MainWindow::saveFile()
{
    QString name = CURRENT->windowFilePath();

    if (name.isEmpty())
        return saveFileAs();

    return saveFile(name);
}

bool MainWindow::saveFileAs()
{
    QString name = QFileDialog::getSaveFileName(this, "", currentPath() + "/" + QFileInfo(CURRENT->windowFilePath()).fileName(),
                                                tr("CLIPS files (*.clp *.bat);;All types (*)"));
    if (name.isEmpty())
        return false;

    return saveFile(name);
}

void MainWindow::renameFile()
{
    QString name = CURRENT->windowFilePath();

    if (saveFileAs())
        QFile::remove(name);
}

void MainWindow::dropUrls(QList<QUrl> urls)
{
    QStringList names;

    for (int i = 0; i < urls.count(); i++) {
        QString name = QDir::toNativeSeparators(urls[i].toLocalFile());

        for (int j = 0; j < tabWidget->count(); j++)
            if (tabWidget->widget(j)->windowFilePath() == name)
               goto L;

        names << name;
    L:;
    }

    if (names.count())
        openFiles(names);
}

void MainWindow::openFiles(QStringList names)
{
    foreach (QString name, names) {
        if (!(tabWidget->count() == 1 && !EDITOR->document()->isModified() && CURRENT->windowFilePath().isEmpty()))
            newFile();

        loadFile(name);
    }
}

void MainWindow::saveAllFiles()
{
    int c = tabWidget->currentIndex();

    for (int i = 0; i < tabWidget->count(); i++) {
        tabWidget->setCurrentIndex(i);
        saveFile();
    }

    tabWidget->setCurrentIndex(c);
}

void MainWindow::closeAllFiles()
{
    for (int i = 0; i < tabWidget->count(); i++) {
        tabWidget->setCurrentIndex(i);

        if (!maybeSave())
            return;
    }

    for (int i = 0; i < tabWidget->count(); i++)
        tabWidget->widget(i)->deleteLater(); // вызывает деструктор Bookmark
        // tabWidget->removeTab(i);
}

void MainWindow::closeLeftFiles()
{
    QWidget *tab = tabWidget->currentWidget();

    int i = 0;

    while (1) {
        tabWidget->setCurrentIndex(i);

        if (tabWidget->currentWidget() == tab)
            break;

        if (maybeSave())
            tabWidget->widget(i)->deleteLater();

        i++;
    }
}

void MainWindow::closeRightFiles()
{
    int index = tabWidget->currentIndex();

    for (int i = index + 1; i < tabWidget->count(); i++) {
        tabWidget->setCurrentIndex(i);

        if (maybeSave())
            tabWidget->widget(i)->deleteLater();
    }

    tabWidget->setCurrentIndex(index);
}

void MainWindow::closeOtherFiles()
{
    closeLeftFiles();
    closeRightFiles();
}

void MainWindow::loadSession(QStringList names, bool open)
{
    if (!open)
        closeAllFiles();

    openFiles(names);
}

void MainWindow::searchReplace(QString str1, QString str2, int flags)
{
    if (flags & SearchBar::ReplaceAll) {
        if (flags & SearchBar::AllFiles)
            for (int i = 0; i < tabWidget->count(); i++) {
                tabWidget->setCurrentIndex(i);
                EDITOR->replaceAll(str1, str2, flags);
            }
        else
            EDITOR->replaceAll(str1, str2, flags);
    } else if ((str2.isNull() && !EDITOR->search(str1, flags) || !str2.isNull() && !EDITOR->replace(str1, str2, flags)) && flags & SearchBar::AllFiles) {
        if (flags & SearchBar::Backward) {
            if (tabWidget->currentIndex() != 0) {
                tabWidget->setCurrentIndex(tabWidget->currentIndex() - 1);
                EDITOR->moveCursor(QTextCursor::End);
            }
        } else if (tabWidget->currentIndex() < tabWidget->count() - 1) {
            tabWidget->setCurrentIndex(tabWidget->currentIndex() + 1);
            EDITOR->moveCursor(QTextCursor::Start);
        }
    }
}

void MainWindow::help()
{
    helpViewer->setIndex(EDITOR->wordHelp());
    helpViewer->showNormal();
    helpViewer->activateWindow();
}

/*
void MainWindow::tabContextMenu(const QPoint &point)
{
    int i = tabWidget->tabBar()->tabAt(point);
    // иначе отработка также на extraArea
    if (tabWidget->tabBar()->tabRect(i).contains(point)) {
        tabWidget->setCurrentIndex(i);
        closeCurrentFile();
    }
}
*/

void MainWindow::tabContextMenu(const QPoint &point)
{
    int i = tabWidget->tabBar()->tabAt(point);
    // иначе отработка также на extraArea
    if (tabWidget->tabBar()->tabRect(i).contains(point)) {
        tabWidget->setCurrentIndex(i);

        TMCLOSELEFT->setDisabled(!i);
        TMCLOSERIGHT->setDisabled(i + 1 == tabWidget->count());
        TMOTHER->setDisabled(tabWidget->count() == 1);
        TMSAVE->setDisabled(CURRENT->windowFilePath().isEmpty() || !EDITOR->document()->isModified());
        TMRENAME->setDisabled(CURRENT->windowFilePath().isEmpty());

        tabMenu->exec(tabWidget->tabBar()->mapToGlobal(point));
    }
}

void MainWindow::modificationChanged(bool changed)
{
    QString str = tabWidget->tabText(tabWidget->currentIndex());

    if (str[str.length() - 1] == '*') {
        if (changed)
            return;

        str.resize(str.length() - 1);
    } else if (changed) {
        str += '*';
    }

    tabWidget->setTabText(tabWidget->currentIndex(), str);
    FMSAVE->setEnabled(changed); // "Save"
}

void MainWindow::cursorPositionChanged()
{
    QTextCursor cursor = EDITOR->textCursor();

    status->setText(tr("Ln: %1 Col: %2 Sel: %3|%4").
                       arg(cursor.blockNumber()  + 1).
                       arg(cursor.columnNumber() + 1).
                       arg(cursor.selectedText().isEmpty() ? 0 : EDITOR->document()->findBlock(cursor.selectionEnd()).blockNumber()   -
                                                                 EDITOR->document()->findBlock(cursor.selectionStart()).blockNumber() + 1).
                       arg(cursor.selectedText().length()));
}

void MainWindow::currentChanged(int i)
{
    if (i == -1) {
        fileNum = 0;
        newFile();
    }

    FMSAVE->setEnabled(EDITOR->document()->isModified());
    cursorPositionChanged();
/*
    connect(EDITOR, SIGNAL(modificationChanged(bool)), SLOT(modificationChanged(bool)), Qt::UniqueConnection);
    connect(EDITOR, SIGNAL(cursorPositionChanged()),   SLOT(cursorPositionChanged()),   Qt::UniqueConnection);
    connect(EDITOR, SIGNAL(dropUrls(QList<QUrl>)),     SLOT(dropUrls(QList<QUrl>)), Qt::UniqueConnection);

    connect(EDITOR, SIGNAL(addBookmark(CodeEditor::Bookmark *)),       bookmarks, SLOT(addBookmark(CodeEditor::Bookmark *)),       Qt::UniqueConnection);
    connect(EDITOR, SIGNAL(moveBookmark(CodeEditor::Bookmark *, int)), bookmarks, SLOT(moveBookmark(CodeEditor::Bookmark *, int)), Qt::UniqueConnection);
    connect(EDITOR, SIGNAL(removeBookmark(CodeEditor::Bookmark *)),    bookmarks, SLOT(removeBookmark(CodeEditor::Bookmark *)),    Qt::UniqueConnection);
*/
}

void MainWindow::updateRecentFiles()
{
    int c = 0;

    QStringListIterator it(config->recentFiles);

    while (it.hasNext()) {
        QString fileName = it.next();

        if (QFileInfo(fileName).exists()) {
            recentFileActs[c]->setText(QString("&%1: %2").arg(c + 1).arg(fileName));
            recentFileActs[c]->setData(fileName);
            recentFileActs[c]->setVisible(true);
            c++;
        } else {
            config->recentFiles.removeAll(fileName);
        }
    }

    FMSEPARATOR->setVisible(c);
}

void MainWindow::addDock(QWidget *widget, QAction *action, QString title, Qt::DockWidgetArea area, Qt::DockWidgetAreas areas)
{
    QDockWidget *dock = new QDockWidget(title);

    action->setData(reinterpret_cast<qlonglong>(dock));

    dock->setObjectName(widget->objectName()); // для сохранения настроек
    dock->setAllowedAreas(areas);
    dock->setWidget(widget);

    addDockWidget(area, dock);
}

void MainWindow::showDock()
{
    QDockWidget *dock = reinterpret_cast<QDockWidget *>(static_cast<QAction *>(sender())->data().toInt());

    if (dock->objectName() == "SearchBar" && dock->isHidden()) {
        QTextCursor cursor = EDITOR->textCursor();
        // текущий выделенный текст в строку поиска
        if (!cursor.selectedText().isEmpty())
            searchBar->searchText(cursor.selectedText());
    }

    dock->setVisible(!dock->isVisible());
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About \"%1\"").arg(PROGNAME),
                             tr("<h3>%1 v%2</h3>"
                                "<p>Based on Qt v%3</p>"
                                "<p>Built on %4 at %5</p>"
                                "<p>Author: <a href='mailto:novikovag@gmail.com?subject=%6'>Artem G. Novikov</a></p>"
                                "<p>Project page: <a href='https://github.com/novikovag/CLIPSEditor'>github</a></p>"
                                "<p>License: <a href='http://www.gnu.org/licenses/gpl.html'>GNU GPL v3</a></p>").
                                arg(PROGNAME, VERSION, QT_VERSION_STR, __DATE__, __TIME__, PROGNAME));
}

bool MainWindow::saveFile(QString &name)
{
    QFile file(name);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr(PROGNAME), tr("Cannot write file %1: %2").arg(name, file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    QApplication::setOverrideCursor(Qt::WaitCursor);

    out << EDITOR->toPlainText();

    QApplication::restoreOverrideCursor();

    setCurrentFile(name);

    EDITOR->document()->setModified(false);

    return true;
}

void MainWindow::loadFile(QString &name)
{
    QStringList names = name.split('#');
    // нет позиции, например загрузка из сессии
    if (names.count() == 1)
        names << "0";

    QFile file(names[0]);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr(PROGNAME), tr("Cannot read file %1: %2").arg(name, file.errorString()));
    } else {
        QApplication::setOverrideCursor(Qt::WaitCursor);

        QTextStream in(&file);
        in.setCodec("UTF-8");

        EDITOR->setPlainText(in.readAll());

        QTextCursor cursor = EDITOR->textCursor();
        cursor.setPosition(names[1].toInt());
        EDITOR->setTextCursor(cursor);
        EDITOR->centerCursor();
        modificationChanged(false);

        QApplication::restoreOverrideCursor();

        setCurrentFile(names[0]);
    }
}

void MainWindow::setCurrentFile(QString &name)
{
    name = QDir::toNativeSeparators(name);

    tabWidget->setTabText(tabWidget->currentIndex(), QFileInfo(name).fileName());
    tabWidget->setTabToolTip(tabWidget->currentIndex(), name);
    CURRENT->setWindowFilePath(name); // "" для новых файлов
    CURRENT->setWindowTitle(QFileInfo(name).fileName());

    config->recentFiles.removeAll(name);
    config->recentFiles.prepend(name);

    if (config->recentFiles.size() > config->maxRecentFiles)
        config->recentFiles.removeLast();

    lastPath = QFileInfo(name).path();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    config->openFiles.clear();
    config->lastFile = FSNAME(tabWidget->currentIndex());

    for (int i = 0; i < tabWidget->count(); i++) {
        tabWidget->setCurrentIndex(i);

        if (!maybeSave()) {
            e->ignore();
            return;
        }

        if (!tabWidget->widget(i)->windowFilePath().isEmpty())
            config->openFiles << FSNAME(i);
    }

    helpViewer->close(); // в деструкторе не работает
    // геометрия сохраняется только в closeEvent
    config->mainWindowGeometry = saveGeometry();
    config->mainWindowState    = saveState();
}

bool MainWindow::maybeSave()
{
    if (EDITOR->document()->isModified()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(this, tr(PROGNAME), tr("Do you want to save your changes?"),
                                                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveFile();

        if (ret == QMessageBox::Cancel)
            return false;
    }

    return true;
}

QString MainWindow::currentPath()
{
    if (!CURRENT->windowFilePath().isEmpty() && QDir(QFileInfo(CURRENT->windowFilePath()).path()).exists())
        return QFileInfo(CURRENT->windowFilePath()).path();

    if (!lastPath.isEmpty() && QDir(lastPath).exists())
        return lastPath;

    return QDir::homePath();
}

#ifdef SETSTYLE
void MainWindow::setStyle()
{
    QString name = QFileDialog::getOpenFileName(this, "", "", tr("Style files (*.css);;All types (*)"));

    QFile file(name);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    qApp->setStyleSheet(QLatin1String(file.readAll()));
}
#endif
