#include <QImage>
#include <QtGlobal>
#include <math.h>
#include "WaveFilter.hpp"
#include "../Utils.hpp"
#include "../Algorithms.hpp"

WaveFilter::WaveFilter(double aRadius, double aPeriod,
    Orientation aOrientation)
    : radius(aRadius),
    period(aPeriod),
    orientation(aOrientation)
{}

WaveFilter::~WaveFilter()
{}

QImage * WaveFilter::filter(const QImage & source, QRect area) const
{
    uint w = source.width();
    uint h = source.height();

    QImage * dest = new QImage(source);
    QRgb * to = reinterpret_cast<QRgb *>(dest->bits());

    const QRgb * rgb =
        reinterpret_cast<const QRgb *>(source.constBits());

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            to[y * w + x] = getPixel(rgb, area, x, y, w, h);
        }
    }

    return dest;
}

uint WaveFilter::getWindowRadius() const
{
    return (uint) radius + 1;
}

QRgb WaveFilter::getPixel(const QRgb * rgb, QRect area,
    uint x, uint y, uint w, uint h) const
{
    /* New coordinates */
    double ncX = x - area.x() + 0.5 - area.width() / 2.0;
    double ncY = y - area.y() + 0.5 - area.height() / 2.0;

    double fromX;
    double fromY;

    if (orientation == Horizontal)
    {
        fromX = ncX;
        fromY = ncY + radius * Utils::getSin(2 * M_PI * ncX / period);
    }
    else /* (orientation == Vertical) */
    {
        fromX = ncX + radius * Utils::getSin(2 * M_PI * ncY / period);
        fromY = ncY;
    }

    /* To old coordinates */
    fromX = fromX + area.x() - 0.5 + area.width() / 2.0;
    fromY = fromY + area.y() - 0.5 + area.height() / 2.0;

    if (fromX < 0.0 || fromX > (w - 1.0) ||
        fromY < 0.0 || fromY > (h - 1.0))
    {
        return qRgb(0, 0, 0);
    }

    return Algorithms::bilinearInterpolatedValue(rgb, fromX, fromY, w, h);
}
