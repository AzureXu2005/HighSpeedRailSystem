#include "AdministratorMainWindow.h"
#include "qscreen.h"


AdministratorMainWindow::AdministratorMainWindow(Users* user, TemplateSet<Users>* user_set, QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    //屏幕居中
    QScreen* screen = QGuiApplication::primaryScreen();  // 获取主屏幕
    QRect screenGeometry = screen->geometry();           // 屏幕几何信息
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    // 移动窗口到屏幕中心
    this->move(x, y);
    mp_user = user;
    mp_user_set = user_set;
    LoadFromTxtFile();
    ui.stackedWidgetAdmin->setCurrentIndex(0);
    InitTableSetting();
    QObject::connect(ui.pushButtonHSRailAdmin, &QPushButton::clicked, [=] {
        ui.stackedWidgetAdmin->setCurrentIndex(0);
        });
    QObject::connect(ui.pushButtonModify, &QPushButton::clicked, [=] {
        ui.stackedWidgetAdmin->setCurrentIndex(1);
        });
    QObject::connect(ui.pushButtonUserAdmin, &QPushButton::clicked, [=] {
        ui.stackedWidgetAdmin->setCurrentIndex(2);
        });
    QObject::connect(ui.pushButtonReBooking, &QPushButton::clicked, [=] {
        ui.stackedWidgetAdmin->setCurrentIndex(3);
        });
    onClickedTrainOperation();
    onClickedStationOperation();
}

AdministratorMainWindow::~AdministratorMainWindow()
{

}

void AdministratorMainWindow::LoadFromTxtFile()
{
    train_set.ReadFromFile("trains.txt");
    train_schedule_set.ReadFromFile("train_schedule.txt");
    order_set.ReadFromFile("orders.txt");
    station_set.ReadFromFile("stations.txt");
    seat_set.ReadFromFile("seats.txt");
}

void AdministratorMainWindow::InitTableSetting()
{
    TrainTableSetting();
    StationTableSetting();
}

void AdministratorMainWindow::TrainTableSetting()
{
    QTableWidget* train_table = ui.tableWidgetTrain;
    int train_count = train_set.size();
    train_table->setCurrentCell(-1, -1);
    train_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    RefreshTrainTable();
    
    train_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    train_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

void AdministratorMainWindow::RefreshTrainTable()
{
    QTableWidget* train_table = ui.tableWidgetTrain;
    int train_count = train_set.size();
    train_table->setRowCount(train_count);
    int col_count = 5;
    for (int row = 0; row < train_count; row++) {
        QString train_id = QString::number(train_set[row].GetTrainId());
        QString train_code = train_set[row].GetTrainCode().c_str();
        QString type = QChar(train_set[row].GetType());
        QString start_station;
        QString end_station;
        int start_station_id = train_set[row].GetStartStation();
        int end_station_id = train_set[row].GetEndStation();
        for (deque<Station>::iterator station_p = station_set.begin(); station_p != station_set.end(); station_p++) {
            if (start_station_id == station_p->GetStationId()) {
                start_station = station_p->GetName().c_str();
            }
            if (end_station_id == station_p->GetStationId()) {
                end_station = station_p->GetName().c_str();
            }
        }

        train_table->setItem(row, 0, new QTableWidgetItem(train_id));
        train_table->setItem(row, 1, new QTableWidgetItem(train_code));
        train_table->setItem(row, 2, new QTableWidgetItem(start_station));
        train_table->setItem(row, 3, new QTableWidgetItem(end_station));
        train_table->setItem(row, 4, new QTableWidgetItem(type));

        for (int col = 0; col < col_count; col++) {
            train_table->item(row, col)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}

void AdministratorMainWindow::TrainSearch()
{
    QTableWidget* train_table = ui.tableWidgetTrain;
    if (ui.lineEditTrainL->text().isEmpty()) {
        for (int row = 0; row < train_table->rowCount(); row++) {
            train_table->setRowHidden(row, false);
        }
    }
    else {
        QString search_text = ui.lineEditTrainL->text();
        for (int row = 0; row < train_table->rowCount(); row++) {
            if (train_table->item(row, 1)->text() == search_text || train_table->item(row, 2)->text() == search_text || train_table->item(row, 3)->text() == search_text) {
                train_table->setRowHidden(row, false);
            }
            else {
                train_table->setRowHidden(row, true);
            }
        }
    }
}

void AdministratorMainWindow::onClickedTrainOperation()
{
    QObject::connect(ui.pushButtonTrainS, &QPushButton::clicked, this, &AdministratorMainWindow::TrainSearch);
    QObject::connect(ui.lineEditTrainL, &QLineEdit::returnPressed, this, &AdministratorMainWindow::TrainSearch);
}

void AdministratorMainWindow::StationTableSetting()
{
    QTableWidget* station_table = ui.tableWidgetStation;
    int station_count = station_set.size();
    station_table->setCurrentCell(-1, -1);
    station_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    RefreshStationTable();

    station_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    station_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

void AdministratorMainWindow::RefreshStationTable()
{
    QTableWidget* station_table = ui.tableWidgetStation;
    int station_count = station_set.size();
    station_table->setRowCount(station_count);
    int col_count = 2;
    for (int row = 0; row < station_count; row++) {
        QString station_id = QString::number(station_set[row].GetStationId());
        QString station_name = station_set[row].GetName().c_str();
        station_table->setItem(row, 0, new QTableWidgetItem(station_id));
        station_table->setItem(row, 1, new QTableWidgetItem(station_name));

        for (int col = 0; col < col_count; col++) {
            station_table->item(row, col)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}

void AdministratorMainWindow::StationSearch()
{
    QTableWidget* station_table = ui.tableWidgetStation;
    if (ui.lineEditStationL->text().isEmpty()) {
        for (int row = 0; row < station_table->rowCount(); row++) {
            station_table->setRowHidden(row, false);
        }
    }
    else {
        QString search_text = ui.lineEditStationL->text();
        for (int row = 0; row < station_table->rowCount(); row++) {
            if (station_table->item(row, 1)->text() == search_text) {
                station_table->setRowHidden(row, false);
            }
            else {
                station_table->setRowHidden(row, true);
            }
        }
    }
}

void AdministratorMainWindow::onClickedStationOperation()
{
    QObject::connect(ui.pushButtonStationS, &QPushButton::clicked, this, &AdministratorMainWindow::StationSearch);
    QObject::connect(ui.lineEditStationL, &QLineEdit::returnPressed, this, &AdministratorMainWindow::StationSearch);
}

void AdministratorMainWindow::ScheduleTableSetting()
{

}

void AdministratorMainWindow::RefreshScheduleTable()
{

}

void AdministratorMainWindow::ScheduleSearch()
{

}

void AdministratorMainWindow::onClickedScheduleOperation()
{

}
