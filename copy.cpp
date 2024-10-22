#include "copy.h"
#include <fstream>
#include <QFile>
#include <QDataStream>
#include <QDirIterator>
#include <memory.h>
#include "fileutils.h"

CopyOperation::CopyOperation(QStringList _sources, QString _dest_path, qint64 _total_size)
    : sources(_sources), dest_path(_dest_path), total_size(_total_size)
{

}

CopyOperation::~CopyOperation()
{

}

void CopyOperation::StartTheOperation()
{

}

bool CopyOperation::CopyFile(const QString &file_source_path, const QString &file_dest_path)
{
    QFile reader(file_source_path);
    QFile writer(file_dest_path);
    if(!reader.open(QIODevice::ReadOnly))
    {
        return false;
    }
    if(!writer.open(QIODevice::WriteOnly))
    {
        return false;
    }
    const size_t buffer_length = 16348;
    char buffer[buffer_length];
    memset(buffer, 0, buffer_length);
    qint64 readed = 0;
    do
    {
        readed = reader.read(buffer, buffer_length);
        writer.write(buffer, readed);
    } while(!reader.atEnd() && readed > 0);

    reader.close();
    writer.close();

    return true;
}

bool CopyOperation::CopyDirectory(const QString &src_path, const QString &dst_path)
{
    // If the source path is not directory use the file copy method
    if(!directory_utils::IsDirectory(src_path))
    {
        return CopyFile(src_path, dst_path);
    }
    // The First Time directory should be created
    QString new_dest = CreateDestDirectory(src_path, dst_path);

    QDirIterator iterator(src_path);
    QSet<QString> not_to_visit{(src_path + "\\" + "."), (src_path + "\\" + ".."), "", (src_path + "/" + ".."), (src_path + "/" + ".")};
    iterator.next();
    iterator.next();

    do
    {
        iterator.next();
        QString current_source_path = iterator.filePath();
        if (not_to_visit.find(current_source_path) != not_to_visit.end())
        {
            continue;
        }
        QString new_dest_path = new_dest + "\\" + fileutils::GetFileName(current_source_path);
        qDebug() << new_dest_path << '\n' << current_source_path << '\n';
        if(directory_utils::IsDirectory(current_source_path))
        {
            if(!directory_utils::CreateDir(new_dest_path))
            {
                qDebug() << "Cannot create directory " << new_dest_path;
                break;
            }
            CopyDirectory(current_source_path, new_dest_path);
        }
        else
        {
            CopyFile(current_source_path, new_dest_path);
        }
        not_to_visit.insert(current_source_path);
    } while(iterator.hasNext());


    return true;
}

void CopyOperation::start_copying()
{
    for(const QString& source: sources)
    {
        CopyDirectory(source, dest_path);
    }
}

QString CopyOperation::CreateDestDirectory(const QString& src_path, const QString& dest_path)
{
    QString source_file_name = fileutils::GetFileName(src_path);
    QString dest_file_name = fileutils::GetFileName(dest_path);
    if(source_file_name != dest_file_name)
    {
        directory_utils::CreateDir(dest_path + "\\" + source_file_name);
        return dest_path + "\\" + source_file_name;
    }
    return dest_path;

}
