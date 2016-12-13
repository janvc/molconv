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

    ui->nDigits->setValue(3);

    ui->atomPositions->setCurrentFont(QFont(QFontDatabase::systemFont(QFontDatabase::FixedFont)));
}

MoleculeInfo::~MoleculeInfo()
{
    delete ui;
}

void MoleculeInfo::setMolecule(molconv::moleculePtr &newMol)
{
    m_molecule = newMol;

    ui->atomList->clear();

    for (int i = 0; i < 2; i++)
    {
        QListWidgetItem *atItem = new QListWidgetItem(QString(""), ui->atomList);
        atItem->setFlags(Qt::NoItemFlags);
        ui->atomList->addItem(atItem);
    }

    for (int i = 0; i < int(m_molecule->size()); i++)
    {
        QListWidgetItem *atItem = new QListWidgetItem(QString::number(i + 1), ui->atomList);
        atItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->atomList->addItem(atItem);
    }

    updateAtomPos();
}

void MoleculeInfo::on_nDigits_valueChanged(int arg1)
{
    aP_prec = arg1;
    if (m_molecule)
        updateAtomPos();
}

void MoleculeInfo::updateAtomPos()
{
    ui->atomPositions->clear();
    QString atomicPositions;

    atomicPositions.append(QString::number(m_molecule->size()));
    atomicPositions.append("\n\n");

    for (int i = 0; i < int(m_molecule->size()); i++)
    {
        atomicPositions.append(QString::fromStdString(m_molecule->atom(i)->symbol()));
        atomicPositions.append(QString("%1").arg(m_molecule->atom(i)->position()(0), aP_prec + 7, 'f', aP_prec));
        atomicPositions.append(QString("%1").arg(m_molecule->atom(i)->position()(1), aP_prec + 7, 'f', aP_prec));
        atomicPositions.append(QString("%1").arg(m_molecule->atom(i)->position()(2), aP_prec + 7, 'f', aP_prec));
        atomicPositions.append("\n");
    }

    ui->atomPositions->insertPlainText(atomicPositions);
}
