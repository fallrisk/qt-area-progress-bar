
#include <algorithm>
#include <vector>
#include <limits>

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QtGlobal>
#include <QStylePainter>

#include "area_progress_bar.h"

AreaProgressBar::AreaProgressBar(QWidget *parent)
    : QWidget(parent)
{
    // Look at how to make the widget style aware.
    // http://doc.qt.io/qt-5/qstyle.html
    _value = 0;
    _minimum = 0;
    _maximum = 100;
    _seed = 1359;
    srand(_seed);
    _usedPixels = new std::vector<Pixel>;
    _unusedPixels = new std::vector<Pixel>;
}

AreaProgressBar::~AreaProgressBar()
{
    delete _usedPixels;
    delete _unusedPixels;
}

void AreaProgressBar::paintEvent(QPaintEvent *)
{
    float area = width() * height();
    float percentCovered = minMaxScale(_value, _minimum, _maximum, 0.0, 1.0);
    float numPixelsToDraw = percentCovered * area;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(QColor(128, 0, 128)));

    // If the number of pixels to draw is less than the current number
    // of used pixels then we need to remove pixels and put them
    // back into the unused list.
    if (numPixelsToDraw < _usedPixels->size()) {
        int diff = _usedPixels->size() - numPixelsToDraw;
        for (int p = 0; p < diff; ++p) {
            _unusedPixels->push_back(_usedPixels->back());
            _usedPixels->pop_back();
        }
    }
    // Need to increase the number of pixels to draw.
    else if (numPixelsToDraw > _usedPixels->size()) {
        int diff = numPixelsToDraw - _usedPixels->size();
        for (int p = 0; p < diff; ++p) {
            _usedPixels->push_back(_unusedPixels->back());
            _unusedPixels->pop_back();
        }
    } else {
        // Do nothing.
        return;
    }

    for (auto pixel : *_usedPixels) {
        painter.drawPoint(pixel.x, pixel.y);
    }

    // Now I draw the text.
    QStylePainter stylePainter(this);

    //stylePainter.drawItemText();
}

void AreaProgressBar::resizeEvent(QResizeEvent *event)
{
    // Make a new list of unused pixels and reset the used pixels
    // to 0.
    _usedPixels->clear();
    _unusedPixels->clear();
    int w = width();
    int h = height();
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            _unusedPixels->push_back(Pixel{x, y});
        }
    }
    // Randomize the pixels.
    std::random_device rng;
    std::mt19937 urng(rng());
    std::shuffle(_unusedPixels->begin(), _unusedPixels->end(), urng);

    QWidget::resizeEvent(event);
}

float AreaProgressBar::minMaxScale(float x, float xMin, float xMax, float yMin, float yMax)
{
    // Will this work with negative numbers, such as:
    // [-6, 20]
    // [-10, -1]
    float xStd = (x - xMin) / (xMax - xMin);
    float xScaled = xStd * (yMax - yMin) + yMin;
    return xScaled;
}

void AreaProgressBar::setValue(int value)
{
    if (_value == value or
            ((value > _maximum or value < _minimum) and
             (_maximum != 0 or _minimum != 0))) {
        return;
    }
    _value = value;
    emit valueChanged(value);
    repaint();
}

void AreaProgressBar::reset()
{
    if (_minimum == INT_MIN) {
        _value = INT_MIN;
    } else {
        _value = _minimum - 1;
    }
    repaint();
}

void AreaProgressBar::setMaximum(int maximum)
{
    setRange(qMin(_minimum, maximum), maximum);
}

void AreaProgressBar::setMinimum(int minimum)
{
    setRange(minimum, qMax(_maximum, minimum));
}

void AreaProgressBar::setRange(int minimum, int maximum)
{
    if (minimum != _minimum or maximum != _maximum) {
        _minimum = minimum;
        maximum = qMax(minimum, maximum);

        if (_value < _minimum - 1 or _value > maximum)
            reset();
        else
            update();
    }
}

int AreaProgressBar::maximum() const {
    return _maximum;
}

int AreaProgressBar::minimum() const {
    return _minimum;
}

int AreaProgressBar::value() const {
    return _value;
}

QString AreaProgressBar::text() const
{
    return QString::number(_value);
}
