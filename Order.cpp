#include "Order.h"
#include "fstream"
#include "sstream"

Order::Order(int o, int u, int t, int seat, int start, int end, Date td, Date ot, string status)
{
	order_id = o;
	user_id = u;
	train_id = t;
	seat_id = seat;
	start_station_id = start;
	end_station_id = end;
	travel_date = td;
	order_time = ot;
	this->status = status;
}

Order::~Order()
{

}

int Order::GetOrderId(void)
{
	return order_id;
}

int Order::GetUserId(void)
{
	return user_id;
}

int Order::GetTrainId(void)
{
	return train_id;
}

int Order::GetSeatId(void)
{
	return seat_id;
}

int Order::GetStartStationId(void)
{
	return start_station_id;
}

int Order::GetEndStationId(void)
{
	return end_station_id;
}

Date Order::GetTravelDate(void)
{
	return travel_date;
}

Date Order::GetOrderTime(void)
{
	return order_time;
}

string Order::GetStatus(void)
{
	return status;
}

void Order::SetOrderId(int o)
{
	order_id = o;
}

void Order::SetUserId(int u)
{
	user_id = u;
}

void Order::SetTrainId(int t)
{
	train_id = t;
}

void Order::SetSeatId(int seat)
{
	seat_id = seat;
}

void Order::SetStartStationId(int start)
{
	start_station_id = start;
}

void Order::SetEndStationId(int end)
{
	end_station_id = end;
}

void Order::SetTravelDate(Date td)
{
	travel_date = td;
}

void Order::SetOrderTime(Date ot)
{
	order_time = ot;
}

void Order::SetStatus(string status)
{
	this->status = status;
}

bool operator==(Order& o1, Order& o2)
{
	if (o1.order_id == o2.order_id) return true;
	else return false;
}

bool operator!=(Order& o1, Order& o2)
{
	if (o1.order_id != o2.order_id) return true;
	else return false;
}

std::istream& operator>>(std::istream& in, Order& order)
{
	string line;
	if (getline(in, line)) {
		stringstream ss(line);

		string order_id_s;
		string user_s;
		string train_s;
		string seat_s;
		string start_s;
		string end_s;
		string travel_date_s;
		string order_time_s;
		Date date1;
		Date date2;
		
		getline(ss, order_id_s, ',');
		order.order_id = stoi(order_id_s);
		getline(ss, user_s, ',');
		order.user_id = stoi(user_s);
		getline(ss, train_s, ',');
		order.train_id = stoi(train_s);
		getline(ss, seat_s, ',');
		order.seat_id = stoi(seat_s);
		getline(ss, start_s, ',');
		order.start_station_id = stoi(start_s);
		getline(ss, end_s, ',');
		order.end_station_id = stoi(end_s);
		getline(ss, travel_date_s, ',');
		order.travel_date = date1(travel_date_s);
		getline(ss, order_time_s, ',');
		order.order_time = date2(order_time_s);
		getline(ss, order.status);

	}
	return in;
}

std::ostream& operator<<(std::ostream& out, Order& order)
{
	out << order.order_id << "," << order.user_id << "," << order.train_id << "," << order.seat_id << ","
		<< order.start_station_id << "," << order.end_station_id << "," << order.travel_date.Format() << ","
		<< order.order_time.Format2() << "," << order.status;
	return out;
}