#include "messagesmodel.h"

#include <QBrush>

MessagesModel::MessagesModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int MessagesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return static_cast<int>(messages_.size());
}

int MessagesModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(index.row()<0 || index.row()>=rowCount())
        return QVariant();

    switch (index.column()) {
    case 0:
        if(role == Qt::DisplayRole)
            return QString::fromStdString(messages_.at(index.row()).first);
        if(role == Qt::BackgroundRole)
            return QBrush(Qt::gray);
        if(role == Qt::ForegroundRole)
            return QBrush(Qt::white);
//        if(Qt::TextAlignmentRole == role)
//            return QVariant();
    case 1:
        if(role == Qt::DisplayRole)
            return QString::fromStdString(messages_.at(index.row()).second);
//        if(Qt::TextAlignmentRole == role)
//            return QVariant();
    default:
        return QVariant();
    }
}

void MessagesModel::add_element(const std::string &name, const std::string &msg)
{
    beginResetModel();
    messages_.emplace_back(name, msg);
    endResetModel();
}

void MessagesModel::set_row_data(const std::vector<std::pair<std::string, std::string> > &data)
{
    beginResetModel();
    messages_ = data;
    endResetModel();
}
