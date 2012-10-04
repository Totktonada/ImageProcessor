#include <QtGui>
#include "SobelFilterDialog.hpp"
    
SobelFilterDialog::SobelFilterDialog(SobelFilterDialogType aType,
    QWidget * parent)
    : QDialog(parent),
    type(aType)
{
    QLabel * firstLabel;
    
    if (type == SobelValues)
    {
        setWindowTitle(tr("Sobel filter"));
        firstLabel = new QLabel("Edge: ", this);
    }
    else /* (type == AverageSobelValues) */
    {
        setWindowTitle(tr("Average sobel filter"));
        firstLabel = new QLabel("What detect: ", this);
    }

    QLabel * secondLabel = new QLabel("What detect: ", this);

    firstLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    secondLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    firstComboBox = new QComboBox(this);

    if (type == SobelValues)
    {
        firstComboBox->addItem("Horizontal");
        firstComboBox->addItem("Vertical");
        firstComboBox->setEditable(false);
        firstComboBox->setCurrentIndex(0);
    }
    else /* (type == AverageSobelValues) */
    {
        firstComboBox->addItem("Increment");
        firstComboBox->addItem("Decrement");
        firstComboBox->addItem("Both");
        firstComboBox->setEditable(false);
        firstComboBox->setCurrentIndex(1);
    }

    secondComboBox = new QComboBox(this);
    secondComboBox->addItem("Increment");
    secondComboBox->addItem("Decrement");
    secondComboBox->addItem("Both");
    secondComboBox->setEditable(false);
    secondComboBox->setCurrentIndex(1);

    QDialogButtonBox * buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    mainLayout->addWidget(firstLabel);
    mainLayout->addWidget(firstComboBox);
    mainLayout->addWidget(secondLabel);
    mainLayout->addWidget(secondComboBox);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

SobelFilterDialog::~SobelFilterDialog()
{}

QString SobelFilterDialog::getFirstValue() const
{
    return firstComboBox->currentText();
}

QString SobelFilterDialog::getSecondValue() const
{
    return secondComboBox->currentText();
}

void SobelFilterDialog::getSobelValues(QWidget * parent, SobelFilter::Edge * edge,
    SobelFilter::WhatDetect * what, bool * ok)
{
    SobelFilterDialog * dialog = new SobelFilterDialog(SobelValues, parent);

    if (dialog->exec() == QDialog::Rejected)
    {
        *ok = false;
        return;
    }

    *ok = true;

    QString str = dialog->getFirstValue();

    *edge =
        (str == "Horizontal") ? SobelFilter::Horizontal :
        SobelFilter::Vertical;

    str = dialog->getSecondValue();

    *what =
        (str == "Increment") ? SobelFilter::Increment :
        (str == "Decrement") ? SobelFilter::Decrement :
        SobelFilter::Both;
}

void SobelFilterDialog::getAverageSobelValues(QWidget * parent,
    SobelFilter::WhatDetect * whatH,
    SobelFilter::WhatDetect * whatV, bool * ok)
{
    SobelFilterDialog * dialog =
        new SobelFilterDialog(AverageSobelValues, parent);

    if (dialog->exec() == QDialog::Rejected)
    {
        *ok = false;
        return;
    }

    *ok = true;

    QString str = dialog->getFirstValue();

    *whatH =
        (str == "Increment") ? SobelFilter::Increment :
        (str == "Decrement") ? SobelFilter::Decrement :
        SobelFilter::Both;

    str = dialog->getSecondValue();

    *whatV =
        (str == "Increment") ? SobelFilter::Increment :
        (str == "Decrement") ? SobelFilter::Decrement :
        SobelFilter::Both;
}
