#include "UserCenter.h"
#include "ui_UserCenter.h"
#include "ODBC.h"
#include "UserSession.h" // 包含您现有的 UserSession.h
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>

UserCenter::UserCenter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserCenter)
{
    ui->setupUi(this);

    // 初始化界面状态
    isEditing = false;
    setInputsEnabled(false); // 默认所有输入框锁定

    // 连接按钮信号
    connect(ui->btnEdit, &QPushButton::clicked, this, &UserCenter::onBtnEditClicked);
    connect(ui->btnPass, &QPushButton::clicked, this, &UserCenter::onBtnPassClicked);
    connect(ui->btnLogout, &QPushButton::clicked, this, &UserCenter::onBtnLogoutClicked);

    // 设置默认头像
    updateAvatarDisplay(":/DefaultAvatar.png");
}

UserCenter::~UserCenter()
{
    delete ui;
}

// [核心] 从 UserSession 加载当前登录用户的数据
void UserCenter::loadCurrentUserData()
{
    // 1. 从您的 UserSession 单例获取当前用户ID (int类型)
    m_currentUserId = UserSession::instance().getUserId();

    // 检查用户是否有效登录
    if (m_currentUserId == -1) {
        QMessageBox::critical(this, "系统错误", "无法获取当前用户信息，请重新登录！");
        // 可以考虑清空界面显示
        ui->editUsername->clear();
        ui->editPhone->clear();
        ui->lblBalanceValue->setText("0.00");
        return;
    }

    // 2. 准备查询语句 (根据您的数据库截图)
    // 表名: users (请确认为您的表名)
    // 列名: username, phone, birthday, balance, avatar_path
    QString sql = QString("SELECT username, phone, birthday, balance, avatar_path FROM users WHERE id = %1").arg(m_currentUserId);
    QSqlQuery query = ODBC::query(sql);

    // 3. 执行查询并更新UI
    if (query.next()) {
        QString name = query.value("username").toString();
        QString phone = query.value("phone").toString();
        QString avatarPath = query.value("avatar_path").toString();
        double balance = query.value("balance").toDouble();
        QDate birth = query.value("birthday").toDate();

        ui->editUsername->setText(name);
        ui->editPhone->setText(phone);
        ui->editBirthday->setDate(birth.isValid() ? birth : QDate(2000, 1, 1));
        ui->lblBalanceValue->setText(QString::number(balance, 'f', 2));

        if (avatarPath.isEmpty() || avatarPath == "NULL") {
            avatarPath = ":/DefaultAvatar.png";
        }
        updateAvatarDisplay(avatarPath);

        // 确保每次加载数据后，界面都恢复到非编辑状态
        setInputsEnabled(false);
        ui->btnEdit->setText("编辑资料");
        ui->btnEdit->setStyleSheet(""); // 清除特定样式，恢复默认
        isEditing = false;

    } else {
        qWarning() << "SQL Error:" << query.lastError().text() << " while fetching user ID:" << m_currentUserId;
        QMessageBox::warning(this, "数据加载失败", "无法从数据库中找到您的用户信息。");
    }
}

// "编辑资料" / "保存修改" 按钮的槽函数
void UserCenter::onBtnEditClicked()
{
    if (!isEditing) {
        // ---- 进入编辑模式 ----
        setInputsEnabled(true);
        ui->btnEdit->setText("保存修改");
        ui->btnEdit->setStyleSheet("background-color: #67C23A; color: white; border-radius: 4px;");
        isEditing = true;
    } else {
        // ---- 点击了“保存修改” ----
        QString newName = ui->editUsername->text().trimmed();
        QString newPhone = ui->editPhone->text().trimmed();
        QDate newBirth = ui->editBirthday->date();

        if (newName.isEmpty()) {
            QMessageBox::warning(this, "输入无效", "用户名不能为空！");
            return;
        }

        // 准备带参数绑定的更新SQL语句，更安全
        QSqlQuery query;
        query.prepare("UPDATE users SET username = :name, phone = :phone, birthday = :bday WHERE id = :id");
        query.bindValue(":name", newName);
        query.bindValue(":phone", newPhone);
        query.bindValue(":bday", newBirth.toString(Qt::ISODate)); // "YYYY-MM-DD"
        query.bindValue(":id", m_currentUserId);

        if (query.exec()) {
            QMessageBox::information(this, "操作成功", "您的个人资料已更新！");
            // 恢复到非编辑状态
            setInputsEnabled(false);
            ui->btnEdit->setText("编辑资料");
            ui->btnEdit->setStyleSheet(""); // 恢复默认样式
            isEditing = false;
        } else {
            QMessageBox::critical(this, "保存失败", "数据库更新错误：" + query.lastError().text());
        }
    }
}

// 控制输入框是否可编辑
void UserCenter::setInputsEnabled(bool enable)
{
    ui->editUsername->setReadOnly(!enable);
    ui->editPhone->setReadOnly(!enable);
    ui->editBirthday->setReadOnly(!enable);

    // 视觉样式切换
    const QString readOnlyStyle = "background-color: transparent; border: none;";
    const QString editableStyle = "background-color: white; border: 1px solid #DCDFE6; border-radius: 4px;";
    QString currentStyle = enable ? editableStyle : readOnlyStyle;

    ui->editUsername->setStyleSheet(currentStyle);
    ui->editPhone->setStyleSheet(currentStyle);
    ui->editBirthday->setStyleSheet(currentStyle);
}

// 更新圆形头像显示
void UserCenter::updateAvatarDisplay(const QString& path)
{
    QPixmap src(path);
    if (src.isNull()) {
        src.load(":/DefaultAvatar.png");
    }
    if (src.isNull()) {
        qWarning() << "Default avatar :/DefaultAvatar.png not found!";
        return;
    }

    int size = qMin(ui->lblAvatarDisplay->width(), ui->lblAvatarDisplay->height());
    if (size <= 0) size = 100; // 如果控件还没显示，给一个默认尺寸

    QPixmap circularPixmap(size, size);
    circularPixmap.fill(Qt::transparent);

    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath path_clip;
    path_clip.addEllipse(0, 0, size, size);
    painter.setClipPath(path_clip);

    // 绘制缩放后的图片以填满圆形区域
    painter.drawPixmap(0, 0, src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    ui->lblAvatarDisplay->setPixmap(circularPixmap);
    ui->lblAvatarDisplay->setScaledContents(true);
}

// "修改密码" 按钮的槽函数
void UserCenter::onBtnPassClicked()
{
    // 这里应该弹出一个新的对话框来处理修改密码的复杂逻辑
    // 例如: ModifyPasswordDialog dialog(m_currentUserId, this);
    // if (dialog.exec() == QDialog::Accepted) {
    //     QMessageBox::information(this, "成功", "密码修改成功！");
    // }
    QMessageBox::information(this, "功能提示", "修改密码功能建议创建一个独立的对话框来实现。");
}

// "退出登录" 按钮的槽函数
void UserCenter::onBtnLogoutClicked()
{
    auto reply = QMessageBox::question(this, "确认退出", "您确定要退出当前账号吗？",
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        UserSession::instance().setUserId(-1); // 清除全局登录状态
        emit logoutSignal(); // 发送信号，让主窗口去处理页面切换
    }
}
