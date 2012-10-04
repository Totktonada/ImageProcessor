#include <limits.h>
#include <QtGui>
#include "WaveFilterDialog.hpp"

#define STD_WAVE_AMPLITUDE 20
#define STD_WAVE_PERIOD 80

WaveFilterDialog::WaveFilterDialog(QWidget * parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Wave filter"));

    QLabel * orientationLabel = new QLabel("Orientation: ", this);
    QLabel * amplitudeLabel = new QLabel("Amplitude: ", this);
    QLabel * periodLabel = new QLabel("Period: ", this);

    orientationLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    amplitudeLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    periodLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    orientationBox = new QComboBox(this);
    orientationBox->addItem("Horizontal");
    orientationBox->addItem("Vertical");
    orientationBox->setEditable(false);
    orientationBox->setCurrentIndex(0);

    amplitudeBox = new QDoubleSpinBox();
    amplitudeBox->setDecimals(1);
    amplitudeBox->setRange(0, INT_MAX);
    amplitudeBox->setValue(STD_WAVE_AMPLITUDE);

    periodBox = new QDoubleSpinBox();
    periodBox->setDecimals(1);
    periodBox->setRange(0, INT_MAX);
    periodBox->setValue(STD_WAVE_PERIOD);

    QDialogButtonBox * buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    mainLayout->addWidget(orientationLabel);
    mainLayout->addWidget(orientationBox);
    mainLayout->addWidget(amplitudeLabel);
    mainLayout->addWidget(amplitudeBox);
    mainLayout->addWidget(periodLabel);
    mainLayout->addWidget(periodBox);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

WaveFilterDialog::~WaveFilterDialog()
{}

WaveFilter::Orientation WaveFilterDialog::getOrientation() const
{
    return (orientationBox->currentText() == "Horizontal") ?
        WaveFilter::Horizontal : WaveFilter::Vertical;
}

double WaveFilterDialog::getAmplitude() const
{
    return amplitudeBox->value();
}

double WaveFilterDialog::getPeriod() const
{
    return periodBox->value();
}

void WaveFilterDialog::getValues(QWidget * parent,
    WaveFilter::Orientation * orientation,
    double * amplitude, double * period, bool * ok)
{
    WaveFilterDialog * dialog = new WaveFilterDialog(parent);

    if (dialog->exec() == QDialog::Rejected)
    {
        *ok = false;
        return;
    }

    *ok = true;

    *orientation = dialog->getOrientation();
    *amplitude = dialog->getAmplitude();
    *period = dialog->getPeriod();
}
