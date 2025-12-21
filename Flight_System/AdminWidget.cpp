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

    // 1. 初始化界面
    setupUiCustom();

    // 2. 初始加载所有数据
    loadUsers();
    loadAdmins();
    loadFlights();
}

AdminWidget::~AdminWidget()
{
    delete ui;
}

void AdminWidget::setupUiCustom()
{
    // 初始化 StackedWidget 索引
    ui->stackedContent->setCurrentIndex(0);
    ui->listMenu->setCurrentRow(0);

    // --- 用户表设置 ---
    ui->tableUser->setColumnCount(5); // ID, 用户名, 密码, 电话, 余额
    ui->tableUser->setHorizontalHeaderLabels(QStringList() << "ID" << "用户名" << "密码" << "电话" << "余额");
    ui->tableUser->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选中
    ui->tableUser->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 禁止直接编辑
    ui->tableUser->horizontalHeader()->setStretchLastSection(true);
    ui->tableUser->setColumnHidden(0, true); // 隐藏ID列，不给看

    // --- 管理员表设置 ---
    ui->tableManager->setColumnCount(4); // ID, 管理员名, 密码, 电话
    ui->tableManager->setHorizontalHeaderLabels(QStringList() << "ID" << "管理员名" << "密码" << "电话");
    ui->tableManager->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableManager->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableManager->horizontalHeader()->setStretchLastSection(true);
    ui->tableManager->setColumnHidden(0, true);

    // --- 航班表设置 ---
    ui->tableFlight->setColumnCount(8); // 航班号, 航司, 出发, 到达, 起飞, 降落, 价格, 载客
    ui->tableFlight->setHorizontalHeaderLabels(QStringList() << "航班号" << "航司" << "出发地" << "目的地" << "起飞时间" << "到达时间" << "价格" << "载客量");
    ui->tableFlight->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableFlight->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableFlight->horizontalHeader()->setStretchLastSection(true);
}

// 菜单切换
void AdminWidget::on_listMenu_currentRowChanged(int currentRow)
{
    ui->stackedContent->setCurrentIndex(currentRow);
    // 切换页面时顺便刷新数据
    switch(currentRow) {
    case 0: loadUsers(); break;
    case 1: loadAdmins(); break;
    case 2: loadFlights(); break;
    }
}

// =========================================================
// 1. 用户信息管理
// =========================================================

void AdminWidget::loadUsers()
{
    ui->tableUser->setRowCount(0); // 清空旧数据

    QSqlQuery query = ODBC::query("SELECT id, username, password, phone, balance FROM users");

    int row = 0;
    while(query.next()) {
        ui->tableUser->insertRow(row);
        ui->tableUser->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->tableUser->setItem(row, 1, new QTableWidgetItem(query.value("username").toString()));
        ui->tableUser->setItem(row, 2, new QTableWidgetItem(query.value("password").toString()));
        ui->tableUser->setItem(row, 3, new QTableWidgetItem(query.value("phone").toString()));
        ui->tableUser->setItem(row, 4, new QTableWidgetItem(query.value("balance").toString()));
        row++;
    }
}

// 点击表格行，自动填充输入框
void AdminWidget::on_tableUser_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    m_selectedUserId = ui->tableUser->item(row, 0)->text().toInt(); // 记录选中ID

    ui->lineEdit_username->setText(ui->tableUser->item(row, 1)->text());
    ui->lineEdit_password->setText(ui->tableUser->item(row, 2)->text());
    ui->lineEdit_phone->setText(ui->tableUser->item(row, 3)->text());
}

