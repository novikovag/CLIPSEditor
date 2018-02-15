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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "config.h"
#include "codeeditor.h"
#include "helpviewer.h"
#include "configdialog.h"
#include "searchbar.h"
#include "sessions.h"
#include "snippets.h"
#include "bookmarks.h"

#define CURRENT   tabWidget->currentWidget()
#define EDITOR    static_cast<CodeEditor *>(tabWidget->currentWidget())
#define FSNAME(a) QString("%1#%2").arg(tabWidget->widget(a)->windowFilePath()).arg(EDITOR->textCursor().position())

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Config *);

private slots:
    void newFile();
    void openFile(QString = "");
    void openFiles(QStringList);
    void openRecentFile() { openFile((static_cast<QAction *>(sender()))->data().toString()); }

    void closeCurrentFile()
    {
        if (maybeSave())
            CURRENT->deleteLater(); // вызывает деструктор Bookmark
    }

    void dropUrls(QList<QUrl>);

#ifdef SETSTYLE
    void setStyle();
#endif
    bool saveCurrentFile();
    bool saveFileAs();
    void saveAllFiles();
    void closeAllFiles();

    void loadSession(QStringList, bool);
    void insertSnippet(QString txt) { EDITOR->insertPlainText(txt); }

    void searchReplace(QString, QString, int);

    void preferences() { configDialog->exec(); }
    void help();
    void about();

    void tabContextMenu(const QPoint &);

    void modificationChanged(bool);
    void cursorPositionChanged();
    void currentChanged(int);
    void updateRecentFiles();

    void showDock();

private:
    void closeEvent(QCloseEvent *);
    bool maybeSave();
    void setCurrentFile(QString &);
    void loadFile(QString &);
    bool saveCurrentFile(QString &);

    QString currentPath();

    void addDock(QWidget *, QAction *, QString, Qt::DockWidgetArea, Qt::DockWidgetAreas = Qt::AllDockWidgetAreas);

    QList<QAction *> menuActs; // [0] - saveAct, [1] - separatorAct
    QList<QAction *> recentFileActs;

    int           fileNum;
    QString	      lastPath;
    
    QTabWidget   *tabWidget;
    ConfigDialog *configDialog;
    Sessions     *sessions;
    Snippets     *snippets;
    SearchBar    *searchBar;
    Bookmarks    *bookmarks;
    HelpViewer   *helpViewer;

    QShortcut    *searchShort;

    Config       *config;

    QLabel       *status;
};

#endif // MAINWINDOW_H
