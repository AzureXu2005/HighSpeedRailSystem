#pragma once
#include "iostream"
#include "string"
#include "Date.h"

class TrainSchedule {
	private:
		int schedule_id;
		int train_id;
		int station_id;
		int stop_order;
		Date arrive_time;
		Date depart_time;
	public:
		TrainSchedule();
		TrainSchedule(int schedule_id, int train_id, int station_id, int stop_order, Date arrive_time, Date depart_time);
		~TrainSchedule();
		
		int GetScheduleId(void);
		int GetTrainId(void);
		int GetStationId(void);
		int GetStopOrder(void);
		Date GetArriveTime(void);
		Date GetDepartTime(void);

		void SetScheduleId(int schedule_id);
		void SetTrainId(int train_id);
		void SetStationId(int station_id);
		void SetStopOrder(int stop_order);
		void SetArriveTime(Date arrive_time);
		void SetDepartTime(Date depart_time);

		bool operator==(const TrainSchedule& other) const;
		bool operator!=(const TrainSchedule& other) const;

		friend std::istream& operator>>(std::istream& in, TrainSchedule& schedule);
		friend std::ostream& operator<<(std::ostream& out, TrainSchedule& schedule);
};