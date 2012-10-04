#include <QImage>
#include <QVector>
#include <QtGlobal>
#include "CustomFilter.hpp"
#include "../Utils.hpp"
#include "../Constants.hpp"

CustomFilter::CustomFilter(double * aKernel, uint aSize)
    : kernel(aKernel),
    wSize(aSize)
{
    wRadius = wSize / 2;
}

CustomFilter::~CustomFilter()
{}

QImage * CustomFilter::filter(const QImage & source, QRect area) const
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
            to[y * w + x] = processWindow(rgb, x, y, w, h);
        }
    }

    return dest;
}

uint CustomFilter::getWindowRadius() const
{
    return wRadius;
}

QRgb CustomFilter::processWindow(const QRgb * rgb,
    uint x, uint y, uint w, uint h) const
{
    double resR = 0.0;
    double resG = 0.0;
    double resB = 0.0;

    for (int wy = -wRadius; wy <= (int) wRadius; ++wy)
    {
        for (int wx = -wRadius; wx <= (int) wRadius; ++wx)
        {
            QRgb pixel = rgb[Utils::coordinate(x + wx, y + wy, w, h)];

            double factor =
                kernel[(wy + wRadius) * wSize + (wx + wRadius)];

            resR += factor * qRed(pixel);
            resG += factor * qGreen(pixel);
            resB += factor * qBlue(pixel);
        }
    }

    resR = Utils::normalize<int>((int) resR, CMAX);
    resG = Utils::normalize<int>((int) resG, CMAX);
    resB = Utils::normalize<int>((int) resB, CMAX);

    return qRgb(resR, resG, resB);
}


CustomFilter * CustomFilter::fromString(const QString & str)
    throw (CustomFilterCreateException)
{
    QStringList lines = str.split(';');
    QStringListIterator linesI(lines);

    int size = lines.size();

    if (size % 2 == 0)
    {
        throw CustomFilterCreateException(
            QObject::tr("supported only odd filter size"));
    }

    if (size < 3)
    {
        throw CustomFilterCreateException(
            QObject::tr("kernel size too small, allowable minimum is 3"));
    }

    double * kernel = new double[size * size];

    uint i = 0;

    while (linesI.hasNext())
    {
        QStringList items = linesI.next().split(',');
        QStringListIterator itemsI(items);

        if (items.size() != size)
        {
            delete[] kernel;
            throw CustomFilterCreateException(
                QObject::tr("different rows and colons counts"));
        }

        while (itemsI.hasNext())
        {
            bool ok;

            kernel[i] = itemsI.next().trimmed().toDouble(&ok);
            ++i;

            if (!ok)
            {
                delete[] kernel;
                throw CustomFilterCreateException(
                    QObject::tr("bad item in custom option argument"));
            }
        }
    }

    return new CustomFilter(kernel, size);
}
