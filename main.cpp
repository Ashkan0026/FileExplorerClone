#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QList>
#include <QString>
#include <QSet>
#include <QFileInfo>
#include <QStringList>
#include <QDirIterator>
#include "fileutils.h"
#include "converter.h"
#include "copy.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QStringList selecteds = {"A:\\courses\\C++\\Videos"};
    QString destination = "C:\\Users\\Ashkan\\Documents\\temp";

    CopyOperation copy_operation(selecteds, destination, 0);
    copy_operation.start_copying();

    return a.exec();
}
