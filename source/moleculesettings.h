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

#ifndef MOLECULESETTINGS_H
#define MOLECULESETTINGS_H

#include <QDockWidget>

#include "molconvwindow.h"

namespace Ui
{
    class MoleculeSettings;
}

namespace molconv
{
    class MoleculeGroup;
}
class MolconvWindow;

class MoleculeSettings : public QDockWidget
{
    Q_OBJECT

public:
    MoleculeSettings(MolconvWindow *window);
    ~MoleculeSettings();

    unsigned long molID() const;
    void moveMolecule(const double x, const double y, const double z, const double phi, const double theta, const double psi);

public slots:
    void setMolecule(const unsigned long newMolID);
    void setGroup(molconv::MoleculeGroup *newGroup);

signals:
    void basisChanged() const;
    void editingFinished() const;

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

    void on_xSlider_sliderReleased();
    void on_ySlider_sliderReleased();
    void on_zSlider_sliderReleased();
    void on_phiSlider_sliderReleased();
    void on_thetaSlider_sliderReleased();
    void on_psiSlider_sliderReleased();

    void on_xSpinBox_editingFinished();
    void on_ySpinBox_editingFinished();
    void on_zSpinBox_editingFinished();
    void on_phiSpinBox_editingFinished();
    void on_thetaSpinBox_editingFinished();
    void on_psiSpinBox_editingFinished();

private:
    void setValues();
    void setBoundaries();
    void updateMolecule();

    Ui::MoleculeSettings *ui;
    MolconvWindow *m_mainWindow;
    unsigned long m_molID;

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
    double factor = 1000.0;

    const double rad2deg = 180.0 / M_PI;
};

#endif // MOLECULESETTINGS_H
