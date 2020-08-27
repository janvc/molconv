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


#include "moleculebasis.h"

namespace molconv {

MoleculeBasis::MoleculeBasis()
{
    m_psi = 0.0;
    m_theta = 0.0;
    m_phi = 0.0;
}

Eigen::Matrix3d MoleculeBasis::axes() const
{
    return euler2rot(m_psi, m_theta, m_phi);
}

/*
 * Calculate the euler angles from a given rotation matrix.
 * The angles are returned in the order <psi, theta, phi>.
 */
std::array<double,3> MoleculeBasis::rot2euler(Eigen::Matrix3d rot)
{
    std::array<double,3> angles;

    // check if the rotation matrix is numerically close to the form
    //
    //    x   x   0    In this case, theta = 0 (+1) or pi (-1)
    //    x   x   0    and the angles psi and phi are not uniquely
    //    0   0  +-1   determined
    //
    // The (3,3) element is a better indicator for this condition because it goes
    // very close to +-1 while the other elements are still quite large (1e-7)
    if ((1.0 - std::abs(double(rot(2,2)))) < 1.0e-10)
    {
        angles[1] = double(rot(2,2)) > 0.0 ? 0.0 : M_PI;
        angles[2] = 0.0;

        // check, in which part of the [0, 2 pi] intervall we are, so
        // that the signs of the transformation matrix elements
        // can be reproduced correctly later
        if (double(rot(0,0)) >= 0.0 && double(rot(0,1)) >= 0.0)     // 0 - pi/2
            angles[0] = std::acos(double(rot(0,0)));
        else if (double(rot(0,0)) < 0.0 && double(rot(0,1)) >= 0.0) // pi/2 - pi
            angles[0] = std::acos(double(rot(0,0)));
        else if (double(rot(0,0)) < 0.0 && double(rot(0,1)) < 0.0)  // pi - 3/2 pi
            angles[0] = 2.0 * M_PI - std::acos(double(rot(0,0)));
        else if (double(rot(0,0)) >= 0.0 && double(rot(0,1)) < 0.0) // 3/2 pi - 2 pi (=0)
            angles[0] = -std::acos(double(rot(0,0)));
    }
    else
    {
        angles[1] = std::acos(double(rot(2,2)));
        angles[2] = std::atan2(double(rot(0,2)),  double(rot(1,2)));
        angles[0] = std::atan2(double(rot(2,0)), -double(rot(2,1)));
    }

    if (angles[2] < 0.0)
        angles[2] += 2.0 * M_PI;

    if (angles[0] < 0.0)
        angles[0] += 2.0 * M_PI;

    return angles;
}

Eigen::Matrix3d MoleculeBasis::euler2rot(const double psi, const double theta, const double phi)
{
    Eigen::Matrix3d rot;

    rot(0,0) =  std::cos(psi) * std::cos(phi) - std::cos(theta) * std::sin(phi) * std::sin(psi);
    rot(0,1) =  std::sin(psi) * std::cos(phi) + std::cos(theta) * std::sin(phi) * std::cos(psi);
    rot(0,2) =  std::sin(phi) * std::sin(theta);
    rot(1,0) = -std::cos(psi) * std::sin(phi) - std::cos(theta) * std::cos(phi) * std::sin(psi);
    rot(1,1) = -std::sin(psi) * std::sin(phi) + std::cos(theta) * std::cos(phi) * std::cos(psi);
    rot(1,2) =  std::cos(phi) * std::sin(theta);
    rot(2,0) =  std::sin(theta) * std::sin(psi);
    rot(2,1) = -std::sin(theta) * std::cos(psi);
    rot(2,2) =  std::cos(theta);

    return rot;
}

}

