#ifndef BANNERWIDGET_H
#define BANNERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>
#include <QList>
#include <QPushButton>

class BannerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BannerWidget(QWidget *parent = nullptr);

    // 设置图片路径列表
    void setImagePaths(const QStringList &paths);

private slots:
    void onTimeout();           // 定时器槽函数
    void onDotClicked();        // 点击小圆点切换

private:
    void updateImage();         // 刷新界面显示
    void initUI();              // 初始化界面

    QLabel *m_imageLabel;       // 用于显示图片
    QTimer *m_timer;            // 定时器
    QStringList m_pathList;     // 图片路径列表
    int m_currentIndex;         // 当前显示第几张

    QList<QPushButton*> m_dots; // 底部的小圆点按钮列表
    QWidget *m_dotContainer;    // 包裹小圆点的容器
};

#endif // BANNERWIDGET_H
