#include <QImage>
#include <QVector>
#include <QtGlobal>
#include "MedianFilter.hpp"
#include "../Utils.hpp"
#include "../Algorithms.hpp"
#include "../Constants.hpp"

MedianFilter::MedianFilter(uint aRadius)
    : radius(aRadius)
{}

MedianFilter::~MedianFilter()
{}

/* TODO: RGB->YUV, filter, YUV->RGB. */
QImage * MedianFilter::filter(const QImage & source, QRect area) const
{
    uint w = source.width();
    uint h = source.height();

    QImage * dest = new QImage(source);
    QRgb * to = reinterpret_cast<QRgb *>(dest->bits());

    const QRgb * rgb =
        reinterpret_cast<const QRgb *>(source.constBits());

    uint wSize = radius * 2 + 1;
    uint wMatrixSize = wSize * wSize;

    QVector<uchar> windowR(wMatrixSize);
    QVector<uchar> windowG(wMatrixSize);
    QVector<uchar> windowB(wMatrixSize);

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            uint i = 0;

            for (int wy = -radius; wy <= (int) radius; ++wy)
            {
                for (int wx = -radius; wx <= (int) radius; ++wx)
                {
                    QRgb pixel = rgb[Utils::coordinate(x + wx, y + wy, w, h)];

                    windowR[i] = qRed(pixel);
                    windowG[i] = qGreen(pixel);
                    windowB[i] = qBlue(pixel);
                    ++i;
                }
            }

            to[y * w + x] = qRgb(median(windowR, wMatrixSize),
                median(windowG, wMatrixSize),
                median(windowB, wMatrixSize));
        }
    }

    return dest;
}

uint MedianFilter::getWindowRadius() const
{
    return radius;
}

uchar MedianFilter::median(QVector<uchar> & vector, uint size) const
{
    if (radius == 1)
    {
        /* TODO use constData, if pixel added into previous window. */
        return Algorithms::networkSortMedian(vector.data());
    }
    else
    {
        return Algorithms::histogramMedian(vector.constData(), size);
    }
}
