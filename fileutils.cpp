#include "fileutils.h"
#include <QDebug>
#include <windows.h>
#include <QDirIterator>
#include <QFileInfo>
#include <QList>

std::pair<int, int> fileutils::NumberOfFilesAndDirectories(QList<QString>& file_paths) {
    int dirs = 0, files = 0;
    for(const QString& path : file_paths) {
        QFileInfo info(path);
        if(info.isFile()) {
            files++;
        } else if(info.isDir()){
            dirs++;
        }
    }
    return std::make_pair(dirs, files);
}

qint64 fileutils::CalculateFileSizes(QList<QString>& file_paths) {
    qint64 file_size = 0;
    for(const QString& path : file_paths) {
        LARGE_INTEGER size;
        size.LowPart = 0;
        size.HighPart = 0;
        HANDLE h_file = CreateFileW(path.toStdWString().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(h_file == NULL || h_file == INVALID_HANDLE_VALUE) {
            qDebug() << "Cannot open " << path;
            continue;
        }
        GetFileSizeEx(h_file, &size);
        file_size += size.QuadPart;
        CloseHandle(h_file);
    }
    return file_size;
}

bool fileutils::IsDestSubOfSrc(QString dest, QString src)
{
    if(dest.startsWith(src))
    {
        return true;
    }
    return false;
}

bool fileutils::FileExists(QString file_path)
{
    QFileInfo info(file_path);
    if(info.exists())
    {
        return true;
    }
    return false;
}

qint64 copy_helper::PreCalculateEntrySize(QString file_path)
{
    QList<QString> files;
    FindFilesRecursively(file_path, files);
    qint64 sizes = fileutils::CalculateFileSizes(files);
    return sizes;
}

void copy_helper::FindFilesRecursively(QString current_path, QList<QString>& files)
{
    QFileInfo file_info(current_path);
    if(!file_info.isDir())
    {
        files.push_back(current_path);
        return;
    }
    QDirIterator iterator(current_path);
    QSet<QString> not_to_visit{(current_path + "\\" + "."), (current_path + "\\" + ".."), "", (current_path + "/" + ".."), (current_path + "/" + ".")};
    iterator.next();
    iterator.next();
    do {
        iterator.next();
        if(not_to_visit.contains(iterator.filePath()))
        {
            continue;
        }
        if(iterator.fileInfo().isDir())
        {
            FindFilesRecursively(iterator.filePath(), files);
        }
        else
        {
            files.push_back(iterator.filePath());
        }
    } while(iterator.hasNext());
}

namespace directory_utils
{
    bool CreateDir(const QString& directory_path)
    {
        bool res = CreateDirectoryW(directory_path.toStdWString().c_str(), NULL) == TRUE ? true : false;
        return res;
    }
}
