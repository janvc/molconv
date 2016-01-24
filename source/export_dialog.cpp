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
#include "export_dialog.h"
#include "ui_export_dialog.h"

ExportDialog::ExportDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExportDialog)
{
    qDebug("this is the constructor of ExportDialog");
    ui->setupUi(this);

    theWindow = static_cast<MolconvWindow*>(parent);
}

ExportDialog::~ExportDialog()
{
    qDebug("this is the destructor of ExportDialog");
    delete ui;
}

void ExportDialog::createMoleculeList()
{
    ui->molExportList->clear();

    for (int i = 0; i < theWindow->nMolecules(); i++)
    {
        std::string currentName = theWindow->getMol(i)->name();
        QListWidgetItem *molItem = new QListWidgetItem(QString::fromStdString(currentName), ui->molExportList);
        molItem->setCheckState(Qt::Unchecked);
        ui->molExportList->addItem(molItem);
    }
}

void ExportDialog::on_buttonBox_accepted()
{
    // create a dummy molecule containing the atoms of all selected molecules:
    molconv::moleculePtr dummyMol(new molconv::Molecule);

    for (int i = 0; i < ui->molExportList->count(); i++)
    {
        if (ui->molExportList->item(i)->checkState() == Qt::Checked)
        {
            for (int j = 0; j < theWindow->getMol(i)->size(); j++)
            {
                dummyMol->addAtomCopy(theWindow->getMol(i)->atom(j));
            }
        }
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"));

    boost::shared_ptr<chemkit::MoleculeFile> theMolFile(new chemkit::MoleculeFile(filename.toStdString()));
    theMolFile->addMolecule(dummyMol);
    theMolFile->write();
}
