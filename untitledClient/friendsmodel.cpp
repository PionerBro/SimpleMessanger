#include "friendsmodel.h"
#include <utility>


FriendsModel::FriendsModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int FriendsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return  (int)friends_.size();
}

int FriendsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant FriendsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(index.row()<0 || index.row()>=rowCount())
        return QVariant();


    switch (role) {
    case Qt::DisplayRole:
        if(index.column() == 0){
            return QString::fromStdString(friends_.at(index.row()).second);
        }else if(index.column() == 1)
            return btn_text_;
    default:
        return QVariant();
    }
}

Qt::ItemFlags FriendsModel::flags(const QModelIndex &index) const
{
    if(index.column() == 0 || index.column()==1)
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    return Qt::ItemIsEnabled;
}

void FriendsModel::set_btn_text(const QString &text)
{
    btn_text_ = text;
}

void FriendsModel::erase_element(uint64_t id)
{
    auto it = std::find_if(friends_.begin(), friends_.end(), [id](const std::pair<uint64_t, std::string>& p){return p.first == id;});
    if(it!=friends_.end()){
        beginResetModel();
        friends_.erase(it);
        endResetModel();
    }
}

void FriendsModel::add_element(uint64_t id, const std::string& name)
{
    beginResetModel();
    friends_.emplace_back(id, name);
    endResetModel();
}

uint64_t FriendsModel::get_element_id(int row) const
{
    return friends_.at(row).first;
}

void FriendsModel::set_row_data(const std::deque<std::pair<uint64_t, std::string>> &friends_deque)
{
    beginResetModel();
    friends_ = friends_deque;
    endResetModel();
}

void FriendsModel::slot_btn_delegate_clicked(int row)
{
    emit signal_btn_delegate_clicked(friends_.at(row).first);
}
