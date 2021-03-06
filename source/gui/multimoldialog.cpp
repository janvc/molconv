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


#ifndef Q_MOC_RUN
    #include <chemkit/moleculefile.h>
#endif
#include "multimoldialog.h"
#include "ui_multimoldialog.h"

MultiMolDialog::MultiMolDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MultiMolDialog)
{
    ui->setupUi(this);
    ui->molList->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

MultiMolDialog::~MultiMolDialog()
{
    delete ui;
}

void MultiMolDialog::createMoleculeList(chemkit::MoleculeFile *file)
{
    ui->molList->clear();
    ui->selectAllBox->setCheckState(Qt::Unchecked);

    for (int i = 0; i < int(file->moleculeCount()); i++)
    {
        QString name = QString::number(i + 1);
        QListWidgetItem *molItem = new QListWidgetItem(name.rightJustified(4, ' '), ui->molList);
        molItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->molList->addItem(molItem);
    }
}

std::vector<bool> MultiMolDialog::molecules() const
{
    std::vector<bool> isChosen;

    for (int i = 0; i < ui->molList->count(); i++)
        if (ui->molList->item(i)->isSelected())
            isChosen.push_back(true);
        else
            isChosen.push_back(false);

    return isChosen;
}

void MultiMolDialog::on_MultiMolDialog_rejected()
{
    for (int i = 0; i < ui->molList->count(); i++)
        ui->molList->item(i)->setSelected(false);
}

void MultiMolDialog::on_selectAllBox_clicked(bool checked)
{
    for (int i = 0; i < ui->molList->count(); i++)
        ui->molList->item(i)->setSelected(checked);
}
