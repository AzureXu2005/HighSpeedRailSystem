#include "HighSpeedRailSystem.h"
#include "qmessagebox.h"
#include "qscreen.h"

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
    QTableWidget* ticket_table = ui.tableWidgetTickets;
    int train_count = train_set.size();
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

