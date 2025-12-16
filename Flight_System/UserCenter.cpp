#include "UserCenter.h"
#include "ui_UserCenter.h"
#include "ODBC.h"
#include "UserSession.h"
#include "ResetPasswordWidget.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent> // 【关键】用于捕获鼠标点击
#include <QFileDialog> // 【关键】用于打开文件选择框

UserCenter::UserCenter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserCenter)
{
    ui->setupUi(this);

    // 1. 初始化状态
    isEditing = false;
    setInputsEnabled(false);

    // 2. 连接按钮信号
    connect(ui->btnEdit, &QPushButton::clicked, this, &UserCenter::onBtnEditClicked);
    connect(ui->btnPass, &QPushButton::clicked, this, &UserCenter::onBtnPassClicked);
    connect(ui->btnLogout, &QPushButton::clicked, this, &UserCenter::onBtnLogoutClicked);

    // 3. 【核心新增】为头像 Label 安装事件过滤器，使其能响应点击
    ui->lblAvatarDisplay->installEventFilter(this);
    // 鼠标放上去变成“手型”，提示用户可以点击
    ui->lblAvatarDisplay->setCursor(Qt::PointingHandCursor);

    // 4. 显示默认头像
    updateAvatarDisplay(":/DefaultAvatar.png");
}

UserCenter::~UserCenter()
{
    delete ui;
}

// 【核心新增】重写事件过滤器：捕获 Label 点击事件
bool UserCenter::eventFilter(QObject *watched, QEvent *event)
{
    // 如果发生事件的控件是 头像显示框
    if (watched == ui->lblAvatarDisplay) {
        // 如果事件类型是 鼠标按下
        if (event->type() == QEvent::MouseButtonPress) {
            // 强转为鼠标事件，判断是否是左键
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                // 执行换头像逻辑
                handleAvatarChange();
                return true; // 表示事件已被处理，不再向下传递
            }
        }
    }
    // 其他控件或事件交给父类默认处理
    return QWidget::eventFilter(watched, event);
}

// 【核心新增】处理头像更换逻辑
void UserCenter::handleAvatarChange()
{
    // 1. 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("选择新头像"),
        "", // 默认打开路径
        tr("图片文件 (*.png *.jpg *.jpeg *.bmp)") // 文件过滤器
        );

    // 如果用户点了取消，fileName 为空
    if (fileName.isEmpty()) {
        return;
    }

    // 2. 检查登录状态
    if (m_currentUserId == -1) {
        QMessageBox::warning(this, "错误", "未登录，无法修改头像");
        return;
    }

    // 3. 更新数据库
    // 注意：这里存储的是本地绝对路径。如果要在多台电脑运行，建议把图片复制到程序目录后再存相对路径。
    QSqlQuery query;
    query.prepare("UPDATE users SET avatar_path = :path WHERE id = :id");
    query.bindValue(":path", fileName);
    query.bindValue(":id", m_currentUserId);

    if (query.exec()) {
        // 4. 更新界面显示
        updateAvatarDisplay(fileName);
        QMessageBox::information(this, "成功", "头像修改成功！");
    } else {
        QMessageBox::critical(this, "失败", "头像保存失败：" + query.lastError().text());
    }
}

void UserCenter::loadCurrentUserData()
{
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

        // 处理头像路径：如果是空或NULL，用默认头像
        if (avatarPath.isEmpty() || avatarPath == "NULL") {
            avatarPath = ":/DefaultAvatar.png";
        }
        updateAvatarDisplay(avatarPath);

        // 重置编辑状态
        setInputsEnabled(false);
        ui->btnEdit->setText("编辑资料");
        ui->btnEdit->setStyleSheet("");
        isEditing = false;
    } else {
        qWarning() << "SQL Error:" << query.lastError().text();
        QMessageBox::warning(this, "数据加载失败", "无法找到用户信息。");
    }
}

void UserCenter::onBtnEditClicked()
{
    if (!isEditing) {
        // 进入编辑模式
        setInputsEnabled(true);
        ui->btnEdit->setText("保存修改");
        ui->btnEdit->setStyleSheet("background-color: #67C23A; color: white; border-radius: 4px;");
        isEditing = true;
    } else {
        // 保存逻辑
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
    ui->editUsername->setReadOnly(!enable);
    ui->editPhone->setReadOnly(!enable);
    ui->editBirthday->setReadOnly(!enable);

    // 视觉反馈：编辑态显示边框，只读态透明
    const QString readOnlyStyle = "background-color: transparent; border: none;";
    const QString editableStyle = "background-color: white; border: 1px solid #DCDFE6; border-radius: 4px;";
    QString currentStyle = enable ? editableStyle : readOnlyStyle;

    ui->editUsername->setStyleSheet(currentStyle);
    ui->editPhone->setStyleSheet(currentStyle);
    ui->editBirthday->setStyleSheet(currentStyle);
}

void UserCenter::updateAvatarDisplay(const QString& path)
{
    QPixmap src(path);
    // 如果加载失败（路径错误或图片损坏），加载默认头像
    if (src.isNull()) {
        src.load(":/DefaultAvatar.png");
    }

    // 获取 Label 的大小来决定图片大小
    int size = qMin(ui->lblAvatarDisplay->width(), ui->lblAvatarDisplay->height());
    if (size <= 0) size = 100; // 防御性默认值

    // 绘制圆形头像
    QPixmap circularPixmap(size, size);
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath path_clip;
    path_clip.addEllipse(0, 0, size, size);
    painter.setClipPath(path_clip);

    // 保持比例缩放并居中绘制
    painter.drawPixmap(0, 0, src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    ui->lblAvatarDisplay->setPixmap(circularPixmap);
    ui->lblAvatarDisplay->setScaledContents(false); // 已经在代码里缩放好了，不需要 Label 再缩放
}

void UserCenter::onBtnPassClicked()
{
    if (m_currentUserId == -1) {
        QMessageBox::warning(this, "错误", "无法获取当前用户信息，请先登录。");
        return;
    }

    // 弹出修改密码窗口
    ResetPasswordWidget *passwordDialog = new ResetPasswordWidget(m_currentUserId, this);
    passwordDialog->setAttribute(Qt::WA_DeleteOnClose);
    passwordDialog->exec();
}

void UserCenter::onBtnLogoutClicked()
{
    auto reply = QMessageBox::question(this, "确认退出", "您确定要退出当前账号吗？",
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        UserSession::instance().setUserId(-1);
        emit logoutSignal();
    }
}
