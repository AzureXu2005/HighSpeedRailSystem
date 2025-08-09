#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HighSpeedRailSystem.h"
#include "Users.h"
#include "Train.h"
#include "TrainSchedule.h"
#include "Order.h"
#include "Station.h"
#include "Seat.h"
#include "TemplateSet.h"


class HighSpeedRailSystem : public QMainWindow
{
    Q_OBJECT

public:
    HighSpeedRailSystem(Users* user, TemplateSet<Users>* user_set, QWidget* parent = nullptr);
    ~HighSpeedRailSystem();
    void LoadFromTxtFile();
    void TicketsTableSetting();
    //void InitFreshTable(QTableWidget* ticket_table, int row, QDate q_depart_date);
    void refreshTicketsTableInterface();
    void OrdersTableSetting();
    void LoadUserInformationSetting();
    void CheckInformation();
    void LogOff();
    void OrderEditChanged();
    void OrderFiltering();
    void RefundOperation();
    void RebookOperation();
    void RefreshTicketTable(string on_station, string off_station, QDate q_depart_date);
    void TicketSearchPretreatment();
    void SearchTickets();
    void onBookingButtonClicked();
    void UpdateCompleted();
    
public:
    int init_table;
    Users* mp_user;
    TemplateSet<Users>* mp_user_set;
    TemplateSet<Train> train_set;
    TemplateSet<TrainSchedule> train_schedule_set;
    TemplateSet<Order> order_set;
    TemplateSet<Station> station_set;
    TemplateSet<Seat> seat_set;

public slots:
    void CompletedAndExit();

private:
    Ui::HighSpeedRailSystemClass ui;


};

