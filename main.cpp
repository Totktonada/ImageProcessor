#include <iostream>
#include <QObject>
#include <QApplication>
#include <QImage>
#include <QtDebug>
#include "exceptions/BadArgsException.hpp"
#include "widgets/MainWindow.hpp"
#include "filters/Filter.hpp"
#include "Arguments.hpp"
#include "Utils.hpp"

#define EXIT_FAIL 1

void helpWrite()
{
    static const char help[] =
"Usage:\n"
"1. ImageProcessor --help | -h | -?\n"
"2. ImageProcessor source_img [filters] [destination_img]\n"
"3. ImageProcessor [source_img]\n"
"\n"
"First form write this help. Second simply process image with specified\n"
"filters. Third runs GUI.\n"
"\n"
"Filters are:\n"
"    -l, --autolevels <fraction> <fraction>\n"
"        fraction: double\n"
"    -c, --autocontrast <fraction> <fraction>\n"
"        fraction: double\n"
"    -x, --grayscale\n"
"    -a, --gray-world\n"
"    -m, --median\n"
"    -g, --gaussian\n"
"    -s, --sobel <edge> <what_detect>\n"
"        edge: several first symbols of the word\n"
"        \"Horizontal\" or \"Vertical\" (case-insensetive)\n"
"        what_detect: first several symbols of the word\n"
"        \"Increment\", \"Decrement\", \"Both\" (case-insensetive)\n"
"    -k, --custom <kernel>\n"
"        kernel: line1; ... ; lineN\n"
"        line: double1, ..., doubleN\n"
"        Example: '-1, -2, -1; 0, 0, 0; 1, 2, 1'\n"
"    -r, --rotate <degrees>\n"
"        degrees: double\n"
"    -z, --resize <ratio>\n"
"        ratio: double\n"
"        sigma: double\n"
"    -w, --wave <orientation> <amplitude> <period>\n"
"        orientation: several first symbols of the word\n"
"    \"Horizontal\" or \"Vertical\" (case-insensetive)\n"
"        amplitude: double\n"
"        period: double, degrees\n"
"    -q, --glass <radius>\n"
"        radius: double\n"
"    -i, --glowing-edge\n"
"    -o, --avg-sobel <what_detect> <what_detect>\n"
"        what_detect: several first symbols of the word\n"
"        \"Increment\", \"Decrement\" or \"Both\" (case-insensetive)\n"
"    -p, --sharpen\n"
"    -e, --maximum <radius>\n"
"        radius: double.\n";

    std::cout << help;
}

/* qFatal allow only constant string (not generated)
 * and so not allow use QObjject::tr */
void fatal(const QString & str, bool help = false)
{
    qDebug() << str;

    if (help)
    {
        helpWrite();
    }

    exit(EXIT_FAIL);
}

void processFilters(QImage *& image, QListIterator<Filter *> i)
{
    while (i.hasNext())
    {
        Filter * f = i.next();

        uint w = image->width();
        uint h = image->height();

        if (! f->isApplicable(w, h))
        {
            fatal(QObject::tr("Too small image (%1x%2), pointed filter"
                " not applicable").arg(w).arg(h));
        }

        QRect area(QPoint(0, 0), QSize(w, h));
        QImage * newImage = f->filter(*image, area);

        delete image;
        image = newImage;
    }
}

int runCli(Arguments & args)
{
    QImage * image = new QImage(args.getSource());

    if (image->format() != QImage::Format_RGB32)
    {
        /* For working with image with QImage::bits()
         * and qRed(), qGreen() and qBlue() functions. */
        *image = image->convertToFormat(QImage::Format_RGB32);
    }

    if (image->isNull())
    {
        fatal(QObject::tr("Unable to load source image."));
    }

    processFilters(image, args.getFilters());

    bool ok = image->save(args.getDestination());

    if (!ok)
    {
        fatal(QObject::tr("Unable to save result image."));
    }

    return 0;
}

int runGui(Arguments & args)
{
    if (! args.getFilters().isEmpty())
    {
        fatal(QObject::tr(
            "Point destination or remove filters arguments."));
    }

    MainWindow mainWindow(args.getSource());
    mainWindow.show();

    return qApp->exec();
}

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Arguments args;
    try
    {
        args.parse(app.arguments());
    } catch(BadArgsException & e)
    {
        fatal(e.toString(), true);
    }

    if (args.hasHelpOption())
    {
        if (! args.getSource().isNull() ||
            ! args.getDestination().isNull() ||
            ! args.getFilters().isEmpty())
        {
            qDebug() << "Option --help pointed,"
                " all others are ignored.";
        }

        helpWrite();
        return 0;
    }

    if (! args.getSource().isNull() &&
        ! args.getDestination().isNull())
    {
        return runCli(args);
    }
    else 
    {
        return runGui(args);
    }
}
