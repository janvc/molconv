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


#include "moleculesettings.h"
#include "listofmolecules.h"
#include "moleculegroup.h"
#include "ui_moleculesettings.h"


MoleculeSettings::MoleculeSettings(MolconvWindow *window)
    : QDockWidget(window)
    , ui(new Ui::MoleculeSettings)
    , settingMolecule(false)
{
    m_mainWindow = window;

    ui->setupUi(this);

    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    setBoundaries();

    connect(m_mainWindow, SIGNAL(new_molecule(unsigned long)), SLOT(setMolecule(unsigned long)));
}

MoleculeSettings::~MoleculeSettings()
{
    delete ui;
}

void MoleculeSettings::setValues()
{
    Eigen::Vector3d origin = m_mainWindow->getMol(m_molID)->internalOriginPosition();

    double x = origin(0);
    double y = origin(1);
    double z = origin(2);

    if (x < Xmin)
    {
        double nXmin = double(floor(x));
        ui->xSlider->setMinimum(int(nXmin * factor));
        ui->xSpinBox->setMinimum(nXmin);
    }
    else if (x > Xmax)
    {
        double nXmax = double(ceil(x));
        ui->xSlider->setMaximum(int(nXmax * factor));
        ui->xSpinBox->setMaximum(nXmax);
    }

    if (y < Ymin)
    {
        double nYmin = double(floor(y));
        ui->ySlider->setMinimum(int(nYmin * factor));
        ui->ySpinBox->setMinimum(nYmin);
    }
    else if (y > Ymax)
    {
        double nYmax = double(ceil(y));
        ui->ySlider->setMaximum(int(nYmax * factor));
        ui->ySpinBox->setMaximum(nYmax);
    }

    if (z < Zmin)
    {
        double nZmin = double(floor(z));
        ui->zSlider->setMinimum(int(nZmin * factor));
        ui->zSpinBox->setMinimum(nZmin);
    }
    else if (z > Zmax)
    {
        double nZmax = double(ceil(z));
        ui->zSlider->setMaximum(int(nZmax * factor));
        ui->zSpinBox->setMaximum(nZmax);
    }

    ui->xSlider->setValue(int(x * factor));
    ui->ySlider->setValue(int(y * factor));
    ui->zSlider->setValue(int(z * factor));

    ui->xSpinBox->setValue(x);
    ui->ySpinBox->setValue(y);
    ui->zSpinBox->setValue(z);

    ui->phiSlider->setValue(int(m_mainWindow->getMol(m_molID)->phi() * rad2deg * factor));
    ui->thetaSlider->setValue(int(m_mainWindow->getMol(m_molID)->theta() * rad2deg * factor));
    ui->psiSlider->setValue(int(m_mainWindow->getMol(m_molID)->psi() * rad2deg * factor));

    ui->phiSpinBox->setValue(m_mainWindow->getMol(m_molID)->phi() * rad2deg);
    ui->thetaSpinBox->setValue(m_mainWindow->getMol(m_molID)->theta() * rad2deg);
    ui->psiSpinBox->setValue(m_mainWindow->getMol(m_molID)->psi() * rad2deg);
}

void MoleculeSettings::setBoundaries()
{
    ui->xSlider->setMinimum(int(Xmin * factor));
    ui->ySlider->setMinimum(int(Ymin * factor));
    ui->zSlider->setMinimum(int(Zmin * factor));
    ui->xSlider->setMaximum(int(Xmax * factor));
    ui->ySlider->setMaximum(int(Ymax * factor));
    ui->zSlider->setMaximum(int(Zmax * factor));
    ui->xSpinBox->setMinimum(Xmin);
    ui->ySpinBox->setMinimum(Ymin);
    ui->zSpinBox->setMinimum(Zmin);
    ui->xSpinBox->setMaximum(Xmax);
    ui->ySpinBox->setMaximum(Ymax);
    ui->zSpinBox->setMaximum(Zmax);

    ui->xSpinBox->setSingleStep(0.001);
    ui->ySpinBox->setSingleStep(0.001);
    ui->zSpinBox->setSingleStep(0.001);

    ui->phiSlider->setMinimum(0);
    ui->thetaSlider->setMinimum(0);
    ui->psiSlider->setMinimum(0);
    ui->phiSlider->setMaximum(int(360.0 * factor));
    ui->thetaSlider->setMaximum(int(180.0 * factor));
    ui->psiSlider->setMaximum(int(360.0 * factor));
    ui->phiSpinBox->setMinimum(0.0);
    ui->thetaSpinBox->setMinimum(0.0);
    ui->psiSpinBox->setMinimum(0.0);
    ui->phiSpinBox->setMaximum(360.0);
    ui->thetaSpinBox->setMaximum(180.0);
    ui->psiSpinBox->setMaximum(360.0);

    ui->phiSpinBox->setSingleStep(0.001);
    ui->thetaSpinBox->setSingleStep(0.001);
    ui->psiSpinBox->setSingleStep(0.001);
}

void MoleculeSettings::updateMolecule()
{
    m_mainWindow->getMol(m_molID)->moveFromParas(
                ui->xSpinBox->value(),
                ui->ySpinBox->value(),
                ui->zSpinBox->value(),
                ui->phiSpinBox->value() / rad2deg,
                ui->thetaSpinBox->value() / rad2deg,
                ui->psiSpinBox->value() / rad2deg);

    emit basisChanged();
}

unsigned long MoleculeSettings::molID() const
{
    return m_molID;
}

