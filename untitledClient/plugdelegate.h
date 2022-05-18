#ifndef PLUGDELEGATE_H
#define PLUGDELEGATE_H

#include <QStyledItemDelegate>

class PlugDelegate : public QStyledItemDelegate
{
public:
    explicit PlugDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // PLUGDELEGATE_H
