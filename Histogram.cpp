#include <string.h>
#include "Histogram.hpp"
#include "Constants.hpp"

Histogram::Histogram()
{
    histR = new uint[COLORS];
    histG = new uint[COLORS];
    histB = new uint[COLORS];

    clear();
}

Histogram::~Histogram()
{
    delete[] histR;
    delete[] histG;
    delete[] histB;
}

void Histogram::clear()
{
    memset(histR, 0, sizeof(uint) * COLORS);
    memset(histG, 0, sizeof(uint) * COLORS);
    memset(histB, 0, sizeof(uint) * COLORS);
}

void Histogram::operator+=(QRgb pixel)
{
    ++histR[qRed(pixel)];
    ++histG[qGreen(pixel)];
    ++histB[qBlue(pixel)];
}

void Histogram::operator-=(QRgb pixel)
{
    --histR[qRed(pixel)];
    --histG[qGreen(pixel)];
    --histB[qBlue(pixel)];
}

void Histogram::operator+=(const Histogram & aHist)
{
    for (uint i = 0; i < COLORS; ++i)
    {
        histR[i] += aHist.histR[i];
        histG[i] += aHist.histG[i];
        histB[i] += aHist.histB[i];
    }
}

void Histogram::operator-=(const Histogram & aHist)
{
    for (uint i = 0; i < COLORS; ++i)
    {
        histR[i] -= aHist.histR[i];
        histG[i] -= aHist.histG[i];
        histB[i] -= aHist.histB[i];
    }
}

QRgb Histogram::median(uint size) const
{
    uint curR = 0;
    uint curG = 0;
    uint curB = 0;

    uchar medianR = 0;
    uchar medianG = 0;
    uchar medianB = 0;

    for (uint i = 0; i < COLORS; ++i)
    {
        curR += histR[i];

        if (curR > size / 2)
        {
            medianR = i;
            break;
        }
    }

    for (uint i = 0; i < COLORS; ++i)
    {
        curG += histG[i];

        if (curG > size / 2)
        {
            medianG = i;
            break;
        }
    }

    for (uint i = 0; i < COLORS; ++i)
    {
        curB += histB[i];

        if (curB > size / 2)
        {
            medianB = i;
            break;
        }
    }

    return qRgb(medianR, medianG, medianB);
}
