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

#include "configdialog.h"

ConfigDialog::ConfigDialog(Config *config)
    : config(config)
{
    setupUi(this);

    cmbLanguage->addItems(config->languages.keys());
    cmbLanguage->setCurrentIndex(cmbLanguage->findText(config->language));

    cmbFont->setCurrentFont(QFont(config->fontFamily));
    spnFontSize->setValue(config->fontSize);
    chbAutoIndent->setChecked(config->autoIndent);
    spnIndentSize->setValue(config->indentSize);
    spnTabSize->setValue(config->tabSize);
    chbWhitespaces->setChecked(config->whitespaces);
    spnVerticalEdge->setValue(config->verticalEdge);

    QMapIterator<QString, QTextCharFormat> it(config->colorScheme);

    while (it.hasNext()) {
        it.next();

        QListWidgetItem *item = new QListWidgetItem(it.key(), lstScheme);

        item->setForeground(it.value().foreground());
        item->setBackground(it.value().background());
        item->setFont(it.value().font());

        QStringList values = config->keywords.keywords.values(it.key());
        values.sort();

        QString toolTip;

        for (int i = 0; i < values.size(); i++)
            toolTip += values.at(i) + "\n";
        // убираем самый последний NL
        toolTip.chop(1);

        item->setToolTip(toolTip);
    }

    lstScheme->setCurrentRow(0);
    tlbForeground->setStyleSheet(STYLE(lstScheme->currentItem()->foreground().color()));
    tlbBackground->setStyleSheet(STYLE(lstScheme->currentItem()->background().color()));
    chbBold->setChecked(lstScheme->currentItem()->font().bold());
    chbItalic->setChecked(lstScheme->currentItem()->font().italic());

    QMenu *menu = new QMenu();

    foreach (QString fKey, config->fKeys.keys()) {
        QStringList data;
        data << fKey << config->fKeys[fKey];

        if (fKey == config->fKey) {
            fKeyAction = menu->addAction(fKey);
            fKeyAction->setData(data);
            tlbFKeys->setText(config->fKey);
            lneFCommand->setText(config->fKeys[fKey]);
        } else {
            menu->addAction(fKey)->setData(data);
        }
    }

    lneFCommand->setToolTip("CURRENT_PATH CURRENT_FILE");
    lneFCommand->setPlaceholderText("cmd.exe /C start C:\\clips\\clipsdos.exe -f CURRENT_PATH\\run.bat");

    tlbFKeys->setMenu(menu);

    setWindowFlags(Qt::WindowCloseButtonHint); // Qt::Dialog
    setFixedSize(size());

    connect(menu,            SIGNAL(triggered(QAction *)),              SLOT(getFKey(QAction *)));
    connect(lneFCommand,     SIGNAL(textEdited(QString)),               SLOT(setFKey(QString)));
    connect(btnStyle,        SIGNAL(clicked(bool)),                     SLOT(setStyle(bool)));
    connect(btnStyleDefault, SIGNAL(clicked(bool)),                     SLOT(setStyle(bool)));
    connect(tlbForeground,   SIGNAL(clicked()),                         SLOT(setColor()));
    connect(tlbBackground,   SIGNAL(clicked()),                         SLOT(setColor()));
    connect(chbBold,         SIGNAL(clicked()),                         SLOT(setFont()));
    connect(chbItalic,       SIGNAL(clicked()),                         SLOT(setFont()));
    connect(lstScheme,       SIGNAL(itemClicked(QListWidgetItem *)),    SLOT(setScheme(QListWidgetItem *)));
    connect(btnBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), SLOT(applay()));
    connect(btnBox->button(QDialogButtonBox::Ok),    SIGNAL(clicked()), SLOT(applay()));
}

void ConfigDialog::applay()
{
    if (config->language != cmbLanguage->currentText())
        QMessageBox::information(this, tr("Restart required"), tr("The language change will take effect after a restart editor"));

    if (config->style != style) {
        qApp->setStyleSheet(style);
        config->style = style;
    }

    config->language = cmbLanguage->currentText();

    foreach (QAction *action, tlbFKeys->menu()->actions()) {
        QStringList data = action->data().toStringList();
        config->fKeys[data[0]] = data[1].trimmed();
    }

    config->fontFamily   = cmbFont->currentFont().family();
    config->fontSize     = spnFontSize->value();
    config->autoIndent   = chbAutoIndent->isChecked();
    config->tabSize      = spnTabSize->value();
    config->indentSize   = spnIndentSize->value();
    config->whitespaces  = chbWhitespaces->isChecked();
    config->verticalEdge = spnVerticalEdge->value();

    for (int i = 0; i < lstScheme->count(); i++) {
        QListWidgetItem *item = lstScheme->item(i);
        QTextCharFormat  format;

        format.setForeground(item->foreground());
        format.setBackground(item->background());
        format.setFontItalic(item->font().italic());
        format.setFontWeight(item->font().weight());
        config->colorScheme[item->text()] = format;
    }

    config->reconfig(Config::Editor);
}

void ConfigDialog::setFont()
{
    QFont font = lstScheme->currentItem()->font();
    font.setItalic(chbItalic->isChecked());
    font.setBold(chbBold->isChecked());
    lstScheme->currentItem()->setFont(font);
}

void ConfigDialog::setScheme(QListWidgetItem *item)
{
    chbBold->setChecked(item->font().bold());
    chbItalic->setChecked(item->font().italic());
    tlbForeground->setStyleSheet(STYLE(item->foreground().color()));
    tlbBackground->setStyleSheet(STYLE(item->background().color()));
}

void ConfigDialog::setColor()
{
    QColor color = QColorDialog::getColor(static_cast<QToolButton *>(sender())->palette().background().color());

    if (color.isValid()) {
        if (sender() == tlbForeground)
            lstScheme->currentItem()->setForeground(color);
        else
            lstScheme->currentItem()->setBackground(color);

        static_cast<QToolButton *>(sender())->setStyleSheet(STYLE(color));
    }
}

void ConfigDialog::getFKey(QAction *a)
{
    fKeyAction = a;

    QStringList data = fKeyAction->data().toStringList();

    config->fKey = data[0];

    tlbFKeys->setText(data[0]);
    lneFCommand->setText(data[1]);
}

void ConfigDialog::setFKey(QString text)
{
    QStringList data = fKeyAction->data().toStringList();
    data[1] = text;
    fKeyAction->setData(data);
}

void ConfigDialog::setStyle(bool)
{
    if (sender() == btnStyle) {
        QString name = QFileDialog::getOpenFileName(this, "", "", tr("Style files (*.css);;All types (*)"));

        if (name.isEmpty())
            return;

        QFile file(name);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            style = QLatin1String(file.readAll());

    } else {
        QFile file(":/rc/style.css");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        style = QLatin1String(file.readAll());
    }
}
