#include <limits.h>
#include <QtGui>
#include "ImageView.hpp"
#include "../Utils.hpp"
#include "../filters/Filter.hpp"

#include "../filters/AutolevelsFilter.hpp"
#include "../filters/AutocontrastFilter.hpp"
#include "../filters/GrayscaleFilter.hpp"
#include "../filters/GrayworldFilter.hpp"
#include "../filters/MedianFilter.hpp"
#include "../filters/GaussianFilter.hpp"
#include "../filters/SobelFilter.hpp"
#include "../filters/CustomFilter.hpp"
#include "../filters/RotateFilter.hpp"
#include "../filters/ResizeFilter.hpp"
#include "../filters/WaveFilter.hpp"
#include "../filters/GlassFilter.hpp"
#include "../filters/GlowingEdgeFilter.hpp"
#include "../filters/AverageSobelFilter.hpp"
#include "../filters/SharpenFilter.hpp"
#include "../filters/MaximumFilter.hpp"

#include "AutoLCFilterDialog.hpp"
#include "SobelFilterDialog.hpp"
#include "WaveFilterDialog.hpp"

#define STD_W_RADIUS 3
#define STD_GAUSSIAN_SIGMA 3.0
#define STD_ROTATE_DEGREES 90.0
#define STD_RESIZE_RATIO 1.25

ImageView::ImageView(QWidget * parent)
    : QWidget(parent)
{
    image = new QImage();
    imagePixmap = QPixmap::fromImage(*image);
    setBackgroundRole(QPalette::Base);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setSelectAll();
}

ImageView::~ImageView()
{
    /* Do not delete image field. */
}

QImage * ImageView::getImage() const
{
    return image;
}

void ImageView::setNewImage(QImage * newImage, bool dropSelect)
{
    setImage(newImage);

    if (dropSelect)
    {
        setSelectAll();
        emit selectWasChanged(getSelect());
    }
}

bool ImageView::isNull() const
{
    return image->isNull();
}

void ImageView::setImage(QImage * newImage)
{
    bool isNullOld = isNull();

    image = newImage;
    imagePixmap = QPixmap::fromImage(*image);

    resize(sizeHint()); /* send size for parent widget */
    update();
    if (isNullOld != isNull())
    {
        emit hasImage(!isNull());
    }
}

