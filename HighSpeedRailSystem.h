#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HighSpeedRailSystem.h"

class HighSpeedRailSystem : public QMainWindow
{
    Q_OBJECT

public:
    HighSpeedRailSystem(QWidget *parent = nullptr);
    ~HighSpeedRailSystem();

private:
    Ui::HighSpeedRailSystemClass ui;
};

