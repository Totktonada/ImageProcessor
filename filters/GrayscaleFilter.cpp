#include <QImage>
#include <QtGlobal>
#include "GrayscaleFilter.hpp"
#include "../Algorithms.hpp"
#include "../Constants.hpp"

GrayscaleFilter::GrayscaleFilter()
{}

GrayscaleFilter::~GrayscaleFilter()
{}

QImage * GrayscaleFilter::filter(const QImage & source) const
{
    uint w = source.width();

    QImage * dest = new QImage(source);
    QRgb * to = reinterpret_cast<QRgb *>(dest->bits());

    const QRgb * rgb =
        reinterpret_cast<const QRgb *>(source.constBits());

    Algorithms::factorSum(rgb, area, to, w,
        STRENGTH_RED, STRENGTH_GREEN, STRENGTH_BLUE);

    return dest;
}

uint GrayscaleFilter::getWindowRadius() const
{
    return 1;
}
