#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stack>
#include <QMainWindow>
#include <QFileSystemModel>
#include <QString>
#include <QListView>
#include <QTreeView>
#include <QItemSelection>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void CurrentPathChangedSignal(QString path);

    void DisableBackButtonSignal();

    void DisableForwardButtonSignal();

    void EnableBackButtonSignal();

    void EnableForwardButtonSignal();

    void StartOpeningProcess();

    void SelectedsChanged();

private slots:
    void CurrentPathChangedSlot(QString path);

    void ShowSelectedInfos();

    void SelectionChanged(const QItemSelection& selecteds, const QItemSelection& diselecteds);

    void OpenFile(QString file_path);

    void OpenFileOnContextMenu();

    void on_treeView_expanded(const QModelIndex &index);

    void on_treeView_clicked(const QModelIndex &index);

    void DisableBackButtonSlot();

    void DisableForwardButtonSlot();

    void on_backButton_clicked();

    void EnableBackButtonSlot();

    void EnableForwardButtonSlot();

    void on_forwardButton_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_lineEdit_editingFinished();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_listView_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    QFileSystemModel* tree_model;
    QFileSystemModel* list_model;
    std::stack<QString> previous_paths;
    std::stack<QString> forward_paths;
    QList<QModelIndex> selected_indexes;
    QString current_path;
    QString copy_cut_path;
    QString context_path;


    void SetListViewCurrentIndex();

    void SetTreeViewCurrentIndex();

    void SetListViewRoot();

    bool PathExists(QString path);

    void StyleTreeView();

    void AddToForwardPath(QString path);

    void AddToPreviousPath(QString path);

    void SetUpConnections();

    void NewPathInserted(QString new_path);

    bool IsDirectory(QString path);

    bool IsFile(QString path);

    int CreateContextListAndExecute(QString file_path);

    QString RetreivePreviousPath();

    QString RetreiveForwardPath();

    QString ShowFileOrDirectory(int files, int dirs);

    QString SetFileSizesText(qint64 sizes, int files);
};
#endif // MAINWINDOW_H
