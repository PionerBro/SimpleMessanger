#include "userinfomodel.h"

#include <QMap>

UserInfoModel::UserInfoModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void UserInfoModel::setUserInfo_(const UserInfo &info)
{
    userInfo_ = info;
}

int UserInfoModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 4;
}

QVariant UserInfoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    switch (row) {
    case 0:
        return QString::fromStdString(userInfo_.login());
    case 1:
        return QString::fromStdString(userInfo_.password());
    case 2:
        return QString::fromStdString(userInfo_.name());
    case 3:
        return QString::fromStdString(userInfo_.surname());
    default:
        return QVariant();
    }
}