void MoleculeSettings::moveMolecule(const double x, const double y, const double z, const double phi, const double theta, const double psi)
{
    ui->xSpinBox->setValue(x);
    ui->ySpinBox->setValue(y);
    ui->zSpinBox->setValue(z);
    ui->phiSpinBox->setValue(phi * rad2deg);
    ui->thetaSpinBox->setValue(theta * rad2deg);
    ui->psiSpinBox->setValue(psi * rad2deg);
}

void MoleculeSettings::setMolecule(const unsigned long newMolID)
{
    settingMolecule = true;

    m_molID = newMolID;

    ui->xSlider->setEnabled(true);
    ui->ySlider->setEnabled(true);
    ui->zSlider->setEnabled(true);
    ui->phiSlider->setEnabled(true);
    ui->psiSlider->setEnabled(true);
    ui->thetaSlider->setEnabled(true);
    ui->xSpinBox->setEnabled(true);
    ui->ySpinBox->setEnabled(true);
    ui->zSpinBox->setEnabled(true);
    ui->phiSpinBox->setEnabled(true);
    ui->psiSpinBox->setEnabled(true);
    ui->thetaSpinBox->setEnabled(true);

    setValues();

    settingMolecule = false;
}

void MoleculeSettings::setGroup(molconv::MoleculeGroup *newGroup)
{
    ui->xSlider->setEnabled(false);
    ui->ySlider->setEnabled(false);
    ui->zSlider->setEnabled(false);
    ui->phiSlider->setEnabled(false);
    ui->psiSlider->setEnabled(false);
    ui->thetaSlider->setEnabled(false);
    ui->xSpinBox->setEnabled(false);
    ui->ySpinBox->setEnabled(false);
    ui->zSpinBox->setEnabled(false);
    ui->phiSpinBox->setEnabled(false);
    ui->psiSpinBox->setEnabled(false);
    ui->thetaSpinBox->setEnabled(false);
}

void MoleculeSettings::on_xSlider_valueChanged(int value)
{
    double realValue = double(value) / factor;
    ui->xSpinBox->setValue(realValue);
}

void MoleculeSettings::on_ySlider_valueChanged(int value)
{
    double realValue = double(value) / factor;
    ui->ySpinBox->setValue(realValue);
}

void MoleculeSettings::on_zSlider_valueChanged(int value)
{
    double realValue = double(value) / factor;
    ui->zSpinBox->setValue(realValue);
}

void MoleculeSettings::on_xSpinBox_valueChanged(double value)
{
    int intValue = int(value * factor);
    ui->xSlider->setValue(intValue);

    if (!settingMolecule)
        updateMolecule();
}

void MoleculeSettings::on_ySpinBox_valueChanged(double value)
{
    int intValue = int(value * factor);
    ui->ySlider->setValue(intValue);

    if (!settingMolecule)
        updateMolecule();
}

void MoleculeSettings::on_zSpinBox_valueChanged(double value)
{
    int intValue = int(value * factor);
    ui->zSlider->setValue(intValue);

    if (!settingMolecule)
        updateMolecule();
}

void MoleculeSettings::on_phiSlider_valueChanged(int value)
{
    double realValue = double(value) / factor;
    ui->phiSpinBox->setValue(realValue);
}

void MoleculeSettings::on_thetaSlider_valueChanged(int value)
{
    double realValue = double(value) / factor;
    ui->thetaSpinBox->setValue(realValue);
}

void MoleculeSettings::on_psiSlider_valueChanged(int value)
{
    double realValue = double(value) / factor;
    ui->psiSpinBox->setValue(realValue);
}

void MoleculeSettings::on_phiSpinBox_valueChanged(double value)
{
    int intValue = int(value * factor);
    ui->phiSlider->setValue(intValue);

    if (!settingMolecule)
        updateMolecule();
}

void MoleculeSettings::on_thetaSpinBox_valueChanged(double value)
{
    int intValue = int(value * factor);
    ui->thetaSlider->setValue(intValue);

    if (!settingMolecule)
        updateMolecule();
}

void MoleculeSettings::on_psiSpinBox_valueChanged(double value)
{
    int intValue = int(value * factor);
    ui->psiSlider->setValue(intValue);

    if (!settingMolecule)
        updateMolecule();
}

void MoleculeSettings::on_xSlider_sliderReleased()
{
    emit editingFinished();
}

void MoleculeSettings::on_ySlider_sliderReleased()
{
    emit editingFinished();
}

void MoleculeSettings::on_zSlider_sliderReleased()
{
    emit editingFinished();
}

void MoleculeSettings::on_phiSlider_sliderReleased()
{
    emit editingFinished();
}

void MoleculeSettings::on_thetaSlider_sliderReleased()
{
    emit editingFinished();
}

void MoleculeSettings::on_psiSlider_sliderReleased()
{
    emit editingFinished();
}

void MoleculeSettings::on_xSpinBox_editingFinished()
{
    emit editingFinished();
}

void MoleculeSettings::on_ySpinBox_editingFinished()
{
    emit editingFinished();
}

void MoleculeSettings::on_zSpinBox_editingFinished()
{
    emit editingFinished();
}

void MoleculeSettings::on_phiSpinBox_editingFinished()
{
    emit editingFinished();
}

void MoleculeSettings::on_thetaSpinBox_editingFinished()
{
    emit editingFinished();
}

void MoleculeSettings::on_psiSpinBox_editingFinished()
{
    emit editingFinished();
}
