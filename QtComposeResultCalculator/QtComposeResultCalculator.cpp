#include "stdafx.h"
#include "QtComposeResultCalculator.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPoint>
#include <QPixmap>
#define BASESIZEBIG 18
#define BASESIZELITT 14

QtComposeResultCalculator::QtComposeResultCalculator(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.pushButton->setToolTip("点击写入文件");

    ui.listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    currentItemText = "";
    //QObject::connect(ui.listWidget, &QWidget::customContextMenuRequested, this, &QtComposeResultCalculator::rightClickListWidget);
    QObject::connect(ui.listWidget, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(rightClickListWidget(const QPoint&)));

    QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(clickButton()));
    QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(clickButton2()));
    QObject::connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabCurrentChanged(int)));
    QObject::connect(ui.lineEdit, SIGNAL(textChanged(QString)), this, SLOT(lineEditChanged(QString)));


    QPixmap pixmap("fix_0000_图层 31.png");
    if (&pixmap)
    {
        for (size_t i = 0; i < 32; i++)
        {
            if (i)
            {
                list.append(pixmap.copy(i * BASESIZELITT + i, BASESIZEBIG + 1, BASESIZELITT, BASESIZELITT));
            }
            else
            {
                list.append(pixmap.copy(i * BASESIZELITT, BASESIZEBIG + 1, BASESIZELITT, BASESIZELITT));
            }
        }
    }

}

QtComposeResultCalculator::~QtComposeResultCalculator()
{}

void QtComposeResultCalculator::write()
{
    //写入
#if 0
    QJsonObject root;

    QJsonObject subobj1;
    subobj1.insert("弹力菇", QString("4"));

    QJsonObject subobj2;
    subobj2.insert("塑料", QString("1"));
    subobj2.insert("钢铁", QString("1"));
    subobj2.insert("动物鳞片", QString("1"));


    root.insert("塑料", subobj1);
    root.insert("塑钢", subobj2);


    QJsonDocument doc(root); //将Json对象，转换成Json文档
    QByteArray json = doc.toJson();
#endif // 0

    // 使用正则表达式 \s|[\r\n] 移出空格和换行
    QString str = "{\"伯希姆丁包裹\":{},\"伯希姆丁原件\":{},\"伯希姆丁电池\":{},\"先进的电子零件\":{},\"钢\":{},\"塑料\":{},\"塑钢\":{},\"强力胶\":{},\"智能传感器\":{},\"木炭\":{},\"木炭包裹\":{},\"润滑油\":{},\"玻璃\":{},\"玻璃包裹\":{},\"玻璃瓶\":{},\"生物燃料瓶\":{},\"电子零件\":{},\"电子零件包裹\":{},\"电脑模块\":{},\"电脑模块包裹\":{},\"究极计算机模块\":{},\"究极计算机模块包裹\":{},\"空包裹\":{},\"超级冷却液\":{},\"超级强化金属\":{},\"超级控制单元\":{},\"超级润滑剂\":{},\"超级肥料\":{},\"超级计算机模块\":{},\"钛矿石\":{},\"钛锭\":{},\"铁矿石\":{},\"铁锭\":{},\"铁锭包裹\":{},\"铜矿石\":{},\"铜矿石包裹\":{},\"铜锭\":{},\"铜锭包裹\":{},\"镜头\":{}}";
    QByteArray json(str.toStdString().c_str());




    // 写入文件
    QFile file("../my.json");
    file.open(QFile::WriteOnly);
    file.write(json);
    file.close();
}
void QtComposeResultCalculator::writeKV(QJsonDocument doc, QString str, QJsonObject val)
{
    //写入
    QJsonObject root = doc.object();
    //QJsonValue::Type t = root.value(str).type();

    //QVariantMap vmap = root.toVariantMap();

    //非 被定义-->如果值没有被设置过,才会继续执行下面的写入值
    if (!root.value(str).isUndefined())
    {
        if (!&val)//值为空,返回
        {
            return;
        }
        //qDebug() << val.size();
        //QJsonObject::iterator it = val.begin();
        //for (it;it<val.end();it++)
        //{
        //    qDebug() << " key:" << it.key(); //<< " value:" << it.value();
        //    QJsonValue::Type t = it.value().type();
        //    if (t)
        //    {
        //        qDebug() << " value:" << it.value().toDouble();
        //    }
        //
        //}
        root[str] = val;
    }
    else root.insert(str, val);

    QJsonDocument docN(root); //将Json对象，转换成Json文档
    QByteArray json = docN.toJson();

    // 写入文件
    QFile file("../my.json");
    file.open(QFile::WriteOnly);
    file.write(json);
    file.close();
}

