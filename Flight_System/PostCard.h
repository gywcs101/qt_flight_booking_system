#ifndef POSTCARD_H
#define POSTCARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

class PostCard : public QWidget
{
    Q_OBJECT
public:
    // 构造函数传入帖子数据
    explicit PostCard(int id, QString title, QString author, QString coverPath, QString avatarPath, QWidget *parent = nullptr);

    int getPostId() const { return m_id; }

signals:
    // 点击卡片时发送信号
    void cardClicked(int postId);

protected:
    // 鼠标点击事件
    void mousePressEvent(QMouseEvent *event) override;

private:
    int m_id;
    QLabel *m_coverLabel;  // 封面图
    QLabel *m_avatarLabel; // 头像
    QLabel *m_nameLabel;   // 作者名
    QLabel *m_titleLabel;  // 标题
};

#endif // POSTCARD_H
