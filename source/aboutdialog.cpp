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


// this define is needed because otherwise there will be a compile-time
// error saying that "EIGEN_DEVICE_FUNC does not name a type" from the
// Macros.h Eigen header file
#define EIGEN_DEVICE_FUNC

#include <string>
#include <eigen3/Eigen/src/Core/util/Macros.h>
#include "config.h"
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    const QString progInfo = "This is molconv version "
                           + QCoreApplication::applicationVersion()
                           + ", Git revision "
                           + QString(SCM_REVISION);

    const QString compileInfo = "Compiled on "
                              + QString(__DATE__) + " "
                              + QString(__TIME__)
                              + " using gcc version "
                              + QString::number(__GNUC__) + "."
                              + QString::number(__GNUC_MINOR__) + "."
                              + QString::number(__GNUC_PATCHLEVEL__);

    const QString qtInfo = "using Qt version " + QString(QT_VERSION_STR);

    const QString eigenInfo = "and Eigen version "
                            + QString::number(EIGEN_WORLD_VERSION) + "."
                            + QString::number(EIGEN_MAJOR_VERSION) + "."
                            + QString::number(EIGEN_MINOR_VERSION);

    ui->progname->setText(progInfo);
    ui->authors->setText("Copyright 2014 - 2016 by Jan von Cosel and Sebastian Lenz");
    ui->compileInfo->setText(compileInfo);
    ui->qtInfo->setText(qtInfo);
    ui->eigenInfo->setText(eigenInfo);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
