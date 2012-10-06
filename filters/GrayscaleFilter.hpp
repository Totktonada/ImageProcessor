#ifndef GRAYSCALE_FILTER_HPP_SENTRY
#define GRAYSCALE_FILTER_HPP_SENTRY

#include "Filter.hpp"
class QImage;

class GrayscaleFilter : public Filter
{
public:
    GrayscaleFilter();
    ~GrayscaleFilter();

    QImage * filter(const QImage & source) const;

    uint getWindowRadius() const;
};

#endif /* GRAYSCALE_FILTER_HPP_SENTRY */
