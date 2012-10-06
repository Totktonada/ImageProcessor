#include <QImage>
#include <QVector>
#include <QtGlobal>
#include "MaximumFilter.hpp"
#include "../Utils.hpp"
#include "../Algorithms.hpp"
#include "../Constants.hpp"

MaximumFilter::MaximumFilter(uint aRadius)
    : radius(aRadius)
{}

MaximumFilter::~MaximumFilter()
{}

/* TODO: RGB->YUV, filter, YUV->RGB. */
QImage * MaximumFilter::filter(const QImage & source) const
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
            uint maxR = 0;
            uint maxG = 0;
            uint maxB = 0;

            for (int wy = -radius; wy <= (int) radius; ++wy)
            {
                for (int wx = -radius; wx <= (int) radius; ++wx)
                {
                    QRgb pixel = rgb[Utils::coordinate(x + wx, y + wy, w, h)];

                    maxR = Utils::max2<uint>(maxR, qRed(pixel));
                    maxG = Utils::max2<uint>(maxG, qGreen(pixel));
                    maxB = Utils::max2<uint>(maxB, qBlue(pixel));
                }
            }

            to[y * w + x] = qRgb(maxR, maxG, maxB);
        }
    }

    return dest;
}

uint MaximumFilter::getWindowRadius() const
{
    return radius;
}
