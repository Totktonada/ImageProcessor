#ifndef BAD_ARGS_EXCEPTION_HPP_SENTRY
#define BAD_ARGS_EXCEPTION_HPP_SENTRY

#include "Exception.hpp"

class BadArgsException : public Exception
{
public:
    BadArgsException(const QString & descr = QString())
        : Exception(descr)
    {}

    ~BadArgsException()
    {}

    const QString getName() const
    {
        return "BadArgsException";
    }
};

#endif /* BAD_ARGS_EXCEPTION_HPP_SENTRY */
