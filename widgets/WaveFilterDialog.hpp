#ifndef WAVE_FILTER_DIALOG_HPP_SENTRY
#define WAVE_FILTER_DIALOG_HPP_SENTRY

#include <QDialog>
#include "../filters/WaveFilter.hpp"
class QComboBox;
class QDoubleSpinBox;

class WaveFilterDialog : public QDialog
{
    Q_OBJECT

public:
    WaveFilterDialog(QWidget * parent = 0);
    ~WaveFilterDialog();

    WaveFilter::Orientation getOrientation() const;

    double getAmplitude() const;

    double getPeriod() const;

    static void getValues(QWidget * parent,
        WaveFilter::Orientation * orientation,
        double * amplitude, double * period, bool * ok);

private:
    QComboBox * orientationBox;
    QDoubleSpinBox * amplitudeBox;
    QDoubleSpinBox * periodBox;
};

#endif /* WAVE_FILTER_DIALOG_HPP_SENTRY */
