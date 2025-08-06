#include "Seat.h"
#include "fstream"
#include "sstream"

Seat::Seat(void)
{
	seat_id = 0;
	train_id = 0;
}

Seat::Seat(int seat_id, int train_id, string seat_no, string seat_type)
{
	this->seat_id = seat_id;
	this->train_id = train_id;
	this->seat_no = seat_no;
	this->seat_type = seat_type;
}

Seat::~Seat() 
{
	
}

int Seat::GetSeatId(void)
{
	return seat_id;
}

int Seat::GetTrainId(void)
{
	return train_id;
}

string Seat::GetSeatNo(void)
{
	return seat_no;
}

string Seat::GetSeatType(void)
{
	return seat_type;
}

void Seat::SetSeatId(int seat_id)
{
	this->seat_id = seat_id;
}

void Seat::SetTrainId(int train_id)
{
	this->train_id = train_id;
}

void Seat::SetSeatNo(string seat_no)
{
	this->seat_no = seat_no;
}

void Seat::SetSeatType(string seat_type)
{
	this->seat_type = seat_type;
}

bool Seat::operator==(const Seat& other) const
{
	if (this->seat_id == other.seat_id) return true;
	else return false;
}

bool Seat::operator!=(const Seat& other) const
{
	if (this->seat_id != other.seat_id) return true;
	else return false;
}

std::istream& operator>>(std::istream& in, Seat& seat)
{
	string line;
	if (getline(in, line)) {
		stringstream ss(line);
		string seat_s;
		string train_s;

		getline(ss, seat_s, ',');
		seat.seat_id = stoi(seat_s);

		getline(ss, train_s, ',');
		seat.train_id = stoi(train_s);

		getline(ss, seat.seat_no, ',');

		getline(ss, seat.seat_type);
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, Seat& seat)
{
	out << seat.seat_id << "," << seat.train_id << "," << seat.seat_no << "," << seat.seat_type;
	return out;
}