#pragma once
#include "iostream"
#include "string"

using namespace std;

class Users {
	private:
		int user_id;
		string username;
		string display_name;
		string password;
		string phone;
		string role;
		string id_card;

	public:
		Users(int user_id, string username, string display_name, string password, string phone, string role, string id_card);
		~Users();

		int GetUserId();
		string GetUsername();
		string GetDisplayName();
		string GetPassword();
		string GetPhone();
		string GetRole();
		string GetIdCard();

		void SetUserId(int user_id);
		void SetUsername(string username);
		void SetDisplayName(string display_name);
		void SetPassword(string password);
		void SetPhone(string phone);
		void SetRole(string role);
		void SetIdCard(string id_card);

		friend bool operator==(Users& u1, Users& u2);
		friend bool operator!=(Users& u1, Users& u2);

		friend std::istream& operator>>(std::istream& in, Users& user);
		friend std::ostream& operator<<(std::ostream& out, Users& user);
};