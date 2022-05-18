#ifndef ALLUSERSWIDGET_H
#define ALLUSERSWIDGET_H

#include <QWidget>
#include <memory>
#include <deque>

namespace Ui {
class AllUsersWidget;
}

class FriendsModel;
class ButtonDelegate;
class PlugDelegate;

class AllUsersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AllUsersWidget(QWidget *parent = nullptr);
    ~AllUsersWidget();
    void update(const std::deque<std::pair<uint64_t, std::string>>& users_deque);
    void delete_user(uint64_t id);
private slots:
    void try_add_friend(uint64_t id);
signals:
    void signal_add_friend(uint64_t id);
private:
    std::unique_ptr<PlugDelegate> plug_delegate_ = nullptr;
    std::unique_ptr<ButtonDelegate> button_delegate_ = nullptr;
    std::unique_ptr<FriendsModel> model_ = nullptr;
    Ui::AllUsersWidget *ui;
};

#endif // ALLUSERSWIDGET_H
