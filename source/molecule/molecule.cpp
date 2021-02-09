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


#include<iostream>
#include<array>
#include<stdexcept>
#include<iomanip>
#include<random>
#include<Eigen/Geometry>
#include<Eigen/Eigenvalues>
#include<chemkit/bondpredictor.h>
#include "molecule.h"
#include "moleculeitem.h"
#include "moleculeoriginonatom.h"
#include "moleculeoriginbetweenatoms.h"
#include "moleculeoriginglobal.h"
#include "moleculeorigingeometriccenter.h"
#include "moleculeorigincenterofmass.h"
#include "moleculebasisonatoms.h"
#include "moleculebasisglobal.h"
#include "moleculebasiscovariancematrix.h"
#include "moleculebasisinertiatensor.h"


namespace molconv
{
    class MoleculePrivate
    {
    public:
        MoleculePrivate()
        {
            m_originalOriginBasis.fill(0);

            // generate the molecule's id:
            int s1 = std::rand();
            int s2 = std::rand();
            m_id = (unsigned long) s1 << 32 | s2;
        }

        MoleculeOrigin *m_origin;
        MoleculeBasis *m_basis;
        std::array<double, 6> m_originalOriginBasis;

        groupPtr m_group;
        std::vector<Eigen::Vector3d> m_intPos;

        MoleculeItem *m_listItem;

        unsigned long m_id;
    };

    Molecule::Molecule()
        : chemkit::Molecule()
        , d(new MoleculePrivate)
    {
    }

    Molecule::Molecule(const chemkit::Molecule &BaseMolecule)
        : chemkit::Molecule(BaseMolecule)
        , d(new MoleculePrivate)
    {
        chemkit::BondPredictor::predictBonds(this);

        std::vector<bool> originBasisList(size(), true);
        d->m_origin = new MoleculeOriginGeometricCenter(moleculePtr(this), originBasisList);
        d->m_basis = new MoleculeBasisCovarianceMatrix(moleculePtr(this), originBasisList);

        initIntPos();
    }

    Molecule::Molecule(const boost::shared_ptr<chemkit::Molecule> &BaseMolPtr)
        : chemkit::Molecule(*BaseMolPtr)
        , d(new MoleculePrivate)
    {
        chemkit::BondPredictor::predictBonds(this);

        std::vector<bool> originBasisList(size(), true);
        d->m_origin = new MoleculeOriginGeometricCenter(moleculePtr(this), originBasisList);
        d->m_basis = new MoleculeBasisCovarianceMatrix(moleculePtr(this), originBasisList);

        initIntPos();
    }

    Molecule::Molecule(const Molecule &originalMolecule)
        : chemkit::Molecule(originalMolecule)
        , d(new MoleculePrivate)
    {
        d->m_origin = originalMolecule.origin();
        d->m_basis = originalMolecule.basis();
        d->m_group = originalMolecule.group();

        initIntPos();
    }

    Molecule::~Molecule() {}

    MoleculeOrigin *Molecule::origin() const
    {
        return d->m_origin;
    }

    MoleculeBasis *Molecule::basis() const
    {
        return d->m_basis;
    }

    Eigen::Vector3d Molecule::originPosition() const
    {
        return d->m_origin ? d->m_origin->position() : Eigen::Vector3d::Zero();
    }

    Eigen::Matrix3d Molecule::basisVectors() const
    {
        return d->m_basis ? d->m_basis->axes() : Eigen::Matrix3d::Identity();
    }

    std::array<int,2> Molecule::originAtoms() const
    {
        return d->m_origin ? d->m_origin->atoms() : std::array<int,2>();
    }

    std::array<int,3> Molecule::basisAtoms() const
    {
        return d->m_basis ? d->m_basis->atoms() : std::array<int,3>();
    }

    std::vector<bool> Molecule::originList() const
    {
        return d->m_origin ? d->m_origin->originList() : std::vector<bool>();
    }

    std::vector<bool> Molecule::basisList() const
    {
        return d->m_basis ? d->m_basis->basisList() : std::vector<bool>();
    }

    double Molecule::originFactor() const
    {
        return d->m_origin ? d->m_origin->factor() : 0.0;
    }

    double Molecule::phi() const
    {
        return d->m_basis ? d->m_basis->phi() : 0.0;
    }

    double Molecule::theta() const
    {
        return d->m_basis ? d->m_basis->theta() : 0.0;
    }

    double Molecule::psi() const
    {
        return d->m_basis ? d->m_basis->psi() : 0.0;
    }

    std::array<double,6> Molecule::originalBasis() const
    {
        return d->m_originalOriginBasis;
    }

    std::vector<Eigen::Vector3d> Molecule::internalPositions() const
    {
        return d->m_intPos;
    }

