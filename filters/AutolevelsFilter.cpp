#include <QImage>
#include <QtGlobal>
#include "AutolevelsFilter.hpp"
#include "../Utils.hpp"
#include "../Algorithms.hpp"
#include "../Constants.hpp"

#define ZERO_AROUND 0.00000001

AutolevelsFilter::AutolevelsFilter(double aSkip)
    : skipL(aSkip),
    skipR(aSkip)
{
}

AutolevelsFilter::AutolevelsFilter(double aSkipL, double aSkipR)
    : skipL(aSkipL),
    skipR(aSkipR)
{
}

AutolevelsFilter::~AutolevelsFilter()
{}

QImage * AutolevelsFilter::filter(const QImage & source) const
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

    double factorR = (maxR - minR <= 0) ? 0 : FCMAX / (maxR - minR);
    double factorG = (maxG - minG <= 0) ? 0 : FCMAX / (maxG - minG);
    double factorB = (maxB - minB <= 0) ? 0 : FCMAX / (maxB - minB);

    Algorithms::subFactorMap(rgb, area, to, w,
        minR, minG, minB,
        factorR, factorG, factorB);

    return dest;
}

uint AutolevelsFilter::getWindowRadius() const
{
    return 1;
}
