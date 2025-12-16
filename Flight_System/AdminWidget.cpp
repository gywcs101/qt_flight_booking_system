#include "AdminWidget.h"
#include "ui_AdminWidget.h"
#include "ODBC.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidgetItem>
#include <QDateTime>

AdminWidget::AdminWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminWidget),
    m_selectedUserId(-1),
    m_selectedAdminId(-1)
{
    ui->setupUi(this);
    setupUiCustom();
}

AdminWidget::~AdminWidget()
{
    delete ui;
}

void AdminWidget::setupUiCustom()
{
    // 1. [核心修复] 不再用代码重复添加菜单项。
    //    请确保这些菜单项已经在 AdminWidget.ui 文件的 listMenu 中设置好了。
    // ui->listMenu->insertItem(0, "用户信���管理");
    // ui->listMenu->insertItem(1, "管理员信息管理");
    // ui->listMenu->insertItem(2, "航班信息管理");

    // 2. 初始化表格样式和表头
    // 用户表
    ui->tableUser->setColumnCount(4);
    ui->tableUser->setHorizontalHeaderLabels(QStringList() << "ID" << "用户名" << "密码" << "手机号");
    ui->tableUser->setColumnHidden(0, true);
    ui->tableUser->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableUser->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableUser->horizontalHeader()->setStretchLastSection(true);

    // 管理员表
    ui->tableManager->setColumnCount(4);
    ui->tableManager->setHorizontalHeaderLabels(QStringList() << "ID" << "管理员名" << "密码" << "手机号");
    ui->tableManager->setColumnHidden(0, true);
    ui->tableManager->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableManager->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableManager->horizontalHeader()->setStretchLastSection(true);

    // 航班表
    ui->tableFlight->setColumnCount(8);
    ui->tableFlight->setHorizontalHeaderLabels(QStringList() << "航班号" << "出发地" << "目的地" << "出发时间" << "到达时间" << "价格" << "核载" << "航空公司");
    ui->tableFlight->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableFlight->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableFlight->horizontalHeader()->setStretchLastSection(true);

    // 3. Qt 会自动连接 on_<objectName>_<signalName> 格式的槽函数。

    // 4. 初始加载所有数据
    loadUsers();
    loadAdmins();
    loadFlights();

    // 5. 默认显示第一个页面
    ui->listMenu->setCurrentRow(0);
}

void AdminWidget::on_listMenu_currentRowChanged(int currentRow)
{
    ui->stackedContent->setCurrentIndex(currentRow);
}

// ======================= 用户管理 (已补全) =======================
void AdminWidget::loadUsers()
{
    ui->tableUser->setRowCount(0);
    QSqlQuery query = ODBC::query("SELECT id, username, password, phone FROM users");
    while (query.next()) {
        int row = ui->tableUser->rowCount();
        ui->tableUser->insertRow(row);
        ui->tableUser->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->tableUser->setItem(row, 1, new QTableWidgetItem(query.value("username").toString()));
        ui->tableUser->setItem(row, 2, new QTableWidgetItem(query.value("password").toString()));
        ui->tableUser->setItem(row, 3, new QTableWidgetItem(query.value("phone").toString()));
    }
}

void AdminWidget::on_tableUser_cellClicked(int row, int col)
{
    Q_UNUSED(col);
    m_selectedUserId = ui->tableUser->item(row, 0)->text().toInt();
    ui->lineEdit_username->setText(ui->tableUser->item(row, 1)->text());
    ui->lineEdit_password->setText(ui->tableUser->item(row, 2)->text());
    ui->lineEdit_phone->setText(ui->tableUser->item(row, 3)->text());
}

