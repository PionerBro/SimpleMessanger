#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "registrationdialog.h"
#include "profilewidget.h"
#include "friendswidget.h"
#include "alluserswidget.h"
#include "roomslist.h"
#include "createroomdialog.h"
#include "roomwidget.h"
#include "settingswidget.h"

#include <memory>
#include <functional>
#include <QMessageBox>
#include <QLabel>
#include <QApplication>

#include <QImage>

using namespace std::placeholders;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      settings_widget_(std::make_unique<SettingsWidget>(recipient_.getUserInfo())),
      room_widget_(std::make_unique<RoomWidget>()),
      rooms_list_(std::make_unique<RoomsList>()),
      all_users_(std::make_unique<AllUsersWidget>()),
      friends_(std::make_unique<FriendsWidget>()),
      profile_(std::make_unique<ProfileWidget>(recipient_.getUserInfo())),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stack_widget->addWidget(profile_.get());
    ui->stack_widget->addWidget(friends_.get());
    ui->stack_widget->addWidget(all_users_.get());
    ui->stack_widget->addWidget(rooms_list_.get());
    ui->stack_widget->addWidget(room_widget_.get());
    ui->stack_widget->addWidget(settings_widget_.get());

    connect(&recipient_, SIGNAL(signal_write_msg(std::string)), this, SLOT(display_msg(std::string)));
    connect(&recipient_, SIGNAL(signal_logout_answer(bool)), this, SLOT(logout_answer(bool)));
    connect(&recipient_, SIGNAL(signal_login_answer(bool)), this, SLOT(login_answer(bool)));
    connect(&recipient_, SIGNAL(signal_friends_list_answer(std::map<uint64_t, std::string>)), this, SLOT(friends_list_answer(std::map<uint64_t, std::string>)));
    connect(&recipient_, SIGNAL(signal_users_list_answer(std::deque<std::pair<uint64_t, std::string>>)), this, SLOT(users_list_answer(std::deque<std::pair<uint64_t, std::string>>)));
    connect(&recipient_, SIGNAL(signal_registration_answer(bool)), this, SLOT(registration_answer(bool)));
    connect(&recipient_, SIGNAL(signal_delete_friend_answer(uint64_t)), this, SLOT(delete_friend_answer(uint64_t)));
    connect(&recipient_, SIGNAL(signal_add_friend_answer(uint64_t, std::string)), this, SLOT(add_friend_answer(uint64_t, std::string)));
    connect(&recipient_, SIGNAL(signal_add_room_answer(uint64_t, std::string)), this, SLOT(add_room_answer(uint64_t, std::string)));
    connect(&recipient_, SIGNAL(signal_open_room_answer(uint64_t, RoomInfo)), this, SLOT(open_room_answer(uint64_t, RoomInfo)));
    connect(&recipient_, SIGNAL(signal_send_message_answer(uint64_t, std::string, std::string)), this, SLOT(send_message_answer(uint64_t, std::string, std::string)));
    connect(&recipient_, SIGNAL(signal_change_profile_info_answer(bool)), this, SLOT(change_profile_info_answer(bool)));
    connect(&recipient_, SIGNAL(signal_lost_connection()), this, SLOT(lost_connection()));
    //connect(&recipient_, SIGNAL(signal_load_image(std::string, uint64_t, uint64_t,uint64_t)), this, SLOT(load_image(std::string,uint64_t, uint64_t, uint64_t)));

    connect(settings_widget_.get(), SIGNAL(signal_accept_profile_changings(UserInfo)), this, SLOT(accept_profile_changings(UserInfo)));
    connect(profile_.get(), SIGNAL(signal_on_friends_btn_clicked()), this, SLOT(on_friends_button_clicked()));
    connect(room_widget_.get(), SIGNAL(signal_send_btn_clicked(uint64_t, std::string)), this, SLOT(onSend_btn_clicked(uint64_t, std::string)));
    connect(rooms_list_.get(), SIGNAL(signal_open_room(uint64_t)),this, SLOT(onOpen_room(uint64_t)));
    connect(rooms_list_.get(), SIGNAL(signal_create_room_btn_clicked()), this, SLOT(onCreate_room_button_clicked()));
    connect(friends_.get(), SIGNAL(signal_all_users_btn_clicked()), this, SLOT(onUsers_btn_clicked()));
    connect(friends_.get(), SIGNAL(signal_delete_friend(uint64_t)), this, SLOT(onDelete_friend_btn_clicked(uint64_t)));
    connect(all_users_.get(), SIGNAL(signal_add_friend(uint64_t)), this, SLOT(onAdd_friend_btn_clicked(uint64_t)));
    login();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::login()
{
    login_dialog_ = std::make_unique<LoginDialog>(this);
    connect(login_dialog_->accept_btn(), SIGNAL(clicked()), this, SLOT(onLogin_btn_clicked()));
    connect(login_dialog_->reg_btn(), SIGNAL(clicked()), this, SLOT(registration()));
    connect(login_dialog_.get(), SIGNAL(rejected()), qApp, SLOT(quit()));
    login_dialog_->show();
}

