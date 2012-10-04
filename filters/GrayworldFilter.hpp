#ifndef GRAYWORLD_FILTER_HPP_SENTRY
#define GRAYWORLD_FILTER_HPP_SENTRY

#include "Filter.hpp"
class QImage;

class GrayworldFilter : public Filter
{
public:
    GrayworldFilter();
    ~GrayworldFilter();

    QImage * filter(const QImage & source, QRect area) const;

    uint getWindowRadius() const;
};

#endif /* GRAYWORLD_FILTER_HPP_SENTRY */
