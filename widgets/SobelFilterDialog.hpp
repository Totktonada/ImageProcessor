#ifndef SOBEL_FILTER_DIALOG_HPP_SENTRY
#define SOBEL_FILTER_DIALOG_HPP_SENTRY

#include <QDialog>
#include "../filters/SobelFilter.hpp"
class QComboBox;

class SobelFilterDialog : public QDialog
{
    Q_OBJECT

public:
    enum SobelFilterDialogType
    {
        SobelValues,
        AverageSobelValues
    };

    SobelFilterDialog(SobelFilterDialogType aType = SobelValues,
        QWidget * parent = 0);
    ~SobelFilterDialog();

    QString getFirstValue() const;
    QString getSecondValue() const;

    static void getSobelValues(QWidget * parent, SobelFilter::Edge * edge,
        SobelFilter::WhatDetect * what, bool * ok);

    static void getAverageSobelValues(QWidget * parent,
        SobelFilter::WhatDetect * whatH,
        SobelFilter::WhatDetect * whatV, bool * ok);

private:
    SobelFilterDialogType type;

    QComboBox * firstComboBox;
    QComboBox * secondComboBox;
};

#endif /* SOBEL_FILTER_DIALOG_HPP_SENTRY */
