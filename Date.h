#pragma once
#include "iostream"
using namespace std;
class Date
{
public:
	//Constructor
	Date(void);
	Date(int y, int m, int d, int h, int mi);

public:
	//Destructor
	virtual~Date(void);
	//Setters
	void SetYear(int y);
	void SetMonth(int m);
	void SetDay(int d);
	void SetHour(int h);
	void SetMinute(int mi);
	//Getters
	int GetYear();
	int GetMonth();
	int GetDay();
	int GetHour();
	int GetMinute();

	bool isExist(void);

	//Overloaded operator() for setting properties of Date object
	Date& operator()(int y, int m, int d, int h, int mi);
	Date& operator()(string ymdhm_s);

	//Converts date attributes to string in"yy-mm-dd" format.
	string Format();

	string Format2();

	string Format3();

	//Overloaded comparison operators
	bool operator==(const Date& other) const;
	bool operator!=(const Date& other) const;

	//Overloaded standard input operator>> and operator<< for I/O operations of Date object
	friend std::istream& operator>>(std::istream& in, Date& date);
	friend std::ostream& operator<<(std::ostream& out, Date& date);

private:

	int year; //1900-9999
	int month; //1-12
	int day; //1-31
	int hour; //0-23
	int minute; //0-59
};