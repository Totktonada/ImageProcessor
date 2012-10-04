#ifndef WAVE_FILTER_HPP_SENTRY
#define WAVE_FILTER_HPP_SENTRY

#include <QtGlobal>
#include <QColor>
#include "Filter.hpp"
class QImage;

class WaveFilter : public Filter
{
public:
    enum Orientation
    {
        Horizontal,
        Vertical
    };

private:
    double radius; /* amplitude */
    double period; /* radians */
    Orientation orientation;

public:
    WaveFilter(double aRadius, double aPeriod, Orientation aOrientation);
    ~WaveFilter();

    QImage * filter(const QImage & source, QRect area) const;

    uint getWindowRadius() const;

private:
    QRgb getPixel(const QRgb * rgb, QRect area,
        uint x, uint y, uint w, uint h) const;
};

#endif /* WAVE_FILTER_HPP_SENTRY */
