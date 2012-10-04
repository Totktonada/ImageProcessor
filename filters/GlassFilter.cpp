#include <QImage>
#include <QtGlobal>
#include "GlassFilter.hpp"
#include "../Algorithms.hpp"

GlassFilter::GlassFilter(double aRadius)
    : radius(aRadius)
{}

GlassFilter::~GlassFilter()
{}

QImage * GlassFilter::filter(const QImage & source, QRect area) const
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
            to[y * w + x] = getPixel(rgb, x, y, w, h);
        }
    }

    return dest;
}

uint GlassFilter::getWindowRadius() const
{
    return (uint) radius + 1;
}

QRgb GlassFilter::getPixel(const QRgb * rgb,
    uint x, uint y, uint w, uint h) const
{
    double fromX = x + radius * (2.0 * qrand() / RAND_MAX - 1);
    double fromY = y + radius * (2.0 * qrand() / RAND_MAX - 1);

    return Algorithms::bilinearInterpolatedValue(rgb, fromX, fromY, w, h);
}
