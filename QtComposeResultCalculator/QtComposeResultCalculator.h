#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtComposeResultCalculator.h"
#include "QtSetCompose.h"

class QtComposeResultCalculator : public QWidget
{
    Q_OBJECT

public:
    QtComposeResultCalculator(QWidget *parent = nullptr);
    ~QtComposeResultCalculator();

    void write();

    void writeKV(QJsonDocument,QString, QJsonObject = QJsonObject());

    void read();
    QJsonDocument readA();

    QStringList readKeys();

    QVariantMap getValue(QString key);

public slots:
    void clickButton();
    void tabCurrentChanged(int cur);
    void lineEditChanged(QString);
    void rightClickListWidget(const QPoint& pos);

    void clickButton2();



    void showLine();

    void listItemPressed(QListWidgetItem* item);

    void insertCompose(QListWidgetItem* item = nullptr);

    void getKVFromChildWindow(QMap<QString,int>);

private:
    QString currentItemText;
    QString fileName;
    QMap<QString,QPixmap>pixMap;
    QVariantMap gVMap;
    QVariantMap readAllForVMap();

    QMultiMap<QString, int> getLine(QString str, int num = 1);

    QMap<QString, int> getLine(QMap<QString, int> map);

    QStringList getLine(QString str, int num, QString st);

private:
    Ui::QtComposeResultCalculatorClass ui;
};
