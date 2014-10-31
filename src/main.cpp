/*
This file is part of QtCodeStats.

QtCodeStats is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

QtCodeStats is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with QtCodeStats. If not, see <http://www.gnu.org/licenses/>.
*/

#include "cmainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //stylesheet
    QFile file(QString(":/style_light.css"));
    file.open(QIODevice::ReadOnly);
    if(file.isOpen() == false) exit(1);
    QString StyleSheet = file.readAll();
    file.close();
    qApp->setStyleSheet(StyleSheet);

    //translations
    QTranslator translator;
    translator.load(":/qtcodestats_fr.qm");
    a.installTranslator(&translator);

    CMainWindow w;
    w.show();

    return a.exec();
}
