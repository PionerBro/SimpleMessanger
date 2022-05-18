#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class QLabel;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    const QPushButton* reg_btn()const;
    const QPushButton* accept_btn()const;
    const QPushButton* reject_btn()const;
    QString get_login()const;
    QString get_password()const;
    void setErrorText(const QString& text);
private slots:
    void set_button_ok_enable();
private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
