#ifndef GAUSSIAN_FILTER_HPP_SENTRY
#define GAUSSIAN_FILTER_HPP_SENTRY

#include <QtGlobal>
#include <QColor>
#include "Filter.hpp"
class QImage;

class GaussianFilter : public Filter
{
    double sigma;
    uint wRadius;

    double * colorCache;

public:
    GaussianFilter(double aSigma);
    ~GaussianFilter();

    QImage * filter(const QImage & source, QRect area) const;

    uint getWindowRadius() const;

private:
    QRgb processRow(const QRgb * rgb,
        uint x, uint y, uint w, uint h) const;

    QRgb processColumn(const QRgb * rgb,
        uint x, uint y, uint w, uint h) const;

    void setColorCache(double * kernel);
};

#endif /* GAUSSIAN_FILTER_HPP_SENTRY */