void AdminWidget::on_btn_add_clicked()
{
    QString u = ui->lineEdit_username->text().trimmed();
    QString p = ui->lineEdit_password->text();
    QString ph = ui->lineEdit_phone->text().trimmed();
    if (u.isEmpty() || p.isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名和密码不能为空");
        return;
    }
    QSqlQuery q;
    q.prepare("INSERT INTO users (username, password, phone) VALUES (?, ?, ?)");
    q.addBindValue(u); q.addBindValue(p); q.addBindValue(ph);
    if (q.exec()) {
        QMessageBox::information(this, "成功", "用户添加成功");
        loadUsers();
        clearUserInputs();
    } else {
        QMessageBox::critical(this, "失败", "添加失败: " + q.lastError().text());
    }
}

void AdminWidget::on_btn_del_clicked()
{
    if (m_selectedUserId == -1) {
        QMessageBox::warning(this, "警告", "请先选择要删除的用户");
        return;
    }
    if (QMessageBox::question(this, "确认", "确定删除该用户吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QSqlQuery q;
        q.prepare("DELETE FROM users WHERE id = ?");
        q.addBindValue(m_selectedUserId);
        if (q.exec()) {
            QMessageBox::information(this, "成功", "用户删除成功");
            loadUsers();
            clearUserInputs();
        } else {
            QMessageBox::critical(this, "失败", "删除失败: " + q.lastError().text());
        }
    }
}

void AdminWidget::on_btn_modify_clicked()
{
    if (m_selectedUserId == -1) {
        QMessageBox::warning(this, "警告", "请先选择要修改的用户");
        return;
    }
    QString u = ui->lineEdit_username->text().trimmed();
    QString p = ui->lineEdit_password->text();
    QString ph = ui->lineEdit_phone->text().trimmed();
    QSqlQuery q;
    q.prepare("UPDATE users SET username = ?, password = ?, phone = ? WHERE id = ?");
    q.addBindValue(u); q.addBindValue(p); q.addBindValue(ph); q.addBindValue(m_selectedUserId);
    if (q.exec()) {
        QMessageBox::information(this, "成功", "用户信息修改成功");
        loadUsers();
        clearUserInputs();
    } else {
        QMessageBox::critical(this, "失败", "修改失败: " + q.lastError().text());
    }
}

void AdminWidget::clearUserInputs()
{
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
    ui->lineEdit_phone->clear();
    m_selectedUserId = -1;
}

// ======================= 管理员管理 (已补全) =======================
void AdminWidget::loadAdmins()
{
    ui->tableManager->setRowCount(0);
    QSqlQuery query = ODBC::query("SELECT id, username, password, phone FROM admins");
    while (query.next()) {
        int row = ui->tableManager->rowCount();
        ui->tableManager->insertRow(row);
        ui->tableManager->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->tableManager->setItem(row, 1, new QTableWidgetItem(query.value("username").toString()));
        ui->tableManager->setItem(row, 2, new QTableWidgetItem(query.value("password").toString()));
        ui->tableManager->setItem(row, 3, new QTableWidgetItem(query.value("phone").toString()));
    }
}

void AdminWidget::on_tableManager_cellClicked(int row, int col)
{
    Q_UNUSED(col);
    m_selectedAdminId = ui->tableManager->item(row, 0)->text().toInt();
    ui->lineEdit_managername->setText(ui->tableManager->item(row, 1)->text());
    ui->lineEdit_password_2->setText(ui->tableManager->item(row, 2)->text());
    ui->lineEdit_phone_2->setText(ui->tableManager->item(row, 3)->text());
}

void AdminWidget::on_btn_add_2_clicked()
{
    QString u = ui->lineEdit_managername->text().trimmed();
    QString p = ui->lineEdit_password_2->text();
    QString ph = ui->lineEdit_phone_2->text().trimmed();
    if (u.isEmpty() || p.isEmpty()) {
        QMessageBox::warning(this, "警告", "管理员名和密码不能为空");
        return;
    }
    QSqlQuery q;
    q.prepare("INSERT INTO admins (username, password, phone) VALUES (?, ?, ?)");
    q.addBindValue(u); q.addBindValue(p); q.addBindValue(ph);
    if (q.exec()) {
        QMessageBox::information(this, "成功", "管理员添加成功");
        loadAdmins();
        clearAdminInputs();
    } else {
        QMessageBox::critical(this, "失败", "添加失败: " + q.lastError().text());
    }
}

void AdminWidget::on_btn_del_2_clicked()
{
    if (m_selectedAdminId == -1) {
        QMessageBox::warning(this, "警告", "请先选择要删除的管理员");
        return;
    }
    if (QMessageBox::question(this, "确认", "确定删除该管理员吗？", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QSqlQuery q;
        q.prepare("DELETE FROM admins WHERE id = ?");
        q.addBindValue(m_selectedAdminId);
        if (q.exec()) {
            QMessageBox::information(this, "成功", "管理员删除成功");
            loadAdmins();
            clearAdminInputs();
        } else {
            QMessageBox::critical(this, "失败", "删除失败: " + q.lastError().text());
        }
    }
}

void AdminWidget::on_btn_modify_2_clicked()
{
    if (m_selectedAdminId == -1) {
        QMessageBox::warning(this, "警告", "请先选择要修改的管理员");
        return;
    }
    QString u = ui->lineEdit_managername->text().trimmed();
    QString p = ui->lineEdit_password_2->text();
    QString ph = ui->lineEdit_phone_2->text().trimmed();
    QSqlQuery q;
    q.prepare("UPDATE admins SET username = ?, password = ?, phone = ? WHERE id = ?");
    q.addBindValue(u); q.addBindValue(p); q.addBindValue(ph); q.addBindValue(m_selectedAdminId);
    if (q.exec()) {
        QMessageBox::information(this, "成功", "管理员信息修改成功");
        loadAdmins();
        clearAdminInputs();
    } else {
        QMessageBox::critical(this, "失败", "修改失败: " + q.lastError().text());
    }
}

void AdminWidget::clearAdminInputs()
{
    ui->lineEdit_managername->clear();
    ui->lineEdit_password_2->clear();
    ui->lineEdit_phone_2->clear();
    m_selectedAdminId = -1;
}

// ======================= 航班管理 =======================
void AdminWidget::loadFlights()
{
    ui->tableFlight->setRowCount(0);
    // ★★★ 请确认您的航班表名 'flights' 及所有列名 ★★★
    QSqlQuery query = ODBC::query("SELECT flight_id, departure_city, arrival_city, departure_time, arrival_time, price, capacity, airline FROM flights");
    while (query.next()) {
        int row = ui->tableFlight->rowCount();
        ui->tableFlight->insertRow(row);
        ui->tableFlight->setItem(row, 0, new QTableWidgetItem(query.value("flight_id").toString()));
        ui->tableFlight->setItem(row, 1, new QTableWidgetItem(query.value("departure_city").toString()));
        ui->tableFlight->setItem(row, 2, new QTableWidgetItem(query.value("arrival_city").toString()));
        ui->tableFlight->setItem(row, 3, new QTableWidgetItem(query.value("departure_time").toDateTime().toString("yyyy-MM-dd HH:mm")));
        ui->tableFlight->setItem(row, 4, new QTableWidgetItem(query.value("arrival_time").toDateTime().toString("yyyy-MM-dd HH:mm")));
        ui->tableFlight->setItem(row, 5, new QTableWidgetItem(QString::number(query.value("price").toDouble(), 'f', 2)));
        ui->tableFlight->setItem(row, 6, new QTableWidgetItem(query.value("capacity").toString()));
        ui->tableFlight->setItem(row, 7, new QTableWidgetItem(query.value("airline").toString()));
    }
}

// [核心修正] 确认航班信息输入框的 objectName
void AdminWidget::on_tableFlight_cellClicked(int row, int col)
{
    Q_UNUSED(col);
    m_selectedFlightId = ui->tableFlight->item(row, 0)->text();
    ui->lineEdit_flightID->setText(m_selectedFlightId);
    ui->lineEdit_departure->setText(ui->tableFlight->item(row, 1)->text());
    ui->lineEdit_destination->setText(ui->tableFlight->item(row, 2)->text());
    ui->lineEdit_dptTime->setText(ui->tableFlight->item(row, 3)->text());
    ui->lineEdit_arrTime->setText(ui->tableFlight->item(row, 4)->text());
    ui->lineEdit_price->setText(ui->tableFlight->item(row, 5)->text());
    // [核心修正] 根据您的UI截图，核载人数的输入框叫 lineEdit_capacity
    ui->lineEdit_capacity->setText(ui->tableFlight->item(row, 6)->text());
}

// ... (航班管理的增删改函数，逻辑不变，但为了完整性全部贴出)
void AdminWidget::on_btn_add_3_clicked() { QString fid=ui->lineEdit_flightID->text(),dep=ui->lineEdit_departure->text(),arr=ui->lineEdit_destination->text(); if(fid.isEmpty()||dep.isEmpty()||arr.isEmpty()){QMessageBox::warning(this,"警告","航班号、出发地、目的地不能为空");return;} QSqlQuery q; q.prepare("INSERT INTO flights (flight_id, departure_city, arrival_city, departure_time, arrival_time, price, capacity, airline) VALUES (?, ?, ?, ?, ?, ?, ?, ?)"); q.addBindValue(fid); q.addBindValue(dep); q.addBindValue(arr); q.addBindValue(QDateTime::fromString(ui->lineEdit_dptTime->text(),"yyyy-MM-dd HH:mm")); q.addBindValue(QDateTime::fromString(ui->lineEdit_arrTime->text(),"yyyy-MM-dd HH:mm")); q.addBindValue(ui->lineEdit_price->text().toDouble()); q.addBindValue(ui->lineEdit_capacity->text().toInt()); q.addBindValue(""); /* 航空公司暂时为空 */ if(q.exec()){QMessageBox::information(this,"成功","航班添加成功");loadFlights();clearFlightInputs();}else{QMessageBox::critical(this,"失败",q.lastError().text());} }
void AdminWidget::on_btn_del_3_clicked() { if(m_selectedFlightId.isEmpty()){QMessageBox::warning(this,"警告","请先选择要删除的航班");return;} if(QMessageBox::question(this,"确认","确定删除该航班吗？")==QMessageBox::Yes){QSqlQuery q; q.prepare("DELETE FROM flights WHERE flight_id = ?"); q.addBindValue(m_selectedFlightId); if(q.exec()){QMessageBox::information(this,"成功","航班删除成功");loadFlights();clearFlightInputs();}else{QMessageBox::critical(this,"失败",q.lastError().text());}} }
void AdminWidget::on_btn_modify_3_clicked() { if(m_selectedFlightId.isEmpty()){QMessageBox::warning(this,"警告","请先选择要修改的航班");return;} QSqlQuery q; q.prepare("UPDATE flights SET flight_id=?, departure_city=?, arrival_city=?, departure_time=?, arrival_time=?, price=?, capacity=? WHERE flight_id=?"); q.addBindValue(ui->lineEdit_flightID->text()); q.addBindValue(ui->lineEdit_departure->text()); q.addBindValue(ui->lineEdit_destination->text()); q.addBindValue(QDateTime::fromString(ui->lineEdit_dptTime->text(),"yyyy-MM-dd HH:mm")); q.addBindValue(QDateTime::fromString(ui->lineEdit_arrTime->text(),"yyyy-MM-dd HH:mm")); q.addBindValue(ui->lineEdit_price->text().toDouble()); q.addBindValue(ui->lineEdit_capacity->text().toInt()); q.addBindValue(m_selectedFlightId); if(q.exec()){QMessageBox::information(this,"成功","航班信息修改成功");loadFlights();clearFlightInputs();}else{QMessageBox::critical(this,"失败",q.lastError().text());} }
void AdminWidget::clearFlightInputs() { ui->lineEdit_flightID->clear(); ui->lineEdit_departure->clear(); ui->lineEdit_destination->clear(); ui->lineEdit_dptTime->clear(); ui->lineEdit_arrTime->clear(); ui->lineEdit_price->clear(); ui->lineEdit_capacity->clear(); m_selectedFlightId.clear(); }
