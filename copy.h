#ifndef COPY_H
#define COPY_H
#include <QObject>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QStringList>

class CopyOperation : QObject
{
    Q_OBJECT
public:
    CopyOperation(QStringList sources, QString dest_path, qint64 total_size);

    virtual ~CopyOperation();

    void StartTheOperation();

signals:
    void copy_started(const QString& source, const QString& dest, const qint64& total_size);

    void copy_progress(const qint64& bytes_copied);

    void file_copy_ended(const QString& filePath);

public slots:
    void start_copying();


private:
    QStringList sources;
    QString dest_path;
    qint64 total_size;

    bool CopyDirectory(const QString& src_path, const QString& dst_path);

    bool CopyFile(const QString& file_source_path, const QString& file_dest_path);

    QString CreateDestDirectory(const QString& src_path, const QString& dest_path);

};

#endif // COPY_H
