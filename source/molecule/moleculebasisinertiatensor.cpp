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


#include <Eigen/Eigenvalues>
#include "moleculebasisinertiatensor.h"

namespace molconv {

MoleculeBasisInertiaTensor::MoleculeBasisInertiaTensor(moleculePtr molecule, std::vector<bool> basisList)
    : MoleculeBasisGlobal(molecule, basisList)
{
    Eigen::Matrix3d inertiaTensor = calcInertiaTensor();

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(inertiaTensor);
    if (solver.info() != Eigen::Success)
    {
        throw std::runtime_error("The inertia tensor could not be diagonalized.\n");
    }

    setEulerAngles(solver.eigenvectors());
}

MoleculeBasisInertiaTensor::MoleculeBasisInertiaTensor(const MoleculeBasisInertiaTensor &basis)
    : MoleculeBasisGlobal(basis)
{
    m_molecule = basis.molecule();
    m_phi = basis.phi();
    m_theta = basis.theta();
    m_psi = basis.psi();
    m_basisList = basis.basisList();
}

MoleculeBasis *MoleculeBasisInertiaTensor::clone()
{
    return new MoleculeBasisInertiaTensor(*this);
}

BasisCode MoleculeBasisInertiaTensor::code() const
{
    return kInertiaVectors;
}

Eigen::Matrix3d MoleculeBasisInertiaTensor::calcInertiaTensor()
{
    Eigen::Matrix3d inertiaTensor = Eigen::Matrix3d::Zero();
    Eigen::Vector3d com = m_molecule->centerOfMass();

    for (int a = 0; a < 3; a++)
    {
        for (int b = 0; b < 3; b++)
        {
            for (int i = 0; i < int(m_molecule->size()); i++)
            {
                if (m_basisList.at(i))
                {
                    Eigen::Vector3d distance = m_molecule->atom(i)->position() - com;

                    double factor = 0.0;
                    if (a == b)
                    {
                        factor = distance.squaredNorm();
                    }
                    factor -= distance(a) * distance(b);

                    inertiaTensor(a, b) += m_molecule->atom(i)->mass() * factor;
                }
            }
        }
    }

    return inertiaTensor;
}

}
