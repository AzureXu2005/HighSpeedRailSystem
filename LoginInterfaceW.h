#pragma once
#include <QObject>
#include "ui_LoginInterface.h"
#include "Users.h"
#include "TemplateSet.h"
#include "qstring.h"
#include "HighSpeedRailSystem.h"
#include "RegisterInterface.h"
#include "AdministratorMainWindow.h"


class LoginInterfaceW  : public QDialog
{
	Q_OBJECT

public:
	LoginInterfaceW(TemplateSet<Users> *p_users, QDialog *parent = Q_NULLPTR);
	~LoginInterfaceW();
	void InitLoginInterW();
	void LoadUsersInformation();
	void ClickAdminButton();
	bool LogOn(Users*& current_user);

public:
	TemplateSet<Users>* p_UsersSet;
	int change_tag;
private:
	Ui::LoginDialog* ui;
};

