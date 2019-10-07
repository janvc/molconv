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


#include "molconv_window.h"
#include "newgroupdialog.h"
#include "ui_newgroupdialog.h"


class NewGroupDialogPrivate
{
public:
    NewGroupDialogPrivate(QWidget *parent)
        : m_window(static_cast<MolconvWindow*>(parent))
    {
    }

    MolconvWindow *m_window;
};

NewGroupDialog::NewGroupDialog(QWidget *parent)
    : QDialog(parent)
    , d(new NewGroupDialogPrivate(parent))
    , ui(new Ui::NewGroupDialog)
{
    ui->setupUi(this);
    ui->molList->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

NewGroupDialog::~NewGroupDialog()
{
    delete ui;
    delete d;
}

void NewGroupDialog::createMoleculeList()
{
    ui->molList->clear();
    ui->selAllCheckBox->setCheckState(Qt::Unchecked);

    for (int i = 0; i < d->m_window->nMolecules(); i++)
    {
        QString name = QString::fromStdString(d->m_window->getMol(i)->name());
        QListWidgetItem *molItem = new QListWidgetItem(name, ui->molList);
        molItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->molList->addItem(molItem);
    }
}

std::vector<bool> NewGroupDialog::molecules() const
{
    std::vector<bool> selectedMolecules;

    for (int i = 0; i < ui->molList->count(); i++)
        if (ui->molList->item(i)->isSelected())
            selectedMolecules.push_back(true);
        else
            selectedMolecules.push_back(false);

    return selectedMolecules;
}

std::string NewGroupDialog::groupName() const
{
    return ui->groupNameLEdit->text().toStdString();
}

bool NewGroupDialog::isStack() const
{
    if (ui->isStackRB->isChecked())
        return true;
    else
        return false;
}

void NewGroupDialog::on_selAllCheckBox_clicked(bool checked)
{
    for (int i = 0; i < ui->molList->count(); i++)
        ui->molList->item(i)->setSelected(checked);
}
