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


#ifndef MOLECULE_H_
#define MOLECULE_H_

#ifndef Q_MOC_RUN
    #include<chemkit/molecule.h>
    #include<chemkit/atom.h>
#endif
#include<Eigen/Core>
#include "types.h"

namespace molconv
{
    class MoleculePrivate;

    class Molecule : public chemkit::Molecule
    {
    public:
        // constructors and destructor(s):
        Molecule();
        Molecule(const chemkit::Molecule &BaseMolecule);
        Molecule(const Molecule &originalMolecule);

        // info about the molecular internal basis:
        origin internalOrigin() const;
        basis internalBasis() const;
        Eigen::Vector3d internalOriginPosition() const;
        Eigen::Matrix3d internalBasisVectors() const;
        std::array<int,2> internalOriginAtoms() const;
        std::array<int,3> internalBasisAtoms() const;
        double internalOriginFactor() const;

        // info about the inertia tensor and the covariance matrix:
        Eigen::Matrix3d inertiaTensor() const;
        Eigen::Matrix3d covarianceMatrix() const;
        Eigen::Vector3d inertiaEigenvalues() const;
        Eigen::Vector3d covarianceEigenvalues() const;
        Eigen::Matrix3d inertiaEigenvectors() const;
        Eigen::Matrix3d covarianceEigenvectors() const;

        // moving the molecule:
        void translate(const Eigen::Vector3d &shiftVector);
        void rotate(const Eigen::Matrix3d &rotationMatrix);
        void rotate(const Eigen::Vector3d &axis, const double angle);

        // changing the internal basis:
        void setOrigin(const origin &newOrigin, const int atom1 = 0, const int atom2 = 0, const double originFactor = 0);
        void setBasis(const basis &newBasis, const int atom1 = 0, const int atom2 = 0, const int atom3 = 0);

    private:
        // private functions:
        Eigen::Matrix3d calcInertiaTensor() const;
        Eigen::Matrix3d calcCovarianceMatrix() const;
        Eigen::Vector3d calcInertiaEigenvalues() const;
        Eigen::Matrix3d calcInertiaEigenvectors() const;
        Eigen::Vector3d calcCovarianceEigenvalues() const;
        Eigen::Matrix3d calcCovarianceEigenvectors() const;
        MoleculePrivate *d;
    };


} // namespace molconv

#endif /* MOLECULE_H_ */
