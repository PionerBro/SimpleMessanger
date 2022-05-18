#include "buttondelegate.h"
#include <QPushButton>
#include <QApplication>

ButtonDelegate::ButtonDelegate(QObject* parent):QStyledItemDelegate(parent)
{

}

QWidget *ButtonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex &index) const
{
    QPushButton* btn = new QPushButton(parent);
    btn->setFlat(true);
    //btn->setText(index.data(Qt::DisplayRole).toString());
    emit signal_btn_delegate_clicked(index.row());
    return btn;
}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 1){
        QStyleOptionButton buttonOption;
        buttonOption.rect = option.rect;
        buttonOption.text = index.data(Qt::DisplayRole).toString();
        QApplication::style()->drawControl(QStyle::CE_PushButtonLabel, &buttonOption, painter);
    }else{
        QStyledItemDelegate::paint(painter, option, index);
    }
}
