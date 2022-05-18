#include "plugdelegate.h"

PlugDelegate::PlugDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *PlugDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return 0;
}
