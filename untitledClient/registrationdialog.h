#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>

namespace Ui {
class RegistrationDialog;
}

class RegistrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationDialog(QWidget *parent = nullptr);
    ~RegistrationDialog();


    const QPushButton* accept_btn()const;
    const QPushButton* reject_btn()const;
    QString get_login()const;
    QString get_pass()const;
    QString get_repeat_pass()const;
    QString get_name()const;
    QString get_surname()const;
    void setErrorText(const QString& text);
private slots:
    void set_button_ok_enable();
private:
    Ui::RegistrationDialog *ui;
};

#endif // REGISTRATIONDIALOG_H
