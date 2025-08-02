#pragma once
#include "iostream"
#include "string"

using namespace std;

class Seat {
	private:
		int seat_id;
		int train_id;
		string seat_no;
		string seat_type;

	public:
		Seat(int seat_id, int train_id, string seat_no, string seat_type);
		~Seat();

		int GetSeatId(void);
		int GetTrainId(void);
		string GetSeatNo(void);
		string GetSeatType(void);

		void SetSeatId(int seat_id);
		void SetTrainId(int train_id);
		void SetSeatNo(string seat_no);
		void SetSeatType(string seat_type);

		friend bool operator==(Seat& s1, Seat& s2);
		friend bool operator!=(Seat& s1, Seat& s2);

		friend std::istream& operator>>(std::istream& in, Seat& seat);
		friend std::ostream& operator<<(std::ostream& out, Seat& seat);
};