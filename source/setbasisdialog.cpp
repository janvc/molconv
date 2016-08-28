/*
 * Copyright 2014 - 2016 Jan von Cosel & Sebastian Lenz
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
#include "setbasisdialog.h"
#include "ui_setbasisdialog.h"

setBasisDialog::setBasisDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setBasisDialog)
{
    ui->setupUi(this);
    m_window = static_cast<MolconvWindow*>(parent);

    m_originAtoms.fill(0);
    m_basisAtoms.fill(0);

    ui->originAllCheckBox->setCheckState(Qt::Checked);
    ui->basisAllCheckBox->setCheckState(Qt::Checked);
}

setBasisDialog::~setBasisDialog()
{
    delete ui;
}

void setBasisDialog::prepare(molconv::moleculePtr &molecule)
{
    ui->originAtomList->clear();
    ui->basisAtomList->clear();

    for (int i = 0; i < int(molecule->size()); i++)
    {
        std::string currentName = molecule->atom(i)->symbol() + std::to_string(i + 1);

        QListWidgetItem *originItem = new QListWidgetItem(QString::fromStdString(currentName), ui->originAtomList);
        if (molecule->originList().at(i))
            originItem->setCheckState(Qt::Checked);
        else
            originItem->setCheckState(Qt::Unchecked);
        ui->originAtomList->addItem(originItem);

        QListWidgetItem *basisItem = new QListWidgetItem(QString::fromStdString(currentName), ui->basisAtomList);
        if (molecule->basisList().at(i))
            basisItem->setCheckState(Qt::Checked);
        else
            basisItem->setCheckState(Qt::Unchecked);
        ui->basisAtomList->addItem(basisItem);
    }

    m_origin = molecule->internalOrigin();
    m_basis = molecule->internalBasis();

    // standard orientation is not available at the moment
    ui->basisStandard->setEnabled(false);

    switch (m_origin)
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
        m_originAtoms = molecule->internalOriginAtoms();
        break;
    case molconv::kCenterBetweenAtoms:
        ui->originAtomLine->setChecked(true);
        m_originAtoms = molecule->internalOriginAtoms();
        break;
    }

    switch (m_basis)
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
        ui->basisAtoms->setChecked(true);
        m_basisAtoms = molecule->internalBasisAtoms();
        break;
    }

    m_molecule = molecule;
}

molconv::origin setBasisDialog::origin() const
{
    return m_origin;
}

molconv::basis setBasisDialog::basis() const
{
    return m_basis;
}

std::array<int,2> setBasisDialog::originAtoms() const
{
    return m_originAtoms;
}

std::array<int,3> setBasisDialog::basisAtoms() const
{
    return m_basisAtoms;
}

std::vector<bool> setBasisDialog::selectedOriginAtoms() const
{
    return m_originList;
}

std::vector<bool> setBasisDialog::selectedBasisAtoms() const
{
    return m_basisList;
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

void setBasisDialog::on_buttonBox_accepted()
{
    // set the origin from the UI elements:
    m_originAtoms.fill(0);
    if (ui->originCOM->isChecked())
        m_origin = molconv::kCenterOfMass;
    else if (ui->originCOG->isChecked())
        m_origin = molconv::kCenterOfGeometry;
    else if (ui->originCOC->isChecked())
        m_origin = molconv::kCenterOfCharge;
    else if (ui->originAtomPos->isChecked())
    {
        m_origin = molconv::kCenterOnAtom;
        m_originAtoms[0] = ui->originAtom->value() - 1;
        m_originAtoms[1] = 0;
    }
    else if (ui->originAtomLine->isChecked())
    {
        m_origin = molconv::kCenterBetweenAtoms;
        m_originAtoms[0] = ui->originALineStart->value() - 1;
        m_originAtoms[0] = ui->originALineEnd->value() - 1;
    }

    // set the basis from the UI elements:
    m_basisAtoms.fill(0);
    if (ui->basisInert->isChecked())
        m_basis = molconv::kInertiaVectors;
    else if (ui->basisCovar->isChecked())
        m_basis = molconv::kCovarianceVectors;
    else if (ui->basisCharge->isChecked())
        m_basis = molconv::kChargeVectors;
    else if (ui->basisAtoms->isChecked())
    {
        m_basis = molconv::kVectorsFromAtoms;
        m_basisAtoms[0] = ui->basisAtom1->value() - 1;
        m_basisAtoms[1] = ui->basisAtom2->value() - 1;
        m_basisAtoms[2] = ui->basisAtom3->value() - 1;
    }


    // if available, set the list of atoms contributing to origin/basis:
    m_originList.clear();
    m_basisList.clear();

    if (ui->originAtomList->isEnabled())
        for (int i = 0; i < int(m_molecule->size()); i++)
        {
            if (ui->originAtomList->item(i)->checkState() ==  Qt::Checked)
                m_originList.push_back(true);
            else
                m_originList.push_back(false);
        }

    if (ui->basisAtomList->isEnabled())
        for (int i = 0; i < int(m_molecule->size()); i++)
        {
            if (ui->basisAtomList->item(i)->checkState() ==  Qt::Checked)
                m_basisList.push_back(true);
            else
                m_basisList.push_back(false);
        }
}

void setBasisDialog::on_originCOM_toggled(bool checked)
{
    ui->originAtomList->setEnabled(checked);
    ui->originAllCheckBox->setEnabled(checked);
}

void setBasisDialog::on_originCOG_toggled(bool checked)
{
    ui->originAtomList->setEnabled(checked);
    ui->originAllCheckBox->setEnabled(checked);
}

void setBasisDialog::on_originCOC_toggled(bool checked)
{
    ui->originAtomList->setEnabled(checked);
    ui->originAllCheckBox->setEnabled(checked);
}

void setBasisDialog::on_basisInert_toggled(bool checked)
{
    ui->basisAtomList->setEnabled(checked);
    ui->basisAllCheckBox->setEnabled(checked);
}

void setBasisDialog::on_basisCovar_toggled(bool checked)
{
    ui->basisAtomList->setEnabled(checked);
    ui->basisAllCheckBox->setEnabled(checked);
}

void setBasisDialog::on_basisCharge_toggled(bool checked)
{
    ui->basisAtomList->setEnabled(checked);
    ui->basisAllCheckBox->setEnabled(checked);
}
