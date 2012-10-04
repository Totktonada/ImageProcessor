#ifndef CUSTOM_FILTER_CREATE_EXCEPTION_HPP_SENTRY
#define CUSTOM_FILTER_CREATE_EXCEPTION_HPP_SENTRY

#include "Exception.hpp"

class CustomFilterCreateException : public Exception
{
public:
    CustomFilterCreateException(const QString & descr = QString())
        : Exception(descr)
    {}

    ~CustomFilterCreateException()
    {}

    const QString getName() const
    {
        return "CustomFilterCreateException";
    }
};

#endif /* CUSTOM_FILTER_CREATE_EXCEPTION_HPP_SENTRY */
