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
#include<Eigen/Geometry>
#include<Eigen/Eigenvalues>
#include<chemkit/bondpredictor.h>
#include<QDebug>
#include "molecule.h"
#include "../moleculelistitem.h"


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
            m_listItem = 0;
        }

        origin m_origin;
        basis m_basis;

        double m_originFactor;
        std::array<int,2> m_originAtoms;
        std::array<int,3> m_basisAtoms;

        groupPtr m_group;
        Eigen::Matrix3Xd m_intPos;

        boost::shared_ptr<MoleculeListItem> m_listItem;
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
        qDebug() << "this is the first constructor of molconv::Molecule.";
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
        qDebug() << "this is the second constructor of molconv::Molecule.";
        chemkit::BondPredictor::predictBonds(this);
    }


    Molecule::Molecule(const boost::shared_ptr<chemkit::Molecule> &BaseMolPtr)
        : chemkit::Molecule(*BaseMolPtr)
        , d(new MoleculePrivate)
    {
        qDebug() << "this is the third constructor of molconv::Molecule.";
        chemkit::BondPredictor::predictBonds(this);
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
        qDebug() << "this is the fourth constructor of molconv::Molecule.";
        d->m_origin = originalMolecule.internalOrigin();
        d->m_basis = originalMolecule.internalBasis();
        d->m_originFactor = originalMolecule.internalOriginFactor();
        d->m_originAtoms = originalMolecule.internalOriginAtoms();
        d->m_basisAtoms = originalMolecule.internalBasisAtoms();
    }

    ///
    /// \brief Molecule::~Molecule
    ///
    /// The default destructor
    ///
    Molecule::~Molecule() {}


    void Molecule::initIntPos()
    {
        d->m_intPos.resize(3, size());

        for (int i = 0; i < size(); i++)
        {
            Eigen::Vector3d origPos = atom(i)->position();
            Eigen::Vector3d intPos = internalBasisVectors().transpose() * (origPos - internalOriginPosition());
            d->m_intPos.col(i) = intPos;
        }
    }

    ///
    /// \brief Molecule::internalOrigin
    /// \return
    ///
    /// returns the type of the molecule's internal origin.
    ///
    origin Molecule::internalOrigin() const
    {
        qDebug() << "entering Molecule::internalOrigin()";
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
        qDebug() << "entering Molecule::internalBasis()";
        return d->m_basis;
    }

    ///
    /// \brief Molecule::internalOriginPosition
    /// \return
    ///
    /// returns the position of the molecule's internal origin.
    ///
    Eigen::Vector3d Molecule::internalOriginPosition() const
    {
        qDebug() << "entering Molecule::internalOriginPosition()";
        Eigen::Vector3d originPosition;

        switch(internalOrigin())
        {
        case kCenterOnZero:
            originPosition = Eigen::Vector3d::Zero();
            break;
        case kCenterOfMass:
            originPosition = centerOfMass();
            break;
        case kCenterOfGeometry:
            originPosition = center();
            break;
        case kCenterOnAtom:
            originPosition = atom(d->m_originAtoms[0])->position();
            break;
        case kCenterBetweenAtoms:
            originPosition = d->m_originFactor * atom(d->m_originAtoms[0])->position()
                           + (1.0 - d->m_originFactor) * atom(d->m_originAtoms[1])->position();
            break;
        }

        return originPosition;
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
        qDebug() << "entering Molecule::internalBasisVectors()";
        Eigen::Matrix3d basisVectors;
        Eigen::Vector3d vector1, vector2, vector3;

        switch(internalBasis())
        {
        case kIdentityVectors:
            basisVectors = Eigen::Matrix3d::Identity();
            break;
        case kCovarianceVectors:
            basisVectors = calcCovarianceEigenvectors();
            break;
        case kInertiaVectors:
            basisVectors = calcInertiaEigenvectors();
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

        return basisVectors;
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
        qDebug() << "entering Molecule::internalOriginAtoms()";
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
        qDebug() << "entering Molecule::internalBasisAtoms()";
        return d->m_basisAtoms;
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
        qDebug() << "entering Molecule::internalOriginFactor()";
        return d->m_originFactor;
    }

    ///
    /// \brief Molecule::inertiaTensor
    /// \return
    ///
    /// returns the inertia tensor of the molecule.
    ///
    Eigen::Matrix3d Molecule::inertiaTensor() const
    {
        qDebug() << "entering Molecule::inertiaTensor()";
        return calcInertiaTensor();
    }

    ///
    /// \brief Molecule::covarianceMatrix
    /// \return
    ///
    /// returns the covariance matrix of the molecule.
    ///
    Eigen::Matrix3d Molecule::covarianceMatrix() const
    {
        qDebug() << "entering Molecule::covarianceMatrix()";
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
        qDebug() << "entering Molecule::inertiaEigenvalues()";
        return calcInertiaEigenvalues();
    }

    ///
    /// \brief Molecule::covarianceEigenvalues
    /// \return
    ///
    /// returns the eigenvalues of the covariance matrix in a vector
    ///
    Eigen::Vector3d Molecule::covarianceEigenvalues() const
    {
        qDebug() << "entering Molecule::covarianceEigenvalues()";
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
        qDebug() << "entering Molecule::inertiaEigenvectors()";
        return calcInertiaEigenvectors();
    }

    ///
    /// \brief Molecule::covarianceEigenvectors
    /// \return
    ///
    /// returns the eigenvectors of the covariance matrix as the columns of the matrix
    ///
    Eigen::Matrix3d Molecule::covarianceEigenvectors() const
    {
        qDebug() << "entering Molecule::covariaceEigenvectors()";
        return calcCovarianceEigenvectors();
    }

    ///
    /// \brief Molecule::translate
    /// \param shiftVector
    ///
    /// Shifts the entire molecule by a given vector.
    ///
    void Molecule::translate(const Eigen::Vector3d &shiftVector)
    {
        qDebug() << "entering Molecule::translate()";
        setCenter(center() + shiftVector);
    }

    ///
    /// \brief Molecule::rotate
    /// \param rotationMatrix
    ///
    /// rotates the molecule about its internal origin using a
    /// rotation matrix.
    ///
    void Molecule::rotate(const Eigen::Matrix3d &rotationMatrix)
    {
        qDebug() << "entering Molecule::rotate(matrix)";
        Eigen::Vector3d intOrigPos = internalOriginPosition();

        for (size_t atiter = 0; atiter < size(); atiter++)
        {
            atom(atiter)->setPosition(intOrigPos + rotationMatrix * (atom(atiter)->position() - intOrigPos));
        }
    }

    ///
    /// \brief Molecule::rotate
    /// \param axis
    /// \param angle
    ///
    /// rotates the molecule about the specified axis about its internal
    /// origin by the specified angle (in radians).
    ///
    void Molecule::rotate(const Eigen::Vector3d &axis, const double angle)
    {
        qDebug() << "entering Molecule::rotate(axis,angle)";
        Eigen::Matrix3d rotationMatrix;
        Eigen::Vector3d rotAxis = axis;

        rotAxis.normalize();

        rotationMatrix = Eigen::AngleAxisd(angle, rotAxis);

        rotate(rotationMatrix);
    }

    ///
    /// \brief Molecule::setPhi
    /// \param newPhi
    ///
    /// rotate the molecule about the z-Axis
    ///
    void Molecule::setPhi(const double newPhi)
    {
        qDebug("entering Molecule::setPhi()");

        Eigen::Matrix3d intBasis = internalBasisVectors();
        double a31 = intBasis(2, 0);
        double a32 = intBasis(2, 1);
        double currentPhi = 180.0 * atan(a31 / a32) / M_PI;

        rotate(Eigen::Vector3d(0, 0, 1), (newPhi - currentPhi) * M_PI / 180.0);
    }

    ///
    /// \brief Molecule::setTheta
    /// \param newTheta
    ///
    /// rotate the molecule about the xi-Axis
    ///
    void Molecule::setTheta(const double newTheta)
    {
    }

    ///
    /// \brief Molecule::setPsi
    /// \param newPsi
    ///
    /// rotate the molecule about the z'-Axis
    ///
    void Molecule::setPsi(const double newPsi)
    {
    }

    ///
    /// \brief Molecule::setOrigin
    /// \param newOrigin
    ///
    /// set the molecule's internal origin to \p newOrigin.
    ///
    void Molecule::setOrigin(const origin &newOrigin, const size_t atom1, const size_t atom2, const double originFactor)
    {
        qDebug() << "entering Molecule::setOrigin()";
        switch (newOrigin)
        {
        case kCenterOnZero:
        case kCenterOfMass:
        case kCenterOfGeometry:
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
    }

    ///
    /// \brief Molecule::setBasis
    /// \param newBasis
    ///
    /// set the molecule's internal coordinate system to \p newBasis.
    ///
    void Molecule::setBasis(const basis &newBasis, const size_t atom1, const size_t atom2, const size_t atom3)
    {
        qDebug() << "entering Molecule::setBasis()";
        switch (newBasis)
        {
        case kIdentityVectors:
        case kCovarianceVectors:
        case kInertiaVectors:
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
    }

    ///
    /// \brief Molecule::cleanUp
    ///
    /// This method cleans up the structure of the molecule, i.e. shift it
    /// so that the origin of the internal basis wquals the origin, rotate it
    /// so that the coordinate axes match the internal basis
    ///
    void Molecule::cleanUp()
    {
        qDebug() << "entering Molecule::cleanUp()";
        translate(-internalOriginPosition());
        rotate(internalBasisVectors().transpose());
    }

    ///
    /// \brief Molecule::addToGroup
    /// \param newGroup
    ///
    /// This method adds a poiner to the new group, that the molecule now belongs to
    ///
    void Molecule::addToGroup(const groupPtr newGroup)
    {
        qDebug("entering Molecule::addToGroup()");
        d->m_group = newGroup;
    }

    ///
    /// \brief Molecule::group
    /// \return
    ///
    /// This method returns the group that this molecule belongs to
    ///
    groupPtr &Molecule::group() const
    {
        qDebug("entering Molecule::group()");
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
        qDebug() << "entering Molecule::calcInertiaTensor()";
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
                        factor = (atom(atiter)->position() - com).squaredNorm();

                    factor -= (atom(atiter)->position() - com)(alpha) * (atom(atiter)->position() - com)(beta);

                    inertiaTensor(alpha, beta) += atom(atiter)->mass() * factor;
                }
            }
        }

        return inertiaTensor;
    }

    ///
    /// \brief Molecule::calcCovarianceMatrix
    /// \return
    ///
    /// calculate the covariance matrix of the molecule
    ///
    Eigen::Matrix3d Molecule::calcCovarianceMatrix() const
    {
        qDebug() << "entering Molecule::calcCovarianceMatrix()";
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
                covarianceMatrix(alpha, beta) /= size();
            }
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
        qDebug() << "entering Molecule::calcIneriaEigenvalues()";
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
        qDebug() << "entering Molecule::calcInertiaEigenvectors()";
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(calcInertiaTensor());

        if (solver.info() != Eigen::Success)
            throw std::runtime_error("The inertia tensor could not be diagonalized.\n");
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
        qDebug() << "entering Molecule::calcCovarianceEigenvalues()";
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
        qDebug() << "entering Molecule::calcCovarianceEigenvectors()";
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(calcCovarianceMatrix());

        if (solver.info() != Eigen::Success)
            throw std::runtime_error("The covariance matrix could not be diagonalized.\n");
        else
            return solver.eigenvectors();
    }

    ///
    /// \brief Molecule::listItem
    /// \return
    ///
    /// return the List Item corresponding to this molecule
    ///
    boost::shared_ptr<MoleculeListItem> Molecule::listItem() const
    {
        return d->m_listItem;
    }

    ///
    /// \brief Molecule::setListItem
    /// \param newItem
    ///
    /// set this molecule's List Item to \p newItem
    ///
    void Molecule::setListItem(boost::shared_ptr<MoleculeListItem> &newItem)
    {
        d->m_listItem = newItem;
    }

} // namespace molconv
