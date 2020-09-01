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


#include "moleculebasisonatoms.h"

namespace molconv {

MoleculeBasisOnAtoms::MoleculeBasisOnAtoms(moleculePtr molecule, const int atom1, const int atom2, const int atom3)
    : MoleculeBasis(molecule)
{
    m_atom1 = atom1;
    m_atom2 = atom2;
    m_atom3 = atom3;

    Eigen::Vector3d vector1, vector2, vector3;

    vector1 = m_molecule->atom(atom2)->position() - m_molecule->atom(atom1)->position();
    vector1.normalize();

    vector2 = m_molecule->atom(atom3)->position() - m_molecule->atom(atom1)->position();
    vector2 -= vector1 * vector1.dot(vector2);
    vector2.normalize();

    vector3 = vector1.cross(vector2);
    vector3.normalize();

    Eigen::Matrix3d rot;
    rot.col(0) = vector1;
    rot.col(1) = vector2;
    rot.col(2) = vector3;

    setEulerAngles(rot);
}

std::vector<bool> MoleculeBasisOnAtoms::basisList() const
{
    return std::vector<bool>();
}

std::array<int,3> MoleculeBasisOnAtoms::atoms() const
{
    std::array<int,3> array;
    array[0] = m_atom1;
    array[1] = m_atom2;
    array[2] = m_atom3;

    return array;
}

}
