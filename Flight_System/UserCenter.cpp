#include "usercenter.h"
#include "ui_usercenter.h"
#include "ODBC.h"           // 引入你的数据库连接类
#include <QSqlQuery>        // 执行SQL语句
#include <QSqlError>        // 获取错误信息
#include <QMessageBox>      // 弹窗
#include <QDebug>
#include <QPainter>         // 用于画圆形头像
#include <QPainterPath>

UserCenter::UserCenter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserCenter)
{
    ui->setupUi(this);

    // 1. 初始化界面状态
    isEditing = false;
    setInputsEnabled(false); // 默认所有输入框锁定，只读

    // 2. 连接按钮信号
    connect(ui->btnEdit, &QPushButton::clicked, this, &UserCenter::onBtnEditClicked);
    connect(ui->btnPass, &QPushButton::clicked, this, &UserCenter::onBtnPassClicked);
    connect(ui->btnLogout, &QPushButton::clicked, this, &UserCenter::onBtnLogoutClicked);

    // 3. 设置一下默认头像（防止数据库还没读的时候是空的）
    updateAvatarDisplay(":/DefaultAvatar.png");
}

UserCenter::~UserCenter()
{
    delete ui;
}

// ==========================================
// 核心功能：从数据库加载数据
// ==========================================
void UserCenter::loadUserData(QString userId)
{
    m_currentUserId = userId; // 存下来，更新的时候要用

    // 1. 确保数据库连接
    if (!ODBC::connectToDB()) {
        QMessageBox::critical(this, "系统错误", "无法连接到数据库！");
        return;
    }

    // 2. 准备查询语句
    QSqlQuery query;
    // 对应你的表结构：id, username, phone, birthday, balance, avatar_path
    QString sql = "SELECT username, phone, birthday, balance, avatar_path FROM user WHERE id = :id";

    query.prepare(sql);
    query.bindValue(":id", m_currentUserId);

    // 3. 执行查询
    if (query.exec()) {
        if (query.next()) {
            // --- 获取数据 ---
            QString name = query.value("username").toString();
            QString phone = query.value("phone").toString();
            QString avatarPath = query.value("avatar_path").toString();
            double balance = query.value("balance").toDouble();

            // 处理生日：如果是 NULL，给一个默认值 2000-01-01，否则转为日期
            QDate birth = query.value("birthday").toDate();
            if (!birth.isValid()) {
                birth = QDate(2000, 1, 1);
            }

            // --- 显示到界面 ---
            ui->editUsername->setText(name);
            ui->editPhone->setText(phone);
            ui->editBirthday->setDate(birth);

            // 显示余额 (保留2位小数)
            ui->lblBalanceValue->setText(QString::number(balance, 'f', 2));

            // 显示圆形头像
            // 如果数据库里头像路径是空的，就用默认头像
            if (avatarPath.isEmpty()) avatarPath = ":/DefaultAvatar.png";
            updateAvatarDisplay(avatarPath);

        } else {
            QMessageBox::warning(this, "错误", "未查询到用户信息 (ID: " + userId + ")");
        }
    } else {
        qDebug() << "SQL Error:" << query.lastError().text();
        QMessageBox::critical(this, "查询失败", query.lastError().text());
    }
}

