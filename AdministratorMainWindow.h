#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AdministratorMainWindow.h"
#include "Users.h"
#include "TemplateSet.h"
#include "Train.h"
#include "Station.h"
#include "TrainSchedule.h"
#include "Order.h"
#include "Seat.h"


class AdministratorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AdministratorMainWindow(Users* user, TemplateSet<Users>* user_set, QWidget* parent = nullptr);
    ~AdministratorMainWindow();
    void InitTableSetting();
    void LoadFromTxtFile();
    void WriteToTxtFile();
    void TrainTableSetting();
    void RefreshTrainTable();
    void TrainSearch();
    void onClickedTrainOperation();
    void StationTableSetting();
    void RefreshStationTable();
    void StationSearch();
    void onClickedStationOperation();
    void ScheduleTableSetting();
    void RefreshScheduleTable();
    void ScheduleSearch();
    void onClickedScheduleOperation();
    void SeatTableSetting();
    void RefreshSeatTable();
    void SeatSearch();
    void onClickedSeatOperation();
    void addRecord(int kind);
    void modifyRecord(int kind);
    void deleteRecord(int kind);
    void OrdersTableSetting();
    void OrderFiltering();
    void OrderEditChanged();
    void RefundOperation();
    void RebookOperation();
    void SearchTickets();
    void RefreshTicketTable(string on_station, string off_station, QDate q_depart_date);
    void onBookingButtonClicked();
    void UpdateCompleted();
    void ExitRebookDlg();
    void UsersTableSetting();
    void RefreshUsersTable();
    void UsersSearch();

public slots:
    void CompletedAndExit();
    void onResetPasswordClicked();  // 处理密码重置/确认修改
    void onToggleLockClicked();     // 处理锁定/解锁

public:
    int rebook_table;
    Users* mp_admin;
    Users* mp_user;
    TemplateSet<Users>* mp_user_set;
    TemplateSet<Train> train_set;
    TemplateSet<TrainSchedule> train_schedule_set;
    TemplateSet<Order> order_set;
    TemplateSet<Station> station_set;
    TemplateSet<Seat> seat_set;

private:
    Ui::AdministratorMainWindow ui;


};

static void showSuccessBox(QWidget* parent);
