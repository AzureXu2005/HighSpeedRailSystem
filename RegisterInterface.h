#pragma once

#include <QObject>
#include "ui_RegisterInterface.h"
#include "Users.h"
#include "TemplateSet.h"

class RegisterInterface  : public QDialog
{
	Q_OBJECT

public:
	RegisterInterface(TemplateSet<Users>* p_users, QDialog *parent = Q_NULLPTR);
	~RegisterInterface();
	void LoadUsersInformation();
	void WriteUsersInformation();
	void Register();

private:
	Ui::RegisterInterfaceDlg *ui;
	TemplateSet<Users>* p_UsersSet;
};

