#include <limits.h>
#include <QtGui>
#include "AutoLCFilterDialog.hpp"

#define STD_SKIP 0.05

AutoLCFilterDialog::AutoLCFilterDialog(QWidget * parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Autolevels or Autocontrast filter"));

    QLabel * skipLeftLabel = new QLabel("Skip left: ", this);
    QLabel * skipRightLabel = new QLabel("Skip right: ", this);

    skipLeftLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    skipRightLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    skipLeftBox = new QDoubleSpinBox();
    skipLeftBox->setDecimals(2);
    skipLeftBox->setRange(0.0, 1.0);
    skipLeftBox->setSingleStep(0.1);
    skipLeftBox->setValue(STD_SKIP);

    skipRightBox = new QDoubleSpinBox();
    skipRightBox->setDecimals(2);
    skipRightBox->setRange(0.0, 1.0);
    skipRightBox->setSingleStep(0.1);
    skipRightBox->setValue(STD_SKIP);

    QDialogButtonBox * buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    mainLayout->addWidget(skipLeftLabel);
    mainLayout->addWidget(skipLeftBox);
    mainLayout->addWidget(skipRightLabel);
    mainLayout->addWidget(skipRightBox);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

AutoLCFilterDialog::~AutoLCFilterDialog()
{}

double AutoLCFilterDialog::getSkipLeft() const
{
    return skipLeftBox->value();
}

double AutoLCFilterDialog::getSkipRight() const
{
    return skipRightBox->value();
}

void AutoLCFilterDialog::getValues(QWidget * parent,
    double * skipLeft, double * skipRight, bool * ok)
{
    AutoLCFilterDialog * dialog = new AutoLCFilterDialog(parent);

    if (dialog->exec() == QDialog::Rejected)
    {
        *ok = false;
        return;
    }

    *ok = true;

    *skipLeft = dialog->getSkipLeft();
    *skipRight = dialog->getSkipRight();
}
