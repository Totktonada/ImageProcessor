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

    QImage * filter(const QImage & source) const;

    uint getWindowRadius() const;

private:
    QImage * simpleFilter(const QImage & source) const;

    QImage * histogramSortFilter(const QImage & source) const;

    /* Nearly constant relatively window size. */
    QImage * constantTimeFilter(const QImage & source) const;
};

#endif /* MEDIAN_FILTER_HPP_SENTRY */
