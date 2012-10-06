#include <QtGlobal>

#include "Arguments.hpp"
#include "exceptions/BadArgsException.hpp"
#include "exceptions/CustomFilterCreateException.hpp"
#include "Utils.hpp"

#include "filters/Filter.hpp"
#include "filters/AutolevelsFilter.hpp"
#include "filters/AutocontrastFilter.hpp"
#include "filters/GrayscaleFilter.hpp"
#include "filters/GrayworldFilter.hpp"
#include "filters/MedianFilter.hpp"
#include "filters/GaussianFilter.hpp"
#include "filters/SobelFilter.hpp"
#include "filters/CustomFilter.hpp"
#include "filters/RotateFilter.hpp"
#include "filters/ResizeFilter.hpp"
#include "filters/WaveFilter.hpp"
#include "filters/GlassFilter.hpp"
#include "filters/GlowingEdgeFilter.hpp"
#include "filters/AverageSobelFilter.hpp"
#include "filters/SharpenFilter.hpp"
#include "filters/MaximumFilter.hpp"

const char * Arguments::optionString[] =
{
    "--help"
    "--autolevels",
    "--autocontrast",
    "--grayscale",
    "--gray-world",
    "--median",
    "--gaussian",
    "--sobel",
    "--custom"
    "--rotate",
    "--resize",
    "--wave",
    "--glass",
    "--glowing-edge",
    "--avg-sobel",
    "--sharpen",
    "--maximum"
};

const char * Arguments::longToShort = "hlcxamgskrzwqiope";

Arguments::Arguments()
    : filters(),
    source(),
    destination(),
    hasHelpOpt(false)
{}

Arguments::~Arguments()
{}

void Arguments::parse(const QStringList & args)
    throw (BadArgsException)
{
    QStringListIterator i(args);

    if (!i.hasNext())
    {
        throw BadArgsException(QObject::tr("no program name argument"));
    }

    i.next(); /* skip the program name */

    while (i.hasNext())
    {
        QString arg = i.next();

        if (arg.startsWith('-') && arg.length() == 2)
        {
            processShortOption(arg[1], i);
        }
        else if (arg.startsWith("--") && arg.length() > 2)
        {
            processLongOption(arg, i);
        }
        else if (source.isNull())
        {
            source = arg;
        }
        else if (destination.isNull())
        {
            destination = arg;
        }
        else
        {
            throw BadArgsException(QObject::tr("unexpected argument"));
        }
    }
}

const QList<Filter *> & Arguments::getFilters() const
{
    return filters;
}

const QString & Arguments::getSource() const
{
    return source;
}

const QString & Arguments::getDestination() const
{
    return destination;
}

bool Arguments::hasHelpOption() const
{
    return hasHelpOpt;
}

uint Arguments::getUInt(QStringListIterator & i) const
    throw (BadArgsException)
{
    bool ok;
    uint res;

    if (!i.hasNext())
    {
        throw BadArgsException(QObject::tr("missed argument for option"));
    }

    res = i.next().toUInt(&ok);

    if (!ok)
    {
        throw BadArgsException(QObject::tr("bad uint argument for option"));
    }

    return res;
}

double Arguments::getDouble(QStringListIterator & i) const
    throw (BadArgsException)
{
    bool ok;
    double res;

    if (!i.hasNext())
    {
        throw BadArgsException(QObject::tr("missed argument for option"));
    }

    res = i.next().toDouble(&ok);

    if (!ok)
    {
        throw BadArgsException(QObject::tr("bad double argument for option"));
    }

    return res;
}

CustomFilter * Arguments::getCustomFilter(QStringListIterator & argI) const
    throw (BadArgsException)
{
    if (! argI.hasNext())
    {
        throw BadArgsException(QObject::tr("missed argument for option"));
    }

    try
    {
        return CustomFilter::fromString(argI.next());
    }
    catch (CustomFilterCreateException & e)
    {
        throw BadArgsException(e.toString());
    }
}

SobelFilter::Edge Arguments::getEdge(
    QStringListIterator & i) const
    throw (BadArgsException)
{
    if (! i.hasNext())
    {
        throw BadArgsException(QObject::tr("missed argument for option"));
    }

    QString str = i.next();

    if (QString("Horizontal").startsWith(str, Qt::CaseInsensitive))
    {
        return SobelFilter::Horizontal;
    }
    else if (QString("Vertical").startsWith(str, Qt::CaseInsensitive))
    {
        return SobelFilter::Vertical;
    }
    else
    {
        throw BadArgsException("bad edge argument");
    }
}

SobelFilter::WhatDetect Arguments::getWhatDetect(
    QStringListIterator & i) const
    throw (BadArgsException)
{
    if (! i.hasNext())
    {
        throw BadArgsException(QObject::tr("missed argument for option"));
    }

    QString str = i.next();

    if (QString("Increment").startsWith(str, Qt::CaseInsensitive))
    {
        return SobelFilter::Increment;
    }
    else if (QString("Decrement").startsWith(str, Qt::CaseInsensitive))
    {
        return SobelFilter::Decrement;
    }
    else if (QString("Both").startsWith(str, Qt::CaseInsensitive))
    {
        return SobelFilter::Both;
    }
    else
    {
        throw BadArgsException("bad \"what detect\" argument");
    }
}

