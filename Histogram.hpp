#ifndef HISTOGRAM_HPP_SENTRY
#define HISTOGRAM_HPP_SENTRY

#include <QtGlobal>
#include <QColor>

class Histogram
{
    uint * histR;
    uint * histG;
    uint * histB;

public:
    Histogram();
    ~Histogram();

    void clear();

    void operator+=(QRgb pixel);

    /* Without zero bound checks */
    void operator-=(QRgb pixel);

    void operator+=(const Histogram & aHist);

    /* Without zero bound checks */
    void operator-=(const Histogram & aHist);

    /* size is pixels count in area */
    QRgb median(uint size) const;
};

#endif /* HISTOGRAM_HPP_SENTRY */
