#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <QWidget>
#include "userinfo.h"

namespace Ui {
class ProfileWidget;
}

class ProfileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileWidget(const UserInfo& info, QWidget *parent = nullptr);
    ~ProfileWidget();

    void update();
    void update_image();
    void update_name();
    void update_friends();
    void setAvatar(QPixmap pixmap);
private slots:
    void on_friends_btn_clicked();
signals:
    void signal_on_friends_btn_clicked();
private:
    const UserInfo& user_info_;
    Ui::ProfileWidget *ui;
};

#endif // PROFILEWIDGET_H
