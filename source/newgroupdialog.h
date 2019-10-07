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


#ifndef NEWGROUPDIALOG_H
#define NEWGROUPDIALOG_H

#include <QDialog>

class NewGroupDialogPrivate;

namespace Ui
{
    class NewGroupDialog;
}

class NewGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGroupDialog(QWidget *parent = 0);
    ~NewGroupDialog();

    void createMoleculeList();
    std::vector<bool> molecules() const;

public slots:
    std::string groupName() const;
    bool isStack() const;

private slots:
    void on_selAllCheckBox_clicked(bool checked);

private:
    NewGroupDialogPrivate *d;
    Ui::NewGroupDialog *ui;
};

#endif // NEWGROUPDIALOG_H
