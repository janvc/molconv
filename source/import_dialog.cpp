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

#include<iostream>
#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<boost/make_shared.hpp>
#endif
#include "import_dialog.h"
#include "ui_import_dialog.h"


class ImportDialogPrivate
{
public:
    ImportDialogPrivate()
    {
    }

    molconv::origin m_origin;
    molconv::basis m_basis;

    std::string m_moleculeName;
    QString m_fileName;
};

ImportDialog::ImportDialog(QWidget *parent)
    : QDialog(parent)
    , d(new ImportDialogPrivate)
    , ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->atom1,SIGNAL(valueChanged(int)),this,SLOT(atoms_changed()));
    connect(ui->atom2,SIGNAL(valueChanged(int)),this,SLOT(atoms_changed()));
    connect(ui->atom3,SIGNAL(valueChanged(int)),this,SLOT(atoms_changed()));

    d->m_origin = molconv::kCenterOfMass;
    d->m_basis = molconv::kInertiaVectors;
}


ImportDialog::~ImportDialog()
{
    delete ui;
}

void ImportDialog::openFile()
{
    chemkit::MoleculeFile *the_molfile;

    the_molfile = new chemkit::MoleculeFile(d->m_fileName.toStdString());
    if (! the_molfile->read())
    {
        std::cerr << "Could not read molecule file " << d->m_fileName.toStdString() << std::endl;
        QMessageBox::critical(this, "Error", QString("Error opening file: %1").arg(the_molfile->errorString().c_str()));
        delete the_molfile;
        ui->settings->setEnabled(false);
        return;
    }

    if (the_molfile->moleculeCount() > 0)
    {
        chemkit::Molecule temp_mol;
        temp_mol = *the_molfile->molecule();

        ui->an->setMaximum(temp_mol.atomCount());
        ui->atom1->setMaximum(temp_mol.atomCount());
        ui->atom2->setMaximum(temp_mol.atomCount());
        ui->atom3->setMaximum(temp_mol.atomCount());
        ui->origin->setEnabled(true);
        ui->basis->setEnabled(true);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void ImportDialog::on_filedialog_clicked()
{
    QStringList filters;
    filters << "Molecule Files (*.cml *.log *.mdl *.mol *.mol2 *.out *.pdb *.sd *.sdf *.xyz)"
            << "CML (*.cml)"
            << "Gaussian output (*.log *.out)"
            << "MDL Mol (*.mdl *.mol *.sd *.sdf)"
            << "Mol2 (*.mol2)"
            << "PDB (*.pdb)"
            << "XYZ (*.xyz)"
            << "All Files (*.*)";

    QSettings settings;
    QString startImportPath = settings.value("importPath").toString();
    d->m_fileName = QFileDialog::getOpenFileName(this, tr("Open File"), startImportPath, filters.join(";;"));

    if (!d->m_fileName.isEmpty())
    {
        settings.setValue("importPath", QFileInfo(d->m_fileName).absolutePath());

        ui->filename->setText(d->m_fileName.split("/").last());
        ui->moleculeName->setText(d->m_fileName.split("/").last().split(".").first());
        openFile();
    }
}

void ImportDialog::on_coa_toggled(bool checked)
{
    if(checked)
    {
        d->m_origin = molconv::kCenterOnAtom;
        ui->an->setEnabled(true);
    }
    else
        ui->an->setEnabled(false);
}

molconv::origin ImportDialog::getOrigin()
{
    return d->m_origin;
}

molconv::basis ImportDialog::getBasis()
{
    return d->m_basis;
}

void ImportDialog::on_atoms_toggled(bool checked)
{
    if(checked)
    {
        d->m_basis = molconv::kVectorsFromAtoms;
        ui->atom1->setEnabled(true);
        ui->atom2->setEnabled(true);
        ui->atom3->setEnabled(true);
        atoms_changed();
    }
    else
    {
        ui->atom1->setEnabled(false);
        ui->atom2->setEnabled(false);
        ui->atom3->setEnabled(false);
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void ImportDialog::atoms_changed()
{
    if (ui->atom1->value() == ui->atom2->value() || ui->atom2->value() == ui->atom3->value() || ui->atom1->value() == ui->atom3->value())
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    else
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

QString ImportDialog::getMoleculeName()
{
    QString moleculeName = ui->moleculeName->text();

    if (moleculeName.isEmpty())
        moleculeName = ui->filename->text().split("/").back();

    return moleculeName;
}

QString ImportDialog::getFileName()
{
    return d->m_fileName;
}

void ImportDialog::on_coc_toggled(bool checked)
{
    if (checked)
        d->m_origin = molconv::kCenterOfCharge;
}

void ImportDialog::on_com_toggled(bool checked)
{
    if (checked)
        d->m_origin = molconv::kCenterOfMass;
}

void ImportDialog::on_cog_toggled(bool checked)
{
    if (checked)
        d->m_origin = molconv::kCenterOfGeometry;
}

void ImportDialog::on_stdori_toggled(bool checked)
{
    if (checked)
        d->m_basis = molconv::kStandardOrientation;
}

void ImportDialog::on_inert_toggled(bool checked)
{
    if (checked)
        d->m_basis = molconv::kInertiaVectors;
}

void ImportDialog::on_covar_toggled(bool checked)
{
    if (checked)
        d->m_basis = molconv::kCovarianceVectors;
}
