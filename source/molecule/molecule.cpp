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
            // generate the molecule's id:
            int s1 = std::rand();
            int s2 = std::rand();
            m_id = (unsigned long) s1 << 32 | s2;
        }

        MoleculeOrigin *m_origin;
        MoleculeBasis *m_basis;
        MoleculeOrigin *m_originalOrigin;
        MoleculeBasis *m_originalBasis;

        groupPtr m_group;
        std::vector<Eigen::Vector3d> m_intPos;

        MoleculeItem *m_listItem;

        unsigned long m_id;
    };

    ///
    /// \brief Molecule::Molecule
    ///
    /// The default constructor that creates an empty molecule.
    ///
    Molecule::Molecule()
        : chemkit::Molecule()
        , d(new MoleculePrivate)
    {
    }

    ///
    /// \brief Molecule::Molecule
    /// \param BaseMolecule
    ///
    /// This constructor takes an object of the base type chemkit::Molecule
    /// as argument and sets the molconv-specific properties to their
    /// default values.
    ///
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

    ///
    /// \brief Molecule::Molecule
    /// \param originalMolecule
    ///
    /// The Copy constructor.
    ///
    Molecule::Molecule(const Molecule &originalMolecule)
        : chemkit::Molecule(originalMolecule)
        , d(new MoleculePrivate)
    {
        d->m_origin = originalMolecule.internalOrigin();
        d->m_basis = originalMolecule.internalBasis();
        d->m_group = originalMolecule.group();

        initIntPos();
    }

    ///
    /// \brief Molecule::~Molecule
    ///
    /// The default destructor
    ///
    Molecule::~Molecule() {}

    ///
    /// \brief Molecule::internalOrigin
    /// \return
    ///
    /// returns the type of the molecule's internal origin.
    ///
    MoleculeOrigin *Molecule::internalOrigin() const
    {
        return d->m_origin;
    }

    ///
    /// \brief Molecule::internalBasis
    /// \return
    ///
    /// returns the type of the molecule's internal basis.
    ///
    MoleculeBasis *Molecule::internalBasis() const
    {
        return d->m_basis;
    }

    ///
    /// \brief Molecule::internalOriginPosition
    /// \return
    ///
    /// return the position of the internal origin
    ///
    Eigen::Vector3d Molecule::internalOriginPosition() const
    {
        return d->m_origin->position();
    }

    ///
    /// \brief Molecule::internalBasisVectors
    /// \return
    ///
    /// returns the basis vectors of the molecule's internal coordinate
    /// system as columns of the matrix.
    ///
    Eigen::Matrix3d Molecule::internalBasisVectors() const
    {
        return d->m_basis->axes();
    }

    ///
    /// \brief Molecule::internalOriginAtoms
    /// \return
    ///
    /// return the numbers of the atoms that define the internal origin. If the origin is
    /// not defined with atoms, the array will contain zeros.
    ///
    std::array<int,2> Molecule::internalOriginAtoms() const
    {
        return d->m_origin->atoms();
    }

    ///
    /// \brief Molecule::internalBasisAtoms
    /// \return
    ///
    /// return the numbers of the atoms that define the internal coordinate
    /// system. If it is not defined with atoms, the array will contain zeros.
    ///
    std::array<int,3> Molecule::internalBasisAtoms() const
    {
        return d->m_basis->atoms();
    }

    ///
    /// \brief Molecule::originList
    /// \return
    ///
    /// return a list of the atoms contributing to the molecular center
    /// of mass/geometry/charge
    ///
    std::vector<bool> Molecule::originList() const
    {
        return d->m_origin->originList();
    }

    ///
    /// \brief Molecule::basisList
    /// \return
    ///
    /// return a list of atoms contributing to the molecular
    /// inertia tensor / covariance matrix / charge tensor
    ///
    std::vector<bool> Molecule::basisList() const
    {
        return d->m_basis->basisList();
    }

    ///
    /// \brief Molecule::internalOriginFactor
    /// \return
    ///
    /// return the scaling factor for when the internal origin is defined
    /// on the connecting line of two atoms. The factor determines if the
    /// origin is on the first atom (f=0), the second atom (f=1) or exactly
    /// between the atoms (f=0.5).
    ///
    double Molecule::internalOriginFactor() const
    {
        return d->m_origin->factor();
    }

    ///
    /// \brief Molecule::phi
    /// \return
    ///
    /// calculate the euler angle phi from the current internal basis
    ///
    double Molecule::phi() const
    {
        return d->m_basis->phi();
    }

    ///
    /// \brief Molecule::theta
    /// \return
    ///
    /// calculate the euler angle theta from the current internal basis
    ///
    double Molecule::theta() const
    {
        return d->m_basis->theta();
    }

    ///
    /// \brief Molecule::psi
    /// \return
    ///
    /// calculate the euler angle psi from the current internal basis
    ///
    double Molecule::psi() const
    {
        return d->m_basis->psi();
    }

    ///
    /// \brief Molecule::origBasis
    /// \return
    ///
    /// return the original values of the molecule's internal orientation
    ///
    std::array<double,6> Molecule::origBasis() const
    {
//        return d->m_originalBasis;
        return std::array<double,6>();
    }

    ///
    /// \brief Molecule::internalPositions
    /// \return
    ///
    /// return the internal positions of the atoms
    ///
    std::vector<Eigen::Vector3d> Molecule::internalPositions() const
    {
        return d->m_intPos;
    }

    ///
    /// \brief Molecule::centerOfCharge
    /// \return
    ///
    /// calculate the molecular center of charge
    ///
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

    ///
    /// \brief Molecule::inertiaTensor
    /// \return
    ///
    /// returns the inertia tensor of the molecule.
    ///
    Eigen::Matrix3d Molecule::inertiaTensor() const
    {
        return calcInertiaTensor();
    }

    ///
    /// \brief Molecule::chargeTensor
    /// \return
    ///
    /// returns the charge tensor of the molecule
    ///
    Eigen::Matrix3d Molecule::chargeTensor() const
    {
        return calcChargeTensor();
    }

    ///
    /// \brief Molecule::covarianceMatrix
    /// \return
    ///
    /// returns the covariance matrix of the molecule.
    ///
    Eigen::Matrix3d Molecule::covarianceMatrix() const
    {
        return calcCovarianceMatrix();
    }

    ///
    /// \brief Molecule::inertiaEigenvalues
    /// \return
    ///
    /// returns the eigenvalues of the inertia tensor in a vector
    ///
    Eigen::Vector3d Molecule::inertiaEigenvalues() const
    {
        return calcInertiaEigenvalues();
    }

    ///
    /// \brief Molecule::chargeEigenvalues
    /// \return
    ///
    /// return the eigenvalues of the charge tensor
    ///
    Eigen::Vector3d Molecule::chargeEigenvalues() const
    {
        return calcChargeEigenvalues();
    }

    ///
    /// \brief Molecule::covarianceEigenvalues
    /// \return
    ///
    /// returns the eigenvalues of the covariance matrix in a vector
    ///
    Eigen::Vector3d Molecule::covarianceEigenvalues() const
    {
        return calcCovarianceEigenvalues();
    }

    ///
    /// \brief Molecule::inertiaEigenvectors
    /// \return
    ///
    /// returns the eigenvectors of the inertia tensor as the columns of the matrix
    ///
    Eigen::Matrix3d Molecule::inertiaEigenvectors() const
    {
        return calcInertiaEigenvectors();
    }

    ///
    /// \brief Molecule::chargeEigenvectors
    /// \return
    ///
    /// returns the eigenvectors of the charge tensor
    ///
    Eigen::Matrix3d Molecule::chargeEigenvectors() const
    {
        return calcChargeEigenvectors();
    }

    ///
    /// \brief Molecule::covarianceEigenvectors
    /// \return
    ///
    /// returns the eigenvectors of the covariance matrix as the columns of the matrix
    ///
    Eigen::Matrix3d Molecule::covarianceEigenvectors() const
    {
        return calcCovarianceEigenvectors();
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
    /// \brief Molecule::calcInertiaTensor
    /// \return
    ///
    /// calculate the inertia tensor of the molecule.
    ///
    Eigen::Matrix3d Molecule::calcInertiaTensor() const
    {
        Eigen::Matrix3d inertiaTensor;
        Eigen::Vector3d com = centerOfMass();

        for (size_t alpha = 0; alpha < 3; alpha++)
            for (size_t beta = 0; beta < 3; beta++)
            {
                inertiaTensor(alpha, beta) = 0.0;

                for (size_t atiter = 0; atiter < size(); atiter++)
                    if (basisList().at(atiter))
                    {
                        double factor = 0.0;

                        if (alpha == beta)
                            factor = (atom(atiter)->position() - com).squaredNorm();

                        factor -= (atom(atiter)->position() - com)(alpha) * (atom(atiter)->position() - com)(beta);

                        inertiaTensor(alpha, beta) += atom(atiter)->mass() * factor;
                    }
            }

        return inertiaTensor;
    }

    ///
    /// \brief Molecule::calcChargeTensor
    /// \return
    ///
    /// calculate the molecular tensor of charge
    ///
    Eigen::Matrix3d Molecule::calcChargeTensor() const
    {
        Eigen::Matrix3d chargeTensor;
        Eigen::Vector3d coc = centerOfCharge();

        for (size_t alpha = 0; alpha < 3; alpha++)
            for (size_t beta = 0; beta < 3; beta++)
            {
                chargeTensor(alpha, beta) = 0.0;

                for (size_t atiter = 0; atiter < size(); atiter++)
                    if (basisList().at(atiter))
                    {
                        double factor = 0.0;

                        if (alpha == beta)
                            factor = (atom(atiter)->position() - coc).squaredNorm();

                        factor -= (atom(atiter)->position() - coc)(alpha) * (atom(atiter)->position() - coc)(beta);

                        chargeTensor(alpha, beta) += double(atom(atiter)->atomicNumber()) * factor;
                    }
            }


        return chargeTensor;
    }

    ///
    /// \brief Molecule::calcCovarianceMatrix
    /// \return
    ///
    /// calculate the covariance matrix of the molecule
    ///
    Eigen::Matrix3d Molecule::calcCovarianceMatrix() const
    {
        Eigen::Matrix3d covarianceMatrix;
        Eigen::Vector3d cog = center();

        int Nactive = 0;
        for (int i = 0; i < int(size()); i++)
            if (basisList().at(i))
                Nactive++;

        for (size_t alpha = 0; alpha < 3; alpha++)
            for (size_t beta = 0; beta < 3; beta++)
            {
                covarianceMatrix(alpha, beta) = 0.0;

                for (size_t atiter = 0; atiter < size(); atiter++)
                    if (basisList().at(atiter))
                        covarianceMatrix(alpha, beta) += (atom(atiter)->position()(alpha) - cog(alpha))
                                                       * (atom(atiter)->position()(beta)  - cog(beta));
                covarianceMatrix(alpha, beta) /= double(Nactive);
            }
        return covarianceMatrix;
    }

    ///
    /// \brief Molecule::calcInertiaEigenvalues
    /// \return
    ///
    /// calculate the eigenvalues of the molecule's inertia tensor
    ///
    Eigen::Vector3d Molecule::calcInertiaEigenvalues() const
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(calcInertiaTensor());

        if (solver.info() != Eigen::Success)
            throw std::runtime_error("The inertia tensor could not be diagonalized.\n");
        else
            return solver.eigenvalues();
    }

    ///
    /// \brief Molecule::calcInertiaEigenvectors
    /// \return
    ///
    /// calculate the eigenvectors of the molecule's inertia tensor
    /// and return them as columns of the matrix.
    ///
    Eigen::Matrix3d Molecule::calcInertiaEigenvectors() const
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(calcInertiaTensor());

        if (solver.info() != Eigen::Success)
            throw std::runtime_error("The inertia tensor could not be diagonalized.\n");
        else
            return solver.eigenvectors();
    }

    ///
    /// \brief Molecule::calcChargeEigenvalues
    /// \return
    ///
    /// calculate the eigenvalues of the molecular charge tensor
    ///
    Eigen::Vector3d Molecule::calcChargeEigenvalues() const
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(calcChargeTensor());

        if (solver.info() != Eigen::Success)
            throw std::runtime_error("The charge tensor could not be diagonalized.\n");
        else
            return solver.eigenvalues();
    }

    ///
    /// \brief Molecule::calcChargeEigenvectors
    /// \return
    ///
    /// calculate the eigenvectors of the molecular charge tensor
    ///
    Eigen::Matrix3d Molecule::calcChargeEigenvectors() const
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(calcChargeTensor());

        if (solver.info() != Eigen::Success)
            throw std::runtime_error("The charge tensor could not be diagonalized.\n");
        else
            return solver.eigenvectors();
    }

    ///
    /// \brief Molecule::calcCovarianceEigenvalues
    /// \return
    ///
    /// calculate the eigenvalues of the molecule's covariance matrix.
    ///
    Eigen::Vector3d Molecule::calcCovarianceEigenvalues() const
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(calcCovarianceMatrix());

        if (solver.info() != Eigen::Success)
            throw std::runtime_error("The covariance matrix could not be diagonalized.\n");
        else
            return solver.eigenvalues();
    }

    ///
    /// \brief Molecule::calcCovarianceEigenvectors
    /// \return
    ///
    /// calculate the eigenvectors of the molecule's covariance matrix
    /// and return them as columns of the matrix.
    ///
    Eigen::Matrix3d Molecule::calcCovarianceEigenvectors() const
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(calcCovarianceMatrix());

        if (solver.info() != Eigen::Success)
            throw std::runtime_error("The covariance matrix could not be diagonalized.\n");
        else
            return solver.eigenvectors();
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
        Eigen::Vector3d origin = Eigen::Vector3d::Zero();
        if (internalOrigin())
        {
            origin = internalOriginPosition();
        }

        Eigen::Matrix3d rotMat = Eigen::Matrix3d::Identity();
        if (internalBasis())
        {
            rotMat = internalBasisVectors();
        }

        for (int i = 0; i < int(size()); i++)
        {
            d->m_intPos.push_back(rotMat.transpose() * (atom(i)->position() - origin));
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
