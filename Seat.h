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
		Seat(void);
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

		bool operator==(const Seat& other) const;
		bool operator!=(const Seat& other) const;

		friend std::istream& operator>>(std::istream& in, Seat& seat);
		friend std::ostream& operator<<(std::ostream& out, Seat& seat);
};