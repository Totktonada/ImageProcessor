#ifndef MEDIAN_FILTER_HPP_SENTRY
#define MEDIAN_FILTER_HPP_SENTRY

#include <QColor>
#include "Filter.hpp"
class QImage;

class MedianFilter : public Filter
{
    uint radius;

public:
    MedianFilter(uint aRadius);
    ~MedianFilter();

    QImage * filter(const QImage & source, QRect area) const;

    uint getWindowRadius() const;

private:
    uchar median(QVector<uchar> & vector, uint size) const;

    QRgb processWindow(QVector<uchar> & windowR,
        QVector<uchar> & windowG, QVector<uchar> & windowB,
        uint wMatrixSize, const QRgb * rgb,
        uint x, uint y, uint w, uint h) const;
};

#endif /* MEDIAN_FILTER_HPP_SENTRY */
