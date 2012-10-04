#ifndef MAXIMUM_FILTER_HPP_SENTRY
#define MAXIMUM_FILTER_HPP_SENTRY

#include <QColor>
#include "Filter.hpp"
class QImage;

class MaximumFilter : public Filter
{
    uint radius;

public:
    MaximumFilter(uint aRadius);
    ~MaximumFilter();

    QImage * filter(const QImage & source, QRect area) const;

    uint getWindowRadius() const;
};

#endif /* MAXIMUM_FILTER_HPP_SENTRY */
