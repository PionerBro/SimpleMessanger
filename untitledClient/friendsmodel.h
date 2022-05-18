#ifndef FRIENDSMODEL_H
#define FRIENDSMODEL_H

#include <QAbstractTableModel>
#include <deque>

class FriendsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FriendsModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void set_row_data(const std::deque<std::pair<uint64_t, std::string>>& friends_deque);
    void set_btn_text(const QString& text);
    void erase_element(uint64_t id);
    void add_element(uint64_t id, const std::string& name);
    uint64_t get_element_id(int row) const;
public slots:
    void slot_btn_delegate_clicked(int row);
signals:
    void signal_btn_delegate_clicked(uint64_t id);
private:
    std::deque<std::pair<uint64_t, std::string>> friends_;
    QString btn_text_ = "";
};

#endif // FRIENDSMODEL_H
