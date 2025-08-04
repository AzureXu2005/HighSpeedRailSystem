#include "Users.h"
#include "fstream"
#include "sstream"

Users::Users()
{
	user_id = 0;
}

Users::Users(int user_id, string username, string display_name, string password, string phone, string role, string id_card)
{
	this->user_id = user_id;
	this->username = username;
	this->display_name = display_name;
	this->password = password;
	this->phone = phone;
	this->role = role;
	this->id_card = id_card;
}

Users::~Users()
{

}

int Users::GetUserId()
{
	return user_id;
}

string Users::GetUsername()
{
	return username;
}

string Users::GetDisplayName()
{
	return display_name;
}

string Users::GetPassword()
{
	return password;
}

string Users::GetPhone()
{
	return phone;
}

string Users::GetRole()
{
	return role;
}

string Users::GetIdCard()
{
	return id_card;
}

void Users::SetUserId(int user_id)
{
	this->user_id = user_id;
}

void Users::SetUsername(string username)
{
	this->username = username;
}

void Users::SetDisplayName(string display_name)
{
	this->display_name = display_name;
}

void Users::SetPassword(string password)
{
	this->password = password;
}

void Users::SetPhone(string phone)
{
	this->phone = phone;
}

void Users::SetRole(string role)
{
	this->role = role;
}

void Users::SetIdCard(string id_card)
{
	this->id_card = id_card;
}

bool Users::operator==(const Users& other) const 
{
	if (this->id_card == other.id_card) return true;
	else return false;
}

bool Users::operator!=(const Users& other) const
{
	if (this->id_card != other.id_card) return true;
	else return false;
}

std::istream& operator>>(std::istream& in, Users& user)
{
	string line;
	if (getline(in, line)) {
		stringstream ss(line);
		string item;

		// 读取user_id
		getline(ss, item, ',');
		user.user_id = stoi(item);

		// 读取username
		getline(ss, user.username, ',');

		// 读取display_name
		getline(ss, user.display_name, ',');

		// 读取password
		getline(ss, user.password, ',');

		// 读取phone
		getline(ss, user.phone, ',');

		// 读取role
		getline(ss, user.role, ',');

		// 读取id_card
		getline(ss, user.id_card);
	}
	return in;
}

std::ostream& operator<<(ostream& out, Users& user) {
	out << user.user_id << "," << user.username << "," << user.display_name << "," << user.password << ","
		<< user.phone << "," << user.role << "," << user.id_card;
	return out;
}