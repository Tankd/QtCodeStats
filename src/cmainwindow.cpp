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
#include "ui_cmainwindow.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>


#include <QJsonObject>

#include <QGridLayout>
#include <QCheckBox>

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QCheckBox>
#include <QFileInfo>

#include "cstatthread.h"


CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow), _thread(0)
{
    ui->setupUi(this);
    ui->resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    readConfig();

    _lastPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();

    on_backButton_clicked();

    _thread = new CStatThread(this);
    connect( _thread, SIGNAL(started()), this, SLOT(threadStarted()));
    connect( _thread, SIGNAL(finished()), this, SLOT(threadFinished()));
    connect( _thread, SIGNAL(fileCountUpdated(uint)), this, SLOT(fileCountUpdated(uint)));
    connect( _thread, SIGNAL(fileSizeUpdated(uint)), this, SLOT(fileSizeUpdated(uint)));
    connect( _thread, SIGNAL(lineCountUpdated(uint)), this, SLOT(lineCountUpdated(uint)));

    connect( _thread, SIGNAL(fileScanned(QString, uint, uint, uint, QString)), this, SLOT(fileScanned(QString, uint, uint, uint, QString)));
}

CMainWindow::~CMainWindow()
{
    delete _thread;
    delete ui;
}

void CMainWindow::readConfig()
{
    _checks.clear();

    QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/config.json";

    //config exist ?
    if( QFileInfo().exists(configPath) == false)
    {
        QDir().mkpath( QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
        QFile file(configPath);
        file.open( QFile::WriteOnly);

        file.write(
                    "{"
                    "  \"filters\": ["
                    "\"*.c\","
                    "\"*.cpp\","
                    "\"*.cxx\","
                    "\"*.cc\","
                    "\"*.h\","
                    "\"*.hpp\","
                    "\"*.hxx\","
                    "\"*.inl\","
                    "\"*.asm\","
                    "\"*.inc\""
                    "],"
                    "\"excludes\" : ["
                    "\"moc_\","
                    "\"ui_\","
                    "\"qrc_\""
                    "]"

                    "}"
                    );


        file.close();

    }

    //read config
    QFile file(configPath);
    file.open( QFile::ReadOnly);
    if(file.isOpen() == false)
    {
        return;
    }


    QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();

    {
        QVariantList types = obj.value("filters").toArray().toVariantList();
        bool right = false;
        int i=0;
        foreach( const QVariant& v, types)
        {
            QCheckBox* box = new QCheckBox(v.toString());
            box->setChecked(true);
            _checks.append(box);
            ui->filtersLayout->addWidget(box, i, right ? 1 : 0);

            if( right == false)
                right = true;
            else
            {
                right = false;
                i++;
            }
        }
    }

    {
        QVariantList types = obj.value("excludes").toArray().toVariantList();
        bool right = false;
        int i=0;
        foreach( const QVariant& v, types)
        {
            QCheckBox* box = new QCheckBox(v.toString());
            box->setChecked(true);
            _excludes.append(box);
            ui->excludesLayout->addWidget(box, i, right ? 1 : 0);

            if( right == false)
                right = true;
            else
            {
                right = false;
                i++;
            }
        }
    }

    //read folders
    QString folderConfigPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/folders.json";
    QFile foldersFile(folderConfigPath);
    foldersFile.open( QIODevice::ReadOnly);
    QVariantList array = QJsonDocument::fromJson( foldersFile.readAll()).array().toVariantList();
    foreach( const QVariant& v, array)
        ui->folderList->addItem(v.toString());

    foldersFile.close();
}

void CMainWindow::saveFolders()
{
     QString folderConfigPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/folders.json";
    QJsonArray array;
    for(int i=0; i < ui->folderList->count(); i++ )
    {
        array.append(ui->folderList->item(i)->text());
    }

    QFile f(folderConfigPath);
    f.open( QIODevice::WriteOnly);
    f.write(QJsonDocument(array).toJson());
    f.close();
}

void CMainWindow::resetStat()
{
    ui->fileCountEdit->setText("0");
    ui->fileSizeEdit->setText("0");
    ui->lineEdit->setText("0");


    while( ui->resultTable->rowCount())
        ui->resultTable->removeRow(0);

}

void CMainWindow::on_clearButton_clicked()
{
    ui->folderList->clear();
}

void CMainWindow::on_addFolderButton_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(
                this,
                "add folders",
                _lastPath);

    if(fileName.isEmpty()) return;


    _lastPath = fileName;
    _lastPath.remove( _lastPath.lastIndexOf("/"), _lastPath.count());


    ui->folderList->addItem( fileName);

}

void CMainWindow::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void CMainWindow::on_countButton_clicked()
{
    if(ui->folderList->count() == 0) return;
    resetStat();
    saveFolders();


    //folders
    QStringList folders;
    for(int i=0; i < ui->folderList->count(); i++ )
    {
        folders.append(ui->folderList->item(i)->text());
    }
    folders.removeDuplicates();
    _thread->setFolders(folders);


    //filters
    QStringList filters;

    foreach( const QCheckBox* box, _checks)
        if(box->isChecked())
            filters << box->text();

    _thread->setFilters( filters);


    //exclusions
    QStringList excludes;

    foreach( const QCheckBox* box, _excludes)
        if(box->isChecked())
            excludes << box->text();

    _thread->setExcludes( excludes);

    _thread->start();
}

void CMainWindow::threadStarted()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget->setEnabled(false);

    this->setWindowTitle("QtCodeStats - running ...");
}

void CMainWindow::threadFinished()
{
    ui->stackedWidget->setEnabled(true);

    this->setWindowTitle("QtCodeStats");
}

void CMainWindow::fileCountUpdated(uint fileCount)
{
    ui->fileCountEdit->setText( QString::number( fileCount));
}

void CMainWindow::fileSizeUpdated(uint fileSize)
{
    ui->fileSizeEdit->setText( QString::number(  fileSize / 1024.0f, 'f', 2 ) + " ko");
}

void CMainWindow::lineCountUpdated(uint lineCount)
{
    ui->lineEdit->setText( QString::number( lineCount));
}

void CMainWindow::fileScanned(QString fileName, uint line, uint size, uint comment, QString filePath)
{
    ui->resultTable->insertRow( ui->resultTable->rowCount());

    ui->resultTable->setItem( ui->resultTable->rowCount()-1, 0, new QTableWidgetItem(fileName));
    ui->resultTable->setItem( ui->resultTable->rowCount()-1, 1, new QTableWidgetItem( QString::number(size / 1024.0f, 'f', 2)));
    ui->resultTable->setItem( ui->resultTable->rowCount()-1, 2, new QTableWidgetItem( QString::number(line)));
    ui->resultTable->setItem( ui->resultTable->rowCount()-1, 3, new QTableWidgetItem( QString::number( 100.0f * (float)comment / (float)line, 'f', 2)));
    ui->resultTable->setItem( ui->resultTable->rowCount()-1, 4, new QTableWidgetItem( filePath));

}

void CMainWindow::on_removeFolderButton_clicked()
{
    delete ui->folderList->currentItem();
}
