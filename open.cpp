#include "open.h"
#include <QDesktopServices>
#include <QDebug>
#include <QMimeType>
#include <QMimeDatabase>
#include <QProcess>
#include <QStringList>
#include <QDir>

OpeningFile::OpeningFile(QString _file_path)
    : file_path(_file_path)
{}

OpeningFile::~OpeningFile() {
    qDebug() << "OpenFile destructor" << '\n';
}

void OpeningFile::StartProcess() {
    QMimeDatabase mime_database;
    QMimeType mime_type = mime_database.mimeTypeForFile(file_path);

    if(mime_type.isValid()) {
        qDebug() << "MIME type : " << mime_type.name();

        QProcess::startDetached("explorer", QStringList() << QDir::toNativeSeparators(file_path));
    }

    emit FileOpeningEnded();
}
