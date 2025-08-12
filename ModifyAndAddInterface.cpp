#include "ModifyAndAddInterface.h"
#include "qmessagebox.h"

ModifyAndAddInterface::ModifyAndAddInterface(int kind, QDialog* parent)
	: QDialog(parent), ui(new Ui::ModifyAndAddDialog)
{
	ui->setupUi(this);
	this->kind = kind;
	p_train = NULL;
	p_schedule = NULL;
	p_station = NULL;
	p_seat = NULL;
	p_station_set = NULL;
	p_train_set = NULL;
	p_seat_set = NULL;
	QObject::connect(ui->pushButtonCancel, &QPushButton::clicked, [=] {
		this->close();
		});
	QObject::connect(ui->pushButtonOK, &QPushButton::clicked, [=] {
		if (kind % 2 == 0) {
			ModifySave();
		}
		else {
			AddSave();
		}
		});
}

ModifyAndAddInterface::~ModifyAndAddInterface()
{
	delete ui;
}

void ModifyAndAddInterface::TrainSetting(Train* train, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set)
{
	p_train = train;
	p_train_set = train_set;
	p_station_set = station_set;
	ui->labelTitle->setText("修改列车信息");
	ui->stackedWidgetMA->setCurrentIndex(0);
	ui->lineEditTrainId->setText(QString::number(p_train->GetTrainId()));
	ui->lineEditTrainId->setEnabled(false);
	ui->lineEditTrainCode->setText(p_train->GetTrainCode().c_str());
	ui->comboBoxTrainType->setCurrentText(QChar(p_train->GetType()));
	int start_station_id = p_train->GetStartStation();
	int end_station_id = p_train->GetEndStation();
	QString start_station;
	QString end_station;
	for (deque<Station>::iterator p = p_station_set->begin(); p != p_station_set->end(); p++) {
		ui->comboBoxTrainSStation->addItem(p->GetName().c_str());
		ui->comboBoxTrainEStation->addItem(p->GetName().c_str());
		if (start_station_id == p->GetStationId()) {
			start_station = p->GetName().c_str();
		}
		if (end_station_id == p->GetStationId()) {
			end_station = p->GetName().c_str();
		}
	}
	ui->comboBoxTrainSStation->setCurrentText(start_station);
	ui->comboBoxTrainEStation->setCurrentText(end_station);
	
}

void ModifyAndAddInterface::StationSetting(Station* station, TemplateSet<Station>* station_set)
{
	p_station = station;
	p_station_set = station_set;
	ui->labelTitle->setText("修改站点信息");
	ui->stackedWidgetMA->setCurrentIndex(1);
	ui->lineEditStationId->setText(QString::number(p_station->GetStationId()));
	ui->lineEditStationId->setEnabled(false);
	ui->lineEditStationName->setText(p_station->GetName().c_str());
}

void ModifyAndAddInterface::ScheduleSetting(TrainSchedule* schedule, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set)
{
	p_schedule = schedule;
	p_train_set = train_set;
	p_station_set = station_set;
	ui->labelTitle->setText("修改时刻表信息");
	ui->stackedWidgetMA->setCurrentIndex(2);
	ui->lineEditScheduleId->setText(QString::number(p_schedule->GetScheduleId()));
	ui->lineEditScheduleId->setEnabled(false);
	int train_id = p_schedule->GetTrainId();
	QString train_code;
	for (deque<Train>::iterator p = p_train_set->begin(); p != p_train_set->end(); p++) {
		ui->comboBoxScheduleTC->addItem(p->GetTrainCode().c_str());
		if (train_id == p->GetTrainId()) {
			train_code = p->GetTrainCode().c_str();
		}
	}
	ui->comboBoxScheduleTC->setCurrentText(train_code);
	int stop_station_id = p_schedule->GetStationId();
	QString stop_station;
	for (deque<Station>::iterator p = p_station_set->begin(); p != p_station_set->end(); p++) {
		ui->comboBoxScheduleAS->addItem(p->GetName().c_str());
		if (stop_station_id == p->GetStationId()) {
			stop_station = p->GetName().c_str();
		}
	}
	ui->comboBoxScheduleAS->setCurrentText(stop_station);
	ui->spinBoxScheduleAO->setValue(p_schedule->GetStopOrder());
	QTime arrive_time(p_schedule->GetArriveTime().GetHour(), p_schedule->GetArriveTime().GetMinute());
	QTime depart_time(p_schedule->GetDepartTime().GetHour(), p_schedule->GetDepartTime().GetMinute());
	ui->timeEditAT->setTime(arrive_time);
	ui->timeEditDT->setTime(depart_time);
}