    Eigen::Vector3d Molecule::centerOfCharge() const
    {
        Eigen::Vector3d coc = Eigen::Vector3d::Zero();
        double totalCharge = 0.0;

        for (int i = 0; i < int(size()); i++)
        {
            coc += atom(i)->position() * double(atom(i)->atomicNumber());
            totalCharge += double(atom(i)->atomicNumber());
        }

        return coc / totalCharge;

    }

    Eigen::Matrix3d Molecule::inertiaTensor() const
    {
        Eigen::Matrix3d inertiaTensor;
        Eigen::Vector3d com = centerOfMass();

        for (size_t alpha = 0; alpha < 3; alpha++)
        {
            for (size_t beta = 0; beta < 3; beta++)
            {
                inertiaTensor(alpha, beta) = 0.0;

                for (size_t atiter = 0; atiter < size(); atiter++)
                {
                    double factor = 0.0;

                    if (alpha == beta)
                    {
                        factor = (atom(atiter)->position() - com).squaredNorm();
                    }

                    factor -= (atom(atiter)->position() - com)(alpha) * (atom(atiter)->position() - com)(beta);

                    inertiaTensor(alpha, beta) += atom(atiter)->mass() * factor;

                }
            }
        }

        return inertiaTensor;
    }

    Eigen::Matrix3d Molecule::chargeTensor() const
    {
        Eigen::Matrix3d chargeTensor;
        Eigen::Vector3d coc = centerOfCharge();

        for (size_t alpha = 0; alpha < 3; alpha++)
        {
            for (size_t beta = 0; beta < 3; beta++)
            {
                chargeTensor(alpha, beta) = 0.0;

                for (size_t atiter = 0; atiter < size(); atiter++)
                {
                    double factor = 0.0;

                    if (alpha == beta)
                        factor = (atom(atiter)->position() - coc).squaredNorm();

                    factor -= (atom(atiter)->position() - coc)(alpha) * (atom(atiter)->position() - coc)(beta);

                    chargeTensor(alpha, beta) += double(atom(atiter)->atomicNumber()) * factor;

                }
            }
        }

        return chargeTensor;
    }

    Eigen::Matrix3d Molecule::covarianceMatrix() const
    {
        Eigen::Matrix3d covarianceMatrix;
        Eigen::Vector3d cog = center();

        for (size_t alpha = 0; alpha < 3; alpha++)
        {
            for (size_t beta = 0; beta < 3; beta++)
            {
                covarianceMatrix(alpha, beta) = 0.0;

                for (size_t atiter = 0; atiter < size(); atiter++)
                {
                    covarianceMatrix(alpha, beta) += (atom(atiter)->position()(alpha) - cog(alpha))
                            * (atom(atiter)->position()(beta)  - cog(beta));

                }
                covarianceMatrix(alpha, beta) /= double(size());
            }
        }

        return covarianceMatrix;
    }

    Eigen::Vector3d Molecule::inertiaEigenvalues() const
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(inertiaTensor());

