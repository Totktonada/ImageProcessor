#ifndef UTILS_HPP_SENTRY
#define UTILS_HPP_SENTRY

#include <math.h>
#include <QtCore/qmath.h>
#include <QColor>
#include <QImage>
#include "Constants.hpp"

class Utils
{
public:
    template <typename T>
    static inline T min2(T x, T y)
    {
        return (x < y) ? x : y;
    }

    template <typename T>
    static inline T min3(T x, T y, T z)
    {
        return (x < y) ? min2<T>(x, z) : min2<T>(y, z);
    }

    template <typename T>
    static inline T min4(T x1, T x2, T x3, T x4)
    {
        return (x1 < x2) ? min3<T>(x1, x3, x4) : min3<T>(x2, x3, x4);
    }

    template <typename T>
    static inline T max2(T x, T y)
    {
        return (x > y) ? x : y;
    }

    template <typename T>
    static inline T max3(T x, T y, T z)
    {
        return (x > y) ? max2<T>(x, z) : max2<T>(y, z);
    }

    template <typename T>
    static inline T max4(T x1, T x2, T x3, T x4)
    {
        return (x1 > x2) ? max3<T>(x1, x3, x4) : max3<T>(x2, x3, x4);
    }

    template <typename T>
    static inline T normalize(T x, T max)
    {
        return (x < 0) ? 0 : ((x > max) ? max : x);
    }

    /* Reflect across edge. */
    static inline uint coordinate(int x, int y, int w, int h)
    {
        uint nx = (x < 0) ? -1 - x : ((x >= w) ? (w - 1) - (x - w) : x);
        uint ny = (y < 0) ? -1 - y : ((y >= h) ? (y - 1) - (y - h) : y);

        return ny * w + nx;
    }

    /* degress -> radians */
    static inline double radians(double degree)
    {
        return degree * M_PI / 180;
    }

    /* BT.709 */
    static inline double brightness(QRgb c)
    {
        return STRENGTH_RED * qRed(c) + STRENGTH_GREEN * qGreen(c) +
            STRENGTH_BLUE * qBlue(c);
    }

    static inline QRect selectImageCoords(QRect area)
    {
        return area.adjusted(0, 0, -2, -2);
    }

    /* Avoid precision problem occured with qSin() and qCos(). */
    static inline void getSinCos(double angle,
        double * sinAngle, double * cosAngle)
    {
        *sinAngle = qSin(angle);

        *cosAngle = (*sinAngle == 1.0 || *sinAngle == -1.0) ? 0.0 :
            qCos(angle);

        if (*cosAngle == 1.0 || *cosAngle == -1.0)
        {
            *sinAngle = 0.0;
        }
    }

    /* Avoid precision problem occured with qSin() and qCos(). */
    static inline double getSin(double angle)
    {
        double cosAngle = qCos(angle);

        return (cosAngle == 1.0 || cosAngle == -1.0) ? 0.0 : qSin(angle);
    }
};

#endif /* UTILS_HPP_SENTRY */