WaveFilter::Orientation Arguments::getOrientation(
    QStringListIterator & i) const
    throw (BadArgsException)
{
    if (! i.hasNext())
    {
        throw BadArgsException(QObject::tr("missed argument for option"));
    }

    QString str = i.next();

    if (QString("Horizontal").startsWith(str, Qt::CaseInsensitive))
    {
        return WaveFilter::Horizontal;
    }
    else if (QString("Vertical").startsWith(str, Qt::CaseInsensitive))
    {
        return WaveFilter::Vertical;
    }
    else
    {
        throw BadArgsException("bad orientation argument");
    }
}

QRect Arguments::getArea(QStringListIterator & i) const
    throw (BadArgsException)
{
    if (!i.hasNext() || !i.peekNext().startsWith(':'))
    {
        return QRect();
    }

    QString str = i.next();
    str.remove(0, 1); /* remove ':' */

    QStringList xy_w_h = str.split('+');

    if (xy_w_h.size() != 3)
    {
        throw BadArgsException("bad area");
    }

    QStringList x_y = xy_w_h.at(0).split('x');

    if (x_y.size() != 2)
    {
        throw BadArgsException("bad area");
    }

    bool ok1, ok2, ok3, ok4;

    uint x = x_y.at(0).toUInt(&ok1);
    uint y = x_y.at(1).toUInt(&ok2);
    uint w = xy_w_h.at(1).toUInt(&ok3);
    uint h = xy_w_h.at(2).toUInt(&ok4);

    if (!ok1 || !ok2 || !ok3 || !ok4 || w == 0 || h == 0)
    {
        throw BadArgsException("bad area");
    }

    return QRect(QPoint(x, y), QSize(w, h));
}

void Arguments::processShortOption(QChar ch, QStringListIterator & i)
    throw (BadArgsException)
{
    Filter * cur = 0;

    SobelFilter::WhatDetect argWhat1;
    SobelFilter::WhatDetect argWhat2;

    WaveFilter::Orientation argOrient;
    double arg1;
    double arg2;

    switch (ch.toAscii())
    {
        case 'h':
        case '?':
            hasHelpOpt = true;
            break;
        case 'l':
            arg1 = Utils::normalize<double>(getDouble(i), 1.0);
            arg2 = Utils::normalize<double>(getDouble(i), 1.0);
            if (arg1 + arg2 > 1.0)
            {
                throw BadArgsException("Summa of fractions > 1.0");
            }
            cur = new AutolevelsFilter(arg1, arg2);
            break;
        case 'c':
            arg1 = Utils::normalize<double>(getDouble(i), 1.0);
            arg2 = Utils::normalize<double>(getDouble(i), 1.0);
            if (arg1 + arg2 > 1.0)
            {
                throw BadArgsException("Summa of fractions > 1.0");
            }
            cur = new AutocontrastFilter(arg1, arg2);
            break;
        case 'x':
            cur = new GrayscaleFilter();
            break;
        case 'a':
            cur = new GrayworldFilter();
            break;
        case 'm':
            cur = new MedianFilter(getUInt(i));
            break;
        case 'g':
            cur = new GaussianFilter(getDouble(i));
            break;
        case 's':
            cur = new SobelFilter(getEdge(i), getWhatDetect(i));
            break;
        case 'k':
            cur = getCustomFilter(i);
            break;
        case 'r':
            arg1 = Utils::radians(getDouble(i));
            cur = new RotateFilter(arg1);
            break;
        case 'z':
            cur = new ResizeFilter(getDouble(i));
            break;
        case 'w':
            argOrient = getOrientation(i);
            arg1 = getDouble(i);
            arg2 = getDouble(i);
            cur = new WaveFilter(arg1, arg2, argOrient);
            break;
        case 'q':
            cur = new GlassFilter(getDouble(i));
            break;
        case 'i':
            cur = new GlowingEdgeFilter();
            break;
        case 'o':
            argWhat1 = getWhatDetect(i);
            argWhat2 = getWhatDetect(i);
            cur = new AverageSobelFilter(argWhat1, argWhat2);
            break;
        case 'p':
            cur = new SharpenFilter();
            break;
        case 'e':
            cur = new MaximumFilter(getUInt(i));
            break;
        default:
            throw BadArgsException(QObject::tr("unrecognized option"));
    }

    if (cur != 0)
    {
        cur->setArea(getArea(i));
        filters.append(cur);
    }
}

void Arguments::processLongOption(const QString & arg,
    QStringListIterator & i)
    throw (BadArgsException)
{
    for (uint j = 0; j < sizeof(optionString) / sizeof(char *); ++j)
    {
        if (arg == optionString[j])
        {
            processShortOption(longToShort[j], i);
            return;
        }
    }

    throw BadArgsException(QObject::tr("unrecognized option"));
}