        if (solver.info() != Eigen::Success)
        {
            throw std::runtime_error("The inertia tensor could not be diagonalized.\n");
        }
        else
        {
            return solver.eigenvalues();
        }
    }

    Eigen::Vector3d Molecule::chargeEigenvalues() const
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(chargeTensor());

        if (solver.info() != Eigen::Success)
        {
            throw std::runtime_error("The charge tensor could not be diagonalized.\n");
        }
        else
        {
            return solver.eigenvalues();
        }
    }

    Eigen::Vector3d Molecule::covarianceEigenvalues() const
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(covarianceMatrix());

        if (solver.info() != Eigen::Success)
        {
            throw std::runtime_error("The covariance matrix could not be diagonalized.\n");
        }
        else
        {
            return solver.eigenvalues();
        }
    }

    Eigen::Matrix3d Molecule::inertiaEigenvectors() const
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(inertiaTensor());

        if (solver.info() != Eigen::Success)
        {
            throw std::runtime_error("The inertia tensor could not be diagonalized.\n");
        }
        else
        {
            return solver.eigenvectors();
        }
    }

    Eigen::Matrix3d Molecule::chargeEigenvectors() const
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(chargeTensor());

        if (solver.info() != Eigen::Success)
        {
            throw std::runtime_error("The charge tensor could not be diagonalized.\n");
        }
        else
        {
            return solver.eigenvectors();
        }
    }

    Eigen::Matrix3d Molecule::covarianceEigenvectors() const
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(covarianceMatrix());

        if (solver.info() != Eigen::Success)
        {
            throw std::runtime_error("The covariance matrix could not be diagonalized.\n");
        }
        else
        {
            return solver.eigenvectors();
        }
    }

    ///
    /// \brief moveFromParas
    /// \param x
    /// \param y
    /// \param z
    /// \param phi
    /// \param psi
    /// \param theta
    ///
    /// take the atomic coordinates w.r.t. the internal basis and transform them according
    /// to the given translation vector and euler angles to construct the position
    /// and orientation of the molecule in the global coordinate system
    ///
    void Molecule::moveFromParas(const double x, const double y, const double z, const double phi, const double theta, const double psi)
    {
        Eigen::Vector3d pos(x, y, z);

        d->m_origin->setPosition(pos);
        d->m_basis->setPsi(psi);
        d->m_basis->setTheta(theta);
        d->m_basis->setPhi(phi);

        Eigen::Matrix3d rot = d->m_basis->axes();

        for (int i = 0; i < int(size()); i++)
            atom(i)->setPosition(pos + rot * d->m_intPos.at(i));
    }

    ///
    /// \brief Molecule::setOrigin
    /// \param newOrigin
    ///
    /// set the molecule's internal origin to \p newOrigin.
    ///
    void Molecule::setOrigin(const OriginCode &newOrigin, const std::vector<bool> originVector, const size_t atom1, const size_t atom2, const double originFactor)
    {
        switch (newOrigin)
        {
        case kCenterOfMass:
            d->m_origin = new MoleculeOriginCenterOfMass(moleculePtr(this), originVector);
            break;
        case kCenterOfGeometry:
            d->m_origin = new MoleculeOriginGeometricCenter(moleculePtr(this), originVector);
            break;
        case kCenterOfCharge:
            break;
        case kCenterOnAtom:
            d->m_origin = new MoleculeOriginOnAtom(moleculePtr(this), atom1);
            break;
        case kCenterBetweenAtoms:
            d->m_origin = new MoleculeOriginBetweenAtoms(moleculePtr(this), atom1, atom2, originFactor);
            break;
        }

        initIntPos();
    }

    ///
    /// \brief Molecule::setBasis
    /// \param newBasis
    ///
    /// set the molecule's internal coordinate system to \p newBasis.
    ///
    void Molecule::setBasis(const BasisCode &newBasis, const std::vector<bool> basisVector, const size_t atom1, const size_t atom2, const size_t atom3)
    {
        switch (newBasis)
        {
        case kCovarianceVectors:
            d->m_basis = new MoleculeBasisCovarianceMatrix(moleculePtr(this), basisVector);
            break;
        case kInertiaVectors:
            d->m_basis = new MoleculeBasisInertiaTensor(moleculePtr(this), basisVector);
            break;
        case kChargeVectors:
            break;
        case kStandardOrientation:
            break;
        case kVectorsFromAtoms:
            d->m_basis = new MoleculeBasisOnAtoms(moleculePtr(this), atom1, atom2, atom3);
            break;
        }
        initIntPos();
    }

    ///
    /// \brief Molecule::addToGroup
    /// \param newGroup
    ///
    /// This method adds a poiner to the new group, that the molecule now belongs to
    ///
    void Molecule::addToGroup(groupPtr newGroup)
    {
        d->m_group = newGroup;
    }

    ///
    /// \brief Molecule::group
    /// \return
    ///
    /// This method returns the group that this molecule belongs to
    ///
    groupPtr Molecule::group() const
    {
        return d->m_group;
    }

    ///
    /// \brief Molecule::initIntPos
    ///
    /// Initialize the atomic positions w.r.t. the internal
    /// coordinate system of the molecule
    ///
    void Molecule::initIntPos()
    {
        // remove any internal coordinates that might already exist:
        d->m_intPos.clear();

        // determine the internal atomic positions:
        Eigen::Vector3d shiftVec = Eigen::Vector3d::Zero();
        if (origin())
        {
            shiftVec = originPosition();
        }

        Eigen::Matrix3d rotMat = Eigen::Matrix3d::Identity();
        if (basis())
        {
            rotMat = basisVectors();
        }

        d->m_originalOriginBasis[0] = shiftVec(0);
        d->m_originalOriginBasis[1] = shiftVec(1);
        d->m_originalOriginBasis[2] = shiftVec(2);
        d->m_originalOriginBasis[3] = phi();
        d->m_originalOriginBasis[4] = theta();
        d->m_originalOriginBasis[5] = psi();

        for (int i = 0; i < int(size()); i++)
        {
            d->m_intPos.push_back(rotMat.transpose() * (atom(i)->position() - shiftVec));
        }
    }

    unsigned long Molecule::molId() const
    {
        return d->m_id;
    }

    void Molecule::initRand()
    {
        std::srand(std::time(0));
    }

} // namespace molconv
