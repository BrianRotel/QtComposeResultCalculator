#include <QLabel>

//可移动准星Label
class CrosshairLabelMove : public QLabel
{
    Q_OBJECT

public:
    explicit CrosshairLabelMove() {
        this->setAttribute(Qt::WA_TransparentForMouseEvents);

        QPixmap pixmap(":/Crosshair.png"); // 替换为你的图片路径

        setPixmap(pixmap);
        setMask(pixmap.mask()); // 使图像的透明部分被视为透明
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint); // 使窗口无边框并始终保持在顶部
        setAttribute(Qt::WA_TranslucentBackground); // 设置背景为透明


        m_Timer = new QTimer(this);
        connect(m_Timer, &QTimer::timeout, this, &CrosshairLabelMove::checkFocus);
        m_Timer->start(100); // 检查频率，可以根据实际需要进行调整
    }

    void setCurrentPos(const QPoint& pos) {
        m_QPoint = pos;
    }
protected:
    void showEvent(QShowEvent* event) override {

        int halfWidth = width() / 2;
        int halfHeight = height() / 2;
        move(m_QPoint.x() - halfWidth, m_QPoint.y() - halfHeight);
        QLabel::showEvent(event);
    }


public slots:
    void updatePosition(const QPoint& newPosition) {
        int halfWidth = width() / 2;
        int halfHeight = height() / 2;
        move(newPosition.x() - halfWidth, newPosition.y() - halfHeight);
    }
    void checkFocus() {
        //这段代码首先检查当前窗口是否是活动窗口，并且是否可见。如果窗口不是活动的，或者不可见，那么就恢复鼠标，取消鼠标事件的透明处理，并隐藏准星。
        if (QApplication::activeWindow() != this || !this->isVisible()) {
            if (QApplication::overrideCursor() != nullptr) {
                QApplication::restoreOverrideCursor(); // 恢复鼠标
                this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
                if (this->isVisible()) {
                    this->hide();
                }
            }
        }
    }

private:
    QPoint m_QPoint;
    QTimer* m_Timer;
};


//弹出准星的可点击Label
class CrosshairLabelClick : public QLabel {
    Q_OBJECT

public:
    CrosshairLabelClick(QWidget* parent = nullptr) : QLabel(parent) {
        QPixmap pixmap(":/Crosshair.png"); // 替换为你的图片路径
        setPixmap(pixmap);

        m_CrosshairLabelMove = new CrosshairLabelMove();

        QObject::connect(this, &CrosshairLabelClick::mousePress, [&]() {
            m_CrosshairLabelMove->show();
            });

        QObject::connect(this, &CrosshairLabelClick::mouseMove, m_CrosshairLabelMove, &CrosshairLabelMove::updatePosition);
        QObject::connect(this, &CrosshairLabelClick::mouseRelease, [&]() {
            m_CrosshairLabelMove->hide();
            });
    }
    void setCurrentPos(const QPoint& pos) {
        m_CrosshairLabelMove->setCurrentPos(pos);
    }
signals:
    void windowInfo(const QString& title, const QString& className, HWND hwnd);
protected:
    void mousePressEvent(QMouseEvent* event) override {
        QLabel::mousePressEvent(event);
        if (event->button() == Qt::LeftButton) {
            emit mousePress(event->globalPos());
            QApplication::setOverrideCursor(QCursor(Qt::BlankCursor)); // 隐藏鼠标
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        QLabel::mouseMoveEvent(event);
        emit mouseMove(event->globalPos());
        HWND hwnd = ::WindowFromPoint(POINT{ event->globalPos().x(), event->globalPos().y() });
        char title[256] = { 0 };
        ::GetWindowTextA(hwnd, title, sizeof(title));
        QString titleStr = QString::fromLocal8Bit(title);

        char className[256] = { 0 };
        ::GetClassNameA(hwnd, className, sizeof(className));
        QString classNameStr = QString::fromLocal8Bit(className);
        emit windowInfo(titleStr, classNameStr, hwnd);
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        QLabel::mouseReleaseEvent(event);
        emit mouseRelease();

        QApplication::restoreOverrideCursor(); // 恢复鼠标
        this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }

    void hideEvent(QHideEvent* event) {
        QWidget::hideEvent(event);
        emit mouseRelease();

        QApplication::restoreOverrideCursor(); // 恢复鼠标
        this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }

signals:
    void mousePress(QPoint startPosition);
    void mouseMove(QPoint currentPosition);
    void mouseRelease();

private:
    CrosshairLabelMove* m_CrosshairLabelMove;
};

