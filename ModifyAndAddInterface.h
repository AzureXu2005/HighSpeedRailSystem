#pragma once

#include <QObject>
#include "ui_ModifyAndAddDlg.h"
#include "TemplateSet.h"
#include "Train.h"
#include "TrainSchedule.h"
#include "Seat.h"
#include "Station.h"

class ModifyAndAddInterface  : public QDialog
{
	Q_OBJECT

public:
	ModifyAndAddInterface(int kind, QDialog* parent = Q_NULLPTR);
	~ModifyAndAddInterface();
	void TrainSetting(Train* train, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set);
	void StationSetting(Station* station, TemplateSet<Station>* station_set);
	void ScheduleSetting(TrainSchedule* schedule, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set);
	void SeatSetting(Seat* seat, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set, TemplateSet<Seat>* seat_set);
	void TrainAddInit(Train* train, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set);
	void StationAddInit(Station* station, TemplateSet<Station>* station_set);
	void ScheduleAddInit(TrainSchedule* schedule, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set);
	void SeatAddInit(Seat* seat, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set, TemplateSet<Seat>* seat_set);
	void ModifySave();
	void AddSave();

signals:
	void AddItem();


public:
	int kind;
	Train* p_train;
	Station* p_station;
	TrainSchedule* p_schedule;
	Seat* p_seat;
	TemplateSet<Station>* p_station_set;
	TemplateSet<Train>* p_train_set;
	TemplateSet<Seat>* p_seat_set;

private:
	Ui::ModifyAndAddDialog* ui;
};

