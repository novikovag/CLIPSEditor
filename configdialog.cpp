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
    chbTabIndents->setChecked(config->tabIndents);
    chbAutoIndent->setChecked(config->autoIndent);
    chbBackUnindent->setChecked(config->backUnindent);
    chbSpaceTabs->setChecked(config->spaceTabs);
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
    }

    lstScheme->setCurrentRow(0);
    tlbForeground->setStyleSheet(STYLE(lstScheme->currentItem()->foreground().color()));
    tlbBackground->setStyleSheet(STYLE(lstScheme->currentItem()->background().color()));
    chbBold->setChecked(lstScheme->currentItem()->font().bold());
    chbItalic->setChecked(lstScheme->currentItem()->font().italic());

    setWindowFlags(Qt::WindowCloseButtonHint); // Qt::Dialog
    setFixedSize(size());

    connect(tlbForeground, SIGNAL(clicked()), SLOT(setColor()));
    connect(tlbBackground, SIGNAL(clicked()), SLOT(setColor()));
    connect(chbBold,       SIGNAL(clicked()), SLOT(setFont()));
    connect(chbItalic,     SIGNAL(clicked()), SLOT(setFont()));
    connect(lstScheme,     SIGNAL(itemClicked(QListWidgetItem *)),      SLOT(setScheme(QListWidgetItem *)));
    connect(btnBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), SLOT(applay()));
    connect(btnBox->button(QDialogButtonBox::Ok),    SIGNAL(clicked()), SLOT(applay()));
}

void ConfigDialog::applay()
{
    if (config->language != cmbLanguage->currentText())
        QMessageBox::information(this, tr("Restart required"), tr("The language change will take effect after a restart editor"));

    config->language     = cmbLanguage->currentText();
    config->fontFamily   = cmbFont->currentFont().family();
    config->fontSize     = spnFontSize->value();
    config->tabIndents   = chbTabIndents->isChecked();
    config->autoIndent   = chbAutoIndent->isChecked();
    config->backUnindent = chbBackUnindent->isChecked();
    config->spaceTabs    = chbSpaceTabs->isChecked();
    config->tabSize      = spnTabSize->value();
    config->indentSize   = spnIndentSize->value();
    config->whitespaces  = chbWhitespaces->isChecked();
    config->verticalEdge = spnVerticalEdge->value();

    for (int i = 0; i < lstScheme->count(); i++) {
        QListWidgetItem *item = lstScheme->item(i);
        QTextCharFormat  fmt;

        fmt.setForeground(item->foreground());
        fmt.setBackground(item->background());
        fmt.setFontItalic(item->font().italic());
        fmt.setFontWeight(item->font().weight());
        config->colorScheme[item->text()] = fmt;
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
