#ifndef ARGUMENTS_HPP_SENTRY
#define ARGUMENTS_HPP_SENTRY

#include <QString>
#include <QStringList>
#include <QtGlobal>
#include "exceptions/BadArgsException.hpp"
#include "filters/SobelFilter.hpp"
#include "filters/WaveFilter.hpp"
class CustomFilter;
class Filter;

class Arguments
{
    static const char * optionString[];
    static const char * longToShort;

    QList<Filter *> filters;
    QString source;
    QString destination;
    bool hasHelpOpt;

public:
    Arguments();
    ~Arguments();

    void parse(const QStringList & args)
        throw (BadArgsException);

    const QList<Filter *> & getFilters() const;

    const QString & getSource() const;
    const QString & getDestination() const;
    bool hasHelpOption() const;

private:
    uint getUInt(QStringListIterator & i) const
        throw (BadArgsException);

    double getDouble(QStringListIterator & i) const
        throw (BadArgsException);

    CustomFilter * getCustomFilter(QStringListIterator & i) const
        throw (BadArgsException);

    SobelFilter::Edge getEdge(QStringListIterator & i) const
        throw (BadArgsException);

    SobelFilter::WhatDetect getWhatDetect(QStringListIterator & i) const
        throw (BadArgsException);

    WaveFilter::Orientation getOrientation(QStringListIterator & i) const
        throw (BadArgsException);

    QRect getArea(QStringListIterator & i) const
        throw (BadArgsException);

    void processShortOption(QChar ch, QStringListIterator & i)
        throw (BadArgsException);

    void processLongOption(const QString & arg, QStringListIterator & i)
        throw (BadArgsException);
};

#endif /* ARGUMENTS_HPP_SENTRY */
