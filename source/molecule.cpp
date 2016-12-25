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


#include<iostream>
#include<array>
#include<stdexcept>
#include<iomanip>
#include<Eigen/Geometry>
#include<Eigen/Eigenvalues>
#include<chemkit/bondpredictor.h>
#include "molecule.h"
#include "moleculeitem.h"


namespace molconv
{
    class MoleculePrivate
    {
    public:
        MoleculePrivate()
        {
            m_origin = kCenterOfGeometry;
            m_basis = kCovarianceVectors;
            m_originFactor = 0.0;
            m_originAtoms.fill(0);
            m_basisAtoms.fill(0);
            m_originalBasis.fill(0.0);
            m_listItem = 0;
            m_phi = 0;
            m_theta = 0;
            m_psi = 0;
        }

        origin m_origin;
        basis m_basis;

        Eigen::Vector3d m_originPosition;

        double m_phi;
        double m_theta;
        double m_psi;

        double m_originFactor;
        std::array<int,2> m_originAtoms;
        std::array<int,3> m_basisAtoms;

        groupPtr m_group;
        std::vector<Eigen::Vector3d> m_intPos;

        MoleculeItem *m_listItem;

        std::vector<bool> m_originList;
        std::vector<bool> m_basisList;

        std::array<double,6> m_originalBasis;
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
        for (int i = 0; i < int(size()); i++)
        {
            d->m_originList.push_back(true);
            d->m_basisList.push_back(true);
        }
        initIntPos();
    }


    Molecule::Molecule(const boost::shared_ptr<chemkit::Molecule> &BaseMolPtr)
        : chemkit::Molecule(*BaseMolPtr)
        , d(new MoleculePrivate)
    {
        chemkit::BondPredictor::predictBonds(this);
        for (int i = 0; i < int(size()); i++)
        {
            d->m_originList.push_back(true);
            d->m_basisList.push_back(true);
        }
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
        d->m_originFactor = originalMolecule.internalOriginFactor();
        d->m_originAtoms = originalMolecule.internalOriginAtoms();
        d->m_basisAtoms = originalMolecule.internalBasisAtoms();
        d->m_group = originalMolecule.group();
        d->m_originList = originalMolecule.originList();
        d->m_basisList = originalMolecule.basisList();
        d->m_listItem = originalMolecule.listItem();

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
    origin Molecule::internalOrigin() const
    {
        return d->m_origin;
    }

    ///
    /// \brief Molecule::internalBasis
    /// \return
    ///
    /// returns the type of the molecule's internal basis.
    ///
    basis Molecule::internalBasis() const
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
        return d->m_originPosition;
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
        return euler2rot(d->m_psi, d->m_theta, d->m_phi);
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
        return d->m_originAtoms;
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
        return d->m_basisAtoms;
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
        return d->m_originList;
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
        return d->m_basisList;
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
        return d->m_originFactor;
    }

    ///
    /// \brief Molecule::phi
    /// \return
    ///
    /// calculate the euler angle phi from the current internal basis
    ///
    double Molecule::phi() const
    {
        return d->m_phi;
    }

    ///
    /// \brief Molecule::theta
    /// \return
    ///
    /// calculate the euler angle theta from the current internal basis
    ///
    double Molecule::theta() const
    {
        return d->m_theta;
    }

    ///
    /// \brief Molecule::psi
    /// \return
    ///
    /// calculate the euler angle psi from the current internal basis
    ///
    double Molecule::psi() const
    {
        return d->m_psi;
    }

