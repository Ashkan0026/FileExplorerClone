#ifndef OPENINGFILE_H
#define OPENINGFILE_H
#include <QString>
#include <QUrl>
#include <QObject>

class OpeningFile : public QObject
{
    Q_OBJECT
public:
    OpeningFile(QString _file_path);

    ~OpeningFile();
signals:
    void FileOpeningEnded();
public slots:
    void StartProcess();

private:
    QString file_path;
    bool opening_result;
};

#endif // OPEN_H
