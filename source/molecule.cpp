/*
 * Copyright 2014 Jan von Cosel & Sebastian Lenz
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


#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<array>
#include<cmath>
#include<Eigen/Eigenvalues>
#include<chemkit/bondpredictor.h>
#include "molecule.h"
#include "utilities.h"


namespace molconv
{
    class MoleculePrivate
    {
        origin m_origin;
        basis m_basis;

        std::array<int,2> originAtoms;
        std::array<int,3> basisAtoms;
    };

    Molecule::Molecule()
        : chemkit::Molecule()
        , d(new MoleculePrivate)
    {
        d->m_origin = kCenterOnZero;
        d->m_basis = kIdentityVectors;
    }

    Molecule::Molecule(const chemkit::Molecule &BaseMolecule)
        : chemkit::Molecule(BaseMolecule)
        , d(new MoleculePrivate)
    {
    }

    Molecule::Molecule(const Molecule &originalMolecule)
    {
    }

    void Molecule::translate(const Eigen::Vector3d &shiftVector)
    {
    }

    void Molecule::rotate(const Eigen::Matrix3d &rotationMatrix)
    {
    }

} // namespace molconv
