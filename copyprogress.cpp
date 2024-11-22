#include "copyprogress.h"
#include "ui_copyprogress.h"
#include <QDebug>
#include <QString>

CopyProgress::CopyProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyProgress)
{
    ui->setupUi(this);
    ui->copy_bar->setValue(0);
}

void CopyProgress::copy_started(const QStringList& _sources, const QString& _dest, qint64 _total_size)
{
    this->sources = _sources;
    this->dest = _dest;
    this->total_size = _total_size;
    this->copied_items = 0;
    this->copied = 0;
    this->timer = new QTimer();
    timer->setInterval(1000);
    ui->OK->setDisabled(true);

    UpdateDialogLables();
}

void CopyProgress::copy_progress(const qint64& bytes_copied)
{
    copied += bytes_copied;

    UpdateDialogLables();
    UpdateDialogProgressBar();
}

void CopyProgress::new_file_copy(const QString& filename)
{
    current_file = filename;
    this->copied_items++;

    UpdateDialogLables();
}

CopyProgress::~CopyProgress()
{
    qDebug() << "Copy progress destructor";
    delete ui;
}

void CopyProgress::on_OK_clicked()
{

}

void CopyProgress::on_Cancel_clicked()
{

}


void CopyProgress::UpdateDialogLables()
{
    ui->currentFile->setText("Currently Copying " + current_file);
    ui->copied_items->setText(QString::number(copied_items) + " items copied");
    SetRemainingTime();
}

void CopyProgress::SetRemainingTime()
{
    double remaining_time = CalculateRemainingTime();
    ui->remainingTime->setText("Remaining Time: " + QString::number(remaining_time));
}

void CopyProgress::UpdateDialogProgressBar()
{
    qint64 percent = (copied * 100) / total_size;
    ui->copy_bar->setValue(static_cast<int>(percent));
}

void CopyProgress::TimerTimeout()
{
    seconds++;
    SetRemainingTime();
}

double CopyProgress::CalculateRemainingTime()
{
    double remaining_bytes = static_cast<double>(total_size - copied);
    double remaining_time = (remaining_bytes * static_cast<double>(seconds)) / static_cast<double>(copied);
    return remaining_time;
}
