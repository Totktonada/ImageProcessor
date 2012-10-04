#ifndef AUTO_L_C_FILTER_DIALOG_HPP_SENTRY
#define AUTO_L_C_FILTER_DIALOG_HPP_SENTRY

#include <QDialog>
class QDoubleSpinBox;

class AutoLCFilterDialog : public QDialog
{
    Q_OBJECT

public:
    AutoLCFilterDialog(QWidget * parent = 0);
    ~AutoLCFilterDialog();

    double getSkipLeft() const;

    double getSkipRight() const;

    static void getValues(QWidget * parent,
        double * skipLeft, double * skipRight, bool * ok);

private:
    QDoubleSpinBox * skipLeftBox;
    QDoubleSpinBox * skipRightBox;
};

#endif /* AUTO_L_C_FILTER_DIALOG_HPP_SENTRY */
