#ifndef ROOMSLIST_H
#define ROOMSLIST_H

#include <QWidget>

namespace Ui {
class RoomsList;
}

class ButtonDelegate;
class FriendsModel;

class RoomsList : public QWidget
{
    Q_OBJECT

public:
    explicit RoomsList(QWidget *parent = nullptr);
    ~RoomsList();

    void update(const std::map<uint64_t, std::string>& rooms_map);
    void add_room(uint64_t room_id, const std::string& room_name);
private slots:
    void try_open_room(uint64_t room_id);
    void on_create_room_btn_clicked();
signals:
    void signal_open_room(uint64_t room_id);
    void signal_create_room_btn_clicked();
private:
    std::unique_ptr<ButtonDelegate> button_delegate_ = nullptr;
    std::unique_ptr<FriendsModel> model_ = nullptr;
    Ui::RoomsList *ui;
};

#endif // ROOMSLIST_H
