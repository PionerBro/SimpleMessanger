#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "client.h"
#include "postrecipient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LoginDialog;
class RegistrationDialog;
class ProfileWidget;
class FriendsWidget;
class AllUsersWidget;
class RoomsList;
class CreateRoomDialog;
class RoomWidget;
class SettingsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void login();
private slots:
    void registration();
    void lost_connection();

    void login_answer(bool result);
    void registration_answer(bool result);
    void logout_answer(bool result);
    void friends_list_answer(std::map<uint64_t, std::string> friends);
    void users_list_answer(std::deque<std::pair<uint64_t, std::string>> users);
    void delete_friend_answer(uint64_t friend_id);
    void add_friend_answer(uint64_t friend_id, std::string name);
    void add_room_answer(uint64_t room_id, std::string room_name);
    void open_room_answer(uint64_t room_id, RoomInfo info);
    void send_message_answer(uint64_t room_id, std::string user_name, std::string msg);
    void change_profile_info_answer(bool result);

    void display_msg(std::string);
    void onSend_btn_clicked(uint64_t room_id, std::string msg);
    void onLogin_btn_clicked();
    void onReg_btn_clicked();
    void on_logout_button_clicked();
    void on_friends_button_clicked();
    void on_profile_button_clicked();
    void on_rooms_button_clicked();
    void on_settings_button_clicked();
    void onCreate_room_button_clicked();
    void onCreate_room(std::string, std::vector<uint64_t>);
    void onOpen_room(uint64_t);
    void onUsers_btn_clicked();
    void onDelete_friend_btn_clicked(uint64_t id);
    void onAdd_friend_btn_clicked(uint64_t);
    void load_image(std::string, uint64_t, uint64_t,uint64_t);
    void accept_profile_changings(UserInfo info);

    void on_createImage_clicked();
    void on_loadImage_clicked();
    void on_pressMe_clicked();
signals:

private:
    PostRecipient recipient_;
    std::unique_ptr<SettingsWidget> settings_widget_ = nullptr;
    std::unique_ptr<RoomWidget> room_widget_ = nullptr;
    std::unique_ptr<CreateRoomDialog> create_room_dialog_ = nullptr;
    std::unique_ptr<RoomsList> rooms_list_ = nullptr;
    std::unique_ptr<AllUsersWidget> all_users_ = nullptr;
    std::unique_ptr<FriendsWidget> friends_ = nullptr;
    std::unique_ptr<ProfileWidget> profile_ = nullptr;
    std::unique_ptr<RegistrationDialog> reg_dialog_ = nullptr;
    std::unique_ptr<LoginDialog> login_dialog_ = nullptr;
    uchar* ptr = nullptr;
    std::string s;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
