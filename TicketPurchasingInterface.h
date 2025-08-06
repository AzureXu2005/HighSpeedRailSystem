#pragma once

#include <QObject>

class TicketPurchasingInterface  : public QObject
{
	Q_OBJECT

public:
	TicketPurchasingInterface(QObject *parent);
	~TicketPurchasingInterface();
};

