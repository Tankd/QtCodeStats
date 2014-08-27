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

#include "cstatthread.h"
#include <QDebug>


CStatThread::CStatThread(QObject *parent) :
    QThread(parent), _fileCount(0), _fileSize(0), _lineCount(0), _commentCount(0)
{
}

QStringList CStatThread::filters() const
{
    return _filters;
}

void CStatThread::setFilters(const QStringList &filters)
{
    _filters = filters;
}


QStringList CStatThread::folders() const
{
    return _folders;
}

void CStatThread::setFolders(const QStringList &folders)
{
    _folders = folders;
}

void CStatThread::run()
{
    //count files
    _fileCount = 0;
    _fileSize = 0;
    _lineCount = 0;

    foreach( const QString& folder, _folders)
    {
        QDir d(folder);
        scanFolder(d);
    }



    exit(0);

}
QStringList CStatThread::excludes() const
{
    return _excludes;
}

void CStatThread::setExcludes(const QStringList &excludes)
{
    //_excludeExp

    _excludes = excludes;
    _excludeExp.clear();

    if( _excludes.isEmpty())
    {
        _excludeExp = "*";
    }
    else
    {

        foreach( const QString& s, _excludes)
        {
            _excludeExp.append( QString("(%1)|").arg(s));
        }

        _excludeExp.remove( _excludeExp.lastIndexOf("|"), 1);
    }
}


void CStatThread::scanFolder(const QDir &d)
{
    //create filtered file list
    QFileInfoList list = d.entryInfoList(_filters);
    _fileCount += list.count();
    emit fileCountUpdated( _fileCount);

    //check stats
    foreach ( const QFileInfo& info, list)
    {
        if(info.absoluteFilePath().contains( QRegExp( _excludeExp)) == false)
            scanFile( info);
    }

    //check subfolders
    foreach( const QFileInfo& info, d.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs))
    {
        if(info.isDir())
            scanFolder( info.absoluteFilePath());
    }
}

void CStatThread::scanFile(const QFileInfo &info)
{
    QFile file( info.absoluteFilePath());
    file.open( QFile::QIODevice::ReadOnly);
    QString data = file.readAll();
    file.close();


    _fileSize += info.size();
    emit fileSizeUpdated( _fileSize);

    uint lines = data.count("\n");

    uint comment = countComments(data);

    _lineCount += lines;
    _commentCount += comment;

    emit lineCountUpdated( _lineCount);

    emit fileScanned( info.fileName(),lines, info.size(), comment, info.absoluteFilePath());
}

uint CStatThread::countComments(QString str)
{
    uint count = 0;

    while( str.count("  "))
        str.replace ("  ", " ");

    while( str.count("\n "))
        str.replace ("\n ", "\n");


    bool inComment = false;
    foreach( const QString& s, str.split("\n"))
    {
        if(inComment)
            count++;
        else if(s.left(2).contains("//"))
            count++;

        if(s.contains("/*"))
            inComment = true;

        if(s.contains("*/"))
            inComment = false;
    }

    return count;
}

