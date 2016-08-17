#include "molecule.h"
#include "setbasisdialog.h"
#include "ui_setbasisdialog.h"

setBasisDialog::setBasisDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setBasisDialog)
{
    ui->setupUi(this);
    m_window = static_cast<MolconvWindow*>(parent);
}

setBasisDialog::~setBasisDialog()
{
    delete ui;
}

void setBasisDialog::prepare(molconv::moleculePtr &molecule)
{
    ui->originAtomList->clear();
    ui->basisAtomList->clear();
    ui->originAllCheckBox->setCheckState(Qt::Checked);
    ui->basisAllCheckBox->setCheckState(Qt::Checked);

    for (int i = 0; i < int(molecule->size()); i++)
    {
        std::string currentName = molecule->atom(i)->symbol() + std::to_string(i + 1);

        QListWidgetItem *originItem = new QListWidgetItem(QString::fromStdString(currentName), ui->originAtomList);
        originItem->setCheckState(Qt::Checked);
        ui->originAtomList->addItem(originItem);

        QListWidgetItem *basisItem = new QListWidgetItem(QString::fromStdString(currentName), ui->basisAtomList);
        basisItem->setCheckState(Qt::Checked);
        ui->basisAtomList->addItem(basisItem);
    }

    molconv::origin currentOrigin = molecule->internalOrigin();
    molconv::basis currentBasis = molecule->internalBasis();

    // standard orientation is not available at the moment
    ui->basisStandard->setEnabled(false);

    switch (currentOrigin)
    {
    case molconv::kCenterOfMass:
        ui->originCOM->setChecked(true);
        break;
    case molconv::kCenterOfGeometry:
        ui->originCOG->setChecked(true);
        break;
    case molconv::kCenterOfCharge:
        ui->originCOC->setChecked(true);
        break;
    case molconv::kCenterOnAtom:
        ui->originAtomPos->setChecked(true);
        break;
    case molconv::kCenterBetweenAtoms:
        ui->originAtomLine->setChecked(true);
        break;
    }

    switch (currentBasis)
    {
    case molconv::kInertiaVectors:
        ui->basisInert->setChecked(true);
        break;
    case molconv::kCovarianceVectors:
        ui->basisCovar->setChecked(true);
        break;
    case molconv::kChargeVectors:
        ui->basisCharge->setChecked(true);
        break;
    case molconv::kVectorsFromAtoms:
        ui->originAtomPos->setChecked(true);
        break;
    }

}

void setBasisDialog::on_originAllCheckBox_stateChanged()
{
    if (ui->originAllCheckBox->checkState() == Qt::Checked)
        for (int i = 0; i < ui->originAtomList->count(); i++)
            ui->originAtomList->item(i)->setCheckState(Qt::Checked);
    else
        for (int i = 0; i < ui->originAtomList->count(); i++)
            ui->originAtomList->item(i)->setCheckState(Qt::Unchecked);
}

void setBasisDialog::on_basisAllCheckBox_stateChanged()
{
    if (ui->basisAllCheckBox->checkState() == Qt::Checked)
        for (int i = 0; i < ui->basisAtomList->count(); i++)
            ui->basisAtomList->item(i)->setCheckState(Qt::Checked);
    else
        for (int i = 0; i < ui->basisAtomList->count(); i++)
            ui->basisAtomList->item(i)->setCheckState(Qt::Unchecked);
}

void setBasisDialog::on_originAtomPos_toggled(bool checked)
{
    ui->originAtom->setEnabled(checked);
}

void setBasisDialog::on_originAtomLine_toggled(bool checked)
{
    ui->originALineStart->setEnabled(checked);
    ui->originALineEnd->setEnabled(checked);
    ui->originALineScaleBox->setEnabled(checked);
    ui->originALineScaleSlider->setEnabled(checked);
}

void setBasisDialog::on_basisAtoms_toggled(bool checked)
{
    ui->basisAtom1->setEnabled(checked);
    ui->basisAtom2->setEnabled(checked);
    ui->basisAtom3->setEnabled(checked);
}

void setBasisDialog::on_originALineScaleSlider_valueChanged(int value)
{
    ui->originALineScaleBox->setValue(double(value / 10000));
}

void setBasisDialog::on_originALineScaleBox_valueChanged(double value)
{
    ui->originALineScaleSlider->setValue(int(value * 10000));
}
