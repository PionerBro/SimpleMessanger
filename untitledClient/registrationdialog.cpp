#include "registrationdialog.h"
#include "ui_registrationdialog.h"
#include <QRegularExpressionValidator>

RegistrationDialog::RegistrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationDialog)
{
    ui->setupUi(this);
    ui->login_edit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]{4,10}"), this));
    ui->pass_edit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]{4,10}"), this));
    ui->pass_repeat_edit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]{4,10}"), this));
    connect(ui->login_edit, SIGNAL(textChanged(const QString&)), this, SLOT(set_button_ok_enable()));
    connect(ui->pass_edit, SIGNAL(textChanged(const QString&)), this, SLOT(set_button_ok_enable()));
    connect(ui->pass_repeat_edit, SIGNAL(textChanged(const QString&)), this, SLOT(set_button_ok_enable()));
    connect(ui->name_edit, SIGNAL(textChanged(const QString&)), this, SLOT(set_button_ok_enable()));
    connect(ui->surname_edit, SIGNAL(textChanged(const QString&)), this, SLOT(set_button_ok_enable()));

}

RegistrationDialog::~RegistrationDialog()
{
    delete ui;
}

const QPushButton *RegistrationDialog::accept_btn() const
{
    return ui->accept_btn;
}

const QPushButton *RegistrationDialog::reject_btn() const
{
    return ui->reject_btn;
}

QString RegistrationDialog::get_login() const
{
    return ui->login_edit->text();
}

QString RegistrationDialog::get_pass() const
{
    return ui->pass_edit->text();
}

QString RegistrationDialog::get_repeat_pass() const
{
    return ui->pass_repeat_edit->text();
}

QString RegistrationDialog::get_name() const
{
    return ui->name_edit->text();
}

QString RegistrationDialog::get_surname() const
{
    return ui->surname_edit->text();
}

void RegistrationDialog::setErrorText(const QString &text)
{
    ui->error_lbl->setText(text);
}

void RegistrationDialog::set_button_ok_enable()
{
    ui->accept_btn->setEnabled(ui->login_edit->text().size()>=4 && ui->pass_edit->text().size()>=4 &&
        ui->pass_repeat_edit->text().size()>=4 && (ui->name_edit->text().size()>0 || ui->surname_edit->text().size()>0));
}
