#include <QImage>
#include <QtGlobal>
#include "AutocontrastFilter.hpp"
#include "../Utils.hpp"
#include "../Algorithms.hpp"
#include "../Constants.hpp"

#define ZERO_AROUND 0.00000001

AutocontrastFilter::AutocontrastFilter(double aSkip)
    : skipL(aSkip),
    skipR(aSkip)
{}

AutocontrastFilter::AutocontrastFilter(double aSkipL, double aSkipR)
    : skipL(aSkipL),
    skipR(aSkipR)
{}

AutocontrastFilter::~AutocontrastFilter()
{}

QImage * AutocontrastFilter::filter(const QImage & source,
    QRect area) const
{
    uint w = source.width();

    QImage * dest = new QImage(source);
    QRgb * to = reinterpret_cast<QRgb *>(dest->bits());

    const QRgb * rgb =
        reinterpret_cast<const QRgb *>(source.constBits());

    int minR = CMIN, maxR = CMAX;
    int minG = CMIN, maxG = CMAX;
    int minB = CMIN, maxB = CMAX;

    if (skipL < ZERO_AROUND && skipR < ZERO_AROUND)
    {
        Algorithms::getMinMaxSimple(rgb, area, w,
            &minR, &minG, &minB,
            &maxR, &maxG, &maxB);
    }
    else
    {
        Algorithms::getMinMaxRobust(rgb, area, w,
            skipL, skipR,
            &minR, &minG, &minB,
            &maxR, &maxG, &maxB);
    }

    int min = Utils::min3<int>(minR, minG, minB);
    int max = Utils::max3<int>(maxR, maxG, maxB);

    double factor = (max - min <= 0) ? 0 : FCMAX / (max - min);

    Algorithms::subFactorMap(rgb, area, to, w, min, factor);

    return dest;
}

uint AutocontrastFilter::getWindowRadius() const
{
    return 1;
}
