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


#include<QDebug>
#include "moleculesettings.h"
#include "listofmolecules.h"
#include "ui_moleculesettings.h"


MoleculeSettings::MoleculeSettings(MolconvWindow *window)
    : QDockWidget(window)
    , ui(new Ui::MoleculeSettings)
    , settingMolecule(false)
{
    qDebug("this is the constructor of MoleculeSettings");

    main_window = window;

    ui->setupUi(this);

    ui->xSlider->setMinimum(-1000);
    ui->xSlider->setMaximum( 1000);
    ui->ySlider->setMinimum(-1000);
    ui->ySlider->setMaximum( 1000);
    ui->zSlider->setMinimum(-1000);
    ui->zSlider->setMaximum( 1000);
    ui->xSpinBox->setMaximum( 100.0);
    ui->xSpinBox->setMinimum(-100.0);
    ui->ySpinBox->setMaximum( 100.0);
    ui->ySpinBox->setMinimum(-100.0);
    ui->zSpinBox->setMaximum( 100.0);
    ui->zSpinBox->setMinimum(-100.0);

    connect(main_window, SIGNAL(new_molecule(molconv::moleculePtr&)), SLOT(setMolecule(molconv::moleculePtr&)));
}

MoleculeSettings::~MoleculeSettings()
{
    qDebug("this is the destructor of MoleculeSettings");
    delete ui;
}

void MoleculeSettings::setValues()
{
    Eigen::Vector3d origin = m_molecule->internalOriginPosition();

    double x = origin(0);
    double y = origin(1);
    double z = origin(2);

    ui->xSlider->setValue(int(x*10.0));
    ui->ySlider->setValue(int(y*10.0));
    ui->zSlider->setValue(int(z*10.0));

    ui->xSpinBox->setValue(x);
    ui->ySpinBox->setValue(y);
    ui->zSpinBox->setValue(z);
}

molconv::moleculePtr MoleculeSettings::molecule() const
{
    return m_molecule;
}

void MoleculeSettings::setMolecule(molconv::moleculePtr &newMolecule)
{
    settingMolecule = true;

    m_molecule = newMolecule;

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

void MoleculeSettings::on_xSlider_valueChanged(int value)
{
    double realValue = double(value) / 10.0;
    ui->xSpinBox->setValue(realValue);
}

void MoleculeSettings::on_ySlider_valueChanged(int value)
{
    double realValue = double(value) / 10.0;
    ui->ySpinBox->setValue(realValue);
}

void MoleculeSettings::on_zSlider_valueChanged(int value)
{
    double realValue = double(value) / 10.0;
    ui->zSpinBox->setValue(realValue);
}

void MoleculeSettings::on_xSpinBox_valueChanged(double value)
{
    int intValue = int(value * 10.0);
    ui->xSlider->setValue(intValue);

    if (!settingMolecule)
        m_molecule->setCenter(value, m_molecule->center()(1), m_molecule->center()(2));
}

void MoleculeSettings::on_ySpinBox_valueChanged(double value)
{
    int intValue = int(value * 10.0);
    ui->ySlider->setValue(intValue);

    if (!settingMolecule)
        m_molecule->setCenter(m_molecule->center()(0), value, m_molecule->center()(2));
}

void MoleculeSettings::on_zSpinBox_valueChanged(double value)
{
    int intValue = int(value * 10.0);
    ui->zSlider->setValue(intValue);

    if (!settingMolecule)
        m_molecule->setCenter(m_molecule->center()(0), m_molecule->center()(1), value);
}

void MoleculeSettings::on_phiSlider_valueChanged(int value)
{
    double realValue = double(value) / 10.0;
    ui->phiSpinBox->setValue(realValue);

    m_molecule->setPhi(realValue);
}

void MoleculeSettings::on_phiSpinBox_valueChanged(double value)
{
    int intValue = int(value * 10.0);
    ui->phiSlider->setValue(intValue);

    m_molecule->setPhi(value);
}
