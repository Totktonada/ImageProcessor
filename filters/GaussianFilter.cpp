#include <string.h>
#include <QImage>
#include <QtGlobal>
#include <QtCore/qmath.h>
#include "GaussianFilter.hpp"
#include "../Utils.hpp"
#include "../Constants.hpp"

GaussianFilter::GaussianFilter(double aSigma)
    : sigma(aSigma)
{
    wRadius = (uint) (sigma * 3 + 1.0);

    double * kernel = new double[wRadius + 1];

    double sum = 0.0;
    kernel[0] = 1.0;

    for (uint w = 1; w <= wRadius; ++w)
    {
        kernel[w] = qExp(- ((int) (w * w)) / (2.0 * sigma * sigma));
        sum += kernel[w];
    }

    sum = sum * 2 + 1.0;

    /* Normalization */
    kernel[0] /= sum;

    for (uint w = 1; w <= wRadius; ++w)
    {
        kernel[w] /= sum;
    }

    setColorCache(kernel);
}

GaussianFilter::~GaussianFilter()
{
    delete[] colorCache;
}

QImage * GaussianFilter::filter(const QImage & source) const
{
    uint w = source.width();
    uint h = source.height();

    QImage * dest = new QImage(source);
    QRgb * to = reinterpret_cast<QRgb *>(dest->bits());

    const QRgb * rgb =
        reinterpret_cast<const QRgb *>(source.constBits());

    QImage * mediumImage = new QImage(source);
    QRgb * medium = reinterpret_cast<QRgb *>(mediumImage->bits());

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            medium[y * w + x] = processRow(rgb, x, y, w, h);
        }
    }

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            to[y * w + x] = processColumn(medium, x, y, w, h);
        }
    }

    delete mediumImage;
    return dest;
}

uint GaussianFilter::getWindowRadius() const
{
    return wRadius;
}

QRgb GaussianFilter::processRow(const QRgb * rgb,
    uint x, uint y, uint w, uint h) const
{
    QRgb pixel1 = rgb[Utils::coordinate(x, y, w, h)];
    QRgb pixel2;

    double resR = colorCache[qRed(pixel1)];
    double resG = colorCache[qGreen(pixel1)];
    double resB = colorCache[qBlue(pixel1)];

    uint base = COLORS;

    for (uint wx = 1; wx <= wRadius; ++wx)
    {
        pixel1 = rgb[Utils::coordinate(x - wx, y, w, h)];
        pixel2 = rgb[Utils::coordinate(x + wx, y, w, h)];

        resR += colorCache[base + qRed(pixel1)] +
            colorCache[base + qRed(pixel2)];
        resG += colorCache[base + qGreen(pixel1)] +
            colorCache[base + qGreen(pixel2)];
        resB += colorCache[base + qBlue(pixel1)] +
            colorCache[base + qBlue(pixel2)];

        base += COLORS;
    }

    return qRgb(resR, resG, resB);
}

QRgb GaussianFilter::processColumn(const QRgb * rgb,
    uint x, uint y, uint w, uint h) const
{
    QRgb pixel1 = rgb[Utils::coordinate(x, y, w, h)];
    QRgb pixel2;

    double resR = colorCache[qRed(pixel1)];
    double resG = colorCache[qGreen(pixel1)];
    double resB = colorCache[qBlue(pixel1)];

    uint base = COLORS;

    for (uint wy = 1; wy <= wRadius; ++wy)
    {
        pixel1 = rgb[Utils::coordinate(x, y - wy, w, h)];
        pixel2 = rgb[Utils::coordinate(x, y + wy, w, h)];

        resR += colorCache[base + qRed(pixel1)] +
            colorCache[base + qRed(pixel2)];
        resG += colorCache[base + qGreen(pixel1)] +
            colorCache[base + qGreen(pixel2)];
        resB += colorCache[base + qBlue(pixel1)] +
            colorCache[base + qBlue(pixel2)];

        base += COLORS;
    }

    return qRgb(resR, resG, resB);
}

void GaussianFilter::setColorCache(double * kernel)
{
    colorCache = new double[(wRadius + 1) * COLORS];

    uint j = 0; /* (j == w * wSize + i) */

    for (uint w = 0; w <= wRadius; ++w)
    {
        for (uint i = 0; i < COLORS; ++i)
        {
            colorCache[j] = kernel[w] * i;
            ++j;
        }
    }
}
