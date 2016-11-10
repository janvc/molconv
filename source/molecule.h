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

class MoleculeItem;

namespace molconv
{
    class MoleculePrivate;
    class MoleculeGroup;


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
        std::vector<bool> originList() const;
        std::vector<bool> basisList() const;
        double internalOriginFactor() const;
        double phi() const;
        double theta() const;
        double psi() const;

        Eigen::Vector3d center() const;
        Eigen::Vector3d centerOfMass() const;
        Eigen::Vector3d centerOfCharge() const;

        // info about the inertia tensor and the covariance matrix:
        Eigen::Matrix3d inertiaTensor() const;
        Eigen::Matrix3d chargeTensor() const;
        Eigen::Matrix3d covarianceMatrix() const;
        Eigen::Vector3d inertiaEigenvalues() const;
        Eigen::Vector3d chargeEigenvalues() const;
        Eigen::Vector3d covarianceEigenvalues() const;
        Eigen::Matrix3d inertiaEigenvectors() const;
        Eigen::Matrix3d chargeEigenvectors() const;
        Eigen::Matrix3d covarianceEigenvectors() const;

        void moveFromParas(const double x, const double y, const double z,
                           const double phi, const double theta, const double psi);

        // changing the internal basis:
        void setOrigin(const origin &newOrigin, const size_t atom1 = 0, const size_t atom2 = 0, const double originFactor = 0.0);
        void setBasis(const basis &newBasis, const size_t atom1 = 0, const size_t atom2 = 0, const size_t atom3 = 0);
        void setOriginList(const std::vector<bool> &newList);
        void setBasisList(const std::vector<bool> &newList);

        // manage groups
        void addToGroup(MoleculeGroup *newGroup);
        MoleculeGroup *group() const;

        MoleculeItem *listItem() const;
        void setListItem(MoleculeItem *newItem);

    private:
        // private functions:
        void setInternalOriginPosition();
        void setEulerAngles();
        Eigen::Matrix3d calcInertiaTensor() const;
        Eigen::Matrix3d calcChargeTensor() const;
        Eigen::Matrix3d calcCovarianceMatrix() const;
        Eigen::Vector3d calcInertiaEigenvalues() const;
        Eigen::Matrix3d calcInertiaEigenvectors() const;
        Eigen::Vector3d calcChargeEigenvalues() const;
        Eigen::Matrix3d calcChargeEigenvectors() const;
        Eigen::Vector3d calcCovarianceEigenvalues() const;
        Eigen::Matrix3d calcCovarianceEigenvectors() const;

        void initIntPos();
        Eigen::Matrix3d euler2rot(const double psi, const double theta, const double phi) const;

        boost::scoped_ptr<MoleculePrivate> d;
    };


} // namespace molconv

#endif /* MOLECULE_H_ */