    ///
    /// \brief Molecule::origBasis
    /// \return
    ///
    /// return the original values of the molecule's internal orientation
    ///
    std::array<double,6> Molecule::origBasis() const
    {
        return d->m_originalBasis;
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
    /// \brief Molecule::center
    /// \return
    ///
    /// calculate the molecular center of geometry considering
    /// the list of atoms contributing to the center
    ///
    Eigen::Vector3d Molecule::center() const
    {
        Eigen::Vector3d cog = Eigen::Vector3d::Zero();
        int Nactive = 0;

        for (int i = 0; i < int(size()); i++)
            if (originList().at(i))
            {
                cog += atom(i)->position();
                Nactive++;
            }

        return cog / double(Nactive);
    }

    ///
    /// \brief Molecule::centerOfMass
    /// \return
    ///
    /// calculate the molecular center of mass considering
    /// the list of atoms contributing to the center
    ///
    Eigen::Vector3d Molecule::centerOfMass() const
    {
        Eigen::Vector3d com = Eigen::Vector3d::Zero();
        double totalMass = 0.0;

        for (int i = 0; i < int(size()); i++)
            if (originList().at(i))
            {
                com += atom(i)->position() * atom(i)->mass();
                totalMass += atom(i)->mass();
            }

        return com / totalMass;
    }

    ///
    /// \brief Molecule::centerOfCharge
    /// \return
    ///
    /// calculate the molecular center of charge considering
    /// the list of atoms contributing to the center
    ///
    Eigen::Vector3d Molecule::centerOfCharge() const
    {
        Eigen::Vector3d coc = Eigen::Vector3d::Zero();
        double totalCharge = 0.0;

        for (int i = 0; i < int(size()); i++)
            if (originList().at(i))
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

        d->m_psi = psi;
        d->m_theta = theta;
        d->m_phi = phi;

        Eigen::Matrix3d rot = euler2rot(psi, theta, phi);

        for (int i = 0; i < int(size()); i++)
            atom(i)->setPosition(pos + rot * d->m_intPos.at(i));

        setInternalOriginPosition();
    }

    ///
    /// \brief Molecule::setOrigin
    /// \param newOrigin
    ///
    /// set the molecule's internal origin to \p newOrigin.
    ///
    void Molecule::setOrigin(const origin &newOrigin, const size_t atom1, const size_t atom2, const double originFactor)
    {
        switch (newOrigin)
        {
        case kCenterOfMass:
        case kCenterOfGeometry:
        case kCenterOfCharge:
            d->m_origin = newOrigin;
            d->m_originAtoms.fill(0);
            break;
        case kCenterOnAtom:
            if (atom1 > 0 && atom1 <= size())
            {
                d->m_origin = newOrigin;
                d->m_originAtoms[0] = atom1 - 1;
                d->m_originAtoms[1] = 0;
            }
            break;
        case kCenterBetweenAtoms:
            if (atom1 > 0 && atom1 <= size() && atom2 > 0 && atom2 <= size()
                          && originFactor >= 0.0 && originFactor <= 1.0)
            {
                d->m_origin = newOrigin;
                d->m_originAtoms[0] = atom1 - 1;
                d->m_originAtoms[1] = atom2 - 1;
                d->m_originFactor = originFactor;
            }
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
    void Molecule::setBasis(const basis &newBasis, const size_t atom1, const size_t atom2, const size_t atom3)
    {
        switch (newBasis)
        {
        case kCovarianceVectors:
        case kInertiaVectors:
        case kChargeVectors:
        case kStandardOrientation:
            d->m_basis = newBasis;
            d->m_basisAtoms.fill(0);
            break;
        case kVectorsFromAtoms:
            if (atom1 > 0 && atom1 <= size() && atom2 > 0 && atom2 <= size() && atom3 > 0 && atom3 <= size())
            {
                if (atom1 != atom2 && atom1 != atom3 && atom2 != atom3)
                {
                    d->m_basis = newBasis;
                    d->m_basisAtoms[0] = atom1 - 1;
                    d->m_basisAtoms[1] = atom2 - 1;
                    d->m_basisAtoms[2] = atom3 - 1;
                }
            }
            break;
        }
        initIntPos();
    }

    ///
    /// \brief Molecule::setOriginList
    /// \param newList
    ///
    /// set the list of atoms contributing to the molecular
    /// center of mass/geometry/charge
    ///
    void Molecule::setOriginList(const std::vector<bool> &newList)
    {
        if (newList.size() == size())
            d->m_originList = newList;
    }

    ///
    /// \brief Molecule::setBasisList
    /// \param newList
    ///
    /// set the list of atoms contributing to the molecular
    /// inertia tensor / covariance matrix / charge tensor
    ///
    void Molecule::setBasisList(const std::vector<bool> &newList)
    {
        if (newList.size() == size())
            d->m_basisList = newList;
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
    /// \brief Molecule::listItem
    /// \return
    ///
    /// return the List Item corresponding to this molecule
    ///
    MoleculeItem *Molecule::listItem() const
    {
        return d->m_listItem;
    }

    ///
    /// \brief Molecule::setListItem
    /// \param newItem
    ///
    /// set this molecule's List Item to \p newItem
    ///
    void Molecule::setListItem(MoleculeItem *newItem)
    {
        d->m_listItem = newItem;
    }

    ///
    /// \brief Molecule::setInternalOriginPosition
    ///
    /// set the position of the internal origin
    ///
    void Molecule::setInternalOriginPosition()
    {
        switch(internalOrigin())
        {
        case kCenterOfMass:
            d->m_originPosition = centerOfMass();
            break;
        case kCenterOfGeometry:
            d->m_originPosition = center();
            break;
        case kCenterOfCharge:
            d->m_originPosition = centerOfCharge();
            break;
        case kCenterOnAtom:
            d->m_originPosition = atom(d->m_originAtoms[0])->position();
            break;
        case kCenterBetweenAtoms:
            d->m_originPosition = d->m_originFactor * atom(d->m_originAtoms[0])->position()
                                + (1.0 - d->m_originFactor) * atom(d->m_originAtoms[1])->position();
            break;
        }
    }

    ///
    /// \brief Molecule::setEulerAngles
    ///
    /// calculate the internal molecular basis and set the
    /// Euler angles accordingly
    ///
    void Molecule::setEulerAngles()
    {
        Eigen::Matrix3d basisVectors;
        Eigen::Vector3d vector1, vector2, vector3;

        switch(internalBasis())
        {
        case kCovarianceVectors:
            basisVectors.col(0) = calcCovarianceEigenvectors().col(2);
            basisVectors.col(1) = calcCovarianceEigenvectors().col(1);
            basisVectors.col(2) = calcCovarianceEigenvectors().col(0);
            break;
        case kInertiaVectors:
            basisVectors = calcInertiaEigenvectors();
            break;
        case kChargeVectors:
            basisVectors = calcChargeEigenvectors();
            break;
        case kVectorsFromAtoms:
            vector1 = atom(d->m_basisAtoms[1])->position() - atom(d->m_basisAtoms[0])->position();
            vector1.normalize();

            vector2 = atom(d->m_basisAtoms[2])->position() - atom(d->m_basisAtoms[0])->position();
            vector2 -= vector1 * vector1.dot(vector2);
            vector2.normalize();

            vector3 = vector1.cross(vector2);
            vector3.normalize();

            basisVectors.col(0) = vector1;
            basisVectors.col(1) = vector2;
            basisVectors.col(2) = vector3;
            break;
        }

        // if the determinant of the internal basis is -1, invert the
        // sign of the middle basis vector:
        if (std::abs(double(basisVectors.determinant()) + 1.0) < 1.0e-12)
            basisVectors.col(1) *= -1.0;

        // determine the initial values of the euler angles:
        double theta = std::acos(double(basisVectors(2,2)));
        double psi, phi;

        if (theta == 0.0 || theta == M_PI)
        {
            psi = std::acos(double(basisVectors(0,0)));
            phi = 0.0;
        }
        else
        {
            phi = std::atan2(double(basisVectors(0,2)),  double(basisVectors(1,2)));
            psi = std::atan2(double(basisVectors(2,0)), -double(basisVectors(2,1)));
        }

        if (phi < 0.0)
            phi += 2.0 * M_PI;

        if (psi < 0.0)
            psi += 2.0 * M_PI;

        d->m_phi = phi;
        d->m_psi = psi;
        d->m_theta = theta;
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
        setInternalOriginPosition();
        setEulerAngles();

        // remove any internal coordinates that might already exist:
        d->m_intPos.clear();

        // set the original values of the internal basis
        d->m_originalBasis[0] = internalOriginPosition()(0);
        d->m_originalBasis[1] = internalOriginPosition()(1);
        d->m_originalBasis[2] = internalOriginPosition()(2);
        d->m_originalBasis[3] = d->m_phi;
        d->m_originalBasis[4] = d->m_theta;
        d->m_originalBasis[5] = d->m_psi;

        // determine the internal atomic positions:
        Eigen::Matrix3d rotMat = internalBasisVectors();

        for (int i = 0; i < int(size()); i++)
            d->m_intPos.push_back(rotMat.transpose() * (atom(i)->position() - internalOriginPosition()));
    }

    Eigen::Matrix3d Molecule::euler2rot(const double psi, const double theta, const double phi) const
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

} // namespace molconv
