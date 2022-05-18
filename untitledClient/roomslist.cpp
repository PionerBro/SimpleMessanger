#include "roomslist.h"
#include "ui_roomslist.h"
#include "friendsmodel.h"
#include "buttondelegate.h"

#include <QMessageBox>

RoomsList::RoomsList(QWidget *parent) :
    QWidget(parent), button_delegate_(std::make_unique<ButtonDelegate>(this)),
    model_(std::make_unique<FriendsModel>(this)),
    ui(new Ui::RoomsList)
{
    ui->setupUi(this);
    connect(button_delegate_.get(), SIGNAL(signal_btn_delegate_clicked(int)), model_.get(), SLOT(slot_btn_delegate_clicked(int)));
    ui->rooms_view->setModel(model_.get());
    ui->rooms_view->setColumnHidden(1, true);
    ui->rooms_view->setItemDelegateForColumn(0, button_delegate_.get());
    ui->rooms_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    connect(model_.get(), SIGNAL(signal_btn_delegate_clicked(uint64_t)), this, SLOT(try_open_room(uint64_t)));
    connect(ui->rooms_view, SIGNAL(clicked(const QModelIndex&)), ui->rooms_view, SLOT(edit(const QModelIndex&)));
}

RoomsList::~RoomsList()
{
    delete ui;
}

void RoomsList::update(const std::map<uint64_t, std::string> &rooms_map)
{
    model_->set_row_data({rooms_map.begin(), rooms_map.end()});
}

void RoomsList::add_room(uint64_t room_id, const std::string &room_name)
{
    model_->add_element(room_id, room_name);
}

void RoomsList::try_open_room(uint64_t room_id)
{
    emit signal_open_room(room_id);
}

void RoomsList::on_create_room_btn_clicked()
{
    emit signal_create_room_btn_clicked();
}
