#ifndef ROOMWIDGET_H
#define ROOMWIDGET_H

#include <QWidget>
#include <memory>

namespace Ui {
class RoomWidget;
}

class MessagesModel;
class RoomInfo;

class RoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoomWidget(QWidget *parent = nullptr);
    ~RoomWidget();

    void add_message(uint64_t room_id, const std::string& name, const std::string& msg);
    void update(uint64_t id, const RoomInfo &data);

private slots:
    void on_send_btn_clicked();
signals:
    void signal_send_btn_clicked(uint64_t, std::string);
private:
    std::unique_ptr<MessagesModel> model_ = nullptr;
    uint64_t id_ = 0;
    std::string name_;
    Ui::RoomWidget *ui;
};

#endif // ROOMWIDGET_H
