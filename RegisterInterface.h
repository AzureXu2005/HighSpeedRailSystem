#pragma once

#include <QObject>
#include "ui_RegisterInterface.h"
#include "Users.h"
#include "TemplateSet.h"

class RegisterInterface  : public QDialog	
{
	Q_OBJECT

public:
	RegisterInterface(TemplateSet<Users>* p_users, QDialog *parent = Q_NULLPTR);	//传递用户集合指针，初始化界面
	~RegisterInterface();	//析构函数，保存新用户信息，释放ui
	void LoadUsersInformation();	//从本地文件中加载所有用户信息
	void WriteUsersInformation();	//新用户信息写入本地文件
	void Register();	//注册功能，检查注册逻辑并新增用户信息

private:
	Ui::RegisterInterfaceDlg *ui;	//注册界面ui
	TemplateSet<Users>* p_UsersSet;	//用户集合指针
};

