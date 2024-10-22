#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QString>

namespace fileutils
{
    std::pair<int, int> NumberOfFilesAndDirectories(QList<QString>& file_paths);

    qint64 CalculateFileSizes(QList<QString>& file_paths);

    bool IsDestSubOfSrc(QString dest, QString src);

    bool FileExists(QString file_path);

    QString GetFileName(const QString& path);
}

namespace copy_helper
{
    qint64 PreCalculateEntrySize(QString file_path);

    void FindFilesRecursively(QString current_path, QList<QString>& files);
}

namespace directory_utils
{
    bool CreateDir(const QString& dir_path);

    bool IsDirectory(const QString& path);
}


#endif // FILEUTILS_H
