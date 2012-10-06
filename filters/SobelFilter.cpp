#include <QImage>
#include <QVector>
#include <QtGlobal>
#include "SobelFilter.hpp"
#include "../Utils.hpp"
#include "../Constants.hpp"

SobelFilter::SobelFilter(Edge aEdge, WhatDetect aWhat)
    : edge(aEdge),
    what(aWhat)
{}

SobelFilter::~SobelFilter()
{}

QImage * SobelFilter::filter(const QImage & source) const
{
    uint w = source.width();
    uint h = source.height();

    QImage * dest = new QImage(source);
    QRgb * to = reinterpret_cast<QRgb *>(dest->bits());

    const QRgb * rgb =
        reinterpret_cast<const QRgb *>(source.constBits());

    ERgb * diffed = new ERgb[w * h];

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            if (edge == Horizontal)
            {
                diffed[y * w + x] = diffY(rgb, x, y, w, h);
            }
            else /* (edge == Vertical) */
            {
                diffed[y * w + x] = diffX(rgb, x, y, w, h);
            }
        }
    }

    for (int y = area.y(); y < area.y() + area.height(); ++y)
    {
        for (int x = area.x(); x < area.x() + area.width(); ++x)
        {
            if (edge == Horizontal)
            {
                to[y * w + x] = addX(diffed, x, y, w, h);
            }
            else /* (edge == Vertical) */
            {
                to[y * w + x] = addY(diffed, x, y, w, h);
            }
        }
    }

    delete[] diffed;
    return dest;
}

uint SobelFilter::getWindowRadius() const
{
    return 1;
}

/* [1; 0; -1] * rgb */
ERgb SobelFilter::diffX(const QRgb * rgb,
    uint x, uint y, uint w, uint h) const
{
    QRgb pixel1 = rgb[Utils::coordinate(x - 1, y, w, h)];
    QRgb pixel2 = rgb[Utils::coordinate(x + 1, y, w, h)];

    int resR = qRed(pixel1) - qRed(pixel2);
    int resG = qGreen(pixel1) - qGreen(pixel2);
    int resB = qBlue(pixel1) - qBlue(pixel2);

    return eRgb(resR, resG, resB);
}

/* [1; 0; -1]^T * rgb */
ERgb SobelFilter::diffY(const QRgb * rgb,
    uint x, uint y, uint w, uint h) const
{
    QRgb pixel1 = rgb[Utils::coordinate(x, y - 1, w, h)];
    QRgb pixel2 = rgb[Utils::coordinate(x, y + 1, w, h)];

    int resR = qRed(pixel1) - qRed(pixel2);
    int resG = qGreen(pixel1) - qGreen(pixel2);
    int resB = qBlue(pixel1) - qBlue(pixel2);

    return eRgb(resR, resG, resB);
}

QRgb SobelFilter::getEdgeValue(ERgb pixel1, ERgb pixel2, ERgb pixel3) const
{
    int resR = eRed(pixel1) + eRed(pixel2) * 2 + eRed(pixel3);
    int resG = eGreen(pixel1) + eGreen(pixel2) * 2 + eGreen(pixel3);
    int resB = eBlue(pixel1) + eBlue(pixel2) * 2 + eBlue(pixel3);

    if (what == Increment)
    {
        resR = Utils::normalize<int>(-resR, CMAX);
        resG = Utils::normalize<int>(-resG, CMAX);
        resB = Utils::normalize<int>(-resB, CMAX);
    }
    else if (what == Decrement)
    {
        resR = Utils::normalize<int>(resR, CMAX);
        resG = Utils::normalize<int>(resG, CMAX);
        resB = Utils::normalize<int>(resB, CMAX);
    }
    else /* (what == Both) */
    {
        resR = resR < 0 ? -resR : resR;
        resR = resR > CMAX ? CMAX : resR;

        resG = resG < 0 ? -resG : resG;
        resG = resG > CMAX ? CMAX : resG;

        resB = resB < 0 ? -resB : resB;
        resB = resB > CMAX ? CMAX : resB;
    }

    return qRgb(resR, resG, resB);
}

/* 1/4 * [1; 2; 1] * rgb */
QRgb SobelFilter::addX(const ERgb * rgb,
    uint x, uint y, uint w, uint h) const
{
    ERgb pixel1 = rgb[Utils::coordinate(x - 1, y, w, h)];
    ERgb pixel2 = rgb[Utils::coordinate(x, y, w, h)];
    ERgb pixel3 = rgb[Utils::coordinate(x + 1, y, w, h)];

    return getEdgeValue(pixel1, pixel2, pixel3);
}

/* 1/4 * [1; 2; 1]^T * rgb */
QRgb SobelFilter::addY(const ERgb * rgb,
    uint x, uint y, uint w, uint h) const
{
    ERgb pixel1 = rgb[Utils::coordinate(x, y - 1, w, h)];
    ERgb pixel2 = rgb[Utils::coordinate(x, y, w, h)];
    ERgb pixel3 = rgb[Utils::coordinate(x, y + 1, w, h)];

    return getEdgeValue(pixel1, pixel2, pixel3);
}
