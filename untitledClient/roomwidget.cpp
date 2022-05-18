#include "roomwidget.h"
#include "ui_roomwidget.h"
#include "messagesmodel.h"
#include "roominfo.h"

RoomWidget::RoomWidget(QWidget *parent) :
    QWidget(parent), model_(std::make_unique<MessagesModel>(this)),
    ui(new Ui::RoomWidget)
{
    ui->setupUi(this);
    ui->view->setModel(model_.get());
    ui->view->setColumnWidth(0, 120);
    ui->members_cbx->view()->setDisabled(true);
    connect(ui->msg_edit, SIGNAL(returnPressed()), this, SLOT(on_send_btn_clicked()));
}

RoomWidget::~RoomWidget()
{
    delete ui;
}

void RoomWidget::add_message(uint64_t room_id, const std::string &name, const std::string &msg)
{
    if(room_id == id_){
        model_->add_element(name, msg);
        ui->view->scrollToBottom();
    }
}

void RoomWidget::update(uint64_t id, const RoomInfo &data)
{
    model_->set_row_data(data.messages());
    name_ = data.name();
    id_ = id;
    ui->name_lbl->setText(QString::fromStdString(name_));
    ui->members_cbx->clear();
    for(const auto& member: data.members())
        ui->members_cbx->addItem(QString::fromStdString(member.second));
    ui->view->scrollToBottom();
}

void RoomWidget::on_send_btn_clicked()
{
    QString msg = ui->msg_edit->text();
    if(msg.size()){
        emit signal_send_btn_clicked(id_, msg.toStdString());
        ui->msg_edit->clear();
    }
}

