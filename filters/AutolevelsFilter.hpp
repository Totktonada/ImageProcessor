#ifndef AUTOLEVELS_FILTER_HPP_SENTRY
#define AUTOLEVELS_FILTER_HPP_SENTRY

#include "Filter.hpp"
#include "../Constants.hpp"
class QImage;

class AutolevelsFilter : public Filter
{
    double skipL;
    double skipR;

public:
    AutolevelsFilter(double aSkip = 0.05);
    AutolevelsFilter(double aSkipL, double aSkipR);
    ~AutolevelsFilter();

    QImage * filter(const QImage & source) const;

    uint getWindowRadius() const;
};

#endif /* AUTOLEVELS_FILTER_HPP_SENTRY */
