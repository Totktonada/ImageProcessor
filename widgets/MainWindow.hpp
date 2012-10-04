#ifndef MAIN_WINDOW_HPP_SENTRY
#define MAIN_WINDOW_HPP_SENTRY

#include <QMainWindow>
class QScrollArea;
class ImageView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString & filePath = QString(), QWidget * parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void open();
    bool save();
    bool saveAs();
    void undo();
    void redo();
    void closeImage();
    void about();

    void imageWasFiltered(bool yes);
    void selectWasChanged(QRect select);

private:
    void setCurrentFile(const QString & filePath);

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void setEnabledActions();

    /* Returns false, if image modifications not saved and
     * user probably want to save it. Otherwise, true. */
    bool maybeSave();

    void loadFile(const QString & filePath);
    bool saveFile(const QString & filePath);

    void clearHistory(bool appendCurrent);
    void appendImageToHistory();
    void setCurrentInHistory(int current);
    void setCurrentLastSaved();
    bool isCurrentLastSaved() const;
    bool hasPreviousInHistory() const;
    bool hasNextInHistory() const;

    /* ---- Fields ---- */

    QList <QImage *> imageHistory;
    int lastSaved;
    int currentInHistory;

    QString currentFilePath;
    QScrollArea * scrollArea;
    ImageView * imageView;

    QMenu * fileMenu;
    QMenu * editMenu;
    QMenu * filtersMenu;
    QMenu * helpMenu;

    QToolBar * fileToolBar;
    QToolBar * editToolBar;

    /* File menu */
    QAction * openAct;
    QAction * saveAct;
    QAction * saveAsAct;
    QAction * closeImageAct;
    QAction * exitAct;

    /* Edit menu */
    QAction * undoAct;
    QAction * redoAct;

    /* Filters menu */
    QAction * autolevelsFilterAct;
    QAction * autocontrastFilterAct;
    QAction * grayscaleFilterAct;
    QAction * grayworldFilterAct;
    QAction * medianFilterAct;
    QAction * gaussianFilterAct;
    QAction * sobelFilterAct;
    QAction * customFilterAct;
    QAction * rotateFilterAct;
    QAction * resizeFilterAct;
    QAction * waveFilterAct;
    QAction * glassFilterAct;
    QAction * glowingEdgeFilterAct;
    QAction * averageSobelFilterAct;
    QAction * sharpenFilterAct;
    QAction * maximumFilterAct;

    /* Help menu */
    QAction * aboutAct;
    QAction * aboutQtAct;
};

#endif /* MAIN_WINDOW_HPP_SENTRY */
