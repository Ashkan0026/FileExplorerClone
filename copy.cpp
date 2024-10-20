#include "copy.h"
#include <fstream>
#include <QFile>
#include <QDataStream>
#include <memory.h>

CopyOperation::CopyOperation(QString _source_path, QString _dest_path, qint64 _total_size)
    : source_path(_source_path), dest_path(_dest_path), total_size(_total_size)
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
    QDataStream in(&reader);
    QDataStream out(&writer);

    size_t buffer_length = 16348;
    char* buffer = new char[buffer_length];
    memset(buffer, 0, buffer_length);
    qint64 readed = 0;
    do
    {


    } while(!in.atEnd() && readed > 0);

}

bool CopyOperation::CopyDirectory(const QString &src_path, const QString &dst_path)
{

}
