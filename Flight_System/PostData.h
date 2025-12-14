#ifndef POSTDATA_H
#define POSTDATA_H

#include <QString>

// 这个结构体用于存放单条动态的所有信息
struct PostData {
    int id;                 // 数据库ID
    QString title;          // 标题
    QString content;        // 详细内容
    QString authorName;     // 作者名字
    QString imagePath;      // 封面/大图路径
    QString avatarPath;     // 头像路径
};

#endif // POSTDATA_H
