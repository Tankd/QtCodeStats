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


#ifndef CSTATTHREAD_H
#define CSTATTHREAD_H

#include <QThread>
#include <QStringList>

#include <QDir>

class CStatThread : public QThread
{
    Q_OBJECT

signals:
    void fileCountUpdated(unsigned fileCount);
    void fileSizeUpdated(unsigned fileSize);
    void lineCountUpdated(unsigned lineCount);



    void fileScanned( QString fileName, uint line, uint size, uint comment, QString filePath);

public:
    explicit CStatThread(QObject *parent = 0);

    QStringList filters() const;
    void setFilters(const QStringList &filters);

    QStringList folders() const;
    void setFolders(const QStringList &folders);

    QStringList excludes() const;
    void setExcludes(const QStringList &excludes);



signals:

protected slots:
    virtual void run();
protected:

    QStringList _folders;
    QStringList _filters;
    QStringList _excludes;
    QString _excludeExp;


    unsigned _fileCount;
    unsigned _fileSize;

    unsigned _lineCount;
    uint _commentCount;


    void scanFolder( const QDir &d); /** scan folder for files */
    void scanFile( const QFileInfo& info); /** scan file for stats */

    uint countComments(QString str); /** count comment lines in str */



};

#endif // CSTATTHREAD_H
