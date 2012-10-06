#include <QImage>
#include <QVector>
#include <QtGlobal>
#include "MedianFilter.hpp"
#include "../Utils.hpp"
#include "../Algorithms.hpp"
#include "../Constants.hpp"
#include "../Histogram.hpp"

MedianFilter::MedianFilter(uint aRadius)
    : radius(aRadius)
{}

MedianFilter::~MedianFilter()
{}

/* TODO: RGB->YUV, filter, YUV->RGB. */
QImage * MedianFilter::filter(const QImage & source) const
{
    if (radius == 1)
    {
        return simpleFilter(source);
    }
    else if (radius <= 6)
    {
        return histogramSortFilter(source);
    }
    else /* (radius >= 7) */
    {
        return constantTimeFilter(source);
    }
}

uint MedianFilter::getWindowRadius() const
{
    return radius;
}

QImage * MedianFilter::simpleFilter(const QImage & source) const
{
    uint w = source.width();
    uint h = source.height();

    QImage * dest = new QImage(source);
    QRgb * to = reinterpret_cast<QRgb *>(dest->bits());

    const QRgb * rgb =
        reinterpret_cast<const QRgb *>(source.constBits());

    uchar * windowR = new uchar[9];
    uchar * windowG = new uchar[9];
    uchar * windowB = new uchar[9];

    QRgb pixel;

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            pixel = rgb[Utils::coordinate(x - 1, y - 1, w, h)];
            windowR[0] = qRed(pixel);
            windowG[0] = qGreen(pixel);
            windowB[0] = qBlue(pixel);

            pixel = rgb[Utils::coordinate(x - 1, y, w, h)];
            windowR[1] = qRed(pixel);
            windowG[1] = qGreen(pixel);
            windowB[1] = qBlue(pixel);

            pixel = rgb[Utils::coordinate(x - 1, y + 1, w, h)];
            windowR[2] = qRed(pixel);
            windowG[2] = qGreen(pixel);
            windowB[2] = qBlue(pixel);

            pixel = rgb[Utils::coordinate(x, y - 1, w, h)];
            windowR[3] = qRed(pixel);
            windowG[3] = qGreen(pixel);
            windowB[3] = qBlue(pixel);

            pixel = rgb[Utils::coordinate(x, y, w, h)];
            windowR[4] = qRed(pixel);
            windowG[4] = qGreen(pixel);
            windowB[4] = qBlue(pixel);

            pixel = rgb[Utils::coordinate(x, y + 1, w, h)];
            windowR[5] = qRed(pixel);
            windowG[5] = qGreen(pixel);
            windowB[5] = qBlue(pixel);

            pixel = rgb[Utils::coordinate(x + 1, y - 1, w, h)];
            windowR[6] = qRed(pixel);
            windowG[6] = qGreen(pixel);
            windowB[6] = qBlue(pixel);

            pixel = rgb[Utils::coordinate(x + 1, y, w, h)];
            windowR[7] = qRed(pixel);
            windowG[7] = qGreen(pixel);
            windowB[7] = qBlue(pixel);

            pixel = rgb[Utils::coordinate(x + 1, y + 1, w, h)];
            windowR[8] = qRed(pixel);
            windowG[8] = qGreen(pixel);
            windowB[8] = qBlue(pixel);

            uchar r = Algorithms::networkSortMedian(windowR);
            uchar g = Algorithms::networkSortMedian(windowG);
            uchar b = Algorithms::networkSortMedian(windowB);

            to[y * w + x] = qRgb(r, g, b);
        }
    }

    delete[] windowR;
    delete[] windowG;
    delete[] windowB;

    return dest;
}

QImage * MedianFilter::histogramSortFilter(const QImage & source) const
{
    uint w = source.width();
    uint h = source.height();

    QImage * dest = new QImage(source);
    QRgb * to = reinterpret_cast<QRgb *>(dest->bits());

    const QRgb * rgb =
        reinterpret_cast<const QRgb *>(source.constBits());

    uint wSize = radius * 2 + 1;
    uint wMatrixSize = wSize * wSize;

    Histogram hist;

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            hist.clear();

            for (int wy = -radius; wy <= (int) radius; ++wy)
            {
                for (int wx = -radius; wx <= (int) radius; ++wx)
                {
                    hist += rgb[Utils::coordinate(x + wx, y + wy, w, h)];
                }
            }

            to[y * w + x] = hist.median(wMatrixSize);
        }
    }

    return dest;
}

QImage * MedianFilter::constantTimeFilter(const QImage & source) const
{
    uint w = source.width();
    uint h = source.height();

    QImage * dest = new QImage(source);
    QRgb * to = reinterpret_cast<QRgb *>(dest->bits());

    const QRgb * rgb =
        reinterpret_cast<const QRgb *>(source.constBits());

    uint wSize = radius * 2 + 1;
    uint wMatrixSize = wSize * wSize;

    uint columns = area.width() + radius * 2;
    Histogram * colsHist = new Histogram[columns];

    int i = 0;

    /* Initialize columns histogram for y == 0 */
    for (int x = area.x() - radius;
        x < area.x() + area.width() + (int) radius;
        ++x)
    {
        for (int y = area.y() - radius;
            y <= area.y() + (int) radius;
            ++y)
        {
            colsHist[i] += rgb[Utils::coordinate(x, y, w, h)];
        }

        ++i;
    }

    Histogram kernelHist;

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        kernelHist.clear();

        for (uint j = 0; j < wSize; ++j)
        {
            kernelHist += colsHist[j];
        }

        i = 0;

        for (int x = area.x(); x < area.x() + (int) columns; ++x)
        {
            if (x < area.x() + area.width())
            {
                to[y * w + x] = kernelHist.median(wMatrixSize);
            }

            if (i + wSize < columns)
            {
                kernelHist -= colsHist[i];
                kernelHist += colsHist[i + wSize];
            }

            colsHist[i] -=
                rgb[Utils::coordinate(x - radius, y - radius, w, h)];
            colsHist[i] +=
                rgb[Utils::coordinate(x - radius, y + radius + 1, w, h)];

            ++i;
        }
    }

    delete[] colsHist;

    return dest;
}
