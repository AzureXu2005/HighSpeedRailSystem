#pragma once
#include "iostream"
#include "string"
#include "Date.h"

using namespace std;

class Order {
	private:
		int order_id;
		int user_id;
		int train_id;
		int seat_id;
		int start_station_id;
		int end_station_id;
		Date travel_date;
		Date order_time;
		string status;
	public:
		Order(void);
		Order(int o, int u, int t, int seat, int start, int end, Date td, Date ot, string status);
		~Order();

		int GetOrderId(void);
		int GetUserId(void);
		int GetTrainId(void);
		int GetSeatId(void);
		int GetStartStationId(void);
		int GetEndStationId(void);
		Date GetTravelDate(void);
		Date GetOrderTime(void);
		string GetStatus(void);

		void SetOrderId(int o);
		void SetUserId(int u);
		void SetTrainId(int t);
		void SetSeatId(int seat);
		void SetStartStationId(int start);
		void SetEndStationId(int end);
		void SetTravelDate(Date td);
		void SetOrderTime(Date ot);
		void SetStatus(string status);
		
		friend bool operator==(Order& o1, Order& o2);
		friend bool operator!=(Order& o1, Order& o2);

		friend std::istream& operator>>(std::istream& in, Order& order);
		friend std::ostream& operator<<(std::ostream& out, Order& order);
};
