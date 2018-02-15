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

#include "config.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile file(":/rc/style.css");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    qApp->setStyleSheet(QLatin1String(file.readAll()));

    Config config;

    QStringList arguments = QCoreApplication::arguments();
    arguments.removeFirst();
    config.openFiles += arguments;

    QString locale = config.languages.value(config.language);

    QTranslator trans1;
    trans1.load(":/lang/clipseditor_" + locale);
    app.installTranslator(&trans1);

    QTranslator trans2;
    trans2.load(":/lang/qtbase_" + locale);
    app.installTranslator(&trans2);

    app.setWindowIcon(QPixmap(":/rc/icon.png"));

    MainWindow window(&config);
    window.show();

    return app.exec();
}