void ModifyAndAddInterface::SeatSetting(Seat* seat, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set, TemplateSet<Seat>* seat_set)
{
	p_seat = seat;
	p_train_set = train_set;
	p_station_set = station_set;
	p_seat_set = seat_set;
	ui->labelTitle->setText("修改座位信息");
	ui->stackedWidgetMA->setCurrentIndex(3);
	ui->lineEditSeatId->setText(QString::number(p_seat->GetSeatId()));
	ui->lineEditSeatId->setEnabled(false);
	int train_id = p_seat->GetTrainId();
	QString train_code;
	for (deque<Train>::iterator p = p_train_set->begin(); p != p_train_set->end(); p++) {
		ui->comboBoxSeatTC->addItem(p->GetTrainCode().c_str());
		if (train_id == p->GetTrainId()) {
			train_code = p->GetTrainCode().c_str();
		}
	}
	ui->comboBoxSeatTC->setCurrentText(train_code);
	ui->lineEditSeatNo->setText(p_seat->GetSeatNo().c_str());
	ui->comboBoxSeatType->setCurrentText(p_seat->GetSeatType().c_str());
}

void ModifyAndAddInterface::TrainAddInit(Train* train, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set)
{
	p_train = train;
	p_train_set = train_set;
	p_station_set = station_set;
	ui->labelTitle->setText("添加列车");
	ui->stackedWidgetMA->setCurrentIndex(0);
	ui->lineEditTrainId->setText(QString::number(p_train->GetTrainId()));
	ui->lineEditTrainId->setEnabled(false);
	ui->comboBoxTrainType->setCurrentIndex(-1);
	ui->comboBoxTrainSStation->setCurrentIndex(-1);
	ui->comboBoxTrainEStation->setCurrentIndex(-1);
	for (deque<Station>::iterator p = p_station_set->begin(); p != p_station_set->end(); p++) {
		ui->comboBoxTrainSStation->addItem(p->GetName().c_str());
		ui->comboBoxTrainEStation->addItem(p->GetName().c_str());
	}
	
}

void ModifyAndAddInterface::StationAddInit(Station* station, TemplateSet<Station>* station_set)
{
	p_station = station;
	p_station_set = station_set;
	ui->labelTitle->setText("添加车站");
	ui->stackedWidgetMA->setCurrentIndex(1);
	ui->lineEditStationId->setText(QString::number(p_station->GetStationId()));
	ui->lineEditStationId->setEnabled(false);

}

void ModifyAndAddInterface::ScheduleAddInit(TrainSchedule* schedule, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set)
{
	p_schedule = schedule;
	p_train_set = train_set;
	p_station_set = station_set;
	ui->labelTitle->setText("添加时刻表");
	ui->stackedWidgetMA->setCurrentIndex(2);
	ui->lineEditScheduleId->setText(QString::number(p_schedule->GetScheduleId()));
	ui->lineEditScheduleId->setEnabled(false);
	ui->comboBoxScheduleTC->setCurrentIndex(-1);
	ui->comboBoxScheduleAS->setCurrentIndex(-1);
	for (deque<Train>::iterator p = p_train_set->begin(); p != p_train_set->end(); p++) {
		ui->comboBoxScheduleTC->addItem(p->GetTrainCode().c_str());
	}
	for (deque<Station>::iterator p = p_station_set->begin(); p != p_station_set->end(); p++) {
		ui->comboBoxScheduleAS->addItem(p->GetName().c_str());
	}
}

