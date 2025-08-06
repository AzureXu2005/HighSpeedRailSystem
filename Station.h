#pragma once
#include "iostream"

using namespace std;

class Station {
	private:
		int station_id;
		string name;
	public:
		Station();
		Station(int station_id, string name);
		~Station();
		int GetStationId();
		string GetName();

		void SetStationId(int station_id);
		void SetName(string name);

		bool operator==(const Station& other) const;
		bool operator!=(const Station& other) const;

		friend std::istream& operator>>(std::istream& in, Station& station);
		friend std::ostream& operator<<(std::ostream& out, Station& station);
};