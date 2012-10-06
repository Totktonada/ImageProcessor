#ifndef FILTER_HPP_SENTRY
#define FILTER_HPP_SENTRY

#include <QRect>
class QImage;

class Filter
{
protected:
    QRect area;

public:
    virtual ~Filter()
    {}

    void setArea(QRect aArea)
    {
        area = aArea;
    }

    QRect getArea() const
    {
        return area;
    }

    virtual QImage * filter(const QImage & source) const = 0;

    bool isApplicable(uint w, uint h)
    {
        uint r = getWindowRadius();

        return !area.isNull() &&
            area.x() + area.width() <= (int) w &&
            area.y() + area.height() <= (int) h &&
            w >= r &&
            h >= r;
    }

    virtual uint getWindowRadius() const = 0;
};

#endif /* FILTER_HPP_SENTRY */
