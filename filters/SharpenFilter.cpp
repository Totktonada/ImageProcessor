#include <QImage>
#include <QVector>
#include <QtGlobal>
#include "SharpenFilter.hpp"
#include "AverageSobelFilter.hpp"
#include "../Utils.hpp"
#include "../Constants.hpp"

SharpenFilter::SharpenFilter()
    : sobel()
{}

SharpenFilter::~SharpenFilter()
{}

QImage * SharpenFilter::filter(const QImage & source, QRect area) const
{
    uint w = source.width();

    QImage * dest = sobel.filter(source, area);
    QRgb * to = reinterpret_cast<QRgb *>(dest->bits());

    const QRgb * rgb =
        reinterpret_cast<const QRgb *>(source.constBits());

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            QRgb pixel1 = rgb[y * w + x];
            QRgb pixel2 = to[y * w + x];

            int resR = qRed(pixel1) - qRed(pixel2);
            int resG = qGreen(pixel1) - qGreen(pixel2);
            int resB = qBlue(pixel1) - qBlue(pixel2);

            resR = (resR < 0) ? 0 : resR;
            resG = (resG < 0) ? 0 : resG;
            resB = (resB < 0) ? 0 : resB;

            to[y * w + x] = qRgb(resR, resG, resB);
        }
    }

    return dest;
}

uint SharpenFilter::getWindowRadius() const
{
    return sobel.getWindowRadius();
}
