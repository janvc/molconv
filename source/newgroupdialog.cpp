/*
 * Copyright 2014 - 2016 Jan von Cosel and Sebastian Lenz
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
#include "newgroupdialog.h"
#include "ui_newgroupdialog.h"

NewGroupDialog::NewGroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewGroupDialog)
{
    qDebug("this is the constructor of NewGroupDialog");
    ui->setupUi(this);
}

NewGroupDialog::~NewGroupDialog()
{
    qDebug("this is the destructor of NewGroupDialog");
    delete ui;
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
