#include "alluserswidget.h"
#include "ui_alluserswidget.h"
#include "friendsmodel.h"
#include "buttondelegate.h"
#include "plugdelegate.h"

AllUsersWidget::AllUsersWidget(QWidget *parent) :
    QWidget(parent), plug_delegate_(std::make_unique<PlugDelegate>(this)),
    button_delegate_(std::make_unique<ButtonDelegate>(this)),
    model_(std::make_unique<FriendsModel>(this)),
    ui(new Ui::AllUsersWidget)
{
    ui->setupUi(this);
    model_->set_btn_text("Добавить в друзья");
    connect(button_delegate_.get(), SIGNAL(signal_btn_delegate_clicked(int)), model_.get(), SLOT(slot_btn_delegate_clicked(int)));
    connect(model_.get(), SIGNAL(signal_btn_delegate_clicked(uint64_t)), this, SLOT(try_add_friend(uint64_t)));
    ui->users_view->setModel(model_.get());
    ui->users_view->setItemDelegateForColumn(0, plug_delegate_.get());
    ui->users_view->setItemDelegateForColumn(1, button_delegate_.get());
    ui->users_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->users_view->resizeColumnToContents(1);
    connect(ui->users_view, SIGNAL(clicked(const QModelIndex&)), ui->users_view, SLOT(edit(const QModelIndex&)));
}

AllUsersWidget::~AllUsersWidget()
{
    delete ui;
}

void AllUsersWidget::update(const std::deque<std::pair<uint64_t, std::string>> &users_deque)
{
    model_->set_row_data(users_deque);
    ui->users_view->resizeColumnToContents(1);
}

void AllUsersWidget::delete_user(uint64_t id)
{
    model_->erase_element(id);
    ui->users_view->resizeColumnToContents(1);
}

void AllUsersWidget::try_add_friend(uint64_t id)
{
    emit signal_add_friend(id);
}
