#ifndef IMAGE_VIEW_HPP_SENTRY
#define IMAGE_VIEW_HPP_SENTRY

#include <QScrollArea>
class QLabel;
class Filter;

class ImageView : public QWidget
{
    Q_OBJECT

public:
    ImageView(QWidget * parent = 0);
    ~ImageView();

    QImage * getImage() const;

    void setNewImage(QImage * newImage, bool dropSelect);

    bool isNull() const;

    QSize getImageSize() const;

signals:
    void hasImage(bool yes);
    void imageWasFiltered(bool yes);
    void selectWasChanged(QRect select);

public slots:
    void startAutolevelsFilter();
    void startAutocontrastFilter();
    void startGrayscaleFilter();
    void startGrayworldFilter();
    void startMedianFilter();
    void startGaussianFilter();
    void startSobelFilter();
    void startCustomFilter();
    void startRotateFilter();
    void startResizeFilter();
    void startWaveFilter();
    void startGlassFilter();
    void startGlowingEdgeFilter();
    void startAverageSobelFilter();
    void startSharpenFilter();
    void startMaximumFilter();

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

    void paintEvent(QPaintEvent * event);

    QSize sizeHint() const;

private:
    /* Not cleared select */
    void setImage(QImage * newImage);

    void startFilter(Filter * f);

    void setSelectAll();
    void setSelectFirstPoint(int x, int y);
    void setSelectSecondPoint(int x, int y);

    /* Get displayed area */
    QRect getSelect() const;

    /* ---- Fields ---- */

    QImage * image;
    QPixmap imagePixmap;

    /* Displayed area (with borders) */
    int selectX;
    int selectY;
    int selectW;
    int selectH;
};

#endif /* IMAGE_VIEW_HPP_SENTRY */
