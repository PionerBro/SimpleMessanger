#include "profilewidget.h"
#include "ui_profilewidget.h"

ProfileWidget::ProfileWidget(const UserInfo& info,QWidget *parent) :
    QWidget(parent), user_info_(info),
    ui(new Ui::ProfileWidget)
{
    ui->setupUi(this);
}

ProfileWidget::~ProfileWidget()
{
    delete ui;
}

void ProfileWidget::update()
{
    update_image();
    update_name();
    update_friends();
}

void ProfileWidget::update_image()
{
    if(!user_info_.image().isEmpty()){
        QImage image(user_info_.image().data(), user_info_.image().width(), user_info_.image().height(),
                     (QImage::Format)user_info_.image().format());
        ui->avatar_lbl->setPixmap(QPixmap::fromImage(image).scaled(ui->avatar_lbl->size(),
                                                                   Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }else{
        ui->avatar_lbl->clear();
        ui->avatar_lbl->setText("No foto");
    }
}

void ProfileWidget::update_name()
{
    ui->name_lbl->setText(QString::fromStdString(user_info_.name()) + " " + QString::fromStdString(user_info_.surname()));
}

void ProfileWidget::update_friends()
{
    ui->friends_btn->setText("Друзья: "+ QString::number(user_info_.friends().size()));
}

void ProfileWidget::setAvatar(QPixmap pixmap)
{
    ui->avatar_lbl->setPixmap(pixmap.scaled(ui->avatar_lbl->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void ProfileWidget::on_friends_btn_clicked()
{
    emit signal_on_friends_btn_clicked();
}
