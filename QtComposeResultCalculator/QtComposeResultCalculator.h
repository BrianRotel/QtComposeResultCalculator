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

    void insertCompose();

    void getKVFromChildWindow(QMap<QString,int>);

private:
    QString currentItemText;
    QList<QPixmap>list;

private:
    Ui::QtComposeResultCalculatorClass ui;
};
