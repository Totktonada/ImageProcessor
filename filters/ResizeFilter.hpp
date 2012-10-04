#ifndef RESIZE_FILTER_HPP_SENTRY
#define RESIZE_FILTER_HPP_SENTRY

#include <QtGlobal>
#include <QColor>
#include "Filter.hpp"
class QImage;

class ResizeFilter : public Filter
{
    double ratio;

public:
    ResizeFilter(double aRatio);
    ~ResizeFilter();

    QImage * filter(const QImage & source, QRect area) const;

    uint getWindowRadius() const;

private:
    QRgb getPixel(const QRgb * rgb, QRect area,
        uint x, uint y, uint w, uint h) const;
};

#endif /* RESIZE_FILTER_HPP_SENTRY */
