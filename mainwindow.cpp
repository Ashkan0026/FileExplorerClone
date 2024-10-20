#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QList>
#include <QFileInfo>
#include <QAbstractItemModel>
#include <QDebug>
#include <iostream>
#include <math.h>
#include <QObject>
#include <QModelIndexList>
#include <QAction>
#include <QThread>
#include <QCursor>
#include "open.h"
#include "fileutils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setSizes(QList<int>() << 20 << 140);
    current_path = "";
    list_model = new QFileSystemModel();
    tree_model = new QFileSystemModel();
    list_model->setRootPath(current_path);
    tree_model->setRootPath(current_path);
    ui->listView->setModel(list_model);
    ui->treeView->setModel(tree_model);
    SetListViewRoot();
    SetTreeViewCurrentIndex();
    StyleTreeView();
    SetUpConnections();
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::SelectionChanged);
    emit DisableBackButtonSignal();
    emit DisableForwardButtonSignal();
}

void MainWindow::SetListViewCurrentIndex() {
    ui->listView->setCurrentIndex(list_model->index(current_path));
}

void MainWindow::SetTreeViewCurrentIndex() {
    ui->treeView->setCurrentIndex(tree_model->index(current_path));
}

void MainWindow::SetListViewRoot() {
    ui->listView->setRootIndex(list_model->index(current_path));
}

void MainWindow::CurrentPathChangedSlot(QString path) {
    if(PathExists(path) || path == "") {
        current_path = path;
        SetListViewRoot();
        SetTreeViewCurrentIndex();
        selected_indexes.clear();
    }
}

void MainWindow::SelectionChanged(const QItemSelection& selecteds, const QItemSelection& diselecteds) {
    QModelIndexList indexes = selecteds.indexes();
    QModelIndexList disselected_list = diselecteds.indexes();
    for(const QModelIndex& index : indexes) {
        selected_indexes.push_back(index);
    }
    for(const QModelIndex& index : disselected_list) {
        selected_indexes.removeOne(index);
    }
    emit SelectedsChanged();
}

bool MainWindow::PathExists(QString path) {
    QFileInfo info(path);
    if(!info.exists()) {
        return false;
    }
    return true;
}

void MainWindow::StyleTreeView() {
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);

}

void MainWindow::AddToForwardPath(QString path) {
    emit EnableForwardButtonSignal();
    this->forward_paths.push(path);
}

void MainWindow::AddToPreviousPath(QString path) {
    emit EnableBackButtonSignal();
    this->previous_paths.push(path);
}

QString MainWindow::RetreiveForwardPath() {
    if(!forward_paths.empty()) {
        QString path = forward_paths.top();
        forward_paths.pop();
        if(forward_paths.empty()) {
            emit DisableForwardButtonSignal();
        }
        return path;
    } else {
        emit DisableForwardButtonSignal();
        return "$";
    }
}

QString MainWindow::RetreivePreviousPath() {
    if(!previous_paths.empty()) {
        QString path = previous_paths.top();
        previous_paths.pop();
        if(previous_paths.empty()) {
            emit DisableBackButtonSignal();
        }
        return path;
    } else {
        emit DisableBackButtonSignal();
        return "$";
    }
}

void MainWindow::DisableBackButtonSlot() {
    ui->backButton->setDisabled(true);
}

void MainWindow::DisableForwardButtonSlot() {
    ui->forwardButton->setDisabled(true);
}

void MainWindow::EnableBackButtonSlot() {
    ui->backButton->setEnabled(true);
}

void MainWindow::EnableForwardButtonSlot() {
    ui->forwardButton->setEnabled(true);
}

void MainWindow::SetUpConnections() {
    connect(this, &MainWindow::CurrentPathChangedSignal, this, &MainWindow::CurrentPathChangedSlot);
    connect(this, &MainWindow::DisableBackButtonSignal, this, &MainWindow::DisableBackButtonSlot);
    connect(this, &MainWindow::DisableForwardButtonSignal, this, &MainWindow::DisableForwardButtonSlot);
    connect(this, &MainWindow::EnableBackButtonSignal, this, &MainWindow::EnableBackButtonSlot);
    connect(this, &MainWindow::EnableForwardButtonSignal, this, &MainWindow::EnableForwardButtonSlot);
    connect(this, &MainWindow::SelectedsChanged, this, &MainWindow::ShowSelectedInfos);
}

void MainWindow::ShowSelectedInfos() {
    QList<QString> paths;
    for(const QModelIndex& selected : selected_indexes) {
        paths.push_back(list_model->filePath(selected));
    }
    std::pair<int, int> files_and_dirs = fileutils::NumberOfFilesAndDirectories(paths);
    int dirs = files_and_dirs.first, files = files_and_dirs.second;
    if(dirs == 0 && files > 0) {
        qint64 file_sizes = fileutils::CalculateFileSizes(paths);
        ui->size_and_files->setText(SetFileSizesText(file_sizes, files));
    } else {
        QString res = ShowFileOrDirectory(files, dirs);
        ui->size_and_files->setText(res);
    }
}

