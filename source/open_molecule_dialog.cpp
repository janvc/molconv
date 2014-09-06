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

#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<boost/make_shared.hpp>
#endif
#include"open_molecule_dialog.h"
#include"ui_open_molecule_dialog.h"

open_molecule_dialog::open_molecule_dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::open_molecule_dialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}


open_molecule_dialog::~open_molecule_dialog()
{
    delete ui;
}

void open_molecule_dialog::openFile(const QString &filename)
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
        ui->an->setMinimum(1);
        ui->origin->setEnabled(true);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        this->the_molecule = temp_mol;
    }
}

void open_molecule_dialog::on_filedialog_clicked()
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

void open_molecule_dialog::on_coa_toggled(bool checked)
{
   if(checked)
       ui->an->setEnabled(true);
   else
       ui->an->setEnabled(false);
}

chemkit::Molecule open_molecule_dialog::getMol()
{
    return this->the_molecule;
}
