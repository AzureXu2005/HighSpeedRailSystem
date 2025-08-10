#include "HighSpeedRailSystem.h"
#include "AdministratorMainWindow.h"
#include "LoginInterfaceW.h"
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

    TemplateSet<Users> users_set;

    LoginInterfaceW* sub = new LoginInterfaceW(&users_set);
    sub->exec();
    return app.exec();
}
