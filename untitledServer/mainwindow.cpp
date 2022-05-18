#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <functional>

using namespace std::placeholders;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&manager_, SIGNAL(write_msg(uint64_t, std::string)), this, SLOT(appendMessage(uint64_t, std::string)));
    setWindowTitle("Server");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendMessage(uint64_t id, std::string msg)
{
    ui->text_edit->appendPlainText("User " + QString::number(id) + ": " + QString::fromStdString(msg) + "\n");
}

void MainWindow::on_pushButton_close_clicked()
{
    close();
}

