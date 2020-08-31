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


#ifndef MOLECULEBASIS_H
#define MOLECULEBASIS_H

#include <Eigen/Core>
#include "types.h"
#include "molecule.h"

namespace molconv {

class MoleculeBasis
{
public:
    MoleculeBasis(moleculePtr molecule);

    // return the axes of the internal basis (aka the
    // rotation matrix) as obtained from the euler angles
    Eigen::Matrix3d axes() const;

    double phi() const;
    double theta() const;
    double psi() const;
    void setPhi(const double newPhi);
    void setTheta(const double newTheta);
    void setPsi(const double newPsi);

    static std::array<double,3> rot2euler(Eigen::Matrix3d rot);
    static Eigen::Matrix3d euler2rot(const double psi, const double theta, const double phi);

    virtual std::vector<bool> basisList() = 0;
    virtual std::array<int,3> atoms() = 0;

protected:
    void setEulerAngles(Eigen::Matrix3d rot);

    moleculePtr m_molecule;

    double m_phi;
    double m_theta;
    double m_psi;
};

}

#endif // MOLECULEBASIS_H

