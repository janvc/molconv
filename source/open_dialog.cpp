/*
 * Copyright 2014 Jan von Cosel
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
#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<boost/make_shared.hpp>
#endif
#include"open_dialog.h"
#include"ui_open_dialog.h"

OpenDialog::OpenDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OpenDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}


OpenDialog::~OpenDialog()
{
    delete ui;
}

void OpenDialog::openFile(const QString &filename)
{
    chemkit::MoleculeFile *the_molfile;
    std::cout << "Opening file " << filename.toStdString() << std::endl;

    the_molfile = new chemkit::MoleculeFile(filename.toStdString());
    if (! the_molfile->read())
    {
        std::cerr << "Could not read molecule file " << filename.toStdString() << std::endl;
        QMessageBox::critical(this, "Error", QString("Error opening file: %1").arg(the_molfile->errorString().c_str()));
        delete the_molfile;
        return;
        ui->settings->setEnabled(false);
    }

    if (the_molfile->moleculeCount() > 0)
    {
        chemkit::Molecule temp_mol = *the_molfile->molecule();
        ui->an->setMaximum(temp_mol.atomCount());
        ui->atom1->setMaximum(temp_mol.atomCount());
        ui->atom2->setMaximum(temp_mol.atomCount());
        ui->atom3->setMaximum(temp_mol.atomCount());
        ui->origin->setEnabled(true);
        ui->basis->setEnabled(true);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        this->the_molecule = molconv::Molecule(temp_mol);
    }
}

void OpenDialog::on_filedialog_clicked()
{
    std::vector<std::string> formats = chemkit::MoleculeFile::formats();
    std::sort(formats.begin(), formats.end());

    QString formatsString;
    foreach(const std::string &format, formats)
        formatsString += QString("*.%1 ").arg(format.c_str());

    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), 0, QString("Molecule Files (%1);;All Files (*.*)").arg(formatsString));

    ui->filename->setText(filename);
    openFile(filename);
}

void OpenDialog::on_coa_toggled(bool checked)
{
   if(checked)
       ui->an->setEnabled(true);
   else
       ui->an->setEnabled(false);
}

molconv::Molecule OpenDialog::getMol()
{
    if(ui->atoms->isChecked() && !ui->coa->isChecked())
        this->the_molecule.set_intbasis(getOrigin(),getBasis(),0,ui->atom1->value(),ui->atom2->value(),ui->atom3->value());
    else if(ui->atoms->isChecked() && ui->coa->isChecked())
        this->the_molecule.set_intbasis(getOrigin(),getBasis(),ui->an->value(),ui->atom1->value(),ui->atom2->value(),ui->atom3->value());
    else if(!ui->atoms->isChecked() && !ui->coa->isChecked())
        this->the_molecule.set_intbasis(getOrigin(),getBasis());

    return this->the_molecule;
}

molconv::origin OpenDialog::getOrigin()
{
    if(ui->zero->isChecked())
        return molconv::ZERO;
    else if(ui->com->isChecked())
        return molconv::COM;
    else if(ui->cog->isChecked())
        return molconv::COG;
    else if(ui->coa->isChecked())
        return molconv::ATOM;
}

molconv::basis OpenDialog::getBasis()
{
    if(ui->identity->isChecked())
        return molconv::IDENTITY;
    else if(ui->covar->isChecked())
        return molconv::COVAR;
    else if(ui->inert->isChecked())
        return molconv::INERT;
    else if(ui->atoms->isChecked())
        return molconv::ATOMS;
}

void OpenDialog::on_atoms_toggled(bool checked)
{
   if(checked)
   {
       ui->atom1->setEnabled(true);
       ui->atom2->setEnabled(true);
       ui->atom3->setEnabled(true);
   } else
   {
       ui->atom1->setEnabled(false);
       ui->atom2->setEnabled(false);
       ui->atom3->setEnabled(false);
   }
}

void OpenDialog::done(int r)
{
    if(QDialog::Accepted == r && ui->atoms->isChecked())
    {
        if(ui->atom1->value() == ui->atom2->value() || ui->atom2->value() == ui->atom3->value() || ui->atom1->value() == ui->atom3->value())
        {
            QMessageBox::critical(this,"Incorrect Data","You have to choose three different atoms for the new basis.");
            return;
        } else
        {
            QDialog::done(r);
            return;
        }
    } else
    {
        QDialog::done(r);
        return;
    }
}
