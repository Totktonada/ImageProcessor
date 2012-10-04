#ifndef FILTER_HPP_SENTRY
#define FILTER_HPP_SENTRY

class QImage;
class QRect;

class Filter
{
public:
    virtual ~Filter()
    {}

    virtual QImage * filter(const QImage & source, QRect area) const = 0;

    bool isApplicable(uint w, uint h)
    {
        uint r = getWindowRadius();

        return w >= r && h >= r;
    }

    virtual uint getWindowRadius() const = 0;
};

#endif /* FILTER_HPP_SENTRY */
