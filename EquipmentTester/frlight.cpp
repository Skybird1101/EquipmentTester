#include "frlight.h"
#include <qpainter.h>
#include <qbrush.h>

FrLight::FrLight(QWidget* parent) :
    QWidget(parent),
    _step(0),
    _timer_id(0),
    _state(kWorking),
    _default_color(127, 127, 127),
    _normal_color(32, 255, 32),
    _working_color(32, 255, 32),
    _warning_color(255, 255, 32),
    _malfunction_color(255, 32, 32) {
    update();
}

void FrLight::set_state(const FrLight::State& state) {
    _state = state;
    if (kWorking == _state) {
        _timer_id = startTimer(16);
    }
    else {
        killTimer(_timer_id);
        _timer_id = 0;
    }
    update();
}

void FrLight::set_default_color(const QColor& default_color) {
    _default_color = default_color;
    update();
}

void FrLight::set_normal_color(const QColor& normal_color) {
    _normal_color = normal_color;
    update();
}

void FrLight::set_working_color(const QColor& working_color) {
    _working_color = working_color;
    update();
}

void FrLight::set_warning_color(const QColor& warning_color) {
    _warning_color = warning_color;
    update();
}

void FrLight::set_malfunction_color(const QColor& malfunction_color) {
    _malfunction_color = malfunction_color;
    update();
}

void FrLight::timerEvent(QTimerEvent*) {
    update();

    --_step;
    if (_step == -00) {
        _step = 00;
    }
}

void FrLight::paintEvent(QPaintEvent*) {
    int side = qMin(width(), height());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 100.0, side / 100.0);


    QColor start_color, mid_color, end_color;

    switch (_state) {
    case kDefault:
        start_color = _default_color;
        mid_color = _default_color;
        end_color = _default_color;
        mid_color.setAlpha(192);
        end_color.setAlpha(0);
        break;
    case kNormal:
        start_color = _normal_color;
        mid_color = _normal_color;
        end_color = _normal_color;
        mid_color.setAlpha(192);
        end_color.setAlpha(0);
        break;
    case kWorking:
        if (0 == _timer_id) {
            _timer_id = startTimer(16);
        }
        start_color = _working_color;
        mid_color = _working_color;
        end_color = _working_color;
        start_color.setAlpha(qAbs(int(_step * 4.25)));
        mid_color.setAlpha(qAbs(int(_step * 3.2)));
        end_color.setAlpha(0);
        break;
    case kWarning:
        start_color = _warning_color;
        mid_color = _warning_color;
        end_color = _warning_color;
        mid_color.setAlpha(192);
        end_color.setAlpha(0);
        break;
    case kMalfunction:
        start_color = _malfunction_color;
        mid_color = _malfunction_color;
        end_color = _malfunction_color;
        mid_color.setAlpha(192);
        end_color.setAlpha(0);
        break;
    default:
        start_color = _default_color;
        mid_color = _default_color;
        end_color = _default_color;
        mid_color.setAlpha(192);
        end_color.setAlpha(0);
        break;
    }

    QRadialGradient radial(0, 0, 20, 0, 0);    //设置圆的原点和焦点在中心,半径50
    radial.setSpread(QGradient::PadSpread);
    radial.setColorAt(0, start_color);
    radial.setColorAt(0.9, mid_color);
    radial.setColorAt(1, end_color);

    painter.setPen(Qt::transparent);
    painter.setBrush(radial);
    painter.drawEllipse(-50, -50, 100, 100);
}
