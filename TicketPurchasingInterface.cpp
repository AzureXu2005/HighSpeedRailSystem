#include "TicketPurchasingInterface.h"
#include "qbuttongroup.h"
#include "qmessagebox.h"


TicketPurchasingInterface::TicketPurchasingInterface(Date depart_date, Users* booking_user, Train* booking_train, Station* start_station, 
	Station* end_station, vector<QString> remaining_para, TemplateSet<Order>* order_set, TemplateSet<TrainSchedule>* train_schedule_set, TemplateSet<Seat>* seat_set,
	QDialog *parent)
	: QDialog(parent), ui(new Ui::TicketPurchasingDlg)
{
	ui->setupUi(this);
	this->depart_date = depart_date;
	this->booking_user = booking_user;
	this->booking_train = booking_train;
	this->start_station = start_station;
	this->end_station = end_station;
	this->order_set = order_set;
	this->train_schedule_set = train_schedule_set;
	this->seat_set = seat_set;

	ui->labelStartStation->setText(start_station->GetName().c_str());
	ui->labelEndStation->setText(end_station->GetName().c_str());
	ui->labelTrainCode->setText(booking_train->GetTrainCode().c_str());
	string type = "(" + string(1, booking_train->GetType()) + ")";
	ui->labelTrainType->setText(type.c_str());
	char depart_date_str[20];
	sprintf_s(depart_date_str, "%d年%d月%d日", depart_date.GetYear(), depart_date.GetMonth(), depart_date.GetDay());
	ui->labelDepartDate->setText(depart_date_str);
	ui->labelName->setText(booking_user->GetUsername().c_str());
	ui->labelIdCard->setText(booking_user->GetIdCard().c_str());
	int train_id = booking_train->GetTrainId();
	int start_station_id = start_station->GetStationId();
	int end_station_id = end_station->GetStationId();
	int start_stop_order = 0;
	int end_stop_order = 0;
	for (deque<TrainSchedule>::iterator p = train_schedule_set->begin(); p != train_schedule_set->end(); p++) {
		if (train_id == p->GetTrainId()) {
			if (start_station_id == p->GetStationId()) {
				ui->labelDepartTime->setText(p->GetDepartTime().Format3().c_str());
				start_stop_order = p->GetStopOrder();
			}
			if (end_station_id == p->GetStationId()) {
				ui->labelArriveTime->setText(p->GetArriveTime().Format3().c_str());
				end_stop_order = p->GetStopOrder();
			}
		}
	}
	ui->labelFare2->setText("￥" + QString::number(50 * (end_stop_order - start_stop_order)));
	ui->labelFare1->setText("￥" + QString::number(90 * (end_stop_order - start_stop_order)));
	ui->labelFare0->setText("￥" + QString::number(150 * (end_stop_order - start_stop_order)));
	ui->labelRemaining0->setText(remaining_para[0]);
	ui->labelRemaining1->setText(remaining_para[1]);
	ui->labelRemaining2->setText(remaining_para[2]);

	QButtonGroup* buttonGroup = new QButtonGroup(this);
	QRadioButton* radioBtn0 = ui->radioButton0;
	QRadioButton* radioBtn1 = ui->radioButton1;
	QRadioButton* radioBtn2 = ui->radioButton2;
	radioBtn2->setChecked(true);
	buttonGroup->addButton(radioBtn0);
	buttonGroup->addButton(radioBtn1);
	buttonGroup->addButton(radioBtn2);
	buttonGroup->setExclusive(true);

	QObject::connect(ui->pushButtonPurchasing, &QPushButton::clicked, this, &TicketPurchasingInterface::PurchasingTicket);
	QObject::connect(ui->pushButtonCancel, &QPushButton::clicked, [=] {
		this->close();
		});
}

TicketPurchasingInterface::~TicketPurchasingInterface()
{
	delete ui;
}

static void showSuccessBox(QWidget* parent)
{
	QMessageBox SuccessBox(parent);
	SuccessBox.setWindowTitle("Success");
	SuccessBox.setText("您的车票已经预定好了，可以到订单页面查看详情。");
	QPixmap iconPixmap(":/new/prefix1/image/success.png");	//调整图标大小
	iconPixmap = iconPixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	SuccessBox.setIconPixmap(iconPixmap);
	SuccessBox.exec();
}

void TicketPurchasingInterface::PurchasingTicket()
{
	Order new_order;
	int order_id = order_set->empty() ? 1 : order_set->back().GetOrderId() + 1;
	QDateTime current_time = QDateTime::currentDateTime();
	Date order_time(current_time.date().year(), current_time.date().month(), current_time.date().day(), current_time.time().hour(), current_time.time().minute());
	int train_id = booking_train->GetTrainId();
	new_order.SetOrderId(order_id);
	new_order.SetUserId(booking_user->GetUserId());
	new_order.SetTrainId(train_id);
	new_order.SetStartStationId(start_station->GetStationId());
	new_order.SetEndStationId(end_station->GetStationId());
	new_order.SetTravelDate(depart_date);
	new_order.SetOrderTime(order_time);
	new_order.SetStatus("booked");
	int seat_id = 0;
	if (ui->radioButton0->isChecked()) {
		seat_id = chooseSeat(train_id, "商务", depart_date);
		if (seat_id == 0) {
			QMessageBox::information(this, "info", "商务座已经卖完了。");
			return;
		}
	}
	else if (ui->radioButton1->isChecked()) {
		seat_id = chooseSeat(train_id, "一等", depart_date);
		if (seat_id == 0) {
			QMessageBox::information(this, "info", "一等座已经卖完了。");
			return;
		}
	}
	else if (ui->radioButton2->isChecked()) {
		seat_id = chooseSeat(train_id, "二等", depart_date);
		if (seat_id == 0) {
			QMessageBox::information(this, "info", "二等座已经卖完了。");
			return;
		}
	}
	new_order.SetSeatId(seat_id);
	order_set->push_back(new_order);
	showSuccessBox(this);
	emit Purchased();
	this->close();
}

int TicketPurchasingInterface::chooseSeat(int train_id, string seat_type, Date& travel_data)
{
	int seat_id = 0;
	for (deque<Seat>::iterator seat_p = seat_set->begin(); seat_p != seat_set->end(); seat_p++) {
		if (seat_p->GetTrainId() == train_id && seat_p->GetSeatType() == seat_type) {
			int order_ok = 1;
			for (deque<Order>::iterator order_p = order_set->begin(); order_p != order_set->end(); order_p++) {
				if (seat_p->GetSeatId() == order_p->GetSeatId() && (order_p->GetStatus() == "booked" || order_p->GetStatus() == "completed") && order_p->GetTravelDate() == travel_data) {
					int start_order_station_id = order_p->GetStartStationId();
					int end_order_station_id = order_p->GetEndStationId();
					int start_station_id = start_station->GetStationId();
					int end_station_id = end_station->GetStationId();
					int start_order_id = 0;
					int end_order_id = 0;
					int current_start_order_id = 0;
					int current_end_order_id = 0;
					for (deque<TrainSchedule>::iterator schedule_p = train_schedule_set->begin(); schedule_p != train_schedule_set->end(); schedule_p++) {
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
					if (!(current_end_order_id <= start_order_id || current_start_order_id >= end_order_id)) {
						order_ok = 0;
						break;
					}
					//order_ok = 0;
					//break;
				}
			}
			if (order_ok == 1) {
				seat_id = seat_p->GetSeatId();
				break;
			}
		}
	}
	return seat_id;
}

