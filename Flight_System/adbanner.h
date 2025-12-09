#ifndef ADBANNER_H
#define ADBANNER_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QList>

class AdBanner : public QWidget
{
    Q_OBJECT
public:
    explicit AdBanner(QWidget *parent = nullptr);
    void addImage(const QString &imagePath); // 添加图片路径
    void setImagePaths(const QStringList &paths); // 【新增】一次性添加多张图片
    void start(int interval = 3000);         // 开始轮播
    void stop();                             // 停止轮播

protected:
    // 自动感知页面切换：当page被隐藏时停止定时器，节省资源
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private slots:
    void showNextImage();
    void onDotClicked(int index);

private:
    void updateImage();

    QLabel *m_imageLabel;        // 图片显示容器
    QWidget *m_indicatorContainer; // 底部小圆点容器
    QHBoxLayout *m_indicatorLayout;
    QButtonGroup *m_btnGroup;

    QList<QString> m_imagePaths; // 图片路径列表
    int m_currentIndex;          // 当前第几张
    QTimer *m_timer;             // 定时器
};

#endif // ADBANNER_H
