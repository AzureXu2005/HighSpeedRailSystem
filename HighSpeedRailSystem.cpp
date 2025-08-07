#include "HighSpeedRailSystem.h"
#include "qmessagebox.h"
#include "qscreen.h"
#include "queue"

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
    this->TicketsTableSetting();
    ui.stackedWidgetUser->setCurrentIndex(0);
    QObject::connect(ui.pushButtonBooking, &QPushButton::clicked, [=] {
        ui.stackedWidgetUser->setCurrentIndex(0);
        });
    QObject::connect(ui.pushButtonSearch, &QPushButton::clicked, [=] {
        ui.stackedWidgetUser->setCurrentIndex(1);
        OrdersTableSetting();
        });
    QObject::connect(ui.pushButtonInfor, &QPushButton::clicked, [=] {
        ui.stackedWidgetUser->setCurrentIndex(2);
        LoadUserInformationSetting();
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
{}


void HighSpeedRailSystem::LoadFromTxtFile()
{
    train_set.ReadFromFile("trains.txt");
    train_schedule_set.ReadFromFile("train_schedule.txt");
    order_set.ReadFromFile("orders.txt");
    station_set.ReadFromFile("stations.txt");
    seat_set.ReadFromFile("seats.txt");
}

void HighSpeedRailSystem::TicketsTableSetting()
{
    ui.dateEditDepart->setDate(QDate::currentDate());
    QTableWidget* ticket_table = ui.tableWidgetTickets;
    int train_count = train_set.size();
    ticket_table->setCurrentCell(-1, -1);
    ticket_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ticket_table->setRowCount(train_count);
    
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
            if (train_id == order_set[i].GetTrainId()) {
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
        ticket_table->setCellWidget(row, 8, bookingItemButton);
    }
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
    ui.dateEditBegin->setDate(QDate::currentDate().addDays(-60));
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
}

void HighSpeedRailSystem::RefundOperation()
{
    QTableWidget* order_table = ui.tableWidgetOrder;
    if (order_table->currentRow() == -1) {
        QMessageBox::warning(this, "警告", "请先选中要退票的订单的行！");
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

void HighSpeedRailSystem::RefreshOrderTable()
{
    
}