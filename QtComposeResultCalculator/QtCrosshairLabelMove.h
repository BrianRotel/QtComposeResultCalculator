#pragma once

#include <QLabel>

//可移动准星Label
class QtCrosshairLabelMove : public QLabel
{
	Q_OBJECT

public:
    QtCrosshairLabelMove(QWidget *parent = nullptr);
	~QtCrosshairLabelMove();

    void setCurrentPos(const QPoint& pos);
protected:
    void showEvent(QShowEvent* event) override;
public slots:
    void updatePosition(const QPoint& newPosition);
    void checkFocus();
private:
    QPoint m_QPoint;
    QTimer* m_Timer;

};