QJsonDocument QtComposeResultCalculator::readA()
{

    //读取文件
    QFile file("../my.json");
    file.open(QFile::ReadOnly);
    QByteArray all = file.readAll();
    file.close();

    return QJsonDocument::fromJson(all);//转换成文档对象
}

void QtComposeResultCalculator::read()
{

    //读取文件
    QFile file("../my.json");
    file.open(QFile::ReadOnly);
    QByteArray all = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(all);//转换成文档对象
    if (doc.isObject())//可以不做格式判断，因为，解析的时候已经知道是什么数据了
    {
        QJsonObject obj = doc.object(); //得到Json对象
        QStringList keys = obj.keys(); //得到所有key
        for (int i = 0; i < keys.size(); i++)
        {
            QString key = keys.at(i);
            QJsonValue value = obj.value(key);
            if (value.isBool())
            {
                qDebug() << key << ":" << value.toBool();
            }
            else if (value.isString())
            {
                qDebug() << key << ":" << value.toString();
            }
            else if (value.isDouble())
            {
                qDebug() << key << ":" << value.toVariant().toInt();
            }
            else if (value.isObject())
            {
                qDebug() << key << ":";
                QJsonObject subObj = value.toObject();
                QStringList subKeys = subObj.keys();
                for (int k = 0; k < subKeys.size(); ++k)
                {
                    QJsonValue subValue = subObj.value(subKeys.at(k));
                    if (subValue.isString())
                    {
                        qDebug() << " " << subKeys.at(k) << ":" << subValue.toString();
                    }
                    else if (subValue.isArray())
                    {
                        qDebug() << " " << subKeys.at(k);
                        QJsonArray array = subValue.toArray();
                        for (int j = 0; j < array.size(); j++)
                        {
                            qDebug() << " " << array[j].toString();
                        }
                    }
                }
            }

        }
    }
}
QStringList QtComposeResultCalculator::readKeys()
{
    //读取文件
    QFile file("../my.json");
    file.open(QFile::ReadOnly);
    QByteArray all = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(all);//转换成文档对象
    if (doc.isObject())//可以不做格式判断，因为，解析的时候已经知道是什么数据了
    {
        QJsonObject obj = doc.object(); //得到Json对象
        return obj.keys(); //得到所有key
    }
}

void QtComposeResultCalculator::tabCurrentChanged(int cur)
{
    qDebug() << " " << cur;
    if (cur == 1)
    {
        QStringList listKey = readKeys();

        ui.listWidget->clear();//效率低,尤其是数据量很大的时候应该会出现卡段,或者电脑配置低的,一些好的优化方法:检测add按钮,设置标记,然后更新listWidget,初始化读入数据,不必每次切换tab页的时候更新数据
        for (size_t i = 0; i < listKey.length(); i++)
        {

            QVariantMap vMap = getValue(listKey.at(i));
            QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
            if (!vMap.isEmpty())
            {
                //QString sStr = " = ";
                QString sStr;
                for (auto it = vMap.begin(); it != vMap.end(); it++)
                {
                    sStr += it.key();
                    sStr += ":";
                    sStr += it.value().toString();
                    sStr += "   ";
                }
                item->setToolTip(sStr);
            }

            item->setText(listKey.at(i));
            ui.listWidget->addItem(item);
            ui.listWidget->setMouseTracking(true);
        }
    }
}

void QtComposeResultCalculator::clickButton()
{
    //write();
    //read();
    QMessageBox::warning(this, "", "写入成功");
}

//设置编辑框的智能提示
void QtComposeResultCalculator::lineEditChanged(QString str)
{
    QStringList listKey = readKeys();
    QStringList setList;

    for (size_t i = 0; i < listKey.length(); i++)
    {
        QString key = listKey.at(i);
        bool isContains = key.contains(str);
        if (isContains)
        {
            setList.append(key);
        }
    }
    QCompleter *completer = new QCompleter(setList, this);
    ui.lineEdit->setCompleter(completer);
}

