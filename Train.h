#pragma once
#include "iostream"

using namespace std;

class Train {
	private:
		int train_id;
		string train_code;
		char type;
		int start_station_id;
		int end_station_id;

	public:
		Train();
		Train(int id, string code, char t, int start, int end);
		~Train();

		int GetTrainId();
		string GetTrainCode();
		char GetType();
		int GetStartStation();
		int GetEndStation();

		void SetTrainId(int id);
		void SetTrainCode(string code);
		void SetType(char type);
		void SetStartStation(int start);
		void SetEndStation(int end);

		bool operator==(const Train& other) const;
		bool operator!=(const Train& other) const;

		friend std::istream& operator>>(std::istream& in, Train& train);
		friend std::ostream& operator<<(std::ostream& out, Train& train);

};