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

#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class CMainWindow;
}


class CStatThread;
class QCheckBox;

/**
 * @brief The CMainWindow class
 * the main window
 */
class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

private slots:
    void on_clearButton_clicked();

    void on_addFolderButton_clicked();

    void on_backButton_clicked();

    void on_countButton_clicked();



    void threadStarted();/** stats generation started */
    void threadFinished();/** stats generation finished */

    void fileCountUpdated(uint fileCount);
    void fileSizeUpdated(uint fileSize);
    void lineCountUpdated(uint lineCount);    
    void fileScanned( QString fileName, uint line, uint size, uint comment, QString filePath);


    void on_removeFolderButton_clicked();

private:
    Ui::CMainWindow *ui;

    QString _lastPath; /** last added path */
    CStatThread* _thread;


    QList<QCheckBox*> _checks; /** filters check box */
    QList<QCheckBox*> _excludes; /** exclusion check box */

    void readConfig(); /** load config from json file*/
    void saveFolders(); /** load last scanned folders */

    void resetStat();

};

#endif // CMAINWINDOW_H
