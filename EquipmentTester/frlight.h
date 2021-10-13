#ifndef FRLIGHT_H
#define FRLIGHT_H

#include <qwidget.h>

class FrLight : public QWidget {
    Q_OBJECT

public:
    enum State {
        kDefault,
        kNormal,
        kWorking,
        kWarning,
        kMalfunction
    };

public:
    explicit FrLight(QWidget* parent = nullptr);

    void set_state(const State& state);

    void set_default_color(const QColor& default_color);
    void set_normal_color(const QColor& normal_color);
    void set_working_color(const QColor& working_color);
    void set_warning_color(const QColor& warning_color);
    void set_malfunction_color(const QColor& malfunction_color);

protected:
    void timerEvent(QTimerEvent*);
    void paintEvent(QPaintEvent*);

private:
    int _step;
    int _timer_id;

    State _state;
	QColor _default_color;
    QColor _normal_color;
    QColor _working_color;
    QColor _warning_color;
    QColor _malfunction_color;

};

#endif // FRLIGHT_