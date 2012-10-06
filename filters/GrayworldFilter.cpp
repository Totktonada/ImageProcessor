#include <QImage>
#include <QtGlobal>
#include "GrayworldFilter.hpp"
#include "../Utils.hpp"
#include "../Algorithms.hpp"
#include "../Constants.hpp"

GrayworldFilter::GrayworldFilter()
{}

GrayworldFilter::~GrayworldFilter()
{}

/* Based on http://scien.stanford.edu/pages/labsite/2000/psych221/projects/00/trek/Matlab/GrayWorldFun.m */
QImage * GrayworldFilter::filter(const QImage & source) const
{
    uint w = source.width();

    QImage * dest = new QImage(source);
    QRgb * to = reinterpret_cast<QRgb *>(dest->bits());

    const QRgb * rgb =
        reinterpret_cast<const QRgb *>(source.constBits());

    qint64 sumR = 0;
    qint64 sumG = 0;
    qint64 sumB = 0;

    uchar maxR = CMIN;
    uchar maxG = CMIN;
    uchar maxB = CMIN;

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            QRgb pixel = rgb[y * w + x];

            maxR = Utils::max2<uchar>(maxR, qRed(pixel));
            maxG = Utils::max2<uchar>(maxG, qGreen(pixel));
            maxB = Utils::max2<uchar>(maxB, qBlue(pixel));

            sumR += qRed(pixel);
            sumG += qGreen(pixel);
            sumB += qBlue(pixel);
        }
    }

    double sumGray = (sumR + sumG + sumB) / 3;

    double factorR = (sumR == 0) ? FCMAX : sumGray / sumR;
    double factorG = (sumG == 0) ? FCMAX : sumGray / sumG;
    double factorB = (sumB == 0) ? FCMAX : sumGray / sumB;

    double max = Utils::max3<double>(
        maxR * factorR,
        maxG * factorG,
        maxB * factorB);

    if (max > FCMAX)
    {
        double factor = FCMAX / max;
        factorR *= factor;
        factorG *= factor;
        factorB *= factor;
    }

    Algorithms::factorMap(rgb, area, to, w,
        factorR, factorG, factorB);

    return dest;
}

uint GrayworldFilter::getWindowRadius() const
{
    return 1;
}
