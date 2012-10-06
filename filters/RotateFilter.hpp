#ifndef ROTATE_FILTER_HPP_SENTRY
#define ROTATE_FILTER_HPP_SENTRY

#include <QtGlobal>
#include <QColor>
#include "Filter.hpp"
class QImage;

class RotateFilter : public Filter
{
    double angle; /* radians */

    /* Cached values */
    double cosAngle;
    double sinAngle;

public:
    RotateFilter(double aAngle);
    ~RotateFilter();

    QImage * filter(const QImage & source) const;

    uint getWindowRadius() const;

private:
    QPointF rotate(int x, int y) const;
    QPointF rotateInverted(int x, int y) const;

    void setPixel(QRgb * to, const QRgb * rgb,
        uint x, uint y, uint w, uint h) const;
};

#endif /* ROTATE_FILTER_HPP_SENTRY */
