#include <QImage>
#include <QtGlobal>
#include "Algorithms.hpp"
#include "Utils.hpp"
#include "Constants.hpp"

void Algorithms::getMinMaxSimple(
    const QRgb * rgb, QRect area, uint w,
    int * minR, int * minG, int * minB,
    int * maxR, int * maxG, int * maxB)
{
    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            QRgb pixel = rgb[y * w + x];

            *minR = Utils::min2<int>(*minR, qRed(pixel));
            *minG = Utils::min2<int>(*minG, qGreen(pixel));
            *minB = Utils::min2<int>(*minB, qBlue(pixel));

            *maxR = Utils::max2<int>(*maxR, qRed(pixel));
            *maxG = Utils::max2<int>(*maxG, qGreen(pixel));
            *maxB = Utils::max2<int>(*maxB, qBlue(pixel));
        }
    }
}

void Algorithms::getMinMaxRobust(
    const QRgb * rgb, QRect area, uint w,
    double skipL, double skipR,
    int * minR, int * minG, int * minB,
    int * maxR, int * maxG, int * maxB)
{
    uint histogramR[COLORS];
    uint histogramG[COLORS];
    uint histogramB[COLORS];

    for (uint i = 0; i < COLORS; ++i)
    {
        histogramR[i] = 0;
        histogramG[i] = 0;
        histogramB[i] = 0;
    }

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            QRgb pixel = rgb[y * w + x];

            ++histogramR[qRed(pixel)];
            ++histogramG[qGreen(pixel)];
            ++histogramB[qBlue(pixel)];
        }
    }

    uint areaSize = area.height() * area.width();

    double skipLeft = areaSize * skipL;
    double skipRight = areaSize * skipR;

    getChannelMinMax(histogramR, skipLeft, skipRight, minR, maxR);
    getChannelMinMax(histogramG, skipLeft, skipRight, minG, maxG);
    getChannelMinMax(histogramB, skipLeft, skipRight, minB, maxB);
}

void Algorithms::factorSum(const QRgb * rgb, QRect area,
    QRgb * to, uint w,
    double factorR, double factorG, double factorB)
{
    static uchar mapR[COLORS];
    static uchar mapG[COLORS];
    static uchar mapB[COLORS];

    for (uint i = 0; i < COLORS; ++i)
    {
        mapR[i] = factorR * i;
        mapG[i] = factorG * i;
        mapB[i] = factorB * i;
    }

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            QRgb pixel = rgb[y * w + x];

            uchar Y =
                mapR[qRed(pixel)] +
                mapG[qGreen(pixel)] +
                mapB[qBlue(pixel)];

            to[y * w + x] = qRgb(Y, Y, Y);
        }
    }
}

void Algorithms::factorMap(const QRgb * rgb, QRect area,
    QRgb * to, uint w,
    double factorR, double factorG, double factorB)
{
    static uchar mapR[COLORS];
    static uchar mapG[COLORS];
    static uchar mapB[COLORS];

    for (uint i = 0; i < COLORS; ++i)
    {
        mapR[i] = factorR * i;
        mapG[i] = factorG * i;
        mapB[i] = factorB * i;
    }

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            QRgb pixel = rgb[y * w + x];

            to[y * w + x] = qRgb(
                mapR[qRed(pixel)],
                mapG[qGreen(pixel)],
                mapB[qBlue(pixel)]);
        }
    }
}

void Algorithms::subFactorMap(const QRgb * rgb, QRect area,
    QRgb * to, uint w,
    uchar subR, uchar subG, uchar subB,
    double factorR, double factorG, double factorB)
{
    static uchar mapR[COLORS];
    static uchar mapG[COLORS];
    static uchar mapB[COLORS];

    for (uint i = 0; i < COLORS; ++i)
    {
        mapR[i] = Utils::normalize<int>(factorR * i, CMAX);
        mapG[i] = Utils::normalize<int>(factorG * i, CMAX);
        mapB[i] = Utils::normalize<int>(factorB * i, CMAX);
    }

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            QRgb pixel = rgb[y * w + x];

            to[y * w + x] = qRgb(
                mapR[Utils::normalize<int>(qRed(pixel) - subR, CMAX)],
                mapG[Utils::normalize<int>(qGreen(pixel) - subG, CMAX)],
                mapB[Utils::normalize<int>(qBlue(pixel) - subB, CMAX)]);
        }
    }
}