void ImageView::startFilter(Filter * f)
{
    uint w = image->width();
    uint h = image->height();

    f->setArea(Utils::selectImageCoords(getSelect()));

    if (! f->isApplicable(w, h))
    {
        QRect area = f->getArea();

        QMessageBox::warning(this, tr("ImageProcessor"),
            tr("Too small image (%1x%2), pointed filter"
            " with pointed area (%3x%4+%5+%6) not applicable")
                .arg(w).arg(h).arg(area.x()).arg(area.y())
                .arg(area.width()).arg(area.height()));

        return;
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    setImage(f->filter(*image));

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    delete f;
    emit imageWasFiltered(true);
}

void ImageView::startAutolevelsFilter()
{
    bool ok;

    double skipLeft;
    double skipRight;

    AutoLCFilterDialog::getValues(this, &skipLeft, &skipRight, &ok);

    if (ok)
    {
        if (skipLeft + skipRight <= 1.0)
        {
            startFilter(new AutolevelsFilter(skipLeft, skipRight));
        }
        else
        {
            QMessageBox::warning(this, tr("ImageProcessor"),
                tr("Summa of fractions > 1.0"));
        }
    }
}

void ImageView::startAutocontrastFilter()
{
    bool ok;

    double skipLeft;
    double skipRight;

    AutoLCFilterDialog::getValues(this, &skipLeft, &skipRight, &ok);

    if (ok)
    {
        if (skipLeft + skipRight <= 1.0)
        {
            startFilter(new AutocontrastFilter(skipLeft, skipRight));
        }
        else
        {
            QMessageBox::warning(this, tr("ImageProcessor"),
                tr("Summa of fractions > 1.0"));
        }
    }
}

void ImageView::startGrayscaleFilter()
{
    startFilter(new GrayscaleFilter());
}

void ImageView::startGrayworldFilter()
{
    startFilter(new GrayworldFilter());
}

void ImageView::startMedianFilter()
{
    bool ok;

    uint wRadius = QInputDialog::getInt(this,
        tr("Median filter"),
        tr("Window radius:"), STD_W_RADIUS, 1, INT_MAX, 1, &ok);

    if (ok)
    {
        startFilter(new MedianFilter(wRadius));
    }
}

void ImageView::startGaussianFilter()
{
    bool ok;

    double sigma = QInputDialog::getDouble(this,
        tr("Gaussian filter"),
        tr("Sigma:"), STD_GAUSSIAN_SIGMA, 0.1, INT_MAX, 1, &ok);

    if (ok)
    {
        startFilter(new GaussianFilter(sigma));
    }
}

void ImageView::startSobelFilter()
{
    bool ok;

    SobelFilter::Edge edge;
    SobelFilter::WhatDetect what;

    SobelFilterDialog::getSobelValues(this, &edge, &what, &ok);

    if (ok)
    {
        startFilter(new SobelFilter(edge, what));
    }
}

void ImageView::startCustomFilter()
{
    bool ok;

    QString text = QInputDialog::getText(this,
        tr("Custom filter"),
        tr("Kernel:"), QLineEdit::Normal,
        "-1, -2, -1; 0, 0, 0; 1, 2, 1", &ok);

    if (ok)
    {
        try
        {
            startFilter(CustomFilter::fromString(text));
        }
        catch(CustomFilterCreateException & e)
        {
            QMessageBox::warning(this, tr("ImageProcessor"),
                e.getDescription());
        }
    }
}

void ImageView::startRotateFilter()
{
    bool ok;

    double degrees = QInputDialog::getDouble(this,
        tr("Rotate filter"),
        tr("Degrees:"), STD_ROTATE_DEGREES, -180.0, 180.0, 1, &ok);

    if (ok)
    {
        startFilter(new RotateFilter(Utils::radians(degrees)));
    }
}

void ImageView::startResizeFilter()
{
    bool ok;

    double ratio = QInputDialog::getDouble(this,
        tr("Resize filter"),
        tr("Ratio:"), STD_RESIZE_RATIO, 0.01, INT_MAX, 2, &ok);

    if (ok)
    {
        startFilter(new ResizeFilter(ratio));
    }
}

void ImageView::startWaveFilter()
{
    bool ok;

    WaveFilter::Orientation orientation;
    double amplitude;
    double period;

    WaveFilterDialog::getValues(this,
        &orientation, &amplitude, &period, &ok);

    if (ok)
    {
        startFilter(new WaveFilter(amplitude, period, orientation));
    }
}

void ImageView::startGlassFilter()
{
    bool ok;

    double wRadius = QInputDialog::getDouble(this,
        tr("Glass filter"),
        tr("Window radius:"), STD_W_RADIUS, 0.1, INT_MAX, 1, &ok);

    if (ok)
    {
        startFilter(new GlassFilter(wRadius));
    }
}

void ImageView::startGlowingEdgeFilter()
{
    startFilter(new GlowingEdgeFilter());
}

void ImageView::startAverageSobelFilter()
{
    bool ok;

    SobelFilter::WhatDetect whatH;
    SobelFilter::WhatDetect whatV;

    SobelFilterDialog::getAverageSobelValues(this, &whatH, &whatV, &ok);

    if (ok)
    {
        startFilter(new AverageSobelFilter(whatH, whatV));
    }
}

void ImageView::startSharpenFilter()
{
    startFilter(new SharpenFilter());
}

void ImageView::startMaximumFilter()
{
    bool ok;

    uint wRadius = QInputDialog::getInt(this,
        tr("Maximum filter"),
        tr("Window radius:"), STD_W_RADIUS, 0, INT_MAX, 1, &ok);

    if (ok)
    {
        startFilter(new MaximumFilter(wRadius));
    }
}

void ImageView::setSelectAll()
{
    QSize size = sizeHint();

    selectX = 0;
    selectY = 0;
    selectW = size.width();
    selectH = size.height();
}

void ImageView::setSelectFirstPoint(int x, int y)
{
    QSize size = sizeHint();
    int maxX = size.width() - 1;
    int maxY = size.height() - 1;

    selectX = Utils::normalize<int>(x, maxX);
    selectY = Utils::normalize<int>(y, maxY);
    selectW = 0;
    selectH = 0;
}

void ImageView::setSelectSecondPoint(int x, int y)
{
    QSize size = sizeHint();
    int maxX = size.width() - 1;
    int maxY = size.height() - 1;

    selectW = Utils::normalize<int>(x, maxX) - selectX;
    selectH = Utils::normalize<int>(y, maxY) - selectY;
}

QRect ImageView::getSelect() const
{
    int x = (selectW < 0) ? selectX + selectW : selectX;
    int y = (selectH < 0) ? selectY + selectH : selectY;
    int w = (selectW < 0) ? -selectW : selectW;
    int h = (selectH < 0) ? -selectH : selectH;

    QRect select(x, y, w, h);
    QRect area = Utils::selectImageCoords(select);

    return area.isValid() ? select : QRect(QPoint(0, 0), sizeHint());
}


QSize ImageView::getImageSize() const
{
    return image->size();
}

void ImageView::mousePressEvent(QMouseEvent * event)
{
    event->accept();

    setSelectFirstPoint(event->x(), event->y());

    update();
    emit selectWasChanged(getSelect());
}

void ImageView::mouseMoveEvent(QMouseEvent * event)
{
    event->accept();

    setSelectSecondPoint(event->x(), event->y());

    update();
    emit selectWasChanged(getSelect());
}

void ImageView::mouseReleaseEvent(QMouseEvent * event)
{
    event->accept();

    if (selectX == event->x() && selectY == event->y())
    {
        setSelectAll();
    }
    else
    {
        setSelectSecondPoint(event->x(), event->y());
    }

    update();
    emit selectWasChanged(getSelect());
}

/* TODO: repaint only drawRegion */
void ImageView::paintEvent(QPaintEvent *)
{
    QRect select = getSelect();

    QPen pen(Qt::DashLine);
    pen.setColor(Qt::white);
    pen.setWidth(1.5);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    p.eraseRect(QRect(QPoint(0, 0), sizeHint()));
    p.drawPixmap(QPoint(1, 1), imagePixmap);

    p.setPen(pen);
    p.setCompositionMode(QPainter::RasterOp_SourceXorDestination);

    /* Why drawRect(QRect) draw rectangle with coordinates
     * (x, y) and (x + w, y + h), but not
     * (x, y) and (x + w - 1, y + h - 1)? */
    p.drawRect(select.x(), select.y(),
        select.width() - 1, select.height() - 1);
}

QSize ImageView::sizeHint() const
{
    return imagePixmap.size() + QSize(2, 2);
}
