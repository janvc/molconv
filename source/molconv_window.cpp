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

void MolconvWindow::openFile(const QString &filename)
{
    qDebug("entering MolconvWindow::openFile()");
    std::cout << "Opening file " << filename.toStdString() << std::endl;
    chemkit::MoleculeFile *l_MolFile = new chemkit::MoleculeFile(filename.toStdString());
    if (! l_MolFile->read())
    {
        std::cerr << "Could not read molecule file " << filename.toStdString() << std::endl;
        QMessageBox::critical(this, "Error", QString("Error opening file: %1").arg(l_MolFile->errorString().c_str()));
        delete l_MolFile;
        return;
    }
    if (l_MolFile->moleculeCount() > 0)
    {
        chemkit::Molecule temp_mol = *l_MolFile->molecule();
        //this->the_molecule_objects.push_back(temp_mol);
    }

    delete l_MolFile;
}

void MolconvWindow::add_molecule()
{
    qDebug("entering MolconvWindow::add_molecule()");
    //this->the_molecule_pointers.push_back(boost::make_shared<molconv::Molecule>(this->the_molecule_objects.back()));
    m_GraphicsItemVector.push_back(new chemkit::GraphicsMoleculeItem(m_system.getMolecule(m_system.size() - 1).get()));
    this->ui->molconv_graphicsview->addItem(m_GraphicsItemVector.back());
    ui->molconv_graphicsview->update();
    emit new_molecule(m_system.getMolecule(m_system.size() - 1).get());
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

void MolconvWindow::quit()
{
    qDebug("entering MolconvWindow::quit()");
    qApp->quit();
}

//void MolconvWindow::closeFile()
//{
//    qDebug("entering MolconvWindow::closeFile()");
//}

void MolconvWindow::clean_up(const int mol_nr, const molconv::Config &config)
{
    qDebug("entering MolconvWindow::clean_up()");
    //this->the_molecule_objects.at(mol_nr).clean_up(config);
}

void MolconvWindow::set_intbasis(const int mol_nr, const molconv::Config &config)
{
    qDebug("entering MolconvWindow::set_intbasis()");
    //this->the_molecule_objects.at(mol_nr).set_basis(config);
}

void MolconvWindow::saveFile()
{
    qDebug("entering MolconvWindow::saveFile()");
}

void MolconvWindow::saveFile(const size_t index, const QString &filename)
{
    qDebug("entering MolconvWindow::saveFile(filename)");
    std::cout << "Saving file " << filename.toStdString() << std::endl;


    chemkit::MoleculeFile *l_MolFile = new chemkit::MoleculeFile(filename.toStdString());
    l_MolFile->addMolecule(m_system.getMolecule(index));

    if (! l_MolFile->write())
    {
        std::cerr << "Could not write molecule file " << filename.toStdString() << std::endl;
        QMessageBox::critical(this, "Error", QString("Error writing to file: %1").arg(l_MolFile->errorString().c_str()));
        delete l_MolFile;
        return;
    }

    delete l_MolFile;
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
