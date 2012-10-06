#ifndef AVERAGE_SOBEL_FILTER_HPP_SENTRY
#define AVERAGE_SOBEL_FILTER_HPP_SENTRY

#include <QtGlobal>
#include <QColor>
#include "Filter.hpp"
#include "SobelFilter.hpp"
class QImage;

class AverageSobelFilter : public Filter
{
    SobelFilter::WhatDetect whatH;
    SobelFilter::WhatDetect whatV;

public:
    AverageSobelFilter(SobelFilter::WhatDetect aWhatH = SobelFilter::Both,
        SobelFilter::WhatDetect aWhatV = SobelFilter::Both);
    ~AverageSobelFilter();

    QImage * filter(const QImage & source) const;

    uint getWindowRadius() const;
};

#endif /* AVERAGE_SOBEL_FILTER_HPP_SENTRY */
