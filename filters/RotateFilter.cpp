#include <QImage>
#include <QtGlobal>
#include "RotateFilter.hpp"
#include "../Utils.hpp"
#include "../Algorithms.hpp"

RotateFilter::RotateFilter(double aAngle)
    : angle(aAngle)
{
    Utils::getSinCos(angle, &sinAngle, &cosAngle);
}

RotateFilter::~RotateFilter()
{}

QImage * RotateFilter::filter(const QImage & source, QRect area) const
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
            to[y * w + x] = qRgb(0, 0, 0);
        }
    }

    QPointF to1 = rotateInverted(area.x(), area.y(), area);
    QPointF to2 = rotateInverted(area.x() + area.width() - 1,
        area.y(), area);
    QPointF to3 = rotateInverted(area.x(),
        area.y() + area.height() - 1, area);
    QPointF to4 = rotateInverted(area.x() + area.width() - 1,
        area.y() + area.height() - 1, area);

    int minX = Utils::normalize<int>(Utils::min4<double>(
        to1.x(), to2.x(), to3.x(), to4.x()), w - 1);
    int maxX = Utils::normalize<int>(Utils::max4<double>(
        to1.x(), to2.x(), to3.x(), to4.x()), w - 1);
    int minY = Utils::normalize<int>(Utils::min4<double>(
        to1.y(), to2.y(), to3.y(), to4.y()), h - 1);
    int maxY = Utils::normalize<int>(Utils::max4<double>(
        to1.y(), to2.y(), to3.y(), to4.y()), h - 1);

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            QPointF from = rotate(x, y, area);

            if (from.x() >= area.x() &&
                from.x() < area.x() + area.width() &&
                from.y() >= area.y() &&
                from.y() < area.y() + area.height())
            {

                to[y * w + x] = Algorithms::bilinearInterpolatedValue(
                    rgb, from.x(), from.y(), w, h);
            }
        }
    }

    return dest;
}

uint RotateFilter::getWindowRadius() const
{
    return 1;
}

/* All equations in new coordinates.
 * ----
 * r*cos(phi)=x
 * r*sin(phi)=y
 * r*cos(phi+angle)=fromX
 * r*sin(phi+angle)=fromY
 * r*(cos(phi)*cos(angle)-sin(phi)*sin(angle))=fromX
 * r*(sin(phi)*cos(angle)+cos(phi)*sin(angle))=fromY
 * fromX = x*cos(angle) - y*sin(angle)
 * fromY = x*sin(angle) + y*cos(angle)
 * ---- */
QPointF RotateFilter::rotate(int x, int y, QRect area) const
{
    /* New coordinates */
    double ncX = x - area.x() + 0.5 - area.width() / 2.0;
    double ncY = y - area.y() + 0.5 - area.height() / 2.0;
    /* Rotate */
    double fromX = ncX * cosAngle - ncY * sinAngle;
    double fromY = ncX * sinAngle + ncY * cosAngle;
    /* To old coordinates */
    fromX = fromX + area.x() - 0.5 + area.width() / 2.0;
    fromY = fromY + area.y() - 0.5 + area.height() / 2.0;

    return QPointF(fromX, fromY);
}

QPointF RotateFilter::rotateInverted(int x, int y, QRect area) const
{
    /* New coordinates */
    double ncX = x - area.x() + 0.5 - area.width() / 2.0;
    double ncY = y - area.y() + 0.5 - area.height() / 2.0;
    /* Inverted rotate */
    double fromX = ncX * cosAngle + ncY * sinAngle;
    double fromY = -ncX * sinAngle + ncY * cosAngle;
    /* To old coordinates */
    fromX = fromX + area.x() - 0.5 + area.width() / 2.0;
    fromY = fromY + area.y() - 0.5 + area.height() / 2.0;

    return QPointF(fromX, fromY);
}
