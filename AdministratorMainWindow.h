#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AdministratorMainWindow.h"
#include "Users.h"
#include "TemplateSet.h"
#include "Train.h"
#include "Station.h"
#include "TrainSchedule.h"
#include "Order.h"
#include "Seat.h"


class AdministratorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AdministratorMainWindow(Users* user, TemplateSet<Users>* user_set, QWidget* parent = nullptr);  //管理员系统界面构造函数
    ~AdministratorMainWindow(); //析构函数
    void InitTableSetting();    //初始化各项表格
    void LoadFromTxtFile();     //从本地文件中加载信息
    void WriteToTxtFile();      //将变量中的内容存储到本地文件
    void TrainTableSetting();   //列车表格设置
    void RefreshTrainTable();   //更新列车表格
    void TrainSearch(); //搜索列车
    void onClickedTrainOperation(); //链接列车各项操作
    void StationTableSetting(); //站点表格设置
    void RefreshStationTable(); //更新站点表格
    void StationSearch();   //搜索站点
    void onClickedStationOperation();   //链接站点各项操作
    void ScheduleTableSetting();    //时刻表表格设置
    void RefreshScheduleTable();    //更新时刻表设置   
    void ScheduleSearch();  //搜索时刻表
    void onClickedScheduleOperation();  //链接时刻表各项操作
    void SeatTableSetting();    //座位表格设置
    void RefreshSeatTable();    //更新座位表格
    void SeatSearch();  //搜索座位
    void onClickedSeatOperation();  //链接座位各项操作
    void addRecord(int kind);   //增加一条记录（kind表示记录的种类）
    void modifyRecord(int kind);    //修改一条记录（kind表示记录的种类）
    void deleteRecord(int kind);    //删除一条记录（kind表示记录的种类）
    void OrdersTableSetting();  //初始化订单表格设置
    void OrderFiltering();  //订单表格过滤操作
    void OrderEditChanged();    //订单界面查询变更
    void RefundOperation(); //退票操作
    void RebookOperation(); //改签操作
    void SearchTickets();   //查询余票
    void RefreshTicketTable(string on_station, string off_station, QDate q_depart_date);    //刷新余票查询表格
    void onBookingButtonClicked();  //预定按钮点击操作
    void UpdateCompleted(); //更新订单信息操作
    void ExitRebookDlg();   //退出改签界面
    void UsersTableSetting();   //用户表格初始化设置
    void RefreshUsersTable();   //更新用户表格
    void UsersSearch(); //用户搜索

public slots:
    void CompletedAndExit();        //预定车票成功执行函数
    void onResetPasswordClicked();  // 处理密码重置/确认修改
    void onToggleLockClicked();     // 处理锁定/解锁

public:
    int rebook_table;   //改签界面是否显示
    Users* mp_admin;    //管理员
    Users* mp_user;     //用户
    TemplateSet<Users>* mp_user_set;    //用户集合指针
    TemplateSet<Train> train_set;   //列车集合
    TemplateSet<TrainSchedule> train_schedule_set;  //时刻表集合
    TemplateSet<Order> order_set;   //订单集合
    TemplateSet<Station> station_set;   //站点集合
    TemplateSet<Seat> seat_set; //座位集合

private:
    Ui::AdministratorMainWindow ui; //管理员订票系统界面ui


};

static void showSuccessBox(QWidget* parent);    //操作成功时展示的提示框
