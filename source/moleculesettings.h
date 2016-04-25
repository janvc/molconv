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

#ifndef MOLECULESETTINGS_H
#define MOLECULESETTINGS_H

#include <QDockWidget>

#include "molconv_window.h"

namespace Ui
{
    class MoleculeSettings;
}

class MolconvWindow;

class MoleculeSettings : public QDockWidget
{
    Q_OBJECT

public:
    //explicit MoleculeSettings(MolconvWindow *window);
    MoleculeSettings(MolconvWindow *window);
    ~MoleculeSettings();

    molconv::moleculePtr molecule() const;

public slots:
    void setMolecule(molconv::moleculePtr &newMolecule);

private slots:
    void on_xSlider_valueChanged(int value);
    void on_ySlider_valueChanged(int value);
    void on_zSlider_valueChanged(int value);
    void on_xSpinBox_valueChanged(double value);
    void on_ySpinBox_valueChanged(double value);
    void on_zSpinBox_valueChanged(double value);

    void on_phiSlider_valueChanged(int value);
    void on_thetaSlider_valueChanged(int value);
    void on_psiSlider_valueChanged(int value);

    void on_phiSpinBox_valueChanged(double value);
    void on_thetaSpinBox_valueChanged(double value);
    void on_psiSpinBox_valueChanged(double value);

private:
    void setValues();
    void setBoundaries();
    void updateMolecule();

    Ui::MoleculeSettings *ui;
    MolconvWindow *main_window;
    molconv::moleculePtr m_molecule;

    bool settingMolecule;

    // parameters for the molecule movement:
    double Xmin = -10.0;
    double Ymin = -10.0;
    double Zmin = -10.0;
    double Xmax =  10.0;
    double Ymax =  10.0;
    double Zmax =  10.0;

    // conversion factor from the int representation of the
    // sliders to the floating point representation of the spinboxes
    double factor = 100.0;

    const double rad2deg = 180.0 / M_PI;
};

#endif // MOLECULESETTINGS_H
