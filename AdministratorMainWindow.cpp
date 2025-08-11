#include "AdministratorMainWindow.h"
#include "qscreen.h"
#include "qmessagebox.h"
#include "TicketPurchasingInterface.h"


AdministratorMainWindow::AdministratorMainWindow(Users* user, TemplateSet<Users>* user_set, QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    //屏幕居中
    QScreen* screen = QGuiApplication::primaryScreen();  // 获取主屏幕
    QRect screenGeometry = screen->geometry();           // 屏幕几何信息
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    // 移动窗口到屏幕中心
    this->move(x, y);
    mp_admin = user;
    mp_user_set = user_set;
    rebook_table = 0;
    ui.pushButtonReBooking->hide();
    LoadFromTxtFile();
    ui.stackedWidgetAdmin->setCurrentIndex(0);
    InitTableSetting();
    QObject::connect(ui.pushButtonHSRailAdmin, &QPushButton::clicked, [=] {
        if (rebook_table == 1) {
            ExitRebookDlg();
        }
        ui.stackedWidgetAdmin->setCurrentIndex(0);
        });
    QObject::connect(ui.pushButtonModify, &QPushButton::clicked, [=] {
        if (rebook_table == 1) {
            ExitRebookDlg();
        }
        UpdateCompleted();
        OrdersTableSetting();
        ui.stackedWidgetAdmin->setCurrentIndex(1);
        });
    QObject::connect(ui.pushButtonUserAdmin, &QPushButton::clicked, [=] {
        if (rebook_table == 1) {
            ExitRebookDlg();
        }
        ui.stackedWidgetAdmin->setCurrentIndex(2);
        });
    QObject::connect(ui.pushButtonReBooking, &QPushButton::clicked, [=] {
        ui.stackedWidgetAdmin->setCurrentIndex(3);
        });
    QObject::connect(ui.pushButtonExitRebook, &QPushButton::clicked, [=] {
        ExitRebookDlg();
        ui.stackedWidgetAdmin->setCurrentIndex(1);
        });
    QObject::connect(ui.dateEditDepart, &QDateEdit::dateChanged, [=] {
            ui.tableWidgetTickets->setEnabled(false);
        });
    QObject::connect(ui.checkBoxTrainNum, &QCheckBox::stateChanged, [=] {
        if (!ui.checkBoxTrainNum->isChecked()) {
            ui.lineEditTrainNum->clear();
            ui.lineEditTrainNum->setEnabled(false);
        }
        else {
            ui.lineEditTrainNum->setEnabled(true);
        }
        });
    QObject::connect(ui.pushButtonBSearch, &QPushButton::clicked, [=] {
        if (ui.dateEditDepart->date() < QDate::currentDate() || ui.dateEditDepart->date() > QDate::currentDate().addDays(15)) {
            ui.tableWidgetTickets->setRowCount(0);
            QMessageBox::information(this, "info", "当前发车时间不能订票。");
            return;
        }
        if (ui.checkBoxTrainNum->isChecked() && ui.lineEditTrainNum->text().isEmpty()) {
            QMessageBox::information(this, "info", "请输入要查询的车次。");
            return;
        }
        ui.tableWidgetTickets->setEnabled(true);
        SearchTickets();
        });
    onClickedTrainOperation();
    onClickedStationOperation();
    onClickedScheduleOperation();
    onClickedSeatOperation();
    OrderEditChanged();

}

AdministratorMainWindow::~AdministratorMainWindow()
{

}

void AdministratorMainWindow::LoadFromTxtFile()
{
    train_set.ReadFromFile("trains.txt");
    train_schedule_set.ReadFromFile("train_schedule.txt");
    order_set.ReadFromFile("orders.txt");
    station_set.ReadFromFile("stations.txt");
    seat_set.ReadFromFile("seats.txt");
}

void AdministratorMainWindow::InitTableSetting()
{
    TrainTableSetting();
    StationTableSetting();
    ScheduleTableSetting();
    SeatTableSetting();
}

