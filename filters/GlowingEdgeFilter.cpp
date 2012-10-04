#include <QImage>
#include <QVector>
#include <QtGlobal>
#include "GlowingEdgeFilter.hpp"
#include "SobelFilter.hpp"
#include "AverageSobelFilter.hpp"
#include "../Utils.hpp"
#include "../Constants.hpp"
#include "../Algorithms.hpp"

#define MEDIAN_RADIUS 5

GlowingEdgeFilter::GlowingEdgeFilter()
    : median(MEDIAN_RADIUS),
    sobel()
{}

GlowingEdgeFilter::~GlowingEdgeFilter()
{}

QImage * GlowingEdgeFilter::filter(const QImage & source,
    QRect area) const
{
    uint w = source.width();

    QImage * medianed = median.filter(source, area);
    QImage * sobeled = sobel.filter(*medianed, area);
    delete medianed;

    QRgb * to = reinterpret_cast<QRgb *>(sobeled->bits());

    const QRgb * rgb =
        reinterpret_cast<const QRgb *>(source.constBits());

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            QRgb pixel1 = rgb[y * w + x];
            QRgb pixel2 = to[y * w + x];

            int resR = qRed(pixel1);
            int resG = qGreen(pixel1);
            int resB = qBlue(pixel1);

            double br1 = Utils::brightness(pixel1);
            double br2 = Utils::brightness(pixel2);

            /* Increase intensive colored and bright pixels */
            resR = (resR > br1 * 2 || br1 > 170.0) ? resR * 2 : 0.1 * resR;
            resG = (resG > br1 * 2 || br1 > 170.0) ? resG * 2 : 0.1 * resG;
            resB = (resB > br1 * 2 || br1 > 170.0) ? resB * 2 : 0.1 * resB;

            /* Add edges */
            resR = Utils::normalize<int>(resR + br2 * br2 / 16, CMAX);
            resG = Utils::normalize<int>(resG + br2 * br2 / 16, CMAX);
            resB = Utils::normalize<int>(resB + br2 * br2 / 16, CMAX);

            to[y * w + x] = qRgb(resR, resG, resB);
        }
    }

    return sobeled;
}

uint GlowingEdgeFilter::getWindowRadius() const
{
    return Utils::max2<uint>(
        median.getWindowRadius(),
        sobel.getWindowRadius());
}
