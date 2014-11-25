/*
 * Copyright 2014 Jan von Cosel & Sebastian Lenz
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

#include<iostream>
#include<QDebug>
#include<QMessageBox>
#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<chemkit/graphicsmoleculeitem.h>
    #include<boost/make_shared.hpp>
#endif

#include "molconv_window.h"
#include "ui_molconv_window.h"
#include "molecules_dock.h"
#include "moleculesettings.h"
#include "open_dialog.h"


MolconvWindow::MolconvWindow(QMainWindow *parent)
    : QMainWindow(parent)
    , ui(new Ui::MolconvWindow)
{
    qDebug("this is the constructor of MolconvWindow");

    ui->setupUi(this);
    m_OpenDialog = new OpenDialog(this);
    connect(m_OpenDialog, SIGNAL(accepted()), this, SLOT(getMoleculeDialog()));
    connect(ui->actionImport_Molecule, SIGNAL(triggered()), SLOT(startOpenDialog()));
    connect(ui->actionQuit, SIGNAL(triggered()), SLOT(quit()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));

    m_ListOfMolecules = new ListOfMolecules(this);
    m_MoleculeSettings = new MoleculeSettings(this);
    addDockWidget(Qt::BottomDockWidgetArea, m_ListOfMolecules);
    addDockWidget(Qt::LeftDockWidgetArea, m_MoleculeSettings);

    ui->molconv_graphicsview->update();
}

MolconvWindow::~MolconvWindow()
{
    qDebug("this is the destructor of MolconvWindow");
    delete ui->molconv_graphicsview;
    delete ui;
}

void MolconvWindow::add_molecule(molconv::moleculePtr temp_mol)
{
    qDebug("entering MolconvWindow::add_molecule(temp_mol)");

    m_system.addMolecule(temp_mol);
    m_GraphicsItemVector.push_back(new chemkit::GraphicsMoleculeItem(m_system.getMolecule(m_system.size() - 1).get()));
    ui->molconv_graphicsview->addItem(m_GraphicsItemVector.back());
    ui->molconv_graphicsview->update();

    emit new_molecule(m_system.getMolecule(m_system.size() - 1).get());
}

void MolconvWindow::toggle_molecule(int position, bool state)
{
    qDebug("entering MolconvWindow::toggle_molecule()");
    if (state)
    {
        m_GraphicsItemVector.at(position)->show();
        ui->molconv_graphicsview->update();
    }
    else
    {
        m_GraphicsItemVector.at(position)->hide();
        ui->molconv_graphicsview->update();
    }
}

void MolconvWindow::about()
{
    QMessageBox::about(this, tr("About molconv"),
                tr("<b>molconv</b> is a programm to manipulate molecules."));
}

void MolconvWindow::quit()
{
    qDebug("entering MolconvWindow::quit()");
    qApp->quit();
}

void MolconvWindow::saveFile()
{
    qDebug("entering MolconvWindow::saveFile()");
}

void MolconvWindow::saveFile(const size_t index, const QString &filename)
{
    qDebug("entering MolconvWindow::saveFile(filename)");
}

void MolconvWindow::startOpenDialog()
{
    qDebug("entering MolconvWindow::openDialog()");

    m_OpenDialog->setModal(true);
    m_OpenDialog->exec();
}

void MolconvWindow::getMoleculeDialog()
{
    qDebug("entering MolconvWindow::get_molecule_Dialog()");

    boost::shared_ptr<molconv::Molecule> temp_mol = m_OpenDialog->getMol();
    temp_mol->cleanUp();
    add_molecule(temp_mol);
}