void ModifyAndAddInterface::SeatAddInit(Seat* seat, TemplateSet<Train>* train_set, TemplateSet<Station>* station_set, TemplateSet<Seat>* seat_set)
{
	p_seat = seat;
	p_train_set = train_set;
	p_station_set = station_set;
	p_seat_set = seat_set;
	ui->labelTitle->setText("添加座位");
	ui->stackedWidgetMA->setCurrentIndex(3);
	ui->lineEditSeatId->setText(QString::number(p_seat->GetSeatId()));
	ui->lineEditSeatId->setEnabled(false);
	ui->comboBoxSeatTC->setCurrentIndex(-1);
	ui->comboBoxSeatType->setCurrentIndex(-1);
	for (deque<Train>::iterator p = p_train_set->begin(); p != p_train_set->end(); p++) {
		ui->comboBoxSeatTC->addItem(p->GetTrainCode().c_str());
	}
}


void ModifyAndAddInterface::ModifySave()
{
	if (kind == 0) {
		if (ui->lineEditTrainCode->text().isEmpty() || ui->comboBoxTrainEStation->currentIndex() == -1 ||
			ui->comboBoxTrainSStation->currentIndex() == -1 || ui->comboBoxTrainType->currentIndex() == -1) {
			QMessageBox::warning(this, "警告", "信息有空，请补全信息。");
			return;
		}
		string train_code = ui->lineEditTrainCode->text().toStdString();
		for(deque<Train>::iterator p = p_train_set->begin(); p != p_train_set->end(); p++) {
			if (train_code == p->GetTrainCode() && p_train->GetTrainId() != p->GetTrainId()) {
				QMessageBox::warning(this, "警告", "列车车次名称重名，请修改列车车次。");
				return;
			}
		}
		char train_type = ui->comboBoxTrainType->currentText().toStdString()[0];
		string start_station = ui->comboBoxTrainSStation->currentText().toStdString();
		string end_station = ui->comboBoxTrainEStation->currentText().toStdString();
		if (start_station == end_station) {
			QMessageBox::warning(this, "警告", "始发站与终点站不能相同。");
			return;
		}
		int start_station_id = 0;
		int end_station_id = 0;
		for (deque<Station>::iterator p = p_station_set->begin(); p != p_station_set->end(); p++) {
			if (start_station == p->GetName()) {
				start_station_id = p->GetStationId();
			}
			if (end_station == p->GetName()) {
				end_station_id = p->GetStationId();
			}
		}
		p_train->SetTrainCode(train_code);
		p_train->SetType(train_type);
		p_train->SetStartStation(start_station_id);
		p_train->SetEndStation(end_station_id);
		QMessageBox::information(this, "info", "已完成修改。\n请仔细检查修改逻辑是否正确。");
		this->close();
	}
	else if (kind == 2) {
		if (ui->lineEditStationName->text().isEmpty()) {
			QMessageBox::warning(this, "警告", "信息有空，请补全信息。");
			return;
		}
		int station_id = ui->lineEditStationId->text().toInt();
		string station_name = ui->lineEditStationName->text().toStdString();
		for (deque<Station>::iterator p = p_station_set->begin(); p != p_station_set->end(); p++) {
			if (station_name == p->GetName() && station_id != p->GetStationId()) {
				QMessageBox::warning(this, "警告", "站点名字有重复，请修改站点名字。");
				return;
			}
		}
		p_station->SetName(station_name);
		QMessageBox::information(this, "info", "已完成修改。\n请仔细检查修改逻辑是否正确。");
		this->close();
	}
	else if (kind == 4) {
		if (ui->comboBoxScheduleTC->currentIndex() == -1 || ui->comboBoxScheduleAS->currentIndex() == -1) {
			QMessageBox::warning(this, "警告", "信息有空，请补全信息。");
			return;
		}
		int schedule_id = ui->lineEditScheduleId->text().toInt();
		string train_code = ui->comboBoxScheduleTC->currentText().toStdString();
		int train_id = 0;
		int end_station_id = 0;
		for (deque<Train>::iterator p = p_train_set->begin(); p != p_train_set->end(); p++) {
			if (train_code == p->GetTrainCode()) {
				train_id = p->GetTrainId();
				end_station_id = p->GetEndStation();
				break;
			}
		}
		string stop_station = ui->comboBoxScheduleAS->currentText().toStdString();
		int stop_station_id = 0;
		for (deque<Station>::iterator p = p_station_set->begin(); p != p_station_set->end(); p++) {
			if (stop_station == p->GetName()) {
				stop_station_id = p->GetStationId();
				break;
			}
		}
		int stop_order = ui->spinBoxScheduleAO->value();
		Date arrive_time;
		if (stop_order == 1) {
			arrive_time(0, 0, 0, 0, 0);
		}
		else {
			arrive_time(0, 0, 0, ui->timeEditAT->time().hour(), ui->timeEditAT->time().minute());
		}
		Date depart_time;
		if (end_station_id == stop_station_id) {
			depart_time(0, 0, 0, 0, 0);
		}
		else {
			depart_time(0, 0, 0, ui->timeEditDT->time().hour(), ui->timeEditDT->time().minute());
		}
		p_schedule->SetTrainId(train_id);
		p_schedule->SetStationId(stop_station_id);
		p_schedule->SetStopOrder(stop_order);
		p_schedule->SetArriveTime(arrive_time);
		p_schedule->SetDepartTime(depart_time);
		QMessageBox::information(this, "info", "已完成修改。\n请仔细检查修改逻辑是否正确。");
		this->close();
		
	}
	else if (kind == 6) {
		if (ui->comboBoxSeatTC->currentIndex() == -1 || ui->comboBoxSeatType->currentIndex() == -1 || ui->lineEditSeatNo->text().isEmpty()) {
			QMessageBox::warning(this, "警告", "信息有空，请补全信息。");
			return;
		}
		int seat_id = ui->lineEditSeatId->text().toInt();
		int train_id = 0;
		string train_code = ui->comboBoxSeatTC->currentText().toStdString();
		for (deque<Train>::iterator p = p_train_set->begin(); p != p_train_set->end(); p++) {
			if (train_code == p->GetTrainCode()) {
				train_id = p->GetTrainId();
				break;
			}
		}
		string seat_no = ui->lineEditSeatNo->text().toStdString();
		for (deque<Seat>::iterator p = p_seat_set->begin(); p != p_seat_set->end(); p++) {
			if (train_id == p->GetTrainId() && seat_no == p->GetSeatNo() && seat_id != p->GetSeatId()) {
				QMessageBox::warning(this, "警告", "当前车次的该座位号已经存在。");
				return;
			}
		}
		string seat_type = ui->comboBoxSeatType->currentText().toStdString();
		p_seat->SetTrainId(train_id);
		p_seat->SetSeatNo(seat_no);
		p_seat->SetSeatType(seat_type);
		QMessageBox::information(this, "info", "已完成修改。\n请仔细检查修改逻辑是否正确。");
		this->close();
	}
}

