#include <QtGui>
#include "MainWindow.hpp"
#include "ImageView.hpp"
#include "Utils.hpp"

#define MESSAGE_SHOWING_TIME_MS 2000

MainWindow::MainWindow(const QString & filePath, QWidget * parent)
    : QMainWindow(parent),
    imageHistory()
{
    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidgetResizable(false);
    scrollArea->setFocusPolicy(Qt::NoFocus);
    scrollArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    imageView = new ImageView(scrollArea);
    scrollArea->setWidget(imageView);

    setCentralWidget(scrollArea);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    setEnabledActions();

    connect(imageView, SIGNAL(imageWasFiltered(bool)), this,
        SLOT(imageWasFiltered(bool)));

    connect(imageView, SIGNAL(selectWasChanged(QRect)), this,
        SLOT(selectWasChanged(QRect)));

    setCurrentFile("");
    clearHistory(true);

    if (! filePath.isNull())
    {
        loadFile(filePath);
    }
}

MainWindow::~MainWindow()
{
    if (! imageHistory.isEmpty())
    {
        clearHistory(false);
    }
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if (maybeSave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::open()
{
    if (maybeSave())
    {
        QString filePath = QFileDialog::getOpenFileName(this);

        if (! filePath.isEmpty())
        {
            loadFile(filePath);
        }
    }
}

bool MainWindow::save()
{
    if (imageView->isNull() || currentFilePath.isEmpty())
    {
        return false;
    }
    else
    {
        return saveFile(currentFilePath);
    }
}

bool MainWindow::saveAs()
{
    if (imageView->isNull())
    {
        return false;
    }
    else
    {
        QString newFilePath = QFileDialog::getSaveFileName(this);

        if (newFilePath.isEmpty())
        {
            return false;
        }

        return saveFile(newFilePath);
    }
}

void MainWindow::undo()
{
    setCurrentInHistory(currentInHistory - 1);
}

void MainWindow::redo()
{
    setCurrentInHistory(currentInHistory + 1);
}

void MainWindow::closeImage()
{
    if (maybeSave() && ! imageView->isNull())
    {
        setCurrentFile("");
        imageView->setNewImage(new QImage(), true);
        clearHistory(true);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About ImageProcessor"),
        tr("The <b>ImageProcessor</b> is educational application,"
        " which designed for image processing."));
}

void MainWindow::imageWasFiltered(bool yes)
{
    if (yes)
    {
        appendImageToHistory();
        setWindowModified(! isCurrentLastSaved());
    }
}

void MainWindow::selectWasChanged(QRect select)
{
    QRect area = Utils::selectImageCoords(select);
    QSize size = imageView->getImageSize();

    QString text = tr("Image size: %1x%2; selected area: %3x%4 - %5x%6")
        .arg(size.width())
        .arg(size.height())
        .arg(area.x())
        .arg(area.y())
        .arg(area.x() + area.width() - 1)
        .arg(area.y() + area.height() - 1);

    statusBar()->showMessage(text);
}

void MainWindow::setCurrentFile(const QString & filePath)
{
    currentFilePath = filePath;

    if (currentFilePath.isEmpty())
    {
        setWindowFilePath("ImageProcessor");
    }
    else
    {
        setWindowFilePath(currentFilePath + " - ImageProcessor");
    }
}

void MainWindow::createActions()
{
    /* ---- File ---- */

    openAct = new QAction(QIcon(":/icons/document-open.svg"),
        tr("&Open..."), this);
    openAct->setStatusTip(tr("Open an image"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/icons/document-save.svg"),
        tr("&Save"), this);
    saveAct->setStatusTip(tr("Save the image under his original name"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(QIcon(":/icons/document-save-as.svg"),
        tr("Save &as..."), this);
    saveAsAct->setStatusTip(tr("Save the image under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    closeImageAct = new QAction(QIcon(":/icons/window-close.svg"),
        tr("&Close image"), this);
    closeImageAct->setStatusTip(tr("Close image and free all resources"));
    connect(closeImageAct, SIGNAL(triggered()), this, SLOT(closeImage()));

    exitAct = new QAction(QIcon(":/icons/application-exit.svg"),
        tr("E&xit"), this);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    /* ---- Edit ---- */

    undoAct = new QAction(QIcon(":/icons/edit-undo.svg"),
        tr("&Undo"), this);
    undoAct->setStatusTip(tr("Undo your changes"));
    undoAct->setShortcut(QKeySequence(tr("Ctrl+Left")));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(QIcon(":/icons/edit-redo.svg"),
        tr("&Redo"), this);
    redoAct->setStatusTip(tr("Redo (repeat) your changes"));
    redoAct->setShortcut(QKeySequence(tr("Ctrl+Right")));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    /* ---- Filters ---- */

    autolevelsFilterAct = new QAction(tr("Autolevels..."), this);
    autolevelsFilterAct->setShortcut(QKeySequence(tr("Ctrl+L")));
    connect(autolevelsFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startAutolevelsFilter()));

    autocontrastFilterAct = new QAction(tr("Autocontrast..."), this);
    autocontrastFilterAct->setShortcut(QKeySequence(tr("Ctrl+C")));
    connect(autocontrastFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startAutocontrastFilter()));

    grayscaleFilterAct = new QAction(tr("Grayscale"), this);
    grayscaleFilterAct->setShortcut(QKeySequence(tr("Ctrl+X")));
    connect(grayscaleFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startGrayscaleFilter()));

    grayworldFilterAct = new QAction(tr("Gray-world"), this);
    grayworldFilterAct->setShortcut(QKeySequence(tr("Ctrl+A")));
    connect(grayworldFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startGrayworldFilter()));

    medianFilterAct = new QAction(tr("Median..."), this);
    medianFilterAct->setShortcut(QKeySequence(tr("Ctrl+M")));
    connect(medianFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startMedianFilter()));

    gaussianFilterAct = new QAction(tr("Gaussian..."), this);
    gaussianFilterAct->setShortcut(QKeySequence(tr("Ctrl+G")));
    connect(gaussianFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startGaussianFilter()));

    sobelFilterAct = new QAction(tr("Sobel..."), this);
    sobelFilterAct->setShortcut(QKeySequence(tr("Ctrl+S")));
    connect(sobelFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startSobelFilter()));

    customFilterAct = new QAction(tr("Custom..."), this);
    customFilterAct->setShortcut(QKeySequence(tr("Ctrl+K")));
    connect(customFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startCustomFilter()));

    rotateFilterAct = new QAction(tr("Rotate..."), this);
    rotateFilterAct->setShortcut(QKeySequence(tr("Ctrl+R")));
    connect(rotateFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startRotateFilter()));

    resizeFilterAct = new QAction(tr("Resize..."), this);
    resizeFilterAct->setShortcut(QKeySequence(tr("Ctrl+Z")));
    connect(resizeFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startResizeFilter()));

    waveFilterAct = new QAction(tr("Wave..."), this);
    waveFilterAct->setShortcut(QKeySequence(tr("Ctrl+W")));
    connect(waveFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startWaveFilter()));

    glassFilterAct = new QAction(tr("Glass..."), this);
    glassFilterAct->setShortcut(QKeySequence(tr("Ctrl+Q")));
    connect(glassFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startGlassFilter()));

    glowingEdgeFilterAct = new QAction(tr("GlowingEdge"), this);
    glowingEdgeFilterAct->setShortcut(QKeySequence(tr("Ctrl+I")));
    connect(glowingEdgeFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startGlowingEdgeFilter()));

    averageSobelFilterAct = new QAction(tr("Average Sobel..."), this);
    averageSobelFilterAct->setShortcut(QKeySequence(tr("Ctrl+O")));
    connect(averageSobelFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startAverageSobelFilter()));

    sharpenFilterAct = new QAction(tr("Sharpen"), this);
    sharpenFilterAct->setShortcut(QKeySequence(tr("Ctrl+P")));
    connect(sharpenFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startSharpenFilter()));

    maximumFilterAct = new QAction(tr("Maximum..."), this);
    maximumFilterAct->setShortcut(QKeySequence(tr("Ctrl+E")));
    connect(maximumFilterAct, SIGNAL(triggered()), imageView,
        SLOT(startMaximumFilter()));

    /* ---- Help ---- */

    aboutAct = new QAction(QIcon(":/icons/help-about.svg"),
        tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the ImageProcessor's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(QIcon(":/icons/qt-logo.svg"),
        tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(closeImageAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    filtersMenu = menuBar()->addMenu(tr("F&ilters"));
    filtersMenu->addAction(autolevelsFilterAct);
    filtersMenu->addAction(autocontrastFilterAct);
    filtersMenu->addAction(grayscaleFilterAct);
    filtersMenu->addAction(grayworldFilterAct);
    filtersMenu->addAction(medianFilterAct);
    filtersMenu->addAction(gaussianFilterAct);
    filtersMenu->addAction(sobelFilterAct);
    filtersMenu->addAction(customFilterAct);
    filtersMenu->addAction(rotateFilterAct);
    filtersMenu->addAction(resizeFilterAct);
    filtersMenu->addAction(waveFilterAct);
    filtersMenu->addAction(glassFilterAct);
    filtersMenu->addAction(glowingEdgeFilterAct);
    filtersMenu->addAction(averageSobelFilterAct);
    filtersMenu->addAction(sharpenFilterAct);
    filtersMenu->addAction(maximumFilterAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(saveAsAct);
    fileToolBar->addAction(closeImageAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAct);
    editToolBar->addAction(redoAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::setEnabledActions()
{
    bool imageLoaded = ! imageView->isNull();

    saveAct->setEnabled(imageLoaded);
    saveAsAct->setEnabled(imageLoaded);
    closeImageAct->setEnabled(imageLoaded);

    connect(imageView, SIGNAL(hasImage(bool)), saveAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), saveAsAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), closeImageAct,
        SLOT(setEnabled(bool)));

    undoAct->setEnabled(imageLoaded);
    redoAct->setEnabled(imageLoaded);

    connect(imageView, SIGNAL(hasImage(bool)), undoAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), redoAct,
        SLOT(setEnabled(bool)));

    imageView->setVisible(imageLoaded);

    connect(imageView, SIGNAL(hasImage(bool)), imageView,
        SLOT(setVisible(bool)));

    autolevelsFilterAct->setEnabled(imageLoaded);
    autocontrastFilterAct->setEnabled(imageLoaded);
    grayscaleFilterAct->setEnabled(imageLoaded);
    grayworldFilterAct->setEnabled(imageLoaded);
    medianFilterAct->setEnabled(imageLoaded);
    gaussianFilterAct->setEnabled(imageLoaded);
    sobelFilterAct->setEnabled(imageLoaded);
    customFilterAct->setEnabled(imageLoaded);
    rotateFilterAct->setEnabled(imageLoaded);
    resizeFilterAct->setEnabled(imageLoaded);
    waveFilterAct->setEnabled(imageLoaded);
    glassFilterAct->setEnabled(imageLoaded);
    glowingEdgeFilterAct->setEnabled(imageLoaded);
    averageSobelFilterAct->setEnabled(imageLoaded);
    sharpenFilterAct->setEnabled(imageLoaded);
    maximumFilterAct->setEnabled(imageLoaded);

    connect(imageView, SIGNAL(hasImage(bool)), autolevelsFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), autocontrastFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), grayscaleFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), grayworldFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), medianFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), gaussianFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), sobelFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), customFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), rotateFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), resizeFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), waveFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), glassFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), glowingEdgeFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), averageSobelFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), sharpenFilterAct,
        SLOT(setEnabled(bool)));
    connect(imageView, SIGNAL(hasImage(bool)), maximumFilterAct,
        SLOT(setEnabled(bool)));
}

