#ifndef GLASS_FILTER_HPP_SENTRY
#define GLASS_FILTER_HPP_SENTRY

#include <QtGlobal>
#include <QColor>
#include "Filter.hpp"
class QImage;

class GlassFilter : public Filter
{
    double radius;

public:
    GlassFilter(double aRadius);
    ~GlassFilter();

    QImage * filter(const QImage & source, QRect area) const;

    uint getWindowRadius() const;

private:
    QRgb getPixel(const QRgb * rgb,
        uint x, uint y, uint w, uint h) const;
};

#endif /* GLASS_FILTER_HPP_SENTRY */
