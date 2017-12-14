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

#include "highlighter.h"

Highlighter::Highlighter(Config *config, CodeEditor *codeEditor)
    : QSyntaxHighlighter(codeEditor->document()), config(config), codeEditor(codeEditor)
{
    errorFmt.setUnderlineColor(Qt::red);
    errorFmt.setUnderlineStyle(QTextCharFormat::WaveUnderline);

    matchFmt.setForeground(Qt::white);
    matchFmt.setBackground(Qt::red);

    mismatchFmt.setForeground(Qt::red);

    connect(codeEditor,                SIGNAL(cursorPositionChanged()),             SLOT(matchParentheses()));
    disconnect(codeEditor->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(_q_reformatBlocks(int, int, int)));
}

void Highlighter::highlightBlock(const QString &txt)
{
    int state = currentBlockState();

    if (state == CodeEditor::Empty)
        return;

    if (state & CodeEditor::Error) {
        int start = state >> CodeEditor::StateShift;

        setFormat(start, txt.length() - start, errorFmt);
    } else {
        QHashIterator<QString, QRegExp> it(config->patterns);

        while (it.hasNext()) {
            it.next();

            const QRegExp& rx = it.value();

            int i   = 0;
            int len = 0;

            while ((i = rx.indexIn(txt, i + len)) > -1)
                setFormat(i, (len = rx.matchedLength()), config->colorScheme.value(it.key()));
        }
        // --- строки и комментарии ---
        state = previousBlockState();

        QQueue<int> indexes;

        if (!(state & CodeEditor::Error) && state & CodeEditor::String)
            indexes.enqueue(0);

        for (int i = 0; i < txt.length(); i++) {
            if (txt[i] == ';' && (indexes.isEmpty() || !(indexes.count() % 2))) {
                setFormat(i, txt.length() - i, config->colorScheme.value("Comments"));
                break;
            }

            if (txt[i] == '"' && !(i && txt[i - 1] == '\\'))
                indexes.enqueue(i);
        }

        while (!indexes.isEmpty()) {
            int start = indexes.dequeue();
            int end   = indexes.isEmpty() ? txt.length() : indexes.dequeue();

            setFormat(start, end - start + 1, config->colorScheme.value("Strings"));
        }
    }
}

void Highlighter::matchParentheses()
{
    if (codeEditor->textCursor().hasSelection())
        return;

    QList<QTextEdit::ExtraSelection> selections;

    QTextCursor cursor = codeEditor->textCursor();

    int pos = cursor.position();

    QChar begin;

    if ((((begin = document()->characterAt(pos))     == '(' || begin == '[') && goodPosition(pos)) ||
        (((begin = document()->characterAt(pos - 1)) == ')' || begin == ']') && goodPosition(pos - 1))) {

        QTextEdit::ExtraSelection selection;

        bool forward = begin == '(' || begin == '[';

        QChar ch, end;

        if (forward) {
            end = begin == '(' ? ')' : ']';
            pos++;
        } else {
            end = begin == ')' ? '(' : '[';
            pos -= 2;
        }

        cursor.movePosition(forward ? QTextCursor::NextCharacter : QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

        QTextCharFormat fmt = mismatchFmt;

        int braceDepth = 1;

        while (!(ch = document()->characterAt(pos)).isNull()) {
            if ((ch == begin || ch == end) && goodPosition(pos)) {
                ch == begin ? braceDepth++ : braceDepth--;

                if (!braceDepth) {
                    QTextCursor cursor = codeEditor->textCursor();
                    cursor.setPosition(pos);
                    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

                    selection.format = matchFmt;
                    selection.cursor = cursor;
                    selections.append(selection);

                    fmt = matchFmt;
                    break;
                }
            }

            forward ? pos++ : pos--;
        }

        selection.format = fmt;
        selection.cursor = cursor;
        selections.append(selection);
    }

    codeEditor->setExtraSelections(selections);
}

bool Highlighter::goodPosition(int pos)
{
    QTextBlock block = document()->findBlock(pos);

    int state = block.userState();

    if (state & CodeEditor::Error || state & CodeEditor::Comment)
        return false;

    state = block.previous().userState();

    bool inString = !(state & CodeEditor::Error) && state & CodeEditor::String;

    QString txt = block.text();

    for (int i = 0; i < (pos - block.position()); i++) { // position - block.position() позиция внутри блока
        if (!inString && txt[i] == ';')
            return false;

        if (txt[i] == '"' && !(i && txt[i - 1] == '\\'))
            inString = !inString;
    }

    return !inString;
}
