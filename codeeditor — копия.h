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

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QtWidgets>

#include "config.h"
#include "highlighter.h"

#define MUNDO      menu->actions().at(0)
#define MREDO      menu->actions().at(1)
#define MCUT       menu->actions().at(3)
#define MCOPY      menu->actions().at(4)
#define MPASTE     menu->actions().at(5)
#define MDELETE    menu->actions().at(6)
#define MSELECTALL menu->actions().at(7)

#define CMCUT      columnMenu->actions().at(0)
#define CMCOPY     columnMenu->actions().at(1)

#define FOLDBOXRECT(cy) QRect(markWidth + lineNumWidth + foldBoxIndent, cy - foldBoxWidth / 2, foldBoxWidth, foldBoxWidth)
#define FULLRESIZE      resizeEvent(new QResizeEvent(QSize(0, 0), size()))
#define FONTWIDTH       fontMetrics().width(QLatin1Char('X'))
#define BLOCKMARK       reinterpret_cast<CodeEditor::Bookmark*>(block.userData())

class Highlighter;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    // при добавлении флага, смотреть метод setBlockState(), поскольку
    // previousBlockState и previousState перезаписывают статус, также как
    // и contentsChange()
    enum BlockState {
        End         = 1,     // 00000001 конец блока
        Begin       = 2,     // 00000010 начало блока
        String      = 4,     // 00000100 признак незавершенной сторки (нет пары для ")
        Comment     = 8,     // 00001000 строка является внешним комментарием
        Nested      = 16,    // 00010000 признак вложенности
        Folded      = 32,    // 00100000 блок свернут
        Error       = 64,    // 01000000 строка содержит ошибку
        Rehighlight = 128,   // 10000000 блок необходимо переподсветить
        //Bookmark    = 256, // 100000000 закладка
        Empty       = -1     // 1111.... новый/пустой блок
    };
    // позиция последнего значащего бита, после которого идет значение вложенности,
    // равно велечине последнего элемента BlockState << 1
    const static int StateShift = 8;

    enum SearchFlag {  // QTextDocument::FindFlag
        Backward   = 1,
        MatchCase  = 2,
        WholeWords = 4,
        RegExp     = 8
    };

    struct Bookmark : public QTextBlockUserData
    {
        Bookmark(CodeEditor *editor, QTextBlock block)
            : editor(editor), block(block), num(block.blockNumber()) { setActive(); }

        void setActive(bool active = true)
        {
            if (active)
                emit editor->addBookmark(this);
            else
                emit editor->removeBookmark(this);

            isActive = active == true;
            editor->extraArea->update();
        }

        void check()
        {
            if (block.blockNumber() != num) {
                if (isActive)
                    emit editor->moveBookmark(this, num);

                num = block.blockNumber();
            }
        }

       ~Bookmark() { emit editor->removeBookmark(this); }

        bool        isActive;
        int         num;

        CodeEditor *editor;
        QTextBlock  block;
    };

public:
    CodeEditor(Config *);

    QString wordHelp();

    bool search(QString,     int);
    bool replace(QString,    QString, int);
    void replaceAll(QString, QString, int);


signals:
    void addBookmark(CodeEditor::Bookmark *);
    void moveBookmark(CodeEditor::Bookmark *, int);
    void removeBookmark(CodeEditor::Bookmark *);
    void dropUrls(QList<QUrl>);

private slots:
    void cut();
    void copy();
    void paste();

    void foldAll()        { foldUnfoldAll(); }
    void unfoldAll()      { foldUnfoldAll(false); }
    void deleteSelected() { textCursor().removeSelectedText(); }
    void toggleComment();
    void toUpperCase()    { convertCase(); }
    void toLowerCase()	  { convertCase(false); }

    void blockCountChanged(int count) { lineNumDigits = qMax(2, QString::number(count).length()); } // 2 - разряда
    void contentsChange(int, int, int);
    void performCompletion();
    void insertCompletion(const QString &);
    void ensureCursorVisible();

    void reconfig(int = (Config::Init | Config::Editor));

private:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

    void keyPressEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void contextMenuEvent(QContextMenuEvent *);
    /*
    void dropEvent(QDropEvent *e)
    {
        if (e->mimeData()->hasUrls())
            emit dropUrls(e->mimeData()->urls());
        else
            QPlainTextEdit::dropEvent(e);
    }
    */
    void dropEvent(QDropEvent *e) { e->mimeData()->hasUrls() ? emit dropUrls(e->mimeData()->urls()) : QPlainTextEdit::dropEvent(e); }


    bool eventFilter(QObject *, QEvent *);

    void extraAreaPaintEvent();
    void extraAreaMouseEvent(QMouseEvent *);

    int  setBlockState(QTextBlock &);
    QTextBlock findBlockByY(int);

    void foldUnfold(QTextBlock &);
    void foldUnfoldAll(bool = true);

    void convertCase(bool toUpper = true) { textCursor().insertText(toUpper ? textCursor().selectedText().toUpper() : textCursor().selectedText().toLower()); }

    Highlighter *highlighter;
    QCompleter  *completer;

    QTextBlock   pointedBlock;

    QWidget     *extraArea;

    int          lineNumDigits;
    int          lineNumWidth;

    int          foldBoxWidth;
    int          foldBoxIndent;
    int          foldBoxLength;

    int          markWidth; // от размера шрифта

    Config      *config;

    QMenu       *menu;

    QImage       mark;

    QList<QKeySequence> shortcuts;
    //---------------------------------
    void columnSelectionOff();

    QTextCursor columnStartCursor;
    QRect       columnStartRect;
    QRect       columnEndRect;

    bool        columnSelection;
    int         columnMax;

    QMenu      *columnMenu;

    QList<QTextEdit::ExtraSelection> columnSelections;
    QList<QString>                   culumnTexts;
};
// конвертация в QVariant
Q_DECLARE_METATYPE(CodeEditor::Bookmark *)

#endif // CODEEDITOR_H