// 添加用户
void AdminWidget::on_btn_add_clicked()
{
    QString u = ui->lineEdit_username->text().trimmed();
    QString p = ui->lineEdit_password->text();
    QString ph = ui->lineEdit_phone->text().trimmed();

    if(u.isEmpty() || p.isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名和密码不能为空");
        return;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO users (username, password, phone, avatar_path) VALUES (?, ?, ?, ':/DefaultAvatar.png')");
    q.addBindValue(u);
    q.addBindValue(p);
    q.addBindValue(ph);

    if(q.exec()) {
        QMessageBox::information(this, "成功", "用户添加成功");
        loadUsers();
        clearUserInputs();
    } else {
        QMessageBox::critical(this, "失败", "添加失败: " + q.lastError().text());
    }
}

// 删除用户
void AdminWidget::on_btn_del_clicked()
{
    if(m_selectedUserId == -1) {
        QMessageBox::warning(this, "警告", "请先点击表格选择要删除的用户");
        return;
    }

    if(QMessageBox::question(this, "确认", "确定要永久删除该用户吗？\n(操作不可撤销)") == QMessageBox::Yes) {
        QSqlQuery q;
        q.prepare("DELETE FROM users WHERE id = ?");
        q.addBindValue(m_selectedUserId);

        if(q.exec()) {
            QMessageBox::information(this, "成功", "删除成功");
            loadUsers();
            clearUserInputs();
        } else {
            QMessageBox::critical(this, "失败", q.lastError().text());
        }
    }
}

// 修改用户
void AdminWidget::on_btn_modify_clicked()
{
    if(m_selectedUserId == -1) return;

    QString u = ui->lineEdit_username->text().trimmed();
    QString p = ui->lineEdit_password->text();
    QString ph = ui->lineEdit_phone->text().trimmed();

    QSqlQuery q;
    q.prepare("UPDATE users SET username=?, password=?, phone=? WHERE id=?");
    q.addBindValue(u); q.addBindValue(p); q.addBindValue(ph); q.addBindValue(m_selectedUserId);

    if(q.exec()) {
        QMessageBox::information(this, "成功", "修改成功");
        loadUsers();
        clearUserInputs();
    } else {
        QMessageBox::critical(this, "失败", q.lastError().text());
    }
}

void AdminWidget::clearUserInputs() {
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
    ui->lineEdit_phone->clear();
    m_selectedUserId = -1;
}

// =========================================================
// 2. 管理员信息管理
// =========================================================

void AdminWidget::loadAdmins()
{
    ui->tableManager->setRowCount(0);
    QSqlQuery query = ODBC::query("SELECT id, username, password, phone FROM admins");

    int row = 0;
    while(query.next()) {
        ui->tableManager->insertRow(row);
        ui->tableManager->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->tableManager->setItem(row, 1, new QTableWidgetItem(query.value("username").toString()));
        ui->tableManager->setItem(row, 2, new QTableWidgetItem(query.value("password").toString()));
        ui->tableManager->setItem(row, 3, new QTableWidgetItem(query.value("phone").toString()));
        row++;
    }
}

void AdminWidget::on_tableManager_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    m_selectedAdminId = ui->tableManager->item(row, 0)->text().toInt();
    ui->lineEdit_managername->setText(ui->tableManager->item(row, 1)->text());
    ui->lineEdit_password_2->setText(ui->tableManager->item(row, 2)->text());
    ui->lineEdit_phone_2->setText(ui->tableManager->item(row, 3)->text());
}

void AdminWidget::on_btn_add_2_clicked()
{
    QString u = ui->lineEdit_managername->text();
    QString p = ui->lineEdit_password_2->text();
    QString ph = ui->lineEdit_phone_2->text();

    if(u.isEmpty() || p.isEmpty()) return;

    QSqlQuery q;
    q.prepare("INSERT INTO admins (username, password, phone, avatar_path) VALUES (?, ?, ?, ':/DefaultAvatar.png')");
    q.addBindValue(u); q.addBindValue(p); q.addBindValue(ph);

    if(q.exec()) {
        QMessageBox::information(this, "成功", "管理员添加成功");
        loadAdmins();
        clearAdminInputs();
    } else {
        QMessageBox::critical(this, "失败", q.lastError().text());
    }
}

void AdminWidget::on_btn_del_2_clicked()
{
    if(m_selectedAdminId == -1) return;

    if(QMessageBox::question(this, "确认", "确定删除该管理员？") == QMessageBox::Yes) {
        QSqlQuery q;
        q.prepare("DELETE FROM admins WHERE id = ?");
        q.addBindValue(m_selectedAdminId);
        if(q.exec()) {
            loadAdmins();
            clearAdminInputs();
        }
    }
}

void AdminWidget::on_btn_modify_2_clicked()
{
    if(m_selectedAdminId == -1) return;

    QString u = ui->lineEdit_managername->text();
    QString p = ui->lineEdit_password_2->text();
    QString ph = ui->lineEdit_phone_2->text();

    QSqlQuery q;
    q.prepare("UPDATE admins SET username=?, password=?, phone=? WHERE id=?");
    q.addBindValue(u); q.addBindValue(p); q.addBindValue(ph); q.addBindValue(m_selectedAdminId);

    if(q.exec()) {
        loadAdmins();
        clearAdminInputs();
        QMessageBox::information(this, "成功", "修改成功");
    }
}

void AdminWidget::clearAdminInputs() {
    ui->lineEdit_managername->clear();
    ui->lineEdit_password_2->clear();
    ui->lineEdit_phone_2->clear();
    m_selectedAdminId = -1;
}

// =========================================================
// 3. 航班信息管理
// =========================================================

