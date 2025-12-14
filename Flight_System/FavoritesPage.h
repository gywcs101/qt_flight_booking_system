#ifndef FAVORITESPAGE_H
#define FAVORITESPAGE_H

#include <QWidget>
class QVBoxLayout; // 前置声明

namespace Ui { class FavoritesPage; }

class FavoritesPage : public QWidget
{
    Q_OBJECT

public:
    explicit FavoritesPage(QWidget *parent = nullptr);
    ~FavoritesPage();


    void loadFavoriteFlights();

private:
    Ui::FavoritesPage *ui;

    // 【新增】显示空状态的辅助函数
    void showEmptyState(QVBoxLayout *layout, QString text);
};

#endif // FAVORITESPAGE_H
