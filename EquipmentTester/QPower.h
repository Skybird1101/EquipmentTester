#ifndef QPOWER_H
#define QPOWER_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class QPower : public QWidget
{
    Q_OBJECT
        Q_PROPERTY(QColor color READ color WRITE setColor);
    Q_PROPERTY(bool state READ state WRITE setState);
public:
    QPower(QWidget* parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    QColor color() const
    {
        return pow_color;
    }

    bool state() const
    {
        return pow_state;
    }
    //~QPower();

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void paintBorder();
    void paintPowerOn();
    void paintPowerOff();

public slots:
    void setColor(QColor);
    void setState(bool);

private:
    QColor pow_color;
    bool pow_state;
};

#endif // QPOWER_H