void AdminWidget::loadFlights()
{
    ui->tableFlight->setRowCount(0);
    // 注意：flight_id 是主键 (varchar)
    QSqlQuery query = ODBC::query("SELECT flight_id, airline, departure_city, arrival_city, departure_time, arrival_time, price, capacity FROM flights");

    int row = 0;
    while(query.next()) {
        ui->tableFlight->insertRow(row);
        ui->tableFlight->setItem(row, 0, new QTableWidgetItem(query.value("flight_id").toString()));
        ui->tableFlight->setItem(row, 1, new QTableWidgetItem(query.value("airline").toString()));
        ui->tableFlight->setItem(row, 2, new QTableWidgetItem(query.value("departure_city").toString()));
        ui->tableFlight->setItem(row, 3, new QTableWidgetItem(query.value("arrival_city").toString()));
        ui->tableFlight->setItem(row, 4, new QTableWidgetItem(query.value("departure_time").toDateTime().toString("yyyy-MM-dd HH:mm")));
        ui->tableFlight->setItem(row, 5, new QTableWidgetItem(query.value("arrival_time").toDateTime().toString("yyyy-MM-dd HH:mm")));
        ui->tableFlight->setItem(row, 6, new QTableWidgetItem(QString::number(query.value("price").toDouble(), 'f', 2)));
        ui->tableFlight->setItem(row, 7, new QTableWidgetItem(query.value("capacity").toString()));
        row++;
    }
}

void AdminWidget::on_tableFlight_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    // 获取航班号 (主键)
    m_selectedFlightId = ui->tableFlight->item(row, 0)->text();

    ui->lineEdit_flightID->setText(m_selectedFlightId);
    // 这里要注意：如果你的输入框没有用来填航司，这里就无法自动填航司了
    ui->lineEdit_departure->setText(ui->tableFlight->item(row, 2)->text());
    ui->lineEdit_destination->setText(ui->tableFlight->item(row, 3)->text());
    ui->lineEdit_dptTime->setText(ui->tableFlight->item(row, 4)->text());
    ui->lineEdit_arrTime->setText(ui->tableFlight->item(row, 5)->text());
    ui->lineEdit_price->setText(ui->tableFlight->item(row, 6)->text());
    ui->lineEdit_capacity->setText(ui->tableFlight->item(row, 7)->text());
}

void AdminWidget::on_btn_add_3_clicked()
{
    QString fid = ui->lineEdit_flightID->text();
    QString dep = ui->lineEdit_departure->text();
    QString arr = ui->lineEdit_destination->text();
    // 假设你有航司输入框，没有就默认写死一个
    QString airline = "四川航空";

    QSqlQuery q;
    q.prepare("INSERT INTO flights (flight_id, airline, departure_city, arrival_city, departure_time, arrival_time, price, capacity, status) "
              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, '计划中')");
    q.addBindValue(fid);
    q.addBindValue(airline);
    q.addBindValue(dep);
    q.addBindValue(arr);
    q.addBindValue(ui->lineEdit_dptTime->text()); // 格式 yyyy-MM-dd HH:mm:ss
    q.addBindValue(ui->lineEdit_arrTime->text());
    q.addBindValue(ui->lineEdit_price->text().toDouble());
    q.addBindValue(ui->lineEdit_capacity->text().toInt());

    if(q.exec()) {
        QMessageBox::information(this, "成功", "航班添加成功");
        loadFlights();
        clearFlightInputs();
    } else {
        QMessageBox::critical(this, "失败", q.lastError().text());
    }
}

void AdminWidget::on_btn_del_3_clicked()
{
    if(m_selectedFlightId.isEmpty()) return;

    if(QMessageBox::question(this, "确认", "确定删除该航班吗？") == QMessageBox::Yes) {
        QSqlQuery q;
        q.prepare("DELETE FROM flights WHERE flight_id = ?");
        q.addBindValue(m_selectedFlightId);
        if(q.exec()) {
            loadFlights();
            clearFlightInputs();
        }
    }
}

void AdminWidget::on_btn_modify_3_clicked()
{
    // 修改航班逻辑略，建议不允许修改主键 flight_id
    // UPDATE flights SET price=?, capacity=? WHERE flight_id=?
}

void AdminWidget::clearFlightInputs() {
    ui->lineEdit_flightID->clear();
    ui->lineEdit_departure->clear();
    ui->lineEdit_destination->clear();
    ui->lineEdit_dptTime->clear();
    ui->lineEdit_arrTime->clear();
    ui->lineEdit_price->clear();
    ui->lineEdit_capacity->clear();
    m_selectedFlightId.clear();
}
