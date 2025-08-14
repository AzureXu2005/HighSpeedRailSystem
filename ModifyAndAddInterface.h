#pragma once

#include <QObject>
#include "ui_ModifyAndAddDlg.h"
#include "TemplateSet.h"
#include "Train.h"
#include "TrainSchedule.h"
#include "Seat.h"
#include "Station.h"

/*增加或修改列车信息、站点信息、时刻表信息、座位信息的界面类*/
class ModifyAndAddInterface  : public QDialog
{
	Q_OBJECT

public:
	ModifyAndAddInterface(int kind, QDialog* parent = Q_NULLPTR);	//构造函数初始化界面并传递操作类型
	~ModifyAndAddInterface();	//析构函数
	void TrainSetting(Train* train, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set);	//列车修改操作界面设置
	void StationSetting(Station* station, TemplateSet<Station>* station_set);	//站点修改操作界面设置
	void ScheduleSetting(TrainSchedule* schedule, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set);	//时刻表修改操作界面设置
	void SeatSetting(Seat* seat, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set, TemplateSet<Seat>* seat_set);	//座位修改操作界面设置
	void TrainAddInit(Train* train, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set);	//列车增加操作界面设置
	void StationAddInit(Station* station, TemplateSet<Station>* station_set);	//站点增加操作界面设置
	void ScheduleAddInit(TrainSchedule* schedule, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set);	//时刻表赠加操作界面设置
	void SeatAddInit(Seat* seat, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set, TemplateSet<Seat>* seat_set);	//座位增加操作界面设置
	void ModifySave();	//修改保存函数，保存修改的信息
	void AddSave();	//增加信息函数，增加一条列车、站点、时刻表或座位信息

signals:
	void AddItem();	//增加信息已准备好的信号


public:
	int kind;	//增加或修改的类别
	Train* p_train;	//增加或修改的列车指针
	Station* p_station;	//增加或修改的站点指针
	TrainSchedule* p_schedule;	//增加或修改的时刻表指针
	Seat* p_seat;	//增加或修改的座位指针
	TemplateSet<Station>* p_station_set;	//车站集合指针
	TemplateSet<Train>* p_train_set;	//列车集合指针
	TemplateSet<Seat>* p_seat_set;	//座位集合指针

private:
	Ui::ModifyAndAddDialog* ui;	//界面设计ui
};

