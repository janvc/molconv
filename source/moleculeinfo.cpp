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

#include "molconv_window.h"
#include "moleculeinfo.h"
#include "ui_moleculeinfo.h"

MoleculeInfo::MoleculeInfo(MolconvWindow *window) :
    QDockWidget(window),
    ui(new Ui::MoleculeInfo)
{
    m_window = window;
    ui->setupUi(this);
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    connect(m_window, SIGNAL(new_molecule(molconv::moleculePtr&)), SLOT(setMolecule(molconv::moleculePtr&)));

    QSettings settings;

    if (settings.value("updateInfoLive").toBool())
        m_updateLive = true;
    else
        m_updateLive = false;

    int digits = settings.contains("infoDigits") ? settings.value("infoDigits").toInt() : 3;
    ui->nDigits->setValue(digits);

    ui->atomPositions->setCurrentFont(QFont(QFontDatabase::systemFont(QFontDatabase::FixedFont)));
    ui->basisProp->setCurrentFont(QFont(QFontDatabase::systemFont(QFontDatabase::FixedFont)));
}

MoleculeInfo::~MoleculeInfo()
{
    delete ui;
}

void MoleculeInfo::setMolecule(molconv::moleculePtr &newMol)
{
    m_molecule = newMol;

    update();
}

void MoleculeInfo::on_nDigits_valueChanged(int nDigits)
{
    QSettings settings;
    settings.setValue("infoDigits", nDigits);
    m_aP_prec = nDigits;
    if (m_molecule)
        update();
}

void MoleculeInfo::updateLive()
{
    if (m_updateLive)
        update();
}

void MoleculeInfo::updateMan()
{
    if (! m_updateLive)
        update();
}

void MoleculeInfo::update()
{
    ui->atomPositions->clear();
    QString atomicPositions;

    atomicPositions.append(QString::number(m_molecule->size()));
    atomicPositions.append("\n\n");

    for (int i = 0; i < int(m_molecule->size()); i++)
    {
        atomicPositions.append(QString::fromStdString(m_molecule->atom(i)->symbol()));
        atomicPositions.append(QString("%1").arg(i + 1, -5));
        atomicPositions.append(QString("%1").arg(m_molecule->atom(i)->position()(0), m_aP_prec + 4, 'f', m_aP_prec));
        atomicPositions.append(QString("%1").arg(m_molecule->atom(i)->position()(1), m_aP_prec + 7, 'f', m_aP_prec));
        atomicPositions.append(QString("%1").arg(m_molecule->atom(i)->position()(2), m_aP_prec + 7, 'f', m_aP_prec));
        atomicPositions.append("\n");
    }

    ui->atomPositions->insertPlainText(atomicPositions);

    ui->basisProp->clear();
    QString basis;
    basis.append(QString("Origin:\n"));
    basis.append(QString("%1\n").arg(m_molecule->internalOriginPosition()(0), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->internalOriginPosition()(1), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->internalOriginPosition()(2), m_aP_prec + 4, 'f', m_aP_prec));

    basis.append(QString("Basis:\n"));
    basis.append(QString("%1").arg(m_molecule->internalBasisVectors()(0,0), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->internalBasisVectors()(0,1), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->internalBasisVectors()(0,2), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->internalBasisVectors()(1,0), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->internalBasisVectors()(1,1), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->internalBasisVectors()(1,2), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->internalBasisVectors()(2,0), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->internalBasisVectors()(2,1), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->internalBasisVectors()(2,2), m_aP_prec + 4, 'f', m_aP_prec));

    basis.append(QString("center of geometry:\n"));
    basis.append(QString("%1\n").arg(m_molecule->center()(0), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->center()(1), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->center()(2), m_aP_prec + 4, 'f', m_aP_prec));

    basis.append(QString("center of mass:\n"));
    basis.append(QString("%1\n").arg(m_molecule->centerOfMass()(0), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->centerOfMass()(1), m_aP_prec + 4, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->centerOfMass()(2), m_aP_prec + 4, 'f', m_aP_prec));

    basis.append(QString("inertia tensor:\n"));
    basis.append(QString("%1").arg(m_molecule->inertiaTensor()(0,0), m_aP_prec + 10, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->inertiaTensor()(0,1), m_aP_prec + 10, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->inertiaTensor()(0,2), m_aP_prec + 10, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->inertiaTensor()(1,0), m_aP_prec + 10, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->inertiaTensor()(1,1), m_aP_prec + 10, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->inertiaTensor()(1,2), m_aP_prec + 10, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->inertiaTensor()(2,0), m_aP_prec + 10, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->inertiaTensor()(2,1), m_aP_prec + 10, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->inertiaTensor()(2,2), m_aP_prec + 10, 'f', m_aP_prec));

    basis.append(QString("covariance matrix:\n"));
    basis.append(QString("%1").arg(m_molecule->covarianceMatrix()(0,0), m_aP_prec + 6, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->covarianceMatrix()(0,1), m_aP_prec + 6, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->covarianceMatrix()(0,2), m_aP_prec + 6, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->covarianceMatrix()(1,0), m_aP_prec + 6, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->covarianceMatrix()(1,1), m_aP_prec + 6, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->covarianceMatrix()(1,2), m_aP_prec + 6, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->covarianceMatrix()(2,0), m_aP_prec + 6, 'f', m_aP_prec));
    basis.append(QString("%1").arg(m_molecule->covarianceMatrix()(2,1), m_aP_prec + 6, 'f', m_aP_prec));
    basis.append(QString("%1\n").arg(m_molecule->covarianceMatrix()(2,2), m_aP_prec + 6, 'f', m_aP_prec));

    ui->basisProp->insertPlainText(basis);
}

void MoleculeInfo::on_doLiveUpdate_toggled(bool checked)
{
    m_updateLive = checked;

    QSettings settings;
    settings.setValue("updateInfoLive", checked);
}