void ModifyAndAddInterface::AddSave()
{
	if (kind == 1) {
		if (ui->lineEditTrainCode->text().isEmpty() || ui->comboBoxTrainEStation->currentIndex() == -1 ||
			ui->comboBoxTrainSStation->currentIndex() == -1 || ui->comboBoxTrainType->currentIndex() == -1) {
			QMessageBox::warning(this, "警告", "信息有空，请补全信息。");
			return;
		}
		string train_code = ui->lineEditTrainCode->text().toStdString();
		for (deque<Train>::iterator p = p_train_set->begin(); p != p_train_set->end(); p++) {
			if (train_code == p->GetTrainCode()) {
				QMessageBox::warning(this, "警告", "该车次名称已经存在，请修改新增列车车次。");
				return;
			}
		}
		char train_type = ui->comboBoxTrainType->currentText().toStdString()[0];
		string start_station = ui->comboBoxTrainSStation->currentText().toStdString();
		string end_station = ui->comboBoxTrainEStation->currentText().toStdString();
		if (start_station == end_station) {
			QMessageBox::warning(this, "警告", "始发站与终点站不能相同。");
			return;
		}
		int start_station_id = 0;
		int end_station_id = 0;
		for (deque<Station>::iterator p = p_station_set->begin(); p != p_station_set->end(); p++) {
			if (start_station == p->GetName()) {
				start_station_id = p->GetStationId();
			}
			if (end_station == p->GetName()) {
				end_station_id = p->GetStationId();
			}
		}
		p_train->SetTrainCode(train_code);
		p_train->SetType(train_type);
		p_train->SetStartStation(start_station_id);
		p_train->SetEndStation(end_station_id);
		emit AddItem();
		this->close();
	}
	else if (kind == 3) {
		if (ui->lineEditStationName->text().isEmpty()) {
			QMessageBox::warning(this, "警告", "信息有空，请补全信息。");
			return;
		}
		int station_id = ui->lineEditStationId->text().toInt();
		string station_name = ui->lineEditStationName->text().toStdString();
		for (deque<Station>::iterator p = p_station_set->begin(); p != p_station_set->end(); p++) {
			if (station_name == p->GetName()) {
				QMessageBox::warning(this, "警告", "站点名字有重复，请修改站点名字。");
				return;
			}
		}
		p_station->SetName(station_name);
		emit AddItem();
		this->close();
	}
	else if (kind == 5) {
		if (ui->comboBoxScheduleTC->currentIndex() == -1 || ui->comboBoxScheduleAS->currentIndex() == -1) {
			QMessageBox::warning(this, "警告", "信息有空，请补全信息。");
			return;
		}
		int schedule_id = ui->lineEditScheduleId->text().toInt();
		string train_code = ui->comboBoxScheduleTC->currentText().toStdString();
		int train_id = 0;
		int end_station_id = 0;
		for (deque<Train>::iterator p = p_train_set->begin(); p != p_train_set->end(); p++) {
			if (train_code == p->GetTrainCode()) {
				train_id = p->GetTrainId();
				end_station_id = p->GetEndStation();
				break;
			}
		}
		string stop_station = ui->comboBoxScheduleAS->currentText().toStdString();
		int stop_station_id = 0;
		for (deque<Station>::iterator p = p_station_set->begin(); p != p_station_set->end(); p++) {
			if (stop_station == p->GetName()) {
				stop_station_id = p->GetStationId();
				break;
			}
		}
		int stop_order = ui->spinBoxScheduleAO->value();
		Date arrive_time;
		if (stop_order == 1) {
			arrive_time(0, 0, 0, 0, 0);
		}
		else {
			arrive_time(0, 0, 0, ui->timeEditAT->time().hour(), ui->timeEditAT->time().minute());
		}
		Date depart_time;
		if (end_station_id == stop_station_id) {
			depart_time(0, 0, 0, 0, 0);
		}
		else {
			depart_time(0, 0, 0, ui->timeEditDT->time().hour(), ui->timeEditDT->time().minute());
		}
		p_schedule->SetTrainId(train_id);
		p_schedule->SetStationId(stop_station_id);
		p_schedule->SetStopOrder(stop_order);
		p_schedule->SetArriveTime(arrive_time);
		p_schedule->SetDepartTime(depart_time);
		emit AddItem();
		this->close();

	}
	else if (kind == 7) {
		if (ui->comboBoxSeatTC->currentIndex() == -1 || ui->comboBoxSeatType->currentIndex() == -1 || ui->lineEditSeatNo->text().isEmpty()) {
			QMessageBox::warning(this, "警告", "信息有空，请补全信息。");
			return;
		}
		int seat_id = ui->lineEditSeatId->text().toInt();
		int train_id = 0;
		string train_code = ui->comboBoxSeatTC->currentText().toStdString();
		for (deque<Train>::iterator p = p_train_set->begin(); p != p_train_set->end(); p++) {
			if (train_code == p->GetTrainCode()) {
				train_id = p->GetTrainId();
				break;
			}
		}
		string seat_no = ui->lineEditSeatNo->text().toStdString();
		for (deque<Seat>::iterator p = p_seat_set->begin(); p != p_seat_set->end(); p++) {
			if (train_id == p->GetTrainId() && seat_no == p->GetSeatNo() && seat_id != p->GetSeatId()) {
				QMessageBox::warning(this, "警告", "当前车次的该座位号已经存在。");
				return;
			}
		}
		string seat_type = ui->comboBoxSeatType->currentText().toStdString();
		p_seat->SetTrainId(train_id);
		p_seat->SetSeatNo(seat_no);
		p_seat->SetSeatType(seat_type);
		emit AddItem();
		this->close();
	}
}
