#ifndef EXCEPTION_HPP_SENTRY
#define EXCEPTION_HPP_SENTRY

class Exception
{
    QString description;

public:
    Exception(const QString & descr = QString())
        : description(descr)
    {}

    virtual ~Exception()
    {}

    const QString getDescription() const
    {
        return description;
    }

    virtual const QString getName() const = 0;

    const QString toString() const
    {
        return getName() + "(" + getDescription() + ")";
    }
};

#endif /* EXCEPTION_HPP_SENTRY */
