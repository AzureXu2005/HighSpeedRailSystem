#include "TrainSchedule.h"
#include "fstream"
#include "sstream"

TrainSchedule::TrainSchedule()
{
	schedule_id = 0;
}

TrainSchedule::TrainSchedule(int schedule_id, int train_id, int station_id, int stop_order, Date arrive_time, Date depart_time)
{
	this->schedule_id = schedule_id;
	this->train_id = train_id;
	this->station_id = station_id;
	this->stop_order = stop_order;
	this->arrive_time = arrive_time;
	this->depart_time = depart_time;
}

TrainSchedule::~TrainSchedule()
{

}

int TrainSchedule::GetScheduleId(void)
{
	return schedule_id;
}

int TrainSchedule::GetTrainId(void)
{
	return train_id;
}

int TrainSchedule::GetStationId(void)
{
	return station_id;
}

int TrainSchedule::GetStopOrder(void)
{
	return stop_order;
}

Date TrainSchedule::GetArriveTime(void)
{
	return arrive_time;
}

Date TrainSchedule::GetDepartTime(void)
{
	return depart_time;
}

void TrainSchedule::SetScheduleId(int schedule_id)
{
	this->schedule_id = schedule_id;
}

void TrainSchedule::SetTrainId(int train_id)
{
	this->train_id = train_id;
}

void TrainSchedule::SetStationId(int station_id)
{
	this->station_id = station_id;
}

void TrainSchedule::SetStopOrder(int stop_order)
{
	this->stop_order = stop_order;
}

void TrainSchedule::SetArriveTime(Date arrive_time)
{
	this->arrive_time = arrive_time;
}

void TrainSchedule::SetDepartTime(Date depart_time)
{
	this->depart_time = depart_time;
}

bool operator==(TrainSchedule& t1, TrainSchedule& t2)
{
	if (t1.schedule_id == t2.schedule_id) return true;
	else return false;
}

bool operator!=(TrainSchedule& t1, TrainSchedule& t2)
{
	if (t1.schedule_id != t2.schedule_id) return true;
	else return false;
}

std::istream& operator>>(std::istream& in, TrainSchedule& schedule)
{
	string line;
	if (getline(in, line)) {
		stringstream ss(line);

		string schedule_s;
		string train_s;
		string station_s;
		string stop_s;
		string arrive_s = "";
		string depart_s = "";
		string hour_str;
		string minute_str;

		getline(ss, schedule_s, ',');
		schedule.schedule_id = stoi(schedule_s);

		getline(ss, train_s, ',');
		schedule.train_id = stoi(train_s);

		getline(ss, station_s, ',');
		schedule.station_id = stoi(station_s);

		getline(ss, stop_s, ',');
		schedule.stop_order = stoi(stop_s);

		getline(ss, arrive_s, ',');
		if (arrive_s != "") {
			size_t colon_pos = arrive_s.find(':');	// 找到冒号的位置
			hour_str = arrive_s.substr(0, colon_pos);
			minute_str = arrive_s.substr(colon_pos + 1);
			schedule.arrive_time = Date(0, 0, 0, stoi(hour_str), stoi(minute_str));
		}
		else {
			schedule.arrive_time = Date(0, 0, 0, 0, 0);
		}

		getline(ss, depart_s);
		if (depart_s != "") {
			size_t colon_pos = depart_s.find(':');	// 找到冒号的位置
			hour_str = depart_s.substr(0, colon_pos);
			minute_str = depart_s.substr(colon_pos + 1);
			schedule.depart_time = Date(0, 0, 0, stoi(hour_str), stoi(minute_str));
		}
		else {
			schedule.depart_time = Date(0, 0, 0, 0, 0);
		}
	}

	return in;
}

std::ostream& operator<<(std::ostream& out, TrainSchedule& schedule)
{
	out << schedule.schedule_id << "," << schedule.train_id << "," << schedule.station_id << ","
		<< schedule.stop_order << "," << schedule.arrive_time.Format3() << "," << schedule.depart_time.Format3();
	return out;
}
