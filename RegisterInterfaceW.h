#pragma once

#include <QObject>
#include "ui_RegisterInterface.h"

class RegisterInterfaceW  : public QDialog
{
	Q_OBJECT

public:
	RegisterInterfaceW(QDialog *parent);
	~RegisterInterfaceW();

private:
	Ui::Dialog* register_ui;
};

