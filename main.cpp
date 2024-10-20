#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QList>
#include <QString>
#include <QSet>
#include <QFileInfo>
#include <QDirIterator>
#include "fileutils.h"
#include "converter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QString path = "A:\\courses\\C++";

    qint64 sizes = copy_helper::PreCalculateEntrySize(path);
    qDebug() << "Sub of " << path << " sizes: " << sizes << '\n';
    qint64 gigabyte = size_converter::ExtractGigabytes(sizes);
    qDebug() << "Size in gigabyte: " << gigabyte;


    return a.exec();
}