bool MainWindow::maybeSave()
{
    if (! isCurrentLastSaved())
    {
        QMessageBox::StandardButton ret =
            QMessageBox::warning(this, tr("ImageProcessor"),
            tr("The image has been modified.\n"
                "Do you want to select file name\n"
                "and save your changes?"),
            QMessageBox::Save | QMessageBox::Discard |
            QMessageBox::Cancel);

        if (ret == QMessageBox::Save)
        {
            return saveAs();
        }

        /* Cancel or discard */
        return (ret == QMessageBox::Discard);
    }

    return true;
}

void MainWindow::loadFile(const QString & filePath)
{
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    QImage * newImage = new QImage(filePath);

    if (newImage->format() != QImage::Format_RGB32)
    {
        /* For working with image with QImage::bits()
         * and qRed(), qGreen() and qBlue() functions. */
        *newImage = newImage->convertToFormat(QImage::Format_RGB32);
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    if (newImage->isNull())
    {
        delete newImage;

        QMessageBox::warning(this, tr("ImageProcessor"),
            tr("Cannot read file %1.").arg(filePath));

        return;
    }

    setCurrentFile(filePath);
    imageView->setNewImage(newImage, true);
    clearHistory(true);

    statusBar()->showMessage(tr("Image loaded"), MESSAGE_SHOWING_TIME_MS);
}

bool MainWindow::saveFile(const QString & filePath)
{
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    bool ok = imageView->getImage()->save(filePath);

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    if (!ok)
    {
        QMessageBox::warning(this, tr("ImageProcessor"),
            tr("Cannot write file %1.").arg(filePath));

        return false;
    }

    setCurrentFile(filePath);
    setCurrentLastSaved();
    setWindowModified(! isCurrentLastSaved());

    statusBar()->showMessage(tr("Image saved"), MESSAGE_SHOWING_TIME_MS);

    return true;
}

void MainWindow::clearHistory(bool appendCurrent)
{
    QListIterator<QImage *> i(imageHistory);

    while (i.hasNext())
    {
        delete i.next();
    }

    imageHistory.clear();
    lastSaved = -1;
    currentInHistory = -1;

    if (appendCurrent)
    {
        appendImageToHistory();
        setCurrentLastSaved();
        setWindowModified(! isCurrentLastSaved());
    }
}

void MainWindow::appendImageToHistory()
{
    /* Delete all after current */
    while (currentInHistory >= 0
        && currentInHistory < imageHistory.size() - 1)
    {
        delete imageHistory.takeLast();
    }

    imageHistory.append(imageView->getImage());
    ++currentInHistory;

    undoAct->setEnabled(hasPreviousInHistory());
    redoAct->setEnabled(hasNextInHistory());
}

void MainWindow::setCurrentInHistory(int current)
{
    currentInHistory = current;
    imageView->setNewImage(imageHistory.at(currentInHistory), false);

    undoAct->setEnabled(hasPreviousInHistory());
    redoAct->setEnabled(hasNextInHistory());

    setWindowModified(! isCurrentLastSaved());
}

void MainWindow::setCurrentLastSaved()
{
    lastSaved = currentInHistory;
}

bool MainWindow::isCurrentLastSaved() const
{
    return (currentInHistory == lastSaved);
}

bool MainWindow::hasPreviousInHistory() const
{
    return (currentInHistory > 0);
}

bool MainWindow::hasNextInHistory() const
{
    return (currentInHistory < (imageHistory.size() - 1));
}