//右键listwidget弹出菜单
void QtComposeResultCalculator::rightClickListWidget(const QPoint& pos)
{
    QMenu* pMenu = new QMenu(this);
    QAction* pDelete = new QAction("删除", pMenu);
    QAction* pShowLine = new QAction("显示路线", pMenu);
    QAction* pRename = new QAction("重命名", pMenu);
    QAction* pCollect = new QAction("收藏", pMenu);
    QAction* pInsert = new QAction("插入合成", pMenu);
    pCollect->setToolTip("置顶");
    connect(pDelete, SIGNAL(triggered(bool)), this, SLOT(delItem()));
    connect(pShowLine, SIGNAL(triggered(bool)), this, SLOT(showLine()));
    connect(pRename, SIGNAL(triggered(bool)), this, SLOT(reNameItem()));
    connect(pCollect, SIGNAL(triggered(bool)), this, SLOT(collectItem()));
    connect(pInsert, SIGNAL(triggered(bool)), this, SLOT(insertCompose()));

    //当ListWidgetitem不为空时显示菜单
    if (ui.listWidget->itemAt(/*mapFromGlobal(QCursor::pos())*/pos) != NULL)
    {
        pMenu->addAction(pDelete);
        pMenu->addAction(pShowLine);
        pMenu->addAction(pInsert);
        pMenu->addAction(pRename);
        pMenu->addAction(pCollect);
        currentItemText = ui.listWidget->itemAt(pos)->text();
    }

    //在鼠标位置显示菜单
    pMenu->exec(QCursor::pos());

    delete pMenu;
}

void QtComposeResultCalculator::clickButton2()
{
    //读取lineEdit
    QString text = ui.lineEdit->text();
    if (!text.isEmpty())
    {
        //写入到文件或者是
        QJsonDocument doc = readA();
        writeKV(doc,text);
        emit ui.tabWidget->currentChanged(1);
    }
}
void QtComposeResultCalculator::showLine()
{
    qDebug() << " showLine : " << currentItemText;
    //QAction* td = qobject_cast<QAction*>(QObject::sender());
    //QString str = td->text();

    ui.tabWidget->setCurrentIndex(0);
}

void QtComposeResultCalculator::insertCompose()
{
    if (currentItemText.isEmpty()) return;
    QStringList listKeys = readKeys();
    if (currentItemText.indexOf("="))
    {
        currentItemText = currentItemText.left(currentItemText.indexOf("=")-1);
    }
    listKeys.remove(listKeys.indexOf(currentItemText));
    QtSetCompose* setCompose = new QtSetCompose(this, listKeys);
    QObject::connect(setCompose, SIGNAL(sendKV(QMap<QString, int>)), this, SLOT(getKVFromChildWindow(QMap<QString, int>)));
    //setCompose->setKeys(readKeys());
    QVariantMap vMap = getValue(currentItemText);
    setCompose->setCurrentValues(vMap);

    setCompose->show();

}

void QtComposeResultCalculator::getKVFromChildWindow(QMap<QString, int> map)
{
    // QMap<QString, int>::iterator it = map.begin();
    QMap<QString, QVariant> vmap;
    QJsonObject ob;
    //qDebug() << "size:" << map.size();

    QVariant v = QVariant::fromValue(map);
    if (!&v) 
    {
        QMessageBox::warning(this, "", "发生了一些错误");
        return;
    }
    if (v.canConvert(QMetaType::QVariantMap))
    {
        vmap = v.toMap();
        v = QVariant::fromValue(vmap);
        if (v.canConvert(QMetaType::QJsonObject))
        {
            ob = v.toJsonObject();
        }
    }
    QJsonDocument doc = readA();
    writeKV(doc, currentItemText,ob);
    //QMap<QString, int> vmap = v.value<QMap<QString, int>>();
    //it = vmap.begin();

    //for (it; it != vmap.end(); it++)
    //{
    //    qDebug() << " key:" << it.key() << " value:" << it.value();
    //}
}

QVariantMap QtComposeResultCalculator::getValue(QString key)
{
    //读取文件
    QFile file("../my.json");
    file.open(QFile::ReadOnly);
    QByteArray all = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(all);//转换成文档对象
    if (doc.isObject())//可以不做格式判断，因为，解析的时候已经知道是什么数据了
    {
        QJsonObject obj = doc.object(); //得到Json对象
        QVariantMap vMap = obj.toVariantMap();
        QVariant value = vMap.value(key);
        if (value.canConvert(QMetaType::QVariantMap))
        {
            return value.toMap();
        }
    }
}
