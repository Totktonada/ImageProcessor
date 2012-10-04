#ifndef AUTOCONTRAST_FILTER_HPP_SENTRY
#define AUTOCONTRAST_FILTER_HPP_SENTRY

#include "Filter.hpp"
class QImage;

class AutocontrastFilter : public Filter
{
    double skipL;
    double skipR;

public:
    AutocontrastFilter(double aSkip = 0.05);
    AutocontrastFilter(double aSkipL, double aSkipR);
    ~AutocontrastFilter();

    QImage * filter(const QImage & source, QRect area) const;

    uint getWindowRadius() const;
};

#endif /* AUTOCONTRAST_FILTER_HPP_SENTRY */
