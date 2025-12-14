#ifndef DETAILDIALOG_H
#define DETAILDIALOG_H

#include <QDialog>
#include "PostData.h"

class DetailDialog : public QDialog {
    Q_OBJECT
public:
    // 构造函数接收 PostData 数据
    explicit DetailDialog(const PostData &data, QWidget *parent = nullptr);

private:
    void setupUi(const PostData &data);
};

#endif // DETAILDIALOG_H
