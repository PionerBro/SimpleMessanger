#include "settingswidget.h"
#include "ui_settingswidget.h"

#include <QFileDialog>

SettingsWidget::SettingsWidget(const UserInfo& info, QWidget *parent) :
    QWidget(parent), info_(info),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
}

void SettingsWidget::update(){
    tmp_info_.clear();
    ui->status_lbl->clear();
    ui->name_edit->setText(QString::fromStdString(info_.name()));
    ui->surname_edit->setText(QString::fromStdString(info_.surname()));
    if(!info_.image().isEmpty()){
        QImage image(info_.image().data(), info_.image().width(), info_.image().height(),
                     (QImage::Format)info_.image().format());
        ui->foto_lbl->setPixmap(QPixmap::fromImage(image).scaled(ui->foto_lbl->size(),
                                                                   Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void SettingsWidget::set_status(bool status)
{
    if(status){
        ui->status_lbl->setText("Изменения успешно сохранены!");
        tmp_info_.clear();
    }else
        ui->status_lbl->setText("Ошибка! Повторите попытку позже!");
}

void SettingsWidget::on_foto_btn_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Выберите изображение", QString(), "Images (*.png *.xpm *.jpg)");
    if(!path.isEmpty()){
        QImage tmp(path);
        tmp_info_.set_image(ImageInfo(tmp.constBits(), tmp.sizeInBytes(), tmp.width(), tmp.height(), tmp.format()));
        ui->foto_lbl->setPixmap(QPixmap::fromImage(tmp).scaled(ui->foto_lbl->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void SettingsWidget::on_accept_changing_btn_clicked()
{
    ui->status_lbl->clear();
    tmp_info_.set_name(ui->name_edit->text().toStdString());
    tmp_info_.set_surname(ui->surname_edit->text().toStdString());
    qDebug()<<tmp_info_.image().isEmpty();
    if(info_.name() != tmp_info_.name() || info_.surname() != tmp_info_.surname() || !tmp_info_.image().isEmpty())
        emit signal_accept_profile_changings(tmp_info_);
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}
