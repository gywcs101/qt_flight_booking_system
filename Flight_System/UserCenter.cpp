#include "UserCenter.h"
#include "ui_UserCenter.h"
#include "ODBC.h"
#include "UserSession.h"
#include "ResetPasswordWidget.h" // [核心修改] 包含 ResetPasswordWidget 的头文件
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

    // ... (构造函数的其他代码保持不变) ...
    isEditing = false;
    setInputsEnabled(false);
    connect(ui->btnEdit, &QPushButton::clicked, this, &UserCenter::onBtnEditClicked);
    connect(ui->btnPass, &QPushButton::clicked, this, &UserCenter::onBtnPassClicked);
    connect(ui->btnLogout, &QPushButton::clicked, this, &UserCenter::onBtnLogoutClicked);
    updateAvatarDisplay(":/DefaultAvatar.png");
}

UserCenter::~UserCenter()
{
    delete ui;
}

void UserCenter::loadCurrentUserData()
{
    // ... (此函数代码保持不变) ...
    m_currentUserId = UserSession::instance().getUserId();
    if (m_currentUserId == -1) {
        QMessageBox::critical(this, "系统错误", "无法获取当前用户信息，请重新登录！");
        ui->editUsername->clear();
        ui->editPhone->clear();
        ui->lblBalanceValue->setText("0.00");
        return;
    }
    QString sql = QString("SELECT username, phone, birthday, balance, avatar_path FROM users WHERE id = %1").arg(m_currentUserId);
    QSqlQuery query = ODBC::query(sql);
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
        setInputsEnabled(false);
        ui->btnEdit->setText("编辑资料");
        ui->btnEdit->setStyleSheet("");
        isEditing = false;
    } else {
        qWarning() << "SQL Error:" << query.lastError().text() << " while fetching user ID:" << m_currentUserId;
        QMessageBox::warning(this, "数据加载失败", "无法从数据库中找到您的用户信息。");
    }
}

void UserCenter::onBtnEditClicked()
{
    // ... (此函数代码保持不变) ...
    if (!isEditing) {
        setInputsEnabled(true);
        ui->btnEdit->setText("保存修改");
        ui->btnEdit->setStyleSheet("background-color: #67C23A; color: white; border-radius: 4px;");
        isEditing = true;
    } else {
        QString newName = ui->editUsername->text().trimmed();
        QString newPhone = ui->editPhone->text().trimmed();
        QDate newBirth = ui->editBirthday->date();
        if (newName.isEmpty()) {
            QMessageBox::warning(this, "输入无效", "用户名不能为空！");
            return;
        }
        QSqlQuery query;
        query.prepare("UPDATE users SET username = :name, phone = :phone, birthday = :bday WHERE id = :id");
        query.bindValue(":name", newName);
        query.bindValue(":phone", newPhone);
        query.bindValue(":bday", newBirth.toString(Qt::ISODate));
        query.bindValue(":id", m_currentUserId);
        if (query.exec()) {
            QMessageBox::information(this, "操作成功", "您的个人资料已更新！");
            setInputsEnabled(false);
            ui->btnEdit->setText("编辑资料");
            ui->btnEdit->setStyleSheet("");
            isEditing = false;
        } else {
            QMessageBox::critical(this, "保存失败", "数据库更新错误：" + query.lastError().text());
        }
    }
}

void UserCenter::setInputsEnabled(bool enable)
{
    // ... (此函数代码保持不变) ...
    ui->editUsername->setReadOnly(!enable);
    ui->editPhone->setReadOnly(!enable);
    ui->editBirthday->setReadOnly(!enable);
    const QString readOnlyStyle = "background-color: transparent; border: none;";
    const QString editableStyle = "background-color: white; border: 1px solid #DCDFE6; border-radius: 4px;";
    QString currentStyle = enable ? editableStyle : readOnlyStyle;
    ui->editUsername->setStyleSheet(currentStyle);
    ui->editPhone->setStyleSheet(currentStyle);
    ui->editBirthday->setStyleSheet(currentStyle);
}

void UserCenter::updateAvatarDisplay(const QString& path)
{
    // ... (此函数代码保持不变) ...
    QPixmap src(path);
    if (src.isNull()) src.load(":/DefaultAvatar.png");
    if (src.isNull()) {
        qWarning() << "Default avatar :/DefaultAvatar.png not found!";
        return;
    }
    int size = qMin(ui->lblAvatarDisplay->width(), ui->lblAvatarDisplay->height());
    if (size <= 0) size = 100;
    QPixmap circularPixmap(size, size);
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path_clip;
    path_clip.addEllipse(0, 0, size, size);
    painter.setClipPath(path_clip);
    painter.drawPixmap(0, 0, src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    ui->lblAvatarDisplay->setPixmap(circularPixmap);
    ui->lblAvatarDisplay->setScaledContents(true);
}

// "修改密码" 按钮的槽函数
void UserCenter::onBtnPassClicked()
{
    // [核心修改] 将原来的提示框替换为创建和显示新窗口的逻辑

    // 1. 检查当前用户ID是否有效，防止未登录时操作
    if (m_currentUserId == -1) {
        QMessageBox::warning(this, "错误", "无法获取当前用户信息，请先登录。");
        return;
    }

    // 2. 创建 ResetPasswordWidget 对话框的实例
    //    将当前用户ID (m_currentUserId) 和父窗口指针 (this) 传递给它
    ResetPasswordWidget *passwordDialog = new ResetPasswordWidget(m_currentUserId, this);

    // 3. 设置对话框在关闭时自动删除，避免内存泄漏
    passwordDialog->setAttribute(Qt::WA_DeleteOnClose);

    // 4. 以模态方式显示对话框。程序会在这里暂停，直到对话框关闭
    passwordDialog->exec();
}

// "退出登录" 按钮的槽函数
void UserCenter::onBtnLogoutClicked()
{
    // ... (此函数代码保持不变) ...
    auto reply = QMessageBox::question(this, "确认退出", "您确定要退出当前账号吗？",
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        UserSession::instance().setUserId(-1);
        emit logoutSignal();
    }
}
