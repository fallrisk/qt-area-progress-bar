#ifndef AREA_PROGRESS_BAR_H
#define AREA_PROGRESS_BAR_H

#include <vector>

#include <QWidget>

struct Pixel {
    int x;
    int y;
};

struct PixelHasher {
    std::size_t operator()(const Pixel & pixel) const;
};

class AreaProgressBar : public QWidget
{
    Q_OBJECT

public:
    AreaProgressBar(QWidget *parent = 0);
    ~AreaProgressBar();

    int maximum() const;
    int minimum() const;

    int value() const;

    virtual QString text() const;

public slots:
    void reset();
    void setMaximum(int maximum);
    void setMinimum(int minimum);
    void setRange(int minimum, int maximum);
    void setValue(int value);

signals:
    void valueChanged(int value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    float minMaxScale(float x, float xMin, float xMax, float yMin, float yMax);

private:
    int _value;
    int _minimum;
    int _maximum;
    uint _seed;
    std::vector<Pixel> * _usedPixels;
    std::vector<Pixel> * _unusedPixels;
};

#endif // AREA_PROGRESS_BAR_H