void MainWindow::registration()
{
    reg_dialog_ = std::make_unique<RegistrationDialog>(this);
    connect(reg_dialog_->accept_btn(), SIGNAL(clicked()), this, SLOT(onReg_btn_clicked()));
    connect(reg_dialog_->reject_btn(), SIGNAL(clicked()), reg_dialog_.get(), SLOT(reject()));
    if(!reg_dialog_->exec())
        reg_dialog_ = nullptr;
}

void MainWindow::lost_connection()
{
    QMessageBox::critical(this, "Ошибка", "Потеряно соединение с сервером!");
}

void MainWindow::login_answer(bool result)
{
    if(result){
        login_dialog_->accept();
        profile_->update();
        rooms_list_->update(recipient_.user_rooms());
        on_profile_button_clicked();
        show();
        login_dialog_ = nullptr;
    }
    else{
        login_dialog_->setErrorText("Неверный логин или пароль!");
    }

}

void MainWindow::registration_answer(bool result)
{
    if(result){
        reg_dialog_->accept();
        reg_dialog_ = nullptr;
    }else{
        reg_dialog_->setErrorText("Такой логин уже существует");
    }
}

void MainWindow::logout_answer(bool result)
{
    if(result){
        hide();
        login();
    }else{

    }
}

void MainWindow::friends_list_answer(std::map<uint64_t, std::string> friends)
{
    friends_->update(friends);
}

void MainWindow::users_list_answer(std::deque<std::pair<uint64_t, std::string>> users)
{
    all_users_->update(users);
    ui->stack_widget->setCurrentWidget(all_users_.get());
}

void MainWindow::delete_friend_answer(uint64_t friend_id)
{
    friends_->delete_friend(friend_id);
    profile_->update();
}

void MainWindow::add_friend_answer(uint64_t friend_id, std::string name)
{
    all_users_->delete_user(friend_id);
    friends_->add_friend(friend_id, name);
    profile_->update();
}

void MainWindow::add_room_answer(uint64_t room_id, std::string room_name)
{
    rooms_list_->add_room(room_id, room_name);
    if(create_room_dialog_)
        create_room_dialog_->accept();
}

void MainWindow::open_room_answer(uint64_t room_id, RoomInfo info)
{
    room_widget_->update(room_id, info);
    ui->stack_widget->setCurrentWidget(room_widget_.get());
}

void MainWindow::send_message_answer(uint64_t room_id, std::string user_name, std::string msg)
{
    room_widget_->add_message(room_id, user_name, msg);
}

void MainWindow::change_profile_info_answer(bool result)
{
    settings_widget_->set_status(result);
}

void MainWindow::display_msg(std::string msg)
{
    ui->text_edit->appendPlainText(QString::fromStdString(msg));
}

void MainWindow::onSend_btn_clicked(uint64_t room_id, std::string msg)
{
    recipient_.send_message_request(room_id, msg);
}

void MainWindow::onLogin_btn_clicked()
{
    std::string login = login_dialog_->get_login().toStdString();
    std::string password = login_dialog_->get_password().toStdString();
    recipient_.login_request(login, password);
}

void MainWindow::onReg_btn_clicked()
{
    if(reg_dialog_->get_pass() != reg_dialog_->get_repeat_pass()){
        reg_dialog_->setErrorText("Пароли не совпадают!");
        return;
    }
    UserInfo info;
    info.set_login(reg_dialog_->get_login().toStdString());
    info.set_password(reg_dialog_->get_pass().toStdString());
    info.set_name(reg_dialog_->get_name().toStdString());
    info.set_surname(reg_dialog_->get_surname().toStdString());
    recipient_.registration_request(info);
}

void MainWindow::on_logout_button_clicked()
{
    recipient_.logout_request();
}


void MainWindow::on_friends_button_clicked()
{
    ui->stack_widget->setCurrentWidget(friends_.get());
}

void MainWindow::on_profile_button_clicked()
{
    profile_->update();
    ui->stack_widget->setCurrentWidget(profile_.get());
}

void MainWindow::on_rooms_button_clicked()
{
    ui->stack_widget->setCurrentWidget(rooms_list_.get());
}

void MainWindow::on_settings_button_clicked()
{
    settings_widget_->update();
    ui->stack_widget->setCurrentWidget(settings_widget_.get());
}

void MainWindow::onCreate_room_button_clicked()
{
    create_room_dialog_ = std::make_unique<CreateRoomDialog>(recipient_.user_friends(), this);
    connect(create_room_dialog_.get(), SIGNAL(signal_create_room(std::string, std::vector<uint64_t>)), this, SLOT(onCreate_room(std::string, std::vector<uint64_t>)));
    if(!create_room_dialog_->exec())
        create_room_dialog_ = nullptr;
}

void MainWindow::onCreate_room(std::string name, std::vector<uint64_t> members_id)
{
    recipient_.create_room_request(name, members_id);
}

void MainWindow::onOpen_room(uint64_t room_id)
{
    recipient_.open_room_request(room_id);
}

void MainWindow::onUsers_btn_clicked()
{
    recipient_.users_list_request();
}

void MainWindow::onDelete_friend_btn_clicked(uint64_t id)
{
    recipient_.delete_friend_request(id);
}

void MainWindow::onAdd_friend_btn_clicked(uint64_t id)
{
    recipient_.add_friend_request(id);
}

void MainWindow::accept_profile_changings(UserInfo info)
{
    recipient_.change_profile_info_request(info);
}




