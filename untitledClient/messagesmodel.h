#ifndef MESSAGESMODEL_H
#define MESSAGESMODEL_H

#include <QAbstractTableModel>

class MessagesModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit MessagesModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void add_element(const std::string& name, const std::string& msg);
    void set_row_data(const std::vector<std::pair<std::string, std::string>>& data);
private:
    std::vector<std::pair<std::string, std::string>> messages_;
};

#endif // MESSAGESMODEL_H
