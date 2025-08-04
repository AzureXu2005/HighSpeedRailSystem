#include "RegisterInterfaceW.h"

RegisterInterfaceW::RegisterInterfaceW(QDialog *parent)
	: QDialog(parent), register_ui(new Ui::Dialog)
{
	register_ui->setupUi(this);
}

RegisterInterfaceW::~RegisterInterfaceW()
{}

