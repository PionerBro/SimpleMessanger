#include "friendswidget.h"
#include "ui_friendswidget.h"
#include "friendsmodel.h"
#include "buttondelegate.h"
#include "plugdelegate.h"

#include <QMessageBox>

FriendsWidget::FriendsWidget(QWidget *parent) :
    QWidget(parent), plug_delegate_(std::make_unique<PlugDelegate>(this)),
    button_delegate_(std::make_unique<ButtonDelegate>(this)),
    model_(std::make_unique<FriendsModel>(this)),
    ui(new Ui::FriendsWidget)
{
    ui->setupUi(this);
    model_->set_btn_text("Удалить");
    connect(button_delegate_.get(), SIGNAL(signal_btn_delegate_clicked(int)), model_.get(), SLOT(slot_btn_delegate_clicked(int)));
    ui->friends_view->setModel(model_.get());
    ui->friends_view->setItemDelegateForColumn(0, plug_delegate_.get());
    ui->friends_view->setItemDelegateForColumn(1, button_delegate_.get());
    ui->friends_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->friends_view->resizeColumnToContents(1);
    connect(model_.get(), SIGNAL(signal_btn_delegate_clicked(uint64_t)), this, SLOT(try_delete_friend(uint64_t)));
    connect(ui->friends_view, SIGNAL(clicked(const QModelIndex&)), ui->friends_view, SLOT(edit(const QModelIndex&)));
}

FriendsWidget::~FriendsWidget()
{
    delete ui;
}

void FriendsWidget::update(const std::map<uint64_t, std::string> &friends_map)
{
    model_->set_row_data({friends_map.begin(), friends_map.end()});
    ui->friends_view->resizeColumnToContents(1);
}

void FriendsWidget::add_friend(uint64_t friend_id, const std::string &name)
{
    model_->add_element(friend_id, name);
    ui->friends_view->resizeColumnToContents(1);
}

void FriendsWidget::delete_friend(uint64_t friend_id)
{
    model_->erase_element(friend_id);
    ui->friends_view->resizeColumnToContents(1);
}

void FriendsWidget::try_delete_friend(uint64_t id)
{
    emit signal_delete_friend(id);
}

void FriendsWidget::on_all_users_btn_clicked()
{
    emit signal_all_users_btn_clicked();
}