QString MainWindow::SetFileSizesText(qint64 sizes, int files) {
    QString res = "";
    res += "Files : " + QString::number(files) + ", Sizes : ";
    qint64 gigabytes = static_cast<qint64>(std::pow(10, 9));
    qint64 megabytes = static_cast<qint64>(std::pow(10, 6));
    qint64 kilobytes = static_cast<qint64>(std::pow(10, 3));
    QString file_size = "";
    while(sizes > 0) {
        if(sizes > gigabytes) {
            qint64 size = sizes / gigabytes;
            sizes = sizes % gigabytes;
            file_size += QString::number(size) + "GB, ";
        } else if(sizes > megabytes) {
            qint64 size = sizes / megabytes;
            sizes = sizes % megabytes;
            file_size += QString::number(size) + "MB, ";
        } else if(sizes > kilobytes) {
            qint64 size = sizes / kilobytes;
            sizes = sizes % kilobytes;
            file_size += QString::number(size) + "KB, ";
        } else {
            file_size += QString::number(sizes) + "B, ";
            sizes = 0;
        }
    }
    return res + (file_size.left(file_size.length() - 2));
}

QString MainWindow::ShowFileOrDirectory(int files, int dirs) {
    QString result = "";
    if(files == 1) {
        result += "File : " + QString::number(files);
    } else if(files > 1) {
        result += "Files : " + QString::number(files);
    }
    if(dirs == 1) {
        if(result.size() > 0) {
            result += ", Directory : " + QString::number(dirs);
        } else {
            result += "Directory : " + QString::number(dirs);
        }
    } else if(dirs > 1) {
        if(result.size() > 0) {
            result += ", Directories : " + QString::number(dirs);
        } else {
            result += "Directories : " + QString::number(dirs);
        }
    }
    return result;
}

void MainWindow::NewPathInserted(QString new_path) {
    if(IsDirectory(new_path)) {
        AddToPreviousPath(current_path);
        emit CurrentPathChangedSignal(new_path);
    } else if(IsFile(new_path)){
        OpeningFile* open_file = new OpeningFile(new_path);
        QThread* thread = new QThread();

        connect(this, &MainWindow::StartOpeningProcess, open_file, &OpeningFile::StartProcess);
        connect(open_file, &OpeningFile::FileOpeningEnded, thread, &QThread::quit);
        connect(open_file, &OpeningFile::FileOpeningEnded, open_file, &OpeningFile::deleteLater);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);
        open_file->moveToThread(thread);
        thread->start();
        emit StartOpeningProcess();
    } else {

    }
}

bool MainWindow::IsFile(QString path) {
    QFileInfo info(path);
    if(info.isFile()) {
        return true;
    } else {
        return false;
    }
}

bool MainWindow::IsDirectory(QString path) {
    QFileInfo info(path);
    if(info.isDir()) {
        return true;
    } else {
        return false;
    }
}

void MainWindow::OpenFile(QString file_path) {
    NewPathInserted(file_path);
}

void MainWindow::OpenFileOnContextMenu() {
    OpenFile(context_path);
}

int MainWindow::CreateContextListAndExecute(QString file_path) {
    QMenu context_menu(this);
    QAction open_action("Open", this);
    context_path = file_path;
    connect(&open_action, &QAction::triggered, this, &MainWindow::OpenFileOnContextMenu);

    QAction copy_action("Copy", this);
    //connect(&copy_action,);
    QAction cut_action("Cut", this);

    QAction paste_action("Delete", this);

    context_menu.addAction(&open_action);
    context_menu.addAction(&copy_action);
    context_menu.addAction(&cut_action);
    context_menu.addAction(&paste_action);

    context_menu.exec(QCursor::pos());
    return 0;
}


MainWindow::~MainWindow()
{
    delete tree_model;
    delete list_model;
    delete ui;
}

void MainWindow::on_treeView_expanded(const QModelIndex &index)
{
    QString path = tree_model->filePath(index);
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString path = tree_model->filePath(index);
    NewPathInserted(path);
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QString path = list_model->filePath(index);
    NewPathInserted(path);
}

void MainWindow::on_backButton_clicked()
{
    QString new_path = RetreivePreviousPath();
    if(new_path == "$") {
        return;
    }
    AddToForwardPath(current_path);
    emit CurrentPathChangedSignal(new_path);
}

void MainWindow::on_forwardButton_clicked()
{
    QString new_path = RetreiveForwardPath();
    if(new_path == "$") {
        return;
    }
    AddToPreviousPath(current_path);
    emit CurrentPathChangedSignal(new_path);
}


void MainWindow::on_lineEdit_editingFinished()
{
    QString path = ui->lineEdit->text();
    NewPathInserted(path);
}

void MainWindow::on_actionCopy_triggered()
{

}

void MainWindow::on_actionCut_triggered()
{

}

void MainWindow::on_actionPaste_triggered()
{

}

void MainWindow::on_listView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->listView->indexAt(pos);
    QString file_path = list_model->filePath(index);
    CreateContextListAndExecute(file_path);
}
