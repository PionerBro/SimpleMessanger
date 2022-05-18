#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include "userinfo.h"

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(const UserInfo& info, QWidget *parent = nullptr);
    ~SettingsWidget();

    void update();
    void set_status(bool status);
private slots:
    void on_foto_btn_clicked();
    void on_accept_changing_btn_clicked();
signals:
    void signal_accept_profile_changings(UserInfo);
private:
    UserInfo tmp_info_;
    const UserInfo& info_;
    Ui::SettingsWidget *ui;
};

#endif // SETTINGSWIDGET_H
