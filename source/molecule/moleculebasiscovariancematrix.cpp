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
#include "moleculebasiscovariancematrix.h"

namespace molconv {

MoleculeBasisCovarianceMatrix::MoleculeBasisCovarianceMatrix(moleculePtr molecule, std::vector<bool> basisList)
    : MoleculeBasisGlobal(molecule, basisList)
{
    Eigen::Matrix3d covarianceMatrix = calcCovarianceMatrix();

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(covarianceMatrix);
    if (solver.info() != Eigen::Success)
    {
        throw std::runtime_error("The covariance matrix could not be diagonalized.\n");
    }

    // rearrange the basis vectors to align the z-axis with the vector of lowest variance
    // (x and z axes are interchanged)
    Eigen::Matrix3d rot;
    rot.col(0) = solver.eigenvectors().col(2);
    rot.col(1) = solver.eigenvectors().col(1);
    rot.col(2) = solver.eigenvectors().col(0);

    // if the determinant of the internal basis is -1, invert the
    // sign of the middle basis vector to make the basis right-handed
    if (std::abs(double(rot.determinant()) + 1.0) < 1.0e-12)
    {
        rot.col(1) *= -1.0;
    }

    std::array<double,3> eulers = rot2euler(rot);
    m_psi = eulers[0];
    m_theta = eulers[1];
    m_phi = eulers[2];
}

Eigen::Matrix3d MoleculeBasisCovarianceMatrix::calcCovarianceMatrix()
{
    Eigen::Matrix3d covarianceMatrix = Eigen::Matrix3d::Zero();
    Eigen::Vector3d center = m_molecule->center();

    int nActive = 0;
    for (int i = 0; i < m_molecule->size(); i++)
    {
        if (m_basisList.at(i))
        {
            nActive++;
        }
    }

    for (int a = 0; a < 3; a++)
    {
        for (int b = 0; b < 3; b++)
        {
            for (int i = 0; i < m_molecule->size(); i++)
            {
                if (m_basisList.at(i))
                {
                    covarianceMatrix(a, b) += (m_molecule->atom(i)->position()(a) - center(a))
                                            * (m_molecule->atom(i)->position()(b) - center(b));
                }
            }
            covarianceMatrix(a, b) /= double(nActive);
        }
    }

    return covarianceMatrix;
}

}

