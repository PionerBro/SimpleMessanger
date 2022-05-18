#ifndef FRIENDSWIDGET_H
#define FRIENDSWIDGET_H

#include <QWidget>
#include <memory>

class FriendsModel;
class ButtonDelegate;
class PlugDelegate;

namespace Ui {
class FriendsWidget;
}

class FriendsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FriendsWidget(QWidget *parent = nullptr);
    ~FriendsWidget();

    void update(const std::map<uint64_t, std::string>& friends_map);
    void add_friend(uint64_t friend_id, const std::string& name);
    void delete_friend(uint64_t friend_id);
private slots:
    void try_delete_friend(uint64_t id);
    void on_all_users_btn_clicked();
signals:
    void signal_delete_friend(uint64_t id);
    void signal_all_users_btn_clicked();
private:
    std::unique_ptr<PlugDelegate> plug_delegate_ = nullptr;
    std::unique_ptr<ButtonDelegate> button_delegate_ = nullptr;
    std::unique_ptr<FriendsModel> model_ = nullptr;
    Ui::FriendsWidget *ui;
};

#endif // FRIENDSWIDGET_H
