#pragma once

#include <QLabel>
#include "QtCrosshairLabelMove.h"

//弹出准星的可点击Label
class QtCrosshairLabelClick : public QLabel
{
	Q_OBJECT

public:
    QtCrosshairLabelClick(QWidget *parent = nullptr);
	~QtCrosshairLabelClick();

    void setCurrentPos(const QPoint& pos);
signals:
    void windowInfo(const QString& title, const QString& className, HWND hwnd);
protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void hideEvent(QHideEvent* event);

signals:
    void mousePress(QPoint startPosition);
    void mouseMove(QPoint currentPosition);
    void mouseRelease();

private:
    QtCrosshairLabelMove* m_CrosshairLabelMove;

};