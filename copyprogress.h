#ifndef COPYPROGRESS_H
#define COPYPROGRESS_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QTimer>

namespace Ui {
class CopyProgress;
}

class CopyProgress : public QDialog
{
    Q_OBJECT

public:
    explicit CopyProgress(QWidget *parent = nullptr);
    ~CopyProgress();

signals:
    void CopyEnded();

public slots:

    void copy_started(const QStringList& sources, const QString& dest, qint64 total_size);

    void copy_progress(const qint64& bytes_copied);

    void new_file_copy(const QString& filename);

private slots:
    void on_OK_clicked();

    void on_Cancel_clicked();

    void TimerTimeout();

private:
    Ui::CopyProgress *ui;
    QStringList sources;
    QTimer* timer;
    QString dest;
    QString current_file;
    qint64 total_size;
    qint64 copied;
    int copied_items;
    int seconds;


    void UpdateDialogLables();
    void UpdateDialogProgressBar();
    double CalculateRemainingTime();
    void SetRemainingTime();
};

#endif // COPYPROGRESS_H
