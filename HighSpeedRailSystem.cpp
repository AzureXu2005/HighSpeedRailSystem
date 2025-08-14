#include "HighSpeedRailSystem.h"
#include "TicketPurchasingInterface.h"
#include "qmessagebox.h"
#include "qscreen.h"
#include "queue"
#include "qtimer.h"

HighSpeedRailSystem::HighSpeedRailSystem(Users *user, TemplateSet<Users>* user_set, QWidget *parent)
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
    mp_user = user;
    mp_user_set = user_set;
    this->LoadFromTxtFile();
    this->LoadUserInformationSetting();
    ui.labelNameT->setText(QString::fromUtf8(mp_user->GetDisplayName().c_str()));
    ui.lineEditTrainNum->setEnabled(false);
    ui.labelRebook->hide();
    ui.pushButtonExitRebook->hide();
    init_table = 1;
    ui.dateEditDepart->setDate(QDate::currentDate());
    this->TicketsTableSetting();
    ui.stackedWidgetUser->setCurrentIndex(0);
    QObject::connect(ui.pushButtonExitRebook, &QPushButton::clicked, [=] {
        init_table = 0;
        ui.lineEditGetOn->setReadOnly(false);
        ui.lineEditGetOff->setReadOnly(false);
        ui.labelRebook->hide();
        ui.pushButtonExitRebook->hide();
        });
    QObject::connect(ui.lineEditGetOn, &QLineEdit::textChanged, [=] {
        if (init_table == 0) ui.tableWidgetTickets->setEnabled(false);
        });
    QObject::connect(ui.lineEditGetOff, &QLineEdit::textChanged, [=] {
        if (init_table == 0) ui.tableWidgetTickets->setEnabled(false);
        });
    QObject::connect(ui.dateEditDepart, &QDateEdit::dateChanged, [=] {
        if (init_table == 1) {
            refreshTicketsTableInterface();
        }
        else {
            ui.tableWidgetTickets->setEnabled(false);
        }
        });

    QObject::connect(ui.pushButtonBooking, &QPushButton::clicked, [=] {
        ui.stackedWidgetUser->setCurrentIndex(0);
        refreshTicketsTableInterface();
        });
    QObject::connect(ui.pushButtonSearch, &QPushButton::clicked, [=] {
        if (init_table == 2) {
            QMessageBox::information(this, "info", "您已退出改签界面。");
            init_table = 0;
            ui.lineEditGetOn->setReadOnly(false);
            ui.lineEditGetOff->setReadOnly(false);
            ui.labelRebook->hide();
            ui.pushButtonExitRebook->hide();
        }
        ui.stackedWidgetUser->setCurrentIndex(1);
        UpdateCompleted();
        OrdersTableSetting();
        });
    QObject::connect(ui.pushButtonInfor, &QPushButton::clicked, [=] {
        if (init_table == 2) {
            QMessageBox::information(this, "info", "您已退出改签界面。");
            init_table = 0;
            ui.lineEditGetOn->setReadOnly(false);
            ui.lineEditGetOff->setReadOnly(false);
            ui.labelRebook->hide();
            ui.pushButtonExitRebook->hide();
        }
        ui.stackedWidgetUser->setCurrentIndex(2);
        LoadUserInformationSetting();
        });
    QObject::connect(ui.pushButtonBSearch, &QPushButton::clicked, [=] {
        ui.tableWidgetTickets->setEnabled(true);
        TicketSearchPretreatment();
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
    QObject::connect(ui.pushButtonCancel, &QPushButton::clicked, [=] {
        LoadUserInformationSetting();
        });
    QObject::connect(ui.pushButtonSave, &QPushButton::clicked, [=] {
        CheckInformation();
        });
    QObject::connect(ui.pushButtonLogOff, &QPushButton::clicked, [=] {
        LogOff();
        });
    this->OrderEditChanged();
}

HighSpeedRailSystem::~HighSpeedRailSystem()
{
    WriteToTxtFile();
}


void HighSpeedRailSystem::LoadFromTxtFile()
{
    train_set.ReadFromFile("trains.txt");
    train_schedule_set.ReadFromFile("train_schedule.txt");
    order_set.ReadFromFile("orders.txt");
    station_set.ReadFromFile("stations.txt");
    seat_set.ReadFromFile("seats.txt");
}

void HighSpeedRailSystem::WriteToTxtFile()
{
    mp_user_set->WriteToFile("users.txt");
    train_set.WriteToFile("trains.txt");
    train_schedule_set.WriteToFile("train_schedule.txt");
    order_set.WriteToFile("orders.txt");
    station_set.WriteToFile("stations.txt");
    seat_set.WriteToFile("seats.txt");
}

void HighSpeedRailSystem::TicketsTableSetting()
{
    QTableWidget* ticket_table = ui.tableWidgetTickets;
    int train_count = train_set.size();
    ticket_table->setCurrentCell(-1, -1);
    ticket_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ticket_table->setRowCount(train_count);
    QDate q_depart_date = ui.dateEditDepart->date();

    for (int row = 0; row < train_count; row++) {
        QString train_code = train_set[row].GetTrainCode().c_str();
        QString start_station;
        QString end_station;
        QString start_time;
        QString end_time;
        QString business_class;
        QString first_class;
        QString second_class;

        int train_id = train_set[row].GetTrainId();
        int start_station_i = train_set[row].GetStartStation();
        int end_station_i = train_set[row].GetEndStation();
        int business_class_sum = 0;
        int first_class_sum = 0;
        int second_class_sum = 0;
        int order_B_num = 0;
        int order_F_num = 0;
        int order_S_num = 0;
        for (int i = 0; i < station_set.size(); i++) {
            if (start_station_i == station_set[i].GetStationId()) {
                start_station = QString::fromStdString(station_set[i].GetName());
            }
            if (end_station_i == station_set[i].GetStationId()) {
                end_station = QString::fromStdString(station_set[i].GetName());
            }
        }
        for (int i = 0; i < train_schedule_set.size(); i++) {
            if (train_id == train_schedule_set[i].GetTrainId() && start_station_i == train_schedule_set[i].GetStationId()) {
                start_time = QString::fromStdString(train_schedule_set[i].GetDepartTime().Format3());
            }
            if (train_id == train_schedule_set[i].GetTrainId() && end_station_i == train_schedule_set[i].GetStationId()) {
                end_time = QString::fromStdString(train_schedule_set[i].GetArriveTime().Format3());
            }
        }
        for (int i = 0; i < seat_set.size(); i++) {
            if (train_id == seat_set[i].GetTrainId()) {
                if (seat_set[i].GetSeatType() == "商务") {
                    business_class_sum++;
                }
                else if(seat_set[i].GetSeatType() == "一等") {
                    first_class_sum++;
                }
                else if (seat_set[i].GetSeatType() == "二等") {
                    second_class_sum++;
                }
            }
        }
        for (int i = 0; i < order_set.size(); i++) {
            Date d_date = order_set[i].GetTravelDate();
            QDate q_every_date(d_date.GetYear(), d_date.GetMonth(), d_date.GetDay());
            if (train_id == order_set[i].GetTrainId() && q_every_date == q_depart_date && (order_set[i].GetStatus() == "booked" || order_set[i].GetStatus() == "completed")) {
                int seat_id = order_set[i].GetSeatId();
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
        business_class = QString::number(business_class_sum-order_B_num);
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
        QPushButton* bookingItemButton = new QPushButton("预订");
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
        ticket_table->setCellWidget(row, 8, bookingItemButton);
        connect(bookingItemButton, &QPushButton::clicked, this, &HighSpeedRailSystem::onBookingButtonClicked);
    }
}

void HighSpeedRailSystem::refreshTicketsTableInterface()
{
    // 清空表格内容
    ui.tableWidgetTickets->clearContents();
    ui.tableWidgetTickets->setRowCount(0);

    // 重新调用设置表格的函数
    if (init_table == 1) TicketsTableSetting();
    else if(init_table == 0 || init_table == 2) SearchTickets();
}

void HighSpeedRailSystem::LoadUserInformationSetting()
{
    ui.labelNameM->setText(mp_user->GetUsername().c_str());
    ui.lineEditUserName->setText(mp_user->GetDisplayName().c_str());
    ui.lineEditUserName->setReadOnly(true);
    ui.lineEditRealName->setText(mp_user->GetUsername().c_str());
    ui.lineEditRealName->setReadOnly(true);
    ui.lineEditIdCard->setText(mp_user->GetIdCard().c_str());
    ui.lineEditIdCard->setReadOnly(true);
    ui.lineEditPhone->setText(mp_user->GetPhone().c_str());
}

static void showSuccessBox(QWidget* parent)
{
    QMessageBox SuccessBox(parent);
    SuccessBox.setWindowTitle("Success");
    SuccessBox.setText("操作成功！");
    QPixmap iconPixmap(":/new/prefix1/image/success.png");	//调整图标大小
    iconPixmap = iconPixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    SuccessBox.setIconPixmap(iconPixmap);
    SuccessBox.exec();
}

void HighSpeedRailSystem::CheckInformation()
{
    string phone = ui.lineEditPhone->text().toStdString();
    if (phone.size() != 11 || !std::all_of(phone.begin(), phone.end(), ::isdigit)) {
        QMessageBox::information(this, "info", "电话必须是11位数字。");
        return;
    }
    QString password = ui.lineEditPassword->text();
    QString new_password = ui.lineEditNewPassword->text();
    QString ensure_password = ui.lineEditEnsurePassword->text();
    if (!password.isEmpty()) {
        if (password != mp_user->GetPassword().c_str()) {
            QMessageBox::warning(this, "警告", "原始密码输入错误！");
            return;
        }
        if (new_password.isEmpty()) {
            QMessageBox::warning(this, "警告", "新密码不能为空！");
            return;
        }
        if (ensure_password != new_password) {
            QMessageBox::warning(this, "警告", "两次密码输入不一致。");
            return;
        }
        mp_user->SetPhone(phone);
        mp_user->SetPassword(new_password.toStdString());
        showSuccessBox(this);
        ui.lineEditPassword->clear();
        ui.lineEditNewPassword->clear();
        ui.lineEditEnsurePassword->clear();
    }
    else {
        mp_user->SetPhone(phone);
        showSuccessBox(this);
    }
}


void HighSpeedRailSystem::LogOff()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "退出", "确定要注销并退出系统吗？", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        for (auto p = mp_user_set->begin(); p != mp_user_set->end(); p++) {
            if (p->GetUserId() == mp_user->GetUserId()) {
                mp_user_set->erase(p);
                break;
            }
        }
        this->close();                          // 关闭当前窗口
    }
}

void HighSpeedRailSystem::OrdersTableSetting()
{
    QTableWidget* order_table = ui.tableWidgetOrder;
    int order_count = 0;
    int col_count = ui.tableWidgetOrder->columnCount();
    ui.dateEditBegin->setDate(QDate::currentDate().addDays(-7));
    ui.dateEditEnd->setDate(QDate::currentDate().addDays(7));
    ui.comboBox->setCurrentIndex(0);
    queue<int> user_order_queue;
    int idx_ = 0;
    for (deque<Order>::iterator p = order_set.begin(); p != order_set.end(); p++, idx_++) {
        if (p->GetUserId() == mp_user->GetUserId()) {
            user_order_queue.push(idx_);
            order_count++;
        }
    }
    order_table->setCurrentCell(-1, -1);
    order_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    order_table->setRowCount(order_count);
    for (int row = 0; row < order_count; row++) {
        int idx = user_order_queue.front();
        user_order_queue.pop();
        QString order_id = QString::number(order_set[idx].GetOrderId());
        QString train_code;
        Date d = order_set[idx].GetTravelDate();
        QString travel_date = d.Format().c_str();
        QDate travel_qdate(d.GetYear(), d.GetMonth(), d.GetDay());
        QString start_station;
        QString end_station;
        QString start_time;
        QString end_time;
        QString seat_no;
        QString seat_type;
        QString fare;
        QString booking_time = order_set[idx].GetOrderTime().Format2().c_str();
        QString status;
        int train_id = order_set[idx].GetTrainId();
        int start_station_id = order_set[idx].GetStartStationId();
        int end_station_id = order_set[idx].GetEndStationId();
        int seat_id = order_set[idx].GetSeatId();
        int start_stop_order = 0;
        int end_stop_order = 0;
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
        if (order_set[idx].GetStatus() == "booked") status = "已预定";
        else if (order_set[idx].GetStatus() == "canceled") status = "已取消";
        else if (order_set[idx].GetStatus() == "completed") status = "已完成";

        order_table->setItem(row, 0, new QTableWidgetItem(order_id));
        order_table->setItem(row, 1, new QTableWidgetItem(train_code));
        order_table->setItem(row, 2, new QTableWidgetItem(travel_date));
        order_table->setItem(row, 3, new QTableWidgetItem(start_station));
        order_table->setItem(row, 4, new QTableWidgetItem(end_station));
        order_table->setItem(row, 5, new QTableWidgetItem(start_time));
        order_table->setItem(row, 6, new QTableWidgetItem(end_time));
        order_table->setItem(row, 7, new QTableWidgetItem(seat_no));
        order_table->setItem(row, 8, new QTableWidgetItem(seat_type));
        order_table->setItem(row, 9, new QTableWidgetItem(fare));
        order_table->setItem(row, 10, new QTableWidgetItem(booking_time));
        order_table->setItem(row, 11, new QTableWidgetItem(status));

        if (travel_qdate<ui.dateEditBegin->date() || travel_qdate>ui.dateEditEnd->date() || status != "已预定") {
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

void HighSpeedRailSystem::OrderEditChanged()
{
    QObject::connect(ui.dateEditBegin, &QDateEdit::dateChanged, this, &HighSpeedRailSystem::OrderFiltering);
    QObject::connect(ui.dateEditEnd, &QDateEdit::dateChanged, this, &HighSpeedRailSystem::OrderFiltering);
    QObject::connect(ui.comboBox, &QComboBox::currentIndexChanged, this, &HighSpeedRailSystem::OrderFiltering);
    QObject::connect(ui.pushButtonRefund, &QPushButton::clicked, this, &HighSpeedRailSystem::RefundOperation);
    QObject::connect(ui.pushButtonRebook, &QPushButton::clicked, this, &HighSpeedRailSystem::RebookOperation);
}

void HighSpeedRailSystem::OrderFiltering()
{
    int col_travel_date = 2;
    int col_status = 11;
    QDate dateEditBegin = ui.dateEditBegin->date();
    QDate dateEditEnd = ui.dateEditEnd->date();
    QString status = ui.comboBox->currentText();
    for (int row = 0; row < ui.tableWidgetOrder->rowCount(); row++) {
        QString date_item = ui.tableWidgetOrder->item(row, col_travel_date)->text();
        QString status_item = ui.tableWidgetOrder->item(row, col_status)->text();
        Date D_date;
        D_date(date_item.toStdString());
        QDate Q_date(D_date.GetYear(), D_date.GetMonth(), D_date.GetDay());
        if (Q_date < dateEditBegin || Q_date > dateEditEnd || status != status_item) {
            ui.tableWidgetOrder->setRowHidden(row, true);
        }
        else {
            ui.tableWidgetOrder->setRowHidden(row, false);
        }
    }
    ui.tableWidgetOrder->setCurrentCell(-1, -1);
}

void HighSpeedRailSystem::RefundOperation()
{
    QTableWidget* order_table = ui.tableWidgetOrder;
    if (order_table->currentRow() == -1) {
        QMessageBox::warning(this, "警告", "请先选中要退票的订单的行！");
        return;
    }
    else if (order_table->item(order_table->currentRow(), 11)->text() != "已预定") {
        QMessageBox::warning(this, "警告", "已完成或已取消的车票不能退票。");
        return;
    }
    else {
        int order_id = 0;
        int col_status = 11;
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

void HighSpeedRailSystem::RebookOperation()
{
    QTableWidget* order_table = ui.tableWidgetOrder;
    int current_row = order_table->currentRow();
    if (current_row == -1) {
        QMessageBox::warning(this, "警告", "请先选中要改签的订单的行！");
        return;
    }
    else if (order_table->item(current_row, 11)->text() != "已预定") {
        QMessageBox::warning(this, "警告", "已完成或已取消的车票不能改签。");
        return;
    }
    else {
        ui.stackedWidgetUser->setCurrentIndex(0);
        init_table = 2;
        ui.lineEditGetOn->setText(order_table->item(current_row, 3)->text());
        ui.lineEditGetOff->setText(order_table->item(current_row, 4)->text());
        ui.lineEditGetOn->setReadOnly(true);
        ui.lineEditGetOff->setReadOnly(true);
        ui.labelRebook->show();
        ui.pushButtonExitRebook->show();
        refreshTicketsTableInterface();

    }
}

void HighSpeedRailSystem::RefreshTicketTable(string on_station, string off_station, QDate q_depart_date)
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
        QPushButton* bookingItemButton = new QPushButton(init_table == 2 ? "改签" : "预约");
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
        connect(bookingItemButton, &QPushButton::clicked, this, &HighSpeedRailSystem::onBookingButtonClicked);
        ticket_table->setCellWidget(row, 8, bookingItemButton);
        ticket_table->setRowHidden(row, false);
        row++;
    }
    ticket_table->setRowCount(row);
}

void HighSpeedRailSystem::TicketSearchPretreatment()
{
    string on_station = ui.lineEditGetOn->text().toStdString();
    string off_station = ui.lineEditGetOff->text().toStdString();
    if (on_station.empty()) {
        QMessageBox::information(this, "info", "请输入出发地。");
        return;
    }
    else if (off_station.empty()) {
        QMessageBox::information(this, "info", "请输入到达地。");
        return;
    }
    int tag_on = 0;
    int tag_off = 0;
    for (deque<Station>::iterator p = station_set.begin(); p != station_set.end(); p++) {
        if (on_station == p->GetName()) {
            tag_on = 1;
        }
        if (off_station == p->GetName()) {
            tag_off = 1;
        }
    }
    if (tag_on == 0) {
        ui.tableWidgetTickets->setRowCount(0);
        QMessageBox::information(this, "info", "出发地不存在。");
        return;
    }
    if (tag_off == 0) {
        ui.tableWidgetTickets->setRowCount(0);
        QMessageBox::information(this, "info", "到达地不存在。");
        return;
    }
    if (on_station == off_station) {
        ui.tableWidgetTickets->setRowCount(0);
        QMessageBox::information(this, "info", "出发地和到达地不能为同一地点。");
        return;
    }
    if (ui.dateEditDepart->date() < QDate::currentDate() || ui.dateEditDepart->date() > QDate::currentDate().addDays(15)) {
        ui.tableWidgetTickets->setRowCount(0);
        QMessageBox::information(this, "info", "当前发车时间不能订票。");
        return;
    }
    if (ui.checkBoxTrainNum->isChecked() && ui.lineEditTrainNum->text().isEmpty()) {
        QMessageBox::information(this, "info", "请输入要查询的车次。");
        return;
    }
    this->SearchTickets();
}

void HighSpeedRailSystem::SearchTickets()
{
    string on_station = ui.lineEditGetOn->text().toStdString();
    string off_station = ui.lineEditGetOff->text().toStdString();
    QDate q_depart_date = ui.dateEditDepart->date();

    if (init_table == 1) init_table = 0;
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

void HighSpeedRailSystem::onBookingButtonClicked()
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

    if (init_table == 2) {
        QObject::connect(purchasingDlg, &TicketPurchasingInterface::Purchased, this, &HighSpeedRailSystem::CompletedAndExit);
    }
    
    purchasingDlg->exec();
    
    refreshTicketsTableInterface();

}

void HighSpeedRailSystem::UpdateCompleted()
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
            if (order_p->GetStatus() == "booked") {
                order_p->SetStatus("completed");
            }
        }
    }
}

void HighSpeedRailSystem::CompletedAndExit()
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
    init_table = 0;
    ui.lineEditGetOn->setReadOnly(false);
    ui.lineEditGetOff->setReadOnly(false);
    ui.labelRebook->hide();
    ui.pushButtonExitRebook->hide();
    order_table->setCurrentCell(-1, -1);
    ui.stackedWidgetUser->setCurrentIndex(1);
    UpdateCompleted();
    OrdersTableSetting();
}