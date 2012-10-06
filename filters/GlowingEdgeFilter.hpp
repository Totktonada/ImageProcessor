#ifndef GLOWING_EDGE_FILTER_HPP_SENTRY
#define GLOWING_EDGE_FILTER_HPP_SENTRY

#include <QtGlobal>
#include <QColor>
#include "Filter.hpp"
#include "MedianFilter.hpp"
#include "AverageSobelFilter.hpp"
class QImage;

class GlowingEdgeFilter : public Filter
{
    MedianFilter * median;
    AverageSobelFilter * sobel;

public:
    GlowingEdgeFilter();
    ~GlowingEdgeFilter();

    QImage * filter(const QImage & source) const;

    uint getWindowRadius() const;
};

#endif /* GLOWING_EDGE_FILTER_HPP_SENTRY */
