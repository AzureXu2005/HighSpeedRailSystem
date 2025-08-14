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
	LoginInterfaceW(TemplateSet<Users> *p_users, QDialog *parent = Q_NULLPTR);	//构造函数，传递User集合指针
	~LoginInterfaceW();	//析构函数，释放ui空间
	void InitLoginInterW();	//初始化登录窗口
	void LoadUsersInformation();	//从本地文件中加载用户信息
	void ClickAdminButton();	//切换管理员和用户按钮执行的操作
	bool LogOn(Users*& current_user);	//登录函数，用于检查用户的身份

public:
	TemplateSet<Users>* p_UsersSet;	//用户集合
	int change_tag;	//当前为“用户登录”还是“管理员登录”的变量
private:
	Ui::LoginDialog* ui;	//Qt ui登录界面
};

