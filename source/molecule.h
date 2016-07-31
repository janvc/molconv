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


#ifndef MOLECULE_H_
#define MOLECULE_H_

#ifndef Q_MOC_RUN
    #include<chemkit/molecule.h>
    #include<chemkit/atom.h>
    #include<boost/scoped_ptr.hpp>
#endif
#include<Eigen/Core>
#include "types.h"
#include "abstractmoleculegroup.h"

namespace molconv
{
    class MoleculePrivate;
    class abstractMoleculeGroup;
    class MoleculeListItem;

    class Molecule : public chemkit::Molecule
    {
    public:
        // constructors and destructor(s):
        Molecule();
        Molecule(const chemkit::Molecule &BaseMolecule);
        Molecule(const boost::shared_ptr<chemkit::Molecule> &BaseMolPtr);
        Molecule(const Molecule &originalMolecule);
        ~Molecule();

        // info about the molecular internal basis:
        origin internalOrigin() const;
        basis internalBasis() const;
        Eigen::Vector3d internalOriginPosition() const;
        Eigen::Matrix3d internalBasisVectors() const;
        std::array<int,2> internalOriginAtoms() const;
        std::array<int,3> internalBasisAtoms() const;
        double internalOriginFactor() const;
        double phi() const;
        double theta() const;
        double psi() const;

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
        void setPhi(const double newPhi);
        void setTheta(const double newTheta);
        void setPsi(const double newPsi);
        void moveFromParas(const double x, const double y, const double z, const double phi, const double theta, const double psi);

        // changing the internal basis:
        void setOrigin(const origin &newOrigin, const size_t atom1 = 0, const size_t atom2 = 0, const double originFactor = 0.0);
        void setBasis(const basis &newBasis, const size_t atom1 = 0, const size_t atom2 = 0, const size_t atom3 = 0);

        // clean up the coordinates
        void cleanUp();

        // manage groups
        void addToGroup(const groupPtr newGroup);
        groupPtr &group() const;

        boost::shared_ptr<MoleculeListItem> listItem() const;
        void setListItem(boost::shared_ptr<MoleculeListItem> &newItem);

    private:
        // private functions:
        Eigen::Matrix3d calcInertiaTensor() const;
        Eigen::Matrix3d calcCovarianceMatrix() const;
        Eigen::Vector3d calcInertiaEigenvalues() const;
        Eigen::Matrix3d calcInertiaEigenvectors() const;
        Eigen::Vector3d calcCovarianceEigenvalues() const;
        Eigen::Matrix3d calcCovarianceEigenvectors() const;

        void initIntPos();
        Eigen::Matrix3d euler2rot(const double psi, const double theta, const double phi) const;

        boost::scoped_ptr<MoleculePrivate> d;
    };


} // namespace molconv

#endif /* MOLECULE_H_ */
