#include <QImage>
#include <QVector>
#include <QtGlobal>
#include <QtCore/qmath.h>
#include "AverageSobelFilter.hpp"
#include "SobelFilter.hpp"
#include "../Utils.hpp"
#include "../Constants.hpp"

AverageSobelFilter::AverageSobelFilter(SobelFilter::WhatDetect aWhatH,
    SobelFilter::WhatDetect aWhatV)
    : whatH(aWhatH),
    whatV(aWhatV)
{}

AverageSobelFilter::~AverageSobelFilter()
{}

QImage * AverageSobelFilter::filter(const QImage & source) const
{
    uint w = source.width();

    SobelFilter fx(SobelFilter::Vertical, whatV);
    SobelFilter fy(SobelFilter::Horizontal, whatH);

    fx.setArea(area);
    fy.setArea(area);

    QImage * diffX = fx.filter(source);
    QRgb * bitsDiffX = reinterpret_cast<QRgb *>(diffX->bits());

    QImage * diffY = fy.filter(source);
    QRgb * bitsDiffY = reinterpret_cast<QRgb *>(diffY->bits());

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            QRgb p1 = bitsDiffX[y * w + x];
            QRgb p2 = bitsDiffY[y * w + x];

            uchar resR = qSqrt(qPow(qRed(p1), 2) + qPow(qRed(p2), 2));
            uchar resG = qSqrt(qPow(qGreen(p1), 2) + qPow(qGreen(p2), 2));
            uchar resB = qSqrt(qPow(qBlue(p1), 2) + qPow(qBlue(p2), 2));

            bitsDiffX[y * w + x] = qRgb(resR, resG, resB);
        }
    }

    delete diffY;
    return diffX;
}

uint AverageSobelFilter::getWindowRadius() const
{
    return 1;
}
