#ifndef CUSTOM_FILTER_HPP_SENTRY
#define CUSTOM_FILTER_HPP_SENTRY

#include <QtGlobal>
#include <QColor>
#include "Filter.hpp"
#include "../exceptions/CustomFilterCreateException.hpp"
class QImage;

class CustomFilter : public Filter
{
    double * kernel;
    uint wSize;
    uint wRadius;

public:
    CustomFilter(double * aKernel, uint aSize);
    ~CustomFilter();

    QImage * filter(const QImage & source) const;

    uint getWindowRadius() const;

    static CustomFilter * fromString(const QString & str)
        throw (CustomFilterCreateException);

private:
    QRgb processWindow(const QRgb * rgb,
        uint x, uint y, uint w, uint h) const;
};

#endif /* CUSTOM_FILTER_HPP_SENTRY */
