#include <QImage>
#include <QtGlobal>
#include "ResizeFilter.hpp"
#include "../Algorithms.hpp"

ResizeFilter::ResizeFilter(double aRatio)
    : ratio(aRatio)
{}

ResizeFilter::~ResizeFilter()
{}

QImage * ResizeFilter::filter(const QImage & source) const
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

uint ResizeFilter::getWindowRadius() const
{
    return 1;
}

QRgb ResizeFilter::getPixel(const QRgb * rgb,
    uint x, uint y, uint w, uint h) const
{
    /* All equations in new coordinates.
     * ----
     * x == fromX * ratio
     * y == fromY * ratio */

    /* New coordinates */
    double ncX = x - area.x() + 0.5 - area.width() / 2.0;
    double ncY = y - area.y() + 0.5 - area.height() / 2.0;

    double fromX = ncX / ratio;
    double fromY = ncY / ratio;

    /* To old coordinates */
    fromX = fromX + area.x() - 0.5 + area.width() / 2.0;
    fromY = fromY + area.y() - 0.5 + area.height() / 2.0;

    if (fromX < area.x() || fromX > (area.x() + area.width() - 1.0) ||
        fromY < area.y() || fromY > (area.y() + area.height() - 1.0))
    {
        return qRgb(0, 0, 0);
    }

    return Algorithms::bilinearInterpolatedValue(rgb, fromX, fromY, w, h);
}
