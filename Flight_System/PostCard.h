#ifndef POSTCARD_H
#define POSTCARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include "PostData.h"  // 必须包含这个头文件

class PostCard : public QWidget
{
    Q_OBJECT
public:
    // 修改构造函数，直接接收 PostData 数据包
    explicit PostCard(const PostData &data, QWidget *parent = nullptr);

signals:
    // 修改信号，点击时把整个数据包发出去，而不是只发个ID
    void cardClicked(const PostData &data);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    PostData m_data; // 存储数据，以便点击时发送

    QLabel *m_coverLabel;
    QLabel *m_avatarLabel;
    QLabel *m_nameLabel;
    QLabel *m_titleLabel;
};

#endif // POSTCARD_H
