#ifndef SOBEL_FILTER_HPP_SENTRY
#define SOBEL_FILTER_HPP_SENTRY

#include <QtGlobal>
#include <QColor>
#include "Filter.hpp"
#include "../ERgb.hpp"
class QImage;

class SobelFilter : public Filter
{
public:
    enum Edge
    {
        Horizontal, /* diff Y */
        Vertical    /* diff X */
    };

    enum WhatDetect
    {
        Increment,
        Decrement,
        Both
    };

private:
    Edge edge;
    WhatDetect what;

public:
    SobelFilter(Edge aEdge, WhatDetect aWhat = Both);
    ~SobelFilter();

    QImage * filter(const QImage & source, QRect area) const;

    uint getWindowRadius() const;

private:
    ERgb diffX(const QRgb * rgb,
        uint x, uint y, uint w, uint h) const;

    ERgb diffY(const QRgb * rgb,
        uint x, uint y, uint w, uint h) const;

    QRgb getEdgeValue(ERgb pixel1, ERgb pixel2, ERgb pixel3) const;

    QRgb addX(const ERgb * rgb,
        uint x, uint y, uint w, uint h) const;

    QRgb addY(const ERgb * rgb,
        uint x, uint y, uint w, uint h) const;
};

#endif /* SOBEL_X_FILTER_HPP_SENTRY */
