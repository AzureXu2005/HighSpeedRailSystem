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
		TemplateSet<Seat>* seat_set, QDialog* parent = Q_NULLPTR);	//车票购买构造函数，传递车票购买需要的参数，并做一些初始化的操作
	~TicketPurchasingInterface();	//析构函数，释放ui空间
	void PurchasingTicket();	//购买车票，检查是否有余票，并生成订单
	int chooseSeat(int train_id, string seat_type, Date& travel_data);	//选择座位，自动获取用户所选类型座位的座位号

public:
	Date depart_date;	//出发日期
	Users* booking_user;	//预定车票的用户
	Train* booking_train;	//预定的列车
	Station* start_station;	//上车站点
	Station* end_station;	//下车站点
	TemplateSet<Order>* order_set;	//订单集合
	TemplateSet<TrainSchedule>* train_schedule_set;	//列车时刻表集合
	TemplateSet<Seat>* seat_set;	//座位集合

signals:
	void Purchased();	//购买成功信号

private:
	Ui::TicketPurchasingDlg* ui;	//购买车票ui界面
};

