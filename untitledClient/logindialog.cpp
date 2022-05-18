#include "logindialog.h"
#include "ui_logindialog.h"
#include <QRegularExpressionValidator>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->login_edit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]{4,10}"), this));
    ui->pass_edit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]{4,10}"), this));
    connect(ui->login_edit, SIGNAL(textChanged(const QString&)), this, SLOT(set_button_ok_enable()));
    connect(ui->pass_edit, SIGNAL(textChanged(const QString&)), this, SLOT(set_button_ok_enable()));
    connect(ui->accept_btn, SIGNAL(clicked()), ui->error_label, SLOT(clear()));
    connect(ui->reject_btn, SIGNAL(clicked()), this, SLOT(reject()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

const QPushButton *LoginDialog::accept_btn() const
{
    return ui->accept_btn;
}

const QPushButton *LoginDialog::reg_btn() const
{
    return ui->reg_btn;
}

const QPushButton *LoginDialog::reject_btn() const
{
    return ui->reject_btn;
}

QString LoginDialog::get_login() const
{
    return ui->login_edit->text();
}

QString LoginDialog::get_password() const
{
    return ui->pass_edit->text();
}

void LoginDialog::setErrorText(const QString &text)
{
    ui->error_label->setText(text);
}

void LoginDialog::set_button_ok_enable()
{
    ui->accept_btn->setEnabled(ui->login_edit->text().size()>=4 && ui->pass_edit->text().size()>=4);
}
