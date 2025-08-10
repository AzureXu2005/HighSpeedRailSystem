#include "LoginInterfaceW.h"
#include "qpushbutton.h"
#include "qmessagebox.h"


using namespace std;

LoginInterfaceW::LoginInterfaceW(TemplateSet<Users>* p_users, QDialog *parent)
	: QDialog(parent),ui(new Ui::LoginDialog)
{
	ui->setupUi(this);
	p_UsersSet = p_users;
	change_tag = 0;
	InitLoginInterW();
	LoadUsersInformation();
	QObject::connect(ui->pushButtonAdmin, &QPushButton::clicked, [=] {
		ui->lineEditName->clear();
		ui->lineEditSecret->clear();
		this->ClickAdminButton();
		});
	QObject::connect(ui->pushButtonSubmit, &QPushButton::clicked, [=] {
		Users* current_user = NULL;
		if (this->LogOn(current_user)) {
			if (change_tag % 2 == 0) {
				HighSpeedRailSystem* window = new HighSpeedRailSystem(current_user, p_UsersSet);
				window->setAttribute(Qt::WA_DeleteOnClose);
				window->show();
				this->close();
			}
			else {
				AdministratorMainWindow* admin_window = new AdministratorMainWindow(current_user, p_UsersSet);
				admin_window->setAttribute(Qt::WA_DeleteOnClose);
				admin_window->show();
				this->close();
			}
		}
		});
	QObject::connect(ui->pushButtonRegister, &QPushButton::clicked, [=] {
		RegisterInterface* register_window = new RegisterInterface(p_UsersSet);
		register_window->exec();
		});
}

LoginInterfaceW::~LoginInterfaceW()
{
	delete ui;
}

void LoginInterfaceW::InitLoginInterW()
{
	ui->lineEditName->clear();
	ui->lineEditSecret->clear();
	ui->lineEditName->setPlaceholderText("用户名");
	ui->lineEditSecret->setPlaceholderText("登录密码");

}

void LoginInterfaceW::LoadUsersInformation()
{
	p_UsersSet->ReadFromFile("users.txt");
}

void LoginInterfaceW::ClickAdminButton()
{
	if (change_tag % 2 == 0) {
		ui->label->setText("管理员账号密码登录");
		ui->pushButtonAdmin->setText("用户登录");
		change_tag = 1;
	}
	else {
		ui->label->setText("用户账号密码登录");
		ui->pushButtonAdmin->setText("管理员登录");
		change_tag = 0;
	}
}

bool LoginInterfaceW::LogOn(Users*& current_user)
{
	string name = ui->lineEditName->text().toUtf8().constData();
	string secret = ui->lineEditSecret->text().toUtf8().constData();
	if (change_tag % 2 == 0) {
		auto user_p = p_UsersSet->begin();
		user_p++;
		while (user_p != p_UsersSet->end()) {
			if (name == user_p->GetDisplayName() && secret == user_p->GetPassword()) {
				current_user = &(*user_p);
				return true;
			}
			user_p++;
		}
		QMessageBox::information(this, "提示", "账号不存在或者密码错误！");
		return false;
	}
	else {
		auto admin = p_UsersSet->begin();
		if (name == admin->GetDisplayName() && secret == admin->GetPassword()) {
			current_user = &(*admin);
			return true;
		}
		else {
			QMessageBox::information(this, "提示", "账号不存在或者密码错误！");
			return false;
		}
	}
}
