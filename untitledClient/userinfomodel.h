#ifndef USERINFOMODEL_H
#define USERINFOMODEL_H

#include <QAbstractTableModel>
#include "userinfo.h"

class UserInfoModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit UserInfoModel(QObject *parent = nullptr);


    void setUserInfo_(const UserInfo& info);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
    UserInfo userInfo_;
};

#endif // USERINFOMODEL_H
