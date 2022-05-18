#include "createroomdialog.h"
#include "ui_createroomdialog.h"
#include "friendsmodel.h"

CreateRoomDialog::CreateRoomDialog(const std::map<uint64_t, std::string> &friends_map, QWidget *parent) :
    QDialog(parent),
    model_(std::make_unique<FriendsModel>(this)),
    ui(new Ui::CreateRoomDialog)
{
    ui->setupUi(this);
    model_->set_row_data({friends_map.begin(), friends_map.end()});
    ui->create_room_view->setModel(model_.get());
    ui->create_room_view->setColumnHidden(1, true);
    ui->create_room_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    connect(ui->reject_btn, SIGNAL(clicked()), this, SLOT(reject()));
}

CreateRoomDialog::~CreateRoomDialog()
{
    delete ui;
}

void CreateRoomDialog::on_accept_btn_clicked()
{
    ui->error_lbl->clear();
    const QItemSelectionModel* sel_model = ui->create_room_view->selectionModel();
    const QModelIndexList selected_rows = sel_model->selectedRows();
    if(selected_rows.size()==0 || ui->room_name_edit->text().size() == 0){
        ui->error_lbl->setText("Выберите название и участников!");
        return;
    }
    std::string name = ui->room_name_edit->text().toStdString();
    std::vector<uint64_t> members_id_vec;
    for(const auto& row:selected_rows)
        members_id_vec.push_back(model_->get_element_id(row.row()));
    emit signal_create_room(name, members_id_vec);
}
