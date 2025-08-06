#include "Station.h"
#include "fstream"
#include "sstream"

Station::Station()
{
	station_id = 0;
}

Station::Station(int station_id, string name)
{
	this->station_id = station_id;
	this->name = name;
}

Station::~Station()
{

}

int Station::GetStationId()
{
	return station_id;
}

string Station::GetName()
{
	return name;
}

void Station::SetStationId(int station_id)
{
	this->station_id = station_id;
}

void Station::SetName(string name)
{
	this->name = name;
}

bool Station::operator==(const Station& other) const
{
	if (this->station_id == other.station_id) return true;
	else return false;
}

bool Station::operator!=(const Station& other) const
{
	if (this->station_id != other.station_id) return true;
	else return false;
}



std::istream& operator>>(std::istream& in, Station& station)
{
	string line;
	if (getline(in, line)) {
		stringstream ss(line);
		string item;

		// ¶ÁÈ¡station_id
		getline(ss, item, ',');
		station.station_id = stoi(item);

		// ¶ÁÈ¡name
		getline(ss, station.name);
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, Station& station)
{
	out << station.station_id << "," << station.name;
	return out;
}