void Algorithms::subFactorMap(const QRgb * rgb, QRect area,
    QRgb * to, uint w, uchar sub, double factor)
{
    static uchar map[COLORS];

    for (uint i = 0; i < COLORS; ++i)
    {
        map[i] = Utils::normalize<int>(factor * i, CMAX);

    }

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            QRgb pixel = rgb[y * w + x];

            to[y * w + x] = qRgb(
                map[Utils::normalize<int>(qRed(pixel) - sub, CMAX)],
                map[Utils::normalize<int>(qGreen(pixel) - sub, CMAX)],
                map[Utils::normalize<int>(qBlue(pixel) - sub, CMAX)]);
        }
    }
}

uchar Algorithms::networkSortMedian(uchar array[])
{
    /* 012 *
     * 345 *
     * 678 */

    /* -------------------------- */
    /*       Sort in lines        */
    firstMax(array[1], array[2]);
    firstMax(array[4], array[5]);
    firstMax(array[7], array[8]);

    firstMax(array[0], array[1]);
    firstMax(array[3], array[4]);
    firstMax(array[6], array[7]);

    firstMax(array[1], array[2]);
    firstMax(array[4], array[5]);
    firstMax(array[7], array[8]);
    /* -------------------------- */

    /* Three group in cols:
     * maximums in her lines (036);
     * middles in lines (147);
     * minimins in lines (258).   */

    /* -------------------------- */
    /*         7 < 4 < 1          */
    firstMax(array[4], array[7]);
    firstMax(array[1], array[4]);
    firstMax(array[4], array[7]);

    /* 6 is min in 036 (maximums) */
    firstMax(array[0], array[3]);
    firstMax(array[3], array[6]);

    /* 2 is max in 258 (minimums) */
    firstMax(array[5], array[8]);
    firstMax(array[2], array[5]);
    /* -------------------------- */

    /* 0, 1, 3 more than others */
    /* 5, 7, 8 less than others */

    /* Median is 2, 4 or 6, sort them. */
    firstMax(array[4], array[2]);
    firstMax(array[6], array[4]);
    firstMax(array[4], array[2]);

    return array[4];
}

QRgb Algorithms::bilinearInterpolatedValue(const QRgb * rgb,
    double x, double y, uint w, uint h)
{
    double shiftX = x - ((int) x);
    double shiftY = y - ((int) y);

    /* 00 01 *
     * 10 11 */
    QRgb p00 = rgb[Utils::coordinate((int) x, (int) y, w, h)];
    QRgb p01 = rgb[Utils::coordinate((int) x + 1, (int) y, w, h)];
    QRgb p10 = rgb[Utils::coordinate((int) x, (int) y + 1, w, h)];
    QRgb p11 = rgb[Utils::coordinate((int) x + 1, (int) y + 1, w, h)];

    double resR =
        (1.0 - shiftX) * (1.0 - shiftY) * qRed(p00) +
        shiftX * (1.0 - shiftY) * qRed(p01) +
        (1.0 - shiftX) * shiftY * qRed(p10) +
        shiftX * shiftY * qRed(p11);

    double resG =
        (1.0 - shiftX) * (1.0 - shiftY) * qGreen(p00) +
        shiftX * (1.0 - shiftY) * qGreen(p01) +
        (1.0 - shiftX) * shiftY * qGreen(p10) +
        shiftX * shiftY * qGreen(p11);

    double resB =
        (1.0 - shiftX) * (1.0 - shiftY) * qBlue(p00) +
        shiftX * (1.0 - shiftY) * qBlue(p01) +
        (1.0 - shiftX) * shiftY * qBlue(p10) +
        shiftX * shiftY * qBlue(p11);

#if 0
    resR = Utils::normalize<int>((int) resR, CMAX);
    resG = Utils::normalize<int>((int) resG, CMAX);
    resB = Utils::normalize<int>((int) resB, CMAX);
#endif

    return qRgb(resR, resG, resB);
}

void Algorithms::firstMax(uchar & x, uchar & y)
{
#if 0
    /* 900 ms */
    uchar tmp;

    if (x < y)
    {
        tmp = x;
        x = y;
        y = tmp;
    }
#else
    /* 800 ms */
    int d = y - x;
    int m = ~(d >> 8);
    x += d & m;
    y -= d & m;
#endif
}

void Algorithms::getChannelMinMax(uint histogram[COLORS],
    double skipLeft, double skipRight,
    int * min, int * max)
{
    uint sum = 0;

    for (int i = 0; i < COLORS; ++i)
    {
        sum += histogram[i];
        
        if (sum >= skipLeft)
        {
            *min = i;
            break;
        }
    }

    sum = 0;

    for (int i = COLORS - 1; i >= 0; --i)
    {
        sum += histogram[i];
        
        if (sum >= skipRight)
        {
            *max = i;
            break;
        }
    }
}
