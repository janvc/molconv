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


#include "moleculesettings.h"
#include "listofmolecules.h"
#include "moleculegroup.h"
#include "ui_moleculesettings.h"


MoleculeSettings::MoleculeSettings(MolconvWindow *window)
    : QDockWidget(window)
    , ui(new Ui::MoleculeSettings)
    , settingMolecule(false)
    , updatingGui(false)
{
    m_mainWindow = window;

    ui->setupUi(this);

    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    setDefaultBoundaries();
}

MoleculeSettings::~MoleculeSettings()
{
    delete ui;
}

void MoleculeSettings::setValuesFromMolecule()
{
    Eigen::Vector3d origin = m_mainWindow->getMol(m_molID)->originPosition();

    x = origin(0);
    y = origin(1);
    z = origin(2);

    phi = m_mainWindow->getMol(m_molID)->phi();
    theta = m_mainWindow->getMol(m_molID)->theta();
    psi = m_mainWindow->getMol(m_molID)->psi();

    if (x < xMin)
    {
        xMin = double(floor(x));
    }
    else if (x > xMax)
    {
        xMax = double(ceil(x));
    }

    if (y < yMin)
    {
        yMin = double(floor(y));
    }
    else if (y > yMax)
    {
        yMax = double(ceil(y));
    }

    if (z < zMin)
    {
        zMin = double(floor(z));
    }
    else if (z > zMax)
    {
        zMax = double(ceil(z));
    }

    updateGuiValues();
}

void MoleculeSettings::updateGuiValues()
{
    ui->xSlider->setValue(int(x * factor));
    ui->ySlider->setValue(int(y * factor));
    ui->zSlider->setValue(int(z * factor));

    ui->xSpinBox->setValue(x);
    ui->ySpinBox->setValue(y);
    ui->zSpinBox->setValue(z);

    ui->phiSlider->setValue(int(phi * rad2deg * factor));
    ui->thetaSlider->setValue(int(theta * rad2deg * factor));
    ui->psiSlider->setValue(int(psi * rad2deg * factor));

    ui->phiSpinBox->setValue(phi * rad2deg);
    ui->thetaSpinBox->setValue(theta * rad2deg);
    ui->psiSpinBox->setValue(psi * rad2deg);

    if (!settingMolecule)
    {
        emit basisChanged(x, y, z, phi, theta, psi);
    }
}


void MoleculeSettings::setGuiBoundaries()
{
    ui->xSlider->setMinimum(int(xMin * factor));
    ui->ySlider->setMinimum(int(yMin * factor));
    ui->zSlider->setMinimum(int(zMin * factor));
    ui->xSlider->setMaximum(int(xMax * factor));
    ui->ySlider->setMaximum(int(yMax * factor));
    ui->zSlider->setMaximum(int(zMax * factor));
    ui->xSpinBox->setMinimum(xMin);
    ui->ySpinBox->setMinimum(yMin);
    ui->zSpinBox->setMinimum(zMin);
    ui->xSpinBox->setMaximum(xMax);
    ui->ySpinBox->setMaximum(yMax);
    ui->zSpinBox->setMaximum(zMax);

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

void MoleculeSettings::setDefaultBoundaries()
{
    xMin = -10.0;
    yMin = -10.0;
    zMin = -10.0;
    xMax =  10.0;
    yMax =  10.0;
    zMax =  10.0;

    setGuiBoundaries();
}

unsigned long MoleculeSettings::molID() const
{
    return m_molID;
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

    setValuesFromMolecule();

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
    if (!updatingGui)
    {
        updatingGui = true;
        x = double(value) / factor;
        updateGuiValues();
        updatingGui = false;
    }
}

void MoleculeSettings::on_ySlider_valueChanged(int value)
{
    if (!updatingGui)
    {
        updatingGui = true;
        y = double(value) / factor;
        updateGuiValues();
        updatingGui = false;
    }
}

void MoleculeSettings::on_zSlider_valueChanged(int value)
{
    if (!updatingGui)
    {
        updatingGui = true;
        z = double(value) / factor;
        updateGuiValues();
        updatingGui = false;
    }
}

void MoleculeSettings::on_xSpinBox_valueChanged(double value)
{
    if (!updatingGui)
    {
        updatingGui = true;
        x = value;
        updateGuiValues();
        updatingGui = false;
    }
}

void MoleculeSettings::on_ySpinBox_valueChanged(double value)
{
    if (!updatingGui)
    {
        updatingGui = true;
        y = value;
        updateGuiValues();
        updatingGui = false;
    }
}

void MoleculeSettings::on_zSpinBox_valueChanged(double value)
{
    if (!updatingGui)
    {
        updatingGui = true;
        z = value;
        updateGuiValues();
        updatingGui = false;
    }
}

void MoleculeSettings::on_phiSlider_valueChanged(int value)
{
    if (!updatingGui)
    {
        updatingGui = true;
        phi = double(value) / (rad2deg * factor);
        updateGuiValues();
        updatingGui = false;
    }
}

void MoleculeSettings::on_thetaSlider_valueChanged(int value)
{
    if (!updatingGui)
    {
        updatingGui = true;
        theta = double(value) / (rad2deg * factor);
        updateGuiValues();
        updatingGui = false;
    }
}

void MoleculeSettings::on_psiSlider_valueChanged(int value)
{
    if (!updatingGui)
    {
        updatingGui = true;
        psi = double(value) / (rad2deg * factor);
        updateGuiValues();
        updatingGui = false;
    }
}

void MoleculeSettings::on_phiSpinBox_valueChanged(double value)
{
    if (!updatingGui)
    {
        updatingGui = true;
        phi = value / rad2deg;
        updateGuiValues();
        updatingGui = false;
    }
}

void MoleculeSettings::on_thetaSpinBox_valueChanged(double value)
{
    if (!updatingGui)
    {
        updatingGui = true;
        theta = value / rad2deg;
        updateGuiValues();
        updatingGui = false;
    }
}

void MoleculeSettings::on_psiSpinBox_valueChanged(double value)
{
    if (!updatingGui)
    {
        updatingGui = true;
        psi = value / rad2deg;
        updateGuiValues();
        updatingGui = false;
    }
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