void AdministratorMainWindow::TrainTableSetting()
{
    QTableWidget* train_table = ui.tableWidgetTrain;
    int train_count = train_set.size();
    train_table->setCurrentCell(-1, -1);
    train_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    RefreshTrainTable();
    
    train_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    train_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

void AdministratorMainWindow::RefreshTrainTable()
{
    QTableWidget* train_table = ui.tableWidgetTrain;
    int train_count = train_set.size();
    train_table->setRowCount(train_count);
    int col_count = 5;
    for (int row = 0; row < train_count; row++) {
        QString train_id = QString::number(train_set[row].GetTrainId());
        QString train_code = train_set[row].GetTrainCode().c_str();
        QString type = QChar(train_set[row].GetType());
        QString start_station;
        QString end_station;
        int start_station_id = train_set[row].GetStartStation();
        int end_station_id = train_set[row].GetEndStation();
        for (deque<Station>::iterator station_p = station_set.begin(); station_p != station_set.end(); station_p++) {
            if (start_station_id == station_p->GetStationId()) {
                start_station = station_p->GetName().c_str();
            }
            if (end_station_id == station_p->GetStationId()) {
                end_station = station_p->GetName().c_str();
            }
        }

        train_table->setItem(row, 0, new QTableWidgetItem(train_id));
        train_table->setItem(row, 1, new QTableWidgetItem(train_code));
        train_table->setItem(row, 2, new QTableWidgetItem(start_station));
        train_table->setItem(row, 3, new QTableWidgetItem(end_station));
        train_table->setItem(row, 4, new QTableWidgetItem(type));

        for (int col = 0; col < col_count; col++) {
            train_table->item(row, col)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}

void AdministratorMainWindow::TrainSearch()
{
    QTableWidget* train_table = ui.tableWidgetTrain;
    if (ui.lineEditTrainL->text().isEmpty()) {
        for (int row = 0; row < train_table->rowCount(); row++) {
            train_table->setRowHidden(row, false);
        }
    }
    else {
        QString search_text = ui.lineEditTrainL->text();
        for (int row = 0; row < train_table->rowCount(); row++) {
            if (train_table->item(row, 1)->text() == search_text || train_table->item(row, 2)->text() == search_text || train_table->item(row, 3)->text() == search_text) {
                train_table->setRowHidden(row, false);
            }
            else {
                train_table->setRowHidden(row, true);
            }
        }
    }
}

void AdministratorMainWindow::onClickedTrainOperation()
{
    QObject::connect(ui.pushButtonTrainS, &QPushButton::clicked, this, &AdministratorMainWindow::TrainSearch);
    QObject::connect(ui.lineEditTrainL, &QLineEdit::returnPressed, this, &AdministratorMainWindow::TrainSearch);
}

void AdministratorMainWindow::StationTableSetting()
{
    QTableWidget* station_table = ui.tableWidgetStation;
    int station_count = station_set.size();
    station_table->setCurrentCell(-1, -1);
    station_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    RefreshStationTable();

    station_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    station_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

void AdministratorMainWindow::RefreshStationTable()
{
    QTableWidget* station_table = ui.tableWidgetStation;
    int station_count = station_set.size();
    station_table->setRowCount(station_count);
    int col_count = 2;
    for (int row = 0; row < station_count; row++) {
        QString station_id = QString::number(station_set[row].GetStationId());
        QString station_name = station_set[row].GetName().c_str();
        station_table->setItem(row, 0, new QTableWidgetItem(station_id));
        station_table->setItem(row, 1, new QTableWidgetItem(station_name));

        for (int col = 0; col < col_count; col++) {
            station_table->item(row, col)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}

void AdministratorMainWindow::StationSearch()
{
    QTableWidget* station_table = ui.tableWidgetStation;
    if (ui.lineEditStationL->text().isEmpty()) {
        for (int row = 0; row < station_table->rowCount(); row++) {
            station_table->setRowHidden(row, false);
        }
    }
    else {
        QString search_text = ui.lineEditStationL->text();
        for (int row = 0; row < station_table->rowCount(); row++) {
            if (station_table->item(row, 1)->text() == search_text) {
                station_table->setRowHidden(row, false);
            }
            else {
                station_table->setRowHidden(row, true);
            }
        }
    }
}

void AdministratorMainWindow::onClickedStationOperation()
{
    QObject::connect(ui.pushButtonStationS, &QPushButton::clicked, this, &AdministratorMainWindow::StationSearch);
    QObject::connect(ui.lineEditStationL, &QLineEdit::returnPressed, this, &AdministratorMainWindow::StationSearch);
}

void AdministratorMainWindow::ScheduleTableSetting()
{
    QTableWidget* schedule_table = ui.tableWidgetSchedule;
    int schedule_count = train_schedule_set.size();
    schedule_table->setCurrentCell(-1, -1);
    schedule_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    RefreshScheduleTable();

    schedule_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    schedule_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

void AdministratorMainWindow::RefreshScheduleTable()
{
    QTableWidget* schedule_table = ui.tableWidgetSchedule;
    int schedule_count = station_set.size();
    schedule_table->setRowCount(schedule_count);
    int col_count = 6;
    for (int row = 0; row < schedule_count; row++) {
        QString schedule_id = QString::number(train_schedule_set[row].GetScheduleId());
        QString train_code;
        QString arrive_station;
        QString stop_order = QString::number(train_schedule_set[row].GetStopOrder());
        QString arrive_time = train_schedule_set[row].GetArriveTime().Format3().c_str();
        QString depart_time = train_schedule_set[row].GetDepartTime().Format3().c_str();

        int train_id = train_schedule_set[row].GetTrainId();
        int station_id = train_schedule_set[row].GetStationId();
        for (deque<Train>::iterator train_p = train_set.begin(); train_p != train_set.end(); train_p++) {
            if (train_id == train_p->GetTrainId()) {
                train_code = train_p->GetTrainCode().c_str();
                break;
            }
        }
        for (deque<Station>::iterator station_p = station_set.begin(); station_p != station_set.end(); station_p++) {
            if (station_id == station_p->GetStationId()) {
                arrive_station = station_p->GetName().c_str();
                break;
            }
        }
        schedule_table->setItem(row, 0, new QTableWidgetItem(schedule_id));
        schedule_table->setItem(row, 1, new QTableWidgetItem(train_code));
        schedule_table->setItem(row, 2, new QTableWidgetItem(arrive_station));
        schedule_table->setItem(row, 3, new QTableWidgetItem(stop_order));
        schedule_table->setItem(row, 4, new QTableWidgetItem(arrive_time));
        schedule_table->setItem(row, 5, new QTableWidgetItem(depart_time));

        for (int col = 0; col < col_count; col++) {
            schedule_table->item(row, col)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}

void AdministratorMainWindow::ScheduleSearch()
{
    QTableWidget* schedule_table = ui.tableWidgetSchedule;
    if (ui.lineEditScheduleL->text().isEmpty()) {
        for (int row = 0; row < schedule_table->rowCount(); row++) {
            schedule_table->setRowHidden(row, false);
        }
    }
    else {
        QString search_text = ui.lineEditScheduleL->text();
        for (int row = 0; row < schedule_table->rowCount(); row++) {
            if (schedule_table->item(row, 1)->text() == search_text) {
                schedule_table->setRowHidden(row, false);
            }
            else {
                schedule_table->setRowHidden(row, true);
            }
        }
    }
}

void AdministratorMainWindow::onClickedScheduleOperation()
{
    QObject::connect(ui.pushButtonScheduleS, &QPushButton::clicked, this, &AdministratorMainWindow::ScheduleSearch);
    QObject::connect(ui.lineEditScheduleL, &QLineEdit::returnPressed, this, &AdministratorMainWindow::ScheduleSearch);
}

void AdministratorMainWindow::SeatTableSetting()
{
    QTableWidget* seat_table = ui.tableWidgetSeat;
    int seat_count = seat_set.size();
    seat_table->setCurrentCell(-1, -1);
    seat_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    RefreshSeatTable();

    seat_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    seat_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

void AdministratorMainWindow::RefreshSeatTable()
{
    QTableWidget* seat_table = ui.tableWidgetSeat;
    int seat_count = seat_set.size();
    seat_table->setRowCount(seat_count);
    int col_count = 4;
    for (int row = 0; row < seat_count; row++) {
        QString seat_id = QString::number(seat_set[row].GetSeatId());
        QString train_code;
        QString seat_no = seat_set[row].GetSeatNo().c_str();
        QString seat_type = seat_set[row].GetSeatType().c_str();
        int train_id = seat_set[row].GetTrainId();
        for (deque<Train>::iterator train_p = train_set.begin(); train_p != train_set.end(); train_p++) {
            if (train_id == train_p->GetTrainId()) {
                train_code = train_p->GetTrainCode().c_str();
                break;
            }
        }

        seat_table->setItem(row, 0, new QTableWidgetItem(seat_id));
        seat_table->setItem(row, 1, new QTableWidgetItem(train_code));
        seat_table->setItem(row, 2, new QTableWidgetItem(seat_no));
        seat_table->setItem(row, 3, new QTableWidgetItem(seat_type));
        
        for (int col = 0; col < col_count; col++) {
            seat_table->item(row, col)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}

void AdministratorMainWindow::SeatSearch()
{
    QTableWidget* seat_table = ui.tableWidgetSeat;
    if (ui.lineEditSeatL->text().isEmpty()) {
        for (int row = 0; row < seat_table->rowCount(); row++) {
            seat_table->setRowHidden(row, false);
        }
    }
    else {
        QString search_text = ui.lineEditSeatL->text();
        for (int row = 0; row < seat_table->rowCount(); row++) {
            if (seat_table->item(row, 1)->text() == search_text) {
                seat_table->setRowHidden(row, false);
            }
            else {
                seat_table->setRowHidden(row, true);
            }
        }
    }
}

void AdministratorMainWindow::onClickedSeatOperation()
{
    QObject::connect(ui.pushButtonSeatS, &QPushButton::clicked, this, &AdministratorMainWindow::SeatSearch);
    QObject::connect(ui.lineEditSeatL, &QLineEdit::returnPressed, this, &AdministratorMainWindow::SeatSearch);
}


void AdministratorMainWindow::OrdersTableSetting()
{
    QTableWidget* order_table = ui.tableWidgetOrder;
    int order_count = order_set.size();
    int col_count = ui.tableWidgetOrder->columnCount();
    ui.comboBoxME->setCurrentIndex(0);
    order_table->setCurrentCell(-1, -1);
    order_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    order_table->setRowCount(order_count);
    for (int row = 0; row < order_count; row++) {
        QString order_id = QString::number(order_set[row].GetOrderId());
        QString id_card;
        QString train_code;
        Date d = order_set[row].GetTravelDate();
        QString travel_date = d.Format().c_str();
        QDate travel_qdate(d.GetYear(), d.GetMonth(), d.GetDay());
        QString start_station;
        QString end_station;
        QString start_time;
        QString end_time;
        QString seat_no;
        QString seat_type;
        QString fare;
        QString booking_time = order_set[row].GetOrderTime().Format2().c_str();
        QString status;
        int train_id = order_set[row].GetTrainId();
        int user_id = order_set[row].GetUserId();
        int start_station_id = order_set[row].GetStartStationId();
        int end_station_id = order_set[row].GetEndStationId();
        int seat_id = order_set[row].GetSeatId();
        int start_stop_order = 0;
        int end_stop_order = 0;
        for (deque<Users>::iterator p = mp_user_set->begin(); p != mp_user_set->end(); p++) {
            if (p->GetUserId() == user_id) {
                id_card = p->GetIdCard().c_str();
                break;
            }
        }
        for (deque<Train>::iterator p = train_set.begin(); p != train_set.end(); p++) {
            if (p->GetTrainId() == train_id) {
                train_code = p->GetTrainCode().c_str();
                break;
            }
        }
        for (deque<Station>::iterator p = station_set.begin(); p != station_set.end(); p++) {
            if (start_station_id == p->GetStationId()) {
                start_station = p->GetName().c_str();
            }
            if (end_station_id == p->GetStationId()) {
                end_station = p->GetName().c_str();
            }
        }
        for (deque<TrainSchedule>::iterator p = train_schedule_set.begin(); p != train_schedule_set.end(); p++) {
            if (train_id == p->GetTrainId()) {
                if (start_station_id == p->GetStationId()) {
                    start_time = p->GetDepartTime().Format3().c_str();
                    start_stop_order = p->GetStopOrder();
                }
                if (end_station_id == p->GetStationId()) {
                    end_time = p->GetArriveTime().Format3().c_str();
                    end_stop_order = p->GetStopOrder();
                }
            }
        }
        for (deque<Seat>::iterator p = seat_set.begin(); p != seat_set.end(); p++) {
            if (seat_id == p->GetSeatId()) {
                seat_no = p->GetSeatNo().c_str();
                seat_type = p->GetSeatType().c_str();
            }
        }
        if (seat_type == "商务") fare = QString::number(150 * (end_stop_order - start_stop_order));
        else if (seat_type == "一等") fare = QString::number(90 * (end_stop_order - start_stop_order));
        else fare = QString::number(50 * (end_stop_order - start_stop_order));
        if (order_set[row].GetStatus() == "booked") status = "已预定";
        else if (order_set[row].GetStatus() == "canceled") status = "已取消";
        else if (order_set[row].GetStatus() == "completed") status = "已完成";

        order_table->setItem(row, 0, new QTableWidgetItem(order_id));
        order_table->setItem(row, 1, new QTableWidgetItem(id_card));
        order_table->setItem(row, 2, new QTableWidgetItem(train_code));
        order_table->setItem(row, 3, new QTableWidgetItem(travel_date));
        order_table->setItem(row, 4, new QTableWidgetItem(start_station));
        order_table->setItem(row, 5, new QTableWidgetItem(end_station));
        order_table->setItem(row, 6, new QTableWidgetItem(start_time));
        order_table->setItem(row, 7, new QTableWidgetItem(end_time));
        order_table->setItem(row, 8, new QTableWidgetItem(seat_no));
        order_table->setItem(row, 9, new QTableWidgetItem(seat_type));
        order_table->setItem(row, 10, new QTableWidgetItem(fare));
        order_table->setItem(row, 11, new QTableWidgetItem(booking_time));
        order_table->setItem(row, 12, new QTableWidgetItem(status));

        if (status != "已预定") {
            order_table->setRowHidden(row, true);
        }
        else {
            order_table->setRowHidden(row, false);
        }

        // 设置行列居中
        for (int col = 0; col < col_count; col++) {
            ui.tableWidgetOrder->item(row, col)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}

void AdministratorMainWindow::OrderEditChanged()
{
    QObject::connect(ui.lineEditMEIdCard, &QLineEdit::returnPressed, this, &AdministratorMainWindow::OrderFiltering);
    QObject::connect(ui.comboBoxME, &QComboBox::currentIndexChanged, this, &AdministratorMainWindow::OrderFiltering);
    QObject::connect(ui.pushButtonRefund, &QPushButton::clicked, this, &AdministratorMainWindow::RefundOperation);
    QObject::connect(ui.pushButtonRebook, &QPushButton::clicked, this, &AdministratorMainWindow::RebookOperation);
}

void AdministratorMainWindow::OrderFiltering()
{
    int col_id_card = 1;
    int col_status = 12;
    QString id_card = ui.lineEditMEIdCard->text();
    QString status = ui.comboBoxME->currentText();
    for (int row = 0; row < ui.tableWidgetOrder->rowCount(); row++) {
        QString id_card_item = ui.tableWidgetOrder->item(row, col_id_card)->text();
        QString status_item = ui.tableWidgetOrder->item(row, col_status)->text();
        
        if (id_card.isEmpty()) {
            if (status == status_item) {
                ui.tableWidgetOrder->setRowHidden(row, false);
            }
            else {
                ui.tableWidgetOrder->setRowHidden(row, true);
            }
        }
        else {
            if (id_card == id_card_item && status == status_item) {
                ui.tableWidgetOrder->setRowHidden(row, false);
            }
            else {
                ui.tableWidgetOrder->setRowHidden(row, true);
            }
        }
    }
    ui.tableWidgetOrder->setCurrentCell(-1, -1);
}

void AdministratorMainWindow::RefundOperation()
{
    QTableWidget* order_table = ui.tableWidgetOrder;
    if (order_table->currentRow() == -1) {
        QMessageBox::warning(this, "警告", "请先选中要退票的订单的行！");
        return;
    }
    else if (order_table->item(order_table->currentRow(), 12)->text() != "已预定") {
        QMessageBox::warning(this, "警告", "已完成或已取消的车票不能退票。");
        return;
    }
    else {
        int order_id = 0;
        int col_status = 12;
        order_id = order_table->item(order_table->currentRow(), 0)->text().toInt();
        for (deque<Order>::iterator p = order_set.begin(); p != order_set.end(); p++) {
            if (p->GetOrderId() == order_id) {
                p->SetStatus("canceled");
                break;
            }
        }
        order_table->item(order_table->currentRow(), col_status)->setText("已取消");
        order_table->setCurrentCell(-1, -1);
        OrderFiltering();
    }
}

void AdministratorMainWindow::RebookOperation()
{
    QTableWidget* order_table = ui.tableWidgetOrder;
    int current_row = order_table->currentRow();
    if (current_row == -1) {
        QMessageBox::warning(this, "警告", "请先选中要改签的订单的行！");
        return;
    }
    else if (order_table->item(current_row, 12)->text() != "已预定") {
        QMessageBox::warning(this, "警告", "已完成或已取消的车票不能改签。");
        return;
    }
    else {
        rebook_table = 1;
        ui.pushButtonReBooking->show();
        ui.stackedWidgetAdmin->setCurrentIndex(3);
        ui.dateEditDepart->setDate(QDate::currentDate());
        ui.lineEditGetOn->setText(order_table->item(current_row, 4)->text());
        ui.lineEditGetOff->setText(order_table->item(current_row, 5)->text());
        ui.lineEditGetOn->setReadOnly(true);
        ui.lineEditGetOff->setReadOnly(true);

        string user_id_card = order_table->item(current_row, 1)->text().toStdString();
        for (int i = 0; i < mp_user_set->size(); i++) {
            if (user_id_card == (*mp_user_set)[i].GetIdCard()) {
                mp_user = &(*mp_user_set)[i];
                break;
            }
        }
        SearchTickets();
    }
}
void AdministratorMainWindow::SearchTickets()
{
    string on_station = ui.lineEditGetOn->text().toStdString();
    string off_station = ui.lineEditGetOff->text().toStdString();
    QDate q_depart_date = ui.dateEditDepart->date();

    RefreshTicketTable(on_station, off_station, q_depart_date);

    QString search_train_num = ui.lineEditTrainNum->text();
    QTableWidget* ticket_table = ui.tableWidgetTickets;
    int rol_train_code = 0;
    if (ui.checkBoxTrainNum->isChecked() && !search_train_num.isEmpty()) {
        for (int row = 0; row < ticket_table->rowCount(); row++) {
            if (ticket_table->item(row, rol_train_code)->text() != search_train_num) {
                ticket_table->setRowHidden(row, true);
            }
            else {
                ticket_table->setRowHidden(row, false);
            }
        }
    }
}

void AdministratorMainWindow::RefreshTicketTable(string on_station, string off_station, QDate q_depart_date)
{
    int on_station_id = 0;
    int off_station_id = 0;
    for (deque<Station>::iterator p = station_set.begin(); p != station_set.end(); p++) {
        if (on_station == p->GetName()) {
            on_station_id = p->GetStationId();
        }
        if (off_station == p->GetName()) {
            off_station_id = p->GetStationId();
        }
    }

    int row = 0;
    QTableWidget* ticket_table = ui.tableWidgetTickets;
    ticket_table->setRowCount(train_set.size());
    for (deque<Train>::iterator train_p = train_set.begin(); train_p != train_set.end(); train_p++) {
        QString train_code = train_p->GetTrainCode().c_str();
        QString start_station = on_station.c_str();
        QString end_station = off_station.c_str();
        QString start_time;
        QString end_time;
        QString business_class;
        QString first_class;
        QString second_class;

        int train_id = train_p->GetTrainId();
        int on_order = 0;   //0代表没有order
        int off_order = 0;
        int business_class_sum = 0;
        int first_class_sum = 0;
        int second_class_sum = 0;
        int order_B_num = 0;
        int order_F_num = 0;
        int order_S_num = 0;
        int start_station_id = 0;
        int end_station_id = 0;
        for (deque<Station>::iterator station_p = station_set.begin(); station_p != station_set.end(); station_p++) {
            if (start_station == station_p->GetName().c_str()) {
                start_station_id = station_p->GetStationId();
            }
            if (end_station == station_p->GetName().c_str()) {
                end_station_id = station_p->GetStationId();
            }
        }

        for (deque<TrainSchedule>::iterator schedule_p = train_schedule_set.begin(); schedule_p != train_schedule_set.end(); schedule_p++) {
            if (train_id == schedule_p->GetTrainId()) {
                if (on_station_id == schedule_p->GetStationId()) {
                    on_order = schedule_p->GetStopOrder();
                    start_time = schedule_p->GetDepartTime().Format3().c_str();
                }
                if (off_station_id == schedule_p->GetStationId()) {
                    off_order = schedule_p->GetStopOrder();
                    end_time = schedule_p->GetArriveTime().Format3().c_str();
                }
            }
        }
        if (on_order == 0 || off_order == 0 || on_order > off_order) {
            continue;
        }
        for (deque<Seat>::iterator seat_p = seat_set.begin(); seat_p != seat_set.end(); seat_p++) {
            if (train_id == seat_p->GetTrainId()) {
                if (seat_p->GetSeatType() == "商务") {
                    business_class_sum++;
                }
                else if (seat_p->GetSeatType() == "一等") {
                    first_class_sum++;
                }
                else if (seat_p->GetSeatType() == "二等") {
                    second_class_sum++;
                }
            }
        }
        for (deque<Order>::iterator order_p = order_set.begin(); order_p != order_set.end(); order_p++) {
            Date d_date = order_p->GetTravelDate();
            QDate q_every_date(d_date.GetYear(), d_date.GetMonth(), d_date.GetDay());
            if (train_id == order_p->GetTrainId() && q_every_date == q_depart_date && (order_p->GetStatus() == "booked" || order_p->GetStatus() == "completed")) {
                int start_order_station_id = order_p->GetStartStationId();
                int end_order_station_id = order_p->GetEndStationId();
                int start_order_id = 0;
                int end_order_id = 0;
                int current_start_order_id = 0;
                int current_end_order_id = 0;
                for (deque<TrainSchedule>::iterator schedule_p = train_schedule_set.begin(); schedule_p != train_schedule_set.end(); schedule_p++) {
                    if (train_id == order_p->GetTrainId() && start_order_station_id == schedule_p->GetStationId()) {
                        start_order_id = schedule_p->GetStopOrder();
                    }
                    if (train_id == order_p->GetTrainId() && end_order_station_id == schedule_p->GetStationId()) {
                        end_order_id = schedule_p->GetStopOrder();
                    }
                    if (train_id == order_p->GetTrainId() && start_station_id == schedule_p->GetStationId()) {
                        current_start_order_id = schedule_p->GetStopOrder();
                    }
                    if (train_id == order_p->GetTrainId() && end_station_id == schedule_p->GetStationId()) {
                        current_end_order_id = schedule_p->GetStopOrder();
                    }
                }
                if (current_end_order_id <= start_order_id || current_start_order_id >= end_order_id) {
                    continue;
                }

                int seat_id = order_p->GetSeatId();
                for (int j = 0; j < seat_set.size(); j++) {
                    if (seat_id == seat_set[j].GetSeatId()) {
                        if (seat_set[j].GetSeatType() == "商务") {
                            order_B_num++;
                        }
                        else if (seat_set[j].GetSeatType() == "一等") {
                            order_F_num++;
                        }
                        else if (seat_set[j].GetSeatType() == "二等") {
                            order_S_num++;
                        }
                    }
                }
            }
        }
        business_class = QString::number(business_class_sum - order_B_num);
        first_class = QString::number(first_class_sum - order_F_num);
        second_class = QString::number(second_class_sum - order_S_num);

        ticket_table->setItem(row, 0, new QTableWidgetItem(train_code));
        ticket_table->setItem(row, 1, new QTableWidgetItem(start_station));
        ticket_table->setItem(row, 2, new QTableWidgetItem(end_station));
        ticket_table->setItem(row, 3, new QTableWidgetItem(start_time));
        ticket_table->setItem(row, 4, new QTableWidgetItem(end_time));
        ticket_table->setItem(row, 5, new QTableWidgetItem(business_class));
        ticket_table->setItem(row, 6, new QTableWidgetItem(first_class));
        ticket_table->setItem(row, 7, new QTableWidgetItem(second_class));
        QPushButton* bookingItemButton = new QPushButton("改签");
        bookingItemButton->setProperty("row", row);
        bool no_seat = business_class.toInt() == 0 && first_class.toInt() == 0 && second_class.toInt() == 0;
        bool date_error = ui.dateEditDepart->date() < QDate::currentDate() || ui.dateEditDepart->date() > QDate::currentDate().addDays(15);
        bool time_error = ui.dateEditDepart->date() == QDate::currentDate() && QTime::fromString(start_time, "hh:mm") < QTime::currentTime();
        if (no_seat || date_error || time_error) {
            bookingItemButton->setEnabled(false);
            bookingItemButton->setStyleSheet(
                "QPushButton {"
                "    border: 1px solid #dddddd;"
                "    border-radius: 3px;"
                "    background-color: rgb(200, 200, 200);"
                "    color: rgb(150, 150, 150);"
                "    margin-top: 1px; "
                "    margin-bottom: 1px; "
                "    margin-left: 5px; "
                "    margin-right: 5px; "
                "}"
            );
        }
        else {
            bookingItemButton->setEnabled(true);
            bookingItemButton->setStyleSheet(
                "QPushButton {"
                "    border: 1px solid #dddddd;"
                "    border-radius: 3px;"
                "    background-color: rgb(255, 85, 85);"
                "    color: rgb(255, 255, 255);"
                "    margin-top: 1px; "
                "    margin-bottom: 1px; "
                "    margin-left: 5px; "
                "    margin-right: 5px; "
                "}"
                "QPushButton:hover {"
                "    background-color: rgb(255, 20, 20);"
                "}"
                "QPushButton:pressed {"
                "    background-color: rgb(234, 39, 39);"
                "}"
            );
        }
        connect(bookingItemButton, &QPushButton::clicked, this, &AdministratorMainWindow::onBookingButtonClicked);
        ticket_table->setCellWidget(row, 8, bookingItemButton);
        ticket_table->setRowHidden(row, false);
        row++;
    }
    ticket_table->setRowCount(row);
}

void AdministratorMainWindow::onBookingButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    if (ui.dateEditDepart->date() < QDate::currentDate() || ui.dateEditDepart->date() > QDate::currentDate().addDays(15)) {
        QMessageBox::warning(this, "提示", "当前日期不能预定车票。");
        return;
    }
    QTableWidget* ticket_table = ui.tableWidgetTickets;
    // 获取存储的行号
    int row = button->property("row").toInt();
    QString start_time = ticket_table->item(row, 3)->text();
    bool time_error = QTime::fromString(start_time, "hh:mm") < QTime::currentTime();
    if (ui.dateEditDepart->date() == QDate::currentDate() && time_error) {
        QMessageBox::warning(this, "提示", "已经错过发车时间了，不能预定。");
        return;
    }
    QString train_code = ticket_table->item(row, 0)->text();
    QString start_station_s = ticket_table->item(row, 1)->text();
    QString end_station_s = ticket_table->item(row, 2)->text();
    QString remaining_tickets0 = ticket_table->item(row, 5)->text();
    QString remaining_tickets1 = ticket_table->item(row, 6)->text();
    QString remaining_tickets2 = ticket_table->item(row, 7)->text();
    vector<QString> remaining_para;
    remaining_para.push_back(remaining_tickets0);
    remaining_para.push_back(remaining_tickets1);
    remaining_para.push_back(remaining_tickets2);

    Train* booking_train = NULL;
    for (int i = 0; i < train_set.size(); i++) {
        if (train_code == train_set[i].GetTrainCode().c_str()) {
            booking_train = &train_set[i];
            break;
        }
    }
    Station* start_station = NULL;
    Station* end_station = NULL;
    for (int i = 0; i < station_set.size(); i++) {
        if (start_station_s == station_set[i].GetName().c_str()) {
            start_station = &station_set[i];
        }
        if (end_station_s == station_set[i].GetName().c_str()) {
            end_station = &station_set[i];
        }
    }
    QDate q_depart_date = ui.dateEditDepart->date();
    Date depart_date(q_depart_date.year(), q_depart_date.month(), q_depart_date.day(), 0, 0);
    TicketPurchasingInterface* purchasingDlg = new TicketPurchasingInterface(depart_date, mp_user, booking_train, start_station,
        end_station, remaining_para, &order_set, &train_schedule_set, &seat_set);

    QObject::connect(purchasingDlg, &TicketPurchasingInterface::Purchased, this, &AdministratorMainWindow::CompletedAndExit);
    purchasingDlg->exec();
}

void AdministratorMainWindow::CompletedAndExit()
{
    QTableWidget* order_table = ui.tableWidgetOrder;
    int current_row = order_table->currentRow();
    int order_id = 0;
    order_id = order_table->item(current_row, 0)->text().toInt();
    for (deque<Order>::iterator order_p = order_set.begin(); order_p != order_set.end(); order_p++) {
        if (order_p->GetOrderId() == order_id) {
            order_p->SetStatus("canceled");
            break;
        }
    }
    rebook_table = 0;
    ui.pushButtonReBooking->hide();
    ui.lineEditGetOn->setReadOnly(false);
    ui.lineEditGetOff->setReadOnly(false);
    order_table->setCurrentCell(-1, -1);
    ui.stackedWidgetAdmin->setCurrentIndex(1);
    UpdateCompleted();
    OrdersTableSetting();
}

void AdministratorMainWindow::UpdateCompleted()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    for (deque<Order>::iterator order_p = order_set.begin(); order_p != order_set.end(); order_p++) {
        int train_id = order_p->GetTrainId();
        int start_station_id = order_p->GetStartStationId();
        QDate travel_date(order_p->GetTravelDate().GetYear(), order_p->GetTravelDate().GetMonth(), order_p->GetTravelDate().GetDay());
        QTime depart_time;
        for (deque<TrainSchedule>::iterator schedule_p = train_schedule_set.begin(); schedule_p != train_schedule_set.end(); schedule_p++) {
            if (train_id == schedule_p->GetTrainId() && start_station_id == schedule_p->GetStationId()) {
                depart_time.setHMS(schedule_p->GetDepartTime().GetHour(), schedule_p->GetDepartTime().GetMinute(), 0);
            }
        }
        QDateTime travel_date_time(travel_date, depart_time);
        if (travel_date_time < dateTime) {
            order_p->SetStatus("completed");
        }
    }
}

void AdministratorMainWindow::ExitRebookDlg()
{
    QMessageBox::information(this, "info", "您已退出改签界面。");
    rebook_table = 0;
    ui.pushButtonReBooking->hide();
    ui.lineEditGetOn->setReadOnly(false);
    ui.lineEditGetOff->setReadOnly(false);
}