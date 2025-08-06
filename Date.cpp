#include "Date.h"
#include "string"

Date::Date(void)
{
	year = 0;
	month = 0;
	day = 0;
	hour = 0;
	minute = 0;
}

Date::Date(int y, int m, int d, int h, int mi)
{
	year = y;
	month = m;
	day = d;
	hour = h;
	minute = mi;
}

Date::~Date(void)
{

}

void Date::SetYear(int y)
{
	year = y;
}

void Date::SetMonth(int m)
{
	month = m;
}

void Date::SetDay(int d)
{
	day = d;
}

void Date::SetHour(int h)
{
	hour = h;
}


void Date::SetMinute(int mi)
{
	minute = mi;
}

int Date::GetYear()
{
	return year;
}

int Date::GetMonth()
{
	return month;
}

int Date::GetDay()
{
	return day;
}

int Date::GetHour()
{
	return hour;
}

int Date::GetMinute()
{
	return minute;
}

bool Date::isExist(void)
{
	if (year == 0 && month == 0 && day == 0 && hour == 0 && minute == 0) return false;
	else return true;
}

Date& Date::operator()(int y, int m, int d, int h, int mi)
{
	year = y;
	month = m;
	day = y;
	hour = h;
	minute = mi;
	return *this;
}

Date& Date::operator()(string ymdhm_s)
{
	const char* ymdhm = ymdhm_s.c_str();
	int y, m, d, h, mi;
	if (sscanf_s(ymdhm, "%d-%d-%d %d:%d", &y, &m, &d, &h, &mi) >= 3) {
		year = y;
		month = m;
		day = y;
		hour = h;
		minute = mi;
		return *this;
	}
	// 尝试匹配仅日期的格式
	else if (sscanf_s(ymdhm, "%d-%d-%d", &year, &month, &day) == 3) {
		year = y;
		month = m;
		day = y;
		hour = 0;
		minute = 0;
		return *this;
	}
	else {
		year = 0;
		month = 0;
		day = 0;
		hour = 0;
		minute = 0;
		return *this;
	}
}

string Date::Format()
{
	string s_year = to_string(year);
	string s_month = to_string(month);
	if (month < 10) {
		s_month.insert(s_month.begin(), '0');
	}
	string s_day = to_string(day);
	if (day < 10) {
		s_day.insert(s_day.begin(), '0');
	}
	string res;
	res = s_year + "-" + s_month + "-" + s_day;
	return res;
}

string Date::Format2()
{
	string s_year = to_string(year);
	string s_month = to_string(month);
	if (month < 10) {
		s_month.insert(s_month.begin(), '0');
	}
	string s_day = to_string(day);
	if (day < 10) {
		s_day.insert(s_day.begin(), '0');
	}
	string s_hour = to_string(hour);
	if (hour < 10) {
		s_hour.insert(s_hour.begin(), '0');
	}
	string s_minute = to_string(minute);
	if (minute < 10) {
		s_minute.insert(s_minute.begin(), '0');
	}
	string res;
	res = s_year + "-" + s_month + "-" + s_day + " " + s_hour + ":" + s_minute;
	return res;
}

string Date::Format3()
{
	string s_hour = to_string(hour);
	if (hour < 10) {
		s_hour.insert(s_hour.begin(), '0');
	}
	string s_minute = to_string(minute);
	if (minute < 10) {
		s_minute.insert(s_minute.begin(), '0');
	}
	string res;
	res = s_hour + ":" + s_minute;
	return res;
}

bool Date::operator==(const Date& other) const
{
	if (this->year == other.year && this->month == other.month && this->day == other.day
		&& this->hour == other.hour && this->minute == other.minute) return true;
	else return false;
}

bool Date::operator!=(const Date& other) const
{
	if (this->year == other.year && this->month == other.month && this->day == other.day
		&& this->hour == other.hour && this->minute == other.minute) return false;
	else return true;
}

std::istream& operator>>(std::istream& in, Date& date)
{
	in >> date.year >> date.month >> date.day >> date.hour >> date.minute;
	return in;
}

std::ostream& operator<<(std::ostream& out, Date& date)
{
	//out << "Year:" << date.year << " Month:" << date.month << " Day:" << date.day;
	out << date.year << " " << date.month << " " << date.day << " " << date.hour << " " << date.minute;
	return out;
}