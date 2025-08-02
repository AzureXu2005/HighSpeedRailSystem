#include "Train.h"
#include "fstream"
#include "sstream"

Train::Train(int id, string code, char t, int start, int end)
{
	train_id = id;
	train_code = code;
	type = t;
	start_station_id = start;
	end_station_id = end;
}

Train::~Train()
{

}

int Train::GetTrainId()
{
	return train_id;
}

string Train::GetTrainCode()
{
	return train_code;
}

char Train::GetType()
{
	return type;
}

int Train::GetStartStation()
{
	return start_station_id;
}

int Train::GetEndStation()
{
	return end_station_id;
}

void Train::SetTrainId(int id)
{
	train_id = id;
}

void Train::SetTrainCode(string code)
{
	train_code = code;
}

void Train::SetType(char type)
{
	this->type = type;
}

void Train::SetStartStation(int start)
{
	start_station_id = start;
}

void Train::SetEndStation(int end)
{
	end_station_id = end;
}

bool operator==(Train& t1, Train& t2)
{
	if (t1.train_code == t2.train_code) return true;
	else return false;
}

bool operator!=(Train& t1, Train& t2)
{
	if (t1.train_code != t2.train_code) return true;
	else return false;
}

std::istream& operator>>(std::istream& in, Train& train)
{
	string line;
	if (getline(in, line)) {
		stringstream ss(line);
		string id;
		string type;
		string start;
		string end;

		getline(ss, id, ',');
		train.train_id = stoi(id);

		getline(ss, train.train_code, ',');
		getline(ss, type, ',');
		train.type = type[0];
		getline(ss, start, ',');
		train.start_station_id = stoi(start);
		getline(ss, end, ',');
		train.start_station_id = stoi(end);
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, Train& train)
{
	out << train.train_id << "," << train.train_code << "," << train.type << "," << train.start_station_id << "," << train.end_station_id;
	return out;
}