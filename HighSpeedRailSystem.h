#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HighSpeedRailSystem.h"
#include "Users.h"
#include "Train.h"
#include "TrainSchedule.h"
#include "Order.h"
#include "Station.h"
#include "Seat.h"
#include "TemplateSet.h"

//用户高铁订票系统主界面
class HighSpeedRailSystem : public QMainWindow
{
    Q_OBJECT

public:
    HighSpeedRailSystem(Users* user, TemplateSet<Users>* user_set, QWidget* parent = nullptr);  //构造函数，初始化订票系统主界面
    ~HighSpeedRailSystem(); //析构函数，调用WriteToTxtFile()保存数据
    void LoadFromTxtFile(); //从本地文件中加载数据
    void WriteToTxtFile();  //将变量中的数据存到本地文件中
    void TicketsTableSetting(); //初始化余票查询表格
    void refreshTicketsTableInterface();    //更新余票查询表格接口
    void OrdersTableSetting();  //初始化订单表格设置
    void LoadUserInformationSetting();  //加载用户基本信息到界面中
    void CheckInformation();    //检查用户信息
    void LogOff();  //注销用户
    void OrderEditChanged();    //订单界面查询变更
    void OrderFiltering();  //订单表格过滤操作
    void RefundOperation(); //退票操作
    void RebookOperation(); //改签操作
    void RefreshTicketTable(string on_station, string off_station, QDate q_depart_date);    //刷新余票查询表格
    void TicketSearchPretreatment();    //余票查询预处理操作
    void SearchTickets();   //查询余票
    void onBookingButtonClicked();  //预定按钮点击操作
    void UpdateCompleted(); //更新订单信息操作

public slots:
    void CompletedAndExit();    //预定车票成功执行函数
    
public:
    int init_table; //余票查询界面情况：0代表通用查询界面，1代表初始化界面，2代表改签时车票查询界面
    Users* mp_user; //当前用户指针
    TemplateSet<Users>* mp_user_set;    //用户集合指针
    TemplateSet<Train> train_set;   //列车集合
    TemplateSet<TrainSchedule> train_schedule_set;  //时刻表结合
    TemplateSet<Order> order_set;   //订单集合
    TemplateSet<Station> station_set;   //站点集合
    TemplateSet<Seat> seat_set; //座位集合

private:
    Ui::HighSpeedRailSystemClass ui;    //用户高铁订票系统主界面ui


};

