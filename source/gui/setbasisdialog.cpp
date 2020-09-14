/*
 * Copyright 2014 - 2019 Jan von Cosel & Sebastian Lenz
 *
 * This file is part of molconv.
 *
 * molconv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * molconv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have recieved a copy of the GNU General Public License
 * along with molconv. If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "molecule.h"
#include "moleculeorigin.h"
#include "moleculebasis.h"
#include "setbasisdialog.h"
#include "ui_setbasisdialog.h"

SetBasisDialog::SetBasisDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setBasisDialog)
{
    ui->setupUi(this);
    m_mainWindow = static_cast<MolconvWindow*>(parent);

    m_originAtoms.fill(0);
    m_basisAtoms.fill(0);
    m_AtomLineScale = 0.0;

    ui->originAllCheckBox->setCheckState(Qt::Checked);
    ui->basisAllCheckBox->setCheckState(Qt::Checked);
}

SetBasisDialog::~SetBasisDialog()
{
    delete ui;
}

void SetBasisDialog::prepare(unsigned long molID)
{
    m_molID = molID;
    molconv::moleculePtr molecule = m_mainWindow->getMol(molID);

    setWindowTitle("Internal basis of molecule '" + QString::fromStdString(molecule->name()) + "'");

    ui->originAtomList->clear();
    ui->basisAtomList->clear();

    for (int i = 0; i < int(molecule->size()); i++)
    {
        std::string currentName = molecule->atom(i)->symbol() + std::to_string(i + 1);

        QListWidgetItem *originItem = new QListWidgetItem(QString::fromStdString(currentName), ui->originAtomList);
        if (molecule->originList().size() > 0 && molecule->originList().at(i))
        {
            originItem->setCheckState(Qt::Checked);
        }
        else
        {
            originItem->setCheckState(Qt::Unchecked);
        }
        ui->originAtomList->addItem(originItem);

        QListWidgetItem *basisItem = new QListWidgetItem(QString::fromStdString(currentName), ui->basisAtomList);
        if (molecule->basisList().size() > 0 && molecule->basisList().at(i))
        {
            basisItem->setCheckState(Qt::Checked);
        }
        else
        {
            basisItem->setCheckState(Qt::Unchecked);
        }
        ui->basisAtomList->addItem(basisItem);
    }

    m_originCode = molecule->internalOrigin()->code();
    m_basisCode = molecule->internalBasis()->code();

    // standard orientation is not available at the moment
    ui->basisStandard->setEnabled(false);

    ui->originAtom->setMaximum(molecule->size());

    switch (m_originCode)
    {
    case molconv::kCenterOfMass:
        ui->originCOM->setChecked(true);
        on_originCOM_toggled(true);
        on_originAtomPos_toggled(false);
        on_originAtomLine_toggled(false);
        break;
    case molconv::kCenterOfGeometry:
        on_originCOG_toggled(true);
        on_originAtomPos_toggled(false);
        on_originAtomLine_toggled(false);
        ui->originCOG->setChecked(true);
        break;
    case molconv::kCenterOfCharge:
        on_originCOC_toggled(true);
        on_originAtomPos_toggled(false);
        on_originAtomLine_toggled(false);
        ui->originCOC->setChecked(true);
        break;
    case molconv::kCenterOnAtom:
        ui->originAtomPos->setChecked(true);
        m_originAtoms = molecule->internalOriginAtoms();
        ui->originAtom->setValue(m_originAtoms[0] + 1);
        on_originCOC_toggled(false);
        on_originAtomLine_toggled(false);
        break;
    case molconv::kCenterBetweenAtoms:
        ui->originAtomLine->setChecked(true);
        on_originAtomPos_toggled(false);
        on_originCOC_toggled(false);
        m_originAtoms = molecule->internalOriginAtoms();
        ui->originALineStart->setValue(m_originAtoms[0] + 1);
        ui->originALineEnd->setValue(m_originAtoms[1] + 1);
        break;
    }

    switch (m_basisCode)
    {
    case molconv::kInertiaVectors:
        ui->basisInert->setChecked(true);
        on_basisInert_toggled(true);
        on_basisAtoms_toggled(false);
        break;
    case molconv::kCovarianceVectors:
        ui->basisCovar->setChecked(true);
        on_basisCovar_toggled(true);
        on_basisAtoms_toggled(false);
        break;
    case molconv::kChargeVectors:
        ui->basisCharge->setChecked(true);
        on_basisCharge_toggled(true);
        on_basisAtoms_toggled(false);
        break;
    case molconv::kStandardOrientation:
        break;
    case molconv::kVectorsFromAtoms:
        on_basisCharge_toggled(false);
        ui->basisAtoms->setChecked(true);
        m_basisAtoms = molecule->internalBasisAtoms();
        ui->basisAtom1->setValue(m_basisAtoms[0] + 1);
        ui->basisAtom2->setValue(m_basisAtoms[1] + 1);
        ui->basisAtom3->setValue(m_basisAtoms[2] + 1);
        break;
    }
}

molconv::OriginCode SetBasisDialog::originCode() const
{
    return m_originCode;
}

molconv::BasisCode SetBasisDialog::basisCode() const
{
    return m_basisCode;
}

std::array<int,2> SetBasisDialog::originAtoms() const
{
    return m_originAtoms;
}

std::array<int,3> SetBasisDialog::basisAtoms() const
{
    return m_basisAtoms;
}

double SetBasisDialog::atomLineScale() const
{
    return m_AtomLineScale;
}

std::vector<bool> SetBasisDialog::selectedOriginAtoms() const
{
    return m_originList;
}

std::vector<bool> SetBasisDialog::selectedBasisAtoms() const
{
    return m_basisList;
}

void SetBasisDialog::on_originAllCheckBox_stateChanged()
{
    if (ui->originAllCheckBox->checkState() == Qt::Checked)
        for (int i = 0; i < ui->originAtomList->count(); i++)
            ui->originAtomList->item(i)->setCheckState(Qt::Checked);
    else
        for (int i = 0; i < ui->originAtomList->count(); i++)
            ui->originAtomList->item(i)->setCheckState(Qt::Unchecked);
}

void SetBasisDialog::on_basisAllCheckBox_stateChanged()
{
    if (ui->basisAllCheckBox->checkState() == Qt::Checked)
        for (int i = 0; i < ui->basisAtomList->count(); i++)
            ui->basisAtomList->item(i)->setCheckState(Qt::Checked);
    else
        for (int i = 0; i < ui->basisAtomList->count(); i++)
            ui->basisAtomList->item(i)->setCheckState(Qt::Unchecked);
}

void SetBasisDialog::on_originAtomPos_toggled(bool checked)
{
    ui->originAtom->setEnabled(checked);
}

void SetBasisDialog::on_originAtomLine_toggled(bool checked)
{
    ui->originALineStart->setEnabled(checked);
    ui->originALineEnd->setEnabled(checked);
    ui->originALineScaleBox->setEnabled(checked);
    ui->originALineScaleSlider->setEnabled(checked);
}

void SetBasisDialog::on_basisAtoms_toggled(bool checked)
{
    ui->basisAtom1->setEnabled(checked);
    ui->basisAtom2->setEnabled(checked);
    ui->basisAtom3->setEnabled(checked);
}

void SetBasisDialog::on_originALineScaleSlider_valueChanged(int value)
{
    double realValue = double(value) / 1000.0;
    ui->originALineScaleBox->setValue(realValue);
}

void SetBasisDialog::on_originALineScaleBox_valueChanged(double value)
{
    int intValue = int(value * 1000.0);
    ui->originALineScaleSlider->setValue(intValue);
}

void SetBasisDialog::on_buttonBox_accepted()
{
    // set the origin from the UI elements:
    m_originAtoms.fill(0);
    if (ui->originCOM->isChecked())
        m_originCode = molconv::kCenterOfMass;
    else if (ui->originCOG->isChecked())
        m_originCode = molconv::kCenterOfGeometry;
    else if (ui->originCOC->isChecked())
        m_originCode = molconv::kCenterOfCharge;
    else if (ui->originAtomPos->isChecked())
    {
        m_originCode = molconv::kCenterOnAtom;
        m_originAtoms[0] = ui->originAtom->value() - 1;
        m_originAtoms[1] = 0;
    }
    else if (ui->originAtomLine->isChecked())
    {
        m_originCode = molconv::kCenterBetweenAtoms;
        m_originAtoms[0] = ui->originALineStart->value() - 1;
        m_originAtoms[1] = ui->originALineEnd->value() - 1;
        m_AtomLineScale = ui->originALineScaleBox->value();
    }

    // set the basis from the UI elements:
    m_basisAtoms.fill(0);
    if (ui->basisInert->isChecked())
        m_basisCode = molconv::kInertiaVectors;
    else if (ui->basisCovar->isChecked())
        m_basisCode = molconv::kCovarianceVectors;
    else if (ui->basisCharge->isChecked())
        m_basisCode = molconv::kChargeVectors;
    else if (ui->basisAtoms->isChecked())
    {
        m_basisCode = molconv::kVectorsFromAtoms;
        m_basisAtoms[0] = ui->basisAtom1->value() - 1;
        m_basisAtoms[1] = ui->basisAtom2->value() - 1;
        m_basisAtoms[2] = ui->basisAtom3->value() - 1;
    }

    // if available, set the list of atoms contributing to origin/basis:
    m_originList.clear();
    m_basisList.clear();

    int nAtoms = m_mainWindow->getMol(m_molID)->size();

    if (ui->originAtomList->isEnabled())
        for (int i = 0; i < nAtoms; i++)
        {
            if (ui->originAtomList->item(i)->checkState() ==  Qt::Checked)
                m_originList.push_back(true);
            else
                m_originList.push_back(false);
        }

    if (ui->basisAtomList->isEnabled())
        for (int i = 0; i < nAtoms; i++)
        {
            if (ui->basisAtomList->item(i)->checkState() ==  Qt::Checked)
                m_basisList.push_back(true);
            else
                m_basisList.push_back(false);
        }

    emit ready();
}

void SetBasisDialog::on_originCOM_toggled(bool checked)
{
    ui->originAtomList->setEnabled(checked);
    ui->originAllCheckBox->setEnabled(checked);
}

void SetBasisDialog::on_originCOG_toggled(bool checked)
{
    ui->originAtomList->setEnabled(checked);
    ui->originAllCheckBox->setEnabled(checked);
}

void SetBasisDialog::on_originCOC_toggled(bool checked)
{
    ui->originAtomList->setEnabled(checked);
    ui->originAllCheckBox->setEnabled(checked);
}

void SetBasisDialog::on_basisInert_toggled(bool checked)
{
    ui->basisAtomList->setEnabled(checked);
    ui->basisAllCheckBox->setEnabled(checked);
}

void SetBasisDialog::on_basisCovar_toggled(bool checked)
{
    ui->basisAtomList->setEnabled(checked);
    ui->basisAllCheckBox->setEnabled(checked);
}

void SetBasisDialog::on_basisCharge_toggled(bool checked)
{
    ui->basisAtomList->setEnabled(checked);
    ui->basisAllCheckBox->setEnabled(checked);
}

void SetBasisDialog::on_originUseSelection_clicked()
{
    std::vector<chemkit::Atom *> selectedAtoms = m_mainWindow->selection();

    for (int i = 0; i < ui->originAtomList->count(); i++)
    {
        chemkit::Atom *testAtom = m_mainWindow->getMol(m_molID)->atom(i);
        if (std::find(selectedAtoms.begin(), selectedAtoms.end(), testAtom) != selectedAtoms.end())
            ui->originAtomList->item(i)->setCheckState(Qt::Checked);
        else
            ui->originAtomList->item(i)->setCheckState(Qt::Unchecked);
    }
}

void SetBasisDialog::on_basisUseSelection_clicked()
{
    std::vector<chemkit::Atom *> selectedAtoms = m_mainWindow->selection();

    for (int i = 0; i < ui->basisAtomList->count(); i++)
    {
        chemkit::Atom *testAtom = m_mainWindow->getMol(m_molID)->atom(i);
        if (std::find(selectedAtoms.begin(), selectedAtoms.end(), testAtom) != selectedAtoms.end())
            ui->basisAtomList->item(i)->setCheckState(Qt::Checked);
        else
            ui->basisAtomList->item(i)->setCheckState(Qt::Unchecked);
    }
}

void SetBasisDialog::on_useBasisButton_clicked()
{
    for (int i = 0; i < ui->originAtomList->count(); i++)
    {
        if (ui->basisAtomList->item(i)->checkState() == Qt::Checked)
            ui->originAtomList->item(i)->setCheckState(Qt::Checked);
        else
            ui->originAtomList->item(i)->setCheckState(Qt::Unchecked);
    }
}

void SetBasisDialog::on_useOriginButton_clicked()
{
    for (int i = 0; i < ui->basisAtomList->count(); i++)
    {
        if (ui->originAtomList->item(i)->checkState() == Qt::Checked)
            ui->basisAtomList->item(i)->setCheckState(Qt::Checked);
        else
            ui->basisAtomList->item(i)->setCheckState(Qt::Unchecked);
    }
}
