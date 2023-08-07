#include <QLabel>

//���ƶ�׼��Label
class CrosshairLabelMove : public QLabel
{
    Q_OBJECT

public:
    explicit CrosshairLabelMove() {
        this->setAttribute(Qt::WA_TransparentForMouseEvents);

        QPixmap pixmap(":/Crosshair.png"); // �滻Ϊ���ͼƬ·��

        setPixmap(pixmap);
        setMask(pixmap.mask()); // ʹͼ���͸�����ֱ���Ϊ͸��
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint); // ʹ�����ޱ߿�ʼ�ձ����ڶ���
        setAttribute(Qt::WA_TranslucentBackground); // ���ñ���Ϊ͸��


        m_Timer = new QTimer(this);
        connect(m_Timer, &QTimer::timeout, this, &CrosshairLabelMove::checkFocus);
        m_Timer->start(100); // ���Ƶ�ʣ����Ը���ʵ����Ҫ���е���
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
        //��δ������ȼ�鵱ǰ�����Ƿ��ǻ���ڣ������Ƿ�ɼ���������ڲ��ǻ�ģ����߲��ɼ�����ô�ͻָ���꣬ȡ������¼���͸������������׼�ǡ�
        if (QApplication::activeWindow() != this || !this->isVisible()) {
            if (QApplication::overrideCursor() != nullptr) {
                QApplication::restoreOverrideCursor(); // �ָ����
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


//����׼�ǵĿɵ��Label
class CrosshairLabelClick : public QLabel {
    Q_OBJECT

public:
    CrosshairLabelClick(QWidget* parent = nullptr) : QLabel(parent) {
        QPixmap pixmap(":/Crosshair.png"); // �滻Ϊ���ͼƬ·��
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
            QApplication::setOverrideCursor(QCursor(Qt::BlankCursor)); // �������
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

        QApplication::restoreOverrideCursor(); // �ָ����
        this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }

    void hideEvent(QHideEvent* event) {
        QWidget::hideEvent(event);
        emit mouseRelease();

        QApplication::restoreOverrideCursor(); // �ָ����
        this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }

signals:
    void mousePress(QPoint startPosition);
    void mouseMove(QPoint currentPosition);
    void mouseRelease();

private:
    CrosshairLabelMove* m_CrosshairLabelMove;
};

