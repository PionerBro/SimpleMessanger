#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "postmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void write(uint64_t id, const QString& msg);
    void accept_client(uint64_t id);
    void read_msg_from_client(uint64_t id, const char* ptr, uint64_t size);
private slots:
    void appendMessage(uint64_t id, std::string msg);
    void on_pushButton_close_clicked();
signals:
    void signal_accept_client(uint64_t);
    void signal_read_msg_from_client(QString);
private:
    PostManager manager_;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
