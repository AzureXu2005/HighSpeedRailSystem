#pragma once

#include <QObject>
#include "ui_TicketPurchasingInterface.h"
#include "Users.h"
#include "Train.h"
#include "Station.h"
#include "TrainSchedule.h"
#include "Order.h"
#include "TemplateSet.h"
#include "Seat.h"


class TicketPurchasingInterface  : public QDialog
{
	Q_OBJECT

public:
	TicketPurchasingInterface(Date depart_date, Users* booking_user, Train* booking_train, Station* start_station,
		Station* end_station, vector<QString> remaining_para, TemplateSet<Order>* order_set, TemplateSet<TrainSchedule>* train_schedule_set,
		TemplateSet<Seat>* seat_set, QDialog* parent = Q_NULLPTR);
	~TicketPurchasingInterface();
	void PurchasingTicket();
	int chooseSeat(int train_id, string seat_type, Date& travel_data);

public:
	Date depart_date;
	Users* booking_user;
	Train* booking_train;
	Station* start_station;
	Station* end_station;
	TemplateSet<Order>* order_set;
	TemplateSet<TrainSchedule>* train_schedule_set;
	TemplateSet<Seat>* seat_set;

signals:
	void Purchased();

private:
	Ui::TicketPurchasingDlg* ui;
};

