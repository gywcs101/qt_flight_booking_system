#include <QWidget>

namespace Ui { class DiscoveryPage; }

class DiscoveryPage : public QWidget
{
    Q_OBJECT

public:
    explicit DiscoveryPage(QWidget *parent = nullptr);
    ~DiscoveryPage();

    void refreshPosts(); // 公开一个函数用于刷新数据

private:
    Ui::DiscoveryPage *ui;
    void loadDataFromDB(); // 内部加载逻辑
};
