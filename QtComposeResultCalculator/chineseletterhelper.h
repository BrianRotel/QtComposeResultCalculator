#ifndef CHINESELETTERHELPER
#define CHINESELETTERHELPER
#include <QObject>

class ChineseLetterHelper
{
    // 根据GB2312编码
public:
    static bool In(wchar_t start, wchar_t end, wchar_t code);

    static char Convert(int n);

    // 获取第一个汉字的首字母
    static QString GetFirstLetter(const QString& src);

    // 获取所有汉字的首字母
    static QString GetFirstLetters(const QString& src);

    // 获取一个汉字编码的汉语拼音
    static QString GetPinyin(int code);

    // 获取所有汉字的汉语拼音
    static QString GetPinyins(const QString& text);

    //下面两种方式需要加载拼音文件
    static QStringList readPYFile(const QString& file = QString());
    //汉字转拼音
    static QString Ch2PY(const QString& src, QStringList listPY);
    //汉字转全拼
    static QString Ch2APY(const QString& src);

};

#endif // CHINESELETTERHELPER_H