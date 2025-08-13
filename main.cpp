#include "HighSpeedRailSystem.h"
#include "AdministratorMainWindow.h"
#include "LoginInterfaceW.h"
#include "ModifyAndAddInterface.h"
#include <QtWidgets/QApplication>
#include "Users.h"
#include "TemplateSet.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //HighSpeedRailSystem window;
    //window.show();
    //AdministratorMainWindow admin_window;
    //admin_window.show();
    //ModifyAndAddInterface* dlg = new ModifyAndAddInterface;
    //dlg->exec();


    TemplateSet<Users> users_set;

    LoginInterfaceW* login = new LoginInterfaceW(&users_set);
    QObject::connect(login, &LoginInterfaceW::destroyed, &app, &QApplication::quit);
    login->show();

    return app.exec();
}
