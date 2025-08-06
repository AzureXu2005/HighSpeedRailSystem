#include "RegisterInterface.h"
#include "qmessagebox.h"
#include "qdatetime.h"

RegisterInterface::RegisterInterface(TemplateSet<Users>* p_users, QDialog *parent)
	: QDialog(parent),ui(new Ui::RegisterInterfaceDlg)
{
	ui->setupUi(this);
	p_UsersSet = p_users;
	QObject::connect(ui->pushButtonOK, &QPushButton::clicked, this, &RegisterInterface::Register);
	QObject::connect(ui->pushButtonCancel, &QPushButton::clicked, this, [=] {
		this->close();
		});
}

RegisterInterface::~RegisterInterface()
{
	delete ui;
}

void RegisterInterface::LoadUsersInformation()
{
	p_UsersSet->ReadFromFile("users.txt");
}

void RegisterInterface::WriteUsersInformation()
{
	p_UsersSet->WriteToFile("users.txt");
}

static bool dateRight(int y, int m, int d)
{
	int month_max[13] = { 0,31,29,31,30,31,30,31,31,30,31,30,31 };
	if (m < 1 || m > 12) return false;
	if (d<1 || d > month_max[m]) return false;
	if (y < 1901) return false;
	if (y % 4 == 0 && y % 100 != 0 || y % 400 == 0) {
		if (m == 2 && d > 29) return false;
	}
	else {
		if (m == 2 && d > 28) return false;
	}
	return true;
}


void RegisterInterface::Register()
{
	string display_name = ui->lineEdit_displayName->text().toUtf8().constData();
	string username = ui->lineEdit_username->text().toUtf8().constData();
	string id_card = ui->lineEdit_idCard->text().toUtf8().constData();
	string phone = ui->lineEdit_phone->text().toUtf8().constData();
	string password = ui->lineEdit_password->text().toUtf8().constData();
	string passwordQ = ui->lineEdit_passwordQ->text().toUtf8().constData();
	if (display_name == "" || username == "" || id_card == "" || phone == "" || password == "" || passwordQ == "") {
		QMessageBox::warning(this, "警告", "所填字段不能有空。");
		return;
	}
	if (phone.size() != 11 || !std::all_of(phone.begin(), phone.end(), ::isdigit)) {
		QMessageBox::information(this, "info", "电话必须是11位数字。");
		return;
	}
	if (password != passwordQ) {
		QMessageBox::information(this, "info", "两次密码输入不一致！");
		return;
	}
	if (id_card.size() == 18) {
		int year = std::stoi(id_card.substr(6, 4));  // "1990" -> 1990
		int month = std::stoi(id_card.substr(10, 2)); // "01" -> 1
		int day = std::stoi(id_card.substr(12, 2));    // "01" -> 1
		if (!dateRight(year, month, day)) {
			QMessageBox::information(this, "info", "身份号填写不正确！");
			return;
		}
		else {
			Users u;
			int user_id = 0;
			Users u_end = p_UsersSet->back();
			user_id = u_end.GetUserId() + 1;
			u.SetUserId(user_id);
			u.SetDisplayName(display_name);
			u.SetUsername(username);
			u.SetIdCard(id_card);
			u.SetPhone(phone);
			u.SetPassword(password);
			u.SetRole("customer");
			if (p_UsersSet->push_back(u)) {
				QMessageBox SuccessBox(this);
				SuccessBox.setWindowTitle("Success");
				SuccessBox.setText("注册成功！");
				QPixmap iconPixmap(":/new/prefix1/image/success.png");	//调整图标大小
				iconPixmap = iconPixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				SuccessBox.setIconPixmap(iconPixmap);
				SuccessBox.exec();
				this->WriteUsersInformation();
				this->close();
				
			}
			else {
				QMessageBox::information(this, "info", "用户已存在。");
				return;
			}
			
		}
	}
	else {
		QMessageBox::information(this, "info", "身份号位数不正确！");
		return;
	}

}