// ==========================================
// 核心功能：编辑与保存
// ==========================================
void UserCenter::onBtnEditClicked()
{
    if (!isEditing) {
        // >>>>> 进入编辑模式 <<<<<
        setInputsEnabled(true);

        // 按钮变身：变成绿色“保存修改”
        ui->btnEdit->setText("保存修改");
        ui->btnEdit->setStyleSheet("QPushButton { background-color: #67C23A; color: white; border-radius: 5px; }"
                                   "QPushButton:hover { background-color: #85ce61; }");
        isEditing = true;

    } else {
        // >>>>> 点击了保存 <<<<<

        // 1. 获取输入框内容
        QString newName = ui->editUsername->text().trimmed();
        QString newPhone = ui->editPhone->text().trimmed();
        QDate newBirth = ui->editBirthday->date();

        // 2. 简单校验
        if (newName.isEmpty()) {
            QMessageBox::warning(this, "提示", "用户名不能为空！");
            return;
        }

        // 3. 执行更新 SQL
        if (ODBC::connectToDB()) {
            QSqlQuery query;
            // 只更新 姓名、手机、生日
            QString sql = "UPDATE user SET username = :u, phone = :p, birthday = :b WHERE id = :id";

            query.prepare(sql);
            query.bindValue(":u", newName);
            query.bindValue(":p", newPhone);
            query.bindValue(":b", newBirth);
            query.bindValue(":id", m_currentUserId);

            if (query.exec()) {
                QMessageBox::information(this, "成功", "个人资料已更新！");

                // 4. 恢复界面到只读模式
                setInputsEnabled(false);
                ui->btnEdit->setText("编辑资料");
                // 恢复蓝色按钮样式
                ui->btnEdit->setStyleSheet("QPushButton { background-color: #409EFF; color: white; border-radius: 5px; }"
                                           "QPushButton:hover { background-color: #66b1ff; }");
                isEditing = false;
            } else {
                QMessageBox::critical(this, "保存失败", "数据库错误：" + query.lastError().text());
            }
        }
    }
}

// ==========================================
// 辅助功能：控制输入框锁定/解锁
// ==========================================
void UserCenter::setInputsEnabled(bool enable)
{
    ui->editUsername->setEnabled(enable);
    ui->editPhone->setEnabled(enable);
    ui->editBirthday->setEnabled(enable);

    // 视觉优化：不可编辑时去掉边框背景，看起来像纯文字；可编辑时显示白色输入框
    if (enable) {
        QString editStyle = "QLineEdit, QDateEdit { background-color: #ffffff; border: 1px solid #dcdfe6; border-radius: 4px; padding: 0 5px; }";
        ui->editUsername->setStyleSheet(editStyle);
        ui->editPhone->setStyleSheet(editStyle);
        ui->editBirthday->setStyleSheet(editStyle);
    } else {
        QString readStyle = "QLineEdit, QDateEdit { background-color: transparent; border: none; color: #606266; font-weight: bold; }";
        ui->editUsername->setStyleSheet(readStyle);
        ui->editPhone->setStyleSheet(readStyle);
        ui->editBirthday->setStyleSheet(readStyle);
    }
}

// ==========================================
// 辅助功能：将图片裁剪为圆形并显示
// ==========================================
void UserCenter::updateAvatarDisplay(QString path)
{
    // 1. 加载图片
    QPixmap src(path);
    if (src.isNull()) {
        // 如果路径不对，尝试加载默认图
        src.load(":/DefaultAvatar.png");
        if (src.isNull()) return; // 实在没有就算了
    }

    // 2. 准备绘制圆形
    int size = 100; // 这里的尺寸要和你 UI 文件里 Label 的固定尺寸一致
    QPixmap circular(size, size);
    circular.fill(Qt::transparent); // 透明背景

    QPainter painter(&circular);
    painter.setRenderHint(QPainter::Antialiasing);           // 抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform);  // 平滑变换

    // 3. 画圆并填充图片
    QPainterPath clipPath;
    clipPath.addEllipse(0, 0, size, size);
    painter.setClipPath(clipPath);

    // 将原图缩放填满圆圈
    painter.drawPixmap(0, 0, size, size, src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    // 4. 设置给 Label
    ui->lblAvatarDisplay->setPixmap(circular);
}

// ==========================================
// 修改密码（逻辑较复杂，建议弹新窗口）
// ==========================================
void UserCenter::onBtnPassClicked()
{
    // 这里留给之后做 ModifyPasswordDialog 使用
    QMessageBox::information(this, "功能提示", "请在此处连接修改密码的弹窗界面。\n当前用户ID: " + m_currentUserId);
}

// ==========================================
// 退出登录
// ==========================================
void UserCenter::onBtnLogoutClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "退出系统", "确定要退出登录吗？",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // 发送信号，让 Main.cpp 或 MainWindow 处理界面切换
        emit logoutSignal();
    }
}
