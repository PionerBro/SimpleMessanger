#ifndef CREATEROOMDIALOG_H
#define CREATEROOMDIALOG_H

#include <QDialog>
#include <deque>

namespace Ui {
class CreateRoomDialog;
}

class FriendsModel;

class CreateRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateRoomDialog(const std::map<uint64_t, std::string> &friends_map, QWidget *parent = nullptr);
    ~CreateRoomDialog();

private slots:
    void on_accept_btn_clicked();
signals:
    void signal_create_room(std::string name, std::vector<uint64_t> members_id);
private:
    std::unique_ptr<FriendsModel> model_ = nullptr;
    Ui::CreateRoomDialog *ui;
};

#endif // CREATEROOMDIALOG_H
