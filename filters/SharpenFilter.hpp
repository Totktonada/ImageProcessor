#ifndef SHARPEN_FILTER_HPP_SENTRY
#define SHARPEN_FILTER_HPP_SENTRY

#include <QtGlobal>
#include <QColor>
#include "Filter.hpp"
#include "AverageSobelFilter.hpp"
class QImage;

class SharpenFilter : public Filter
{
    AverageSobelFilter * sobel;

public:
    SharpenFilter();
    ~SharpenFilter();

    QImage * filter(const QImage & source) const;

    uint getWindowRadius() const;
};

#endif /* SHARPEN_FILTER_HPP_SENTRY */
