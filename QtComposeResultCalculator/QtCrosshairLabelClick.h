#pragma once

#include <QLabel>
#include "QtCrosshairLabelMove.h"

struct WindowInfo
{
    HWND windowHwnd;
    QString windowSHwnd;
    QString windowTitle;
    QString windowClass;
    QString windowStyle;
    QString windowRect;
};
//弹出准星的可点击Label
class QtCrosshairLabelClick : public QLabel
{
	Q_OBJECT

public:
    QtCrosshairLabelClick(QWidget *parent = nullptr);
	~QtCrosshairLabelClick();

    void setCurrentPos(const QPoint& pos);
signals:
    void windowInfo(const WindowInfo& title);
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
    WindowInfo info;
};