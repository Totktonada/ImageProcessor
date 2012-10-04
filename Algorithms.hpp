#ifndef ALGORITHMS_HPP_SENTRY
#define ALGORITHMS_HPP_SENTRY

#include <QtGlobal>
#include <QColor>
#include "Constants.hpp"

/* Image related algorithms are worked with 32 bit image. */

class Algorithms
{
public:
    static void getMinMaxSimple(
        const QRgb * rgb, QRect area, uint w,
        int * minR, int * minG, int * minB,
        int * maxR, int * maxG, int * maxB);

    static void getMinMaxRobust(
        const QRgb * rgb, QRect area, uint w,
        double skipL, double skipR,
        int * minR, int * minG, int * minB,
        int * maxR, int * maxG, int * maxB);

    static void factorSum(const QRgb * rgb, QRect area,
        QRgb * dest, uint w,
        double factorR, double factorG, double factorB);

    static void factorMap(const QRgb * rgb, QRect area,
        QRgb * dest, uint w,
        double factorR, double factorG, double factorB);

    static void subFactorMap(const QRgb * rgb, QRect area,
        QRgb * to, uint w,
        uchar subR, uchar subG, uchar subB,
        double factorR, double factorG, double factorB);

    static void subFactorMap(const QRgb * rgb, QRect area,
        QRgb * to, uint w, uchar sub, double factor);

    /* size == 9 */
    static uchar networkSortMedian(uchar array[]);

    static uchar histogramMedian(const uchar array[], uint size);

    static QRgb bilinearInterpolatedValue(const QRgb * rgb,
        double x, double y, uint w, uint h);

private:
    static void firstMax(uchar & x, uchar & y);

    static void getChannelMinMax(uint histogram[COLORS],
        double skipLeft, double skipRight,
        int * min, int * max);
};

#endif /* ALGORITHMS_HPP_SENTRY */
