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


//#include<iostream>
//#include<iomanip>
//#include<fstream>
//#include<string>
#include<array>
//#include<cmath>
//#include<Eigen/Eigenvalues>
#include<chemkit/bondpredictor.h>
#include "molecule.h"
//#include "utilities.h"


namespace molconv
{
    class MoleculePrivate
    {
        void initializeDefault()
        {
            m_origin = kCenterOnZero;
            m_basis = kIdentityVectors;
            m_originFactor = 0.0;
            m_originAtoms.fill(0);
            m_basisAtoms.fill(0);
        }

        origin m_origin;
        basis m_basis;

        double m_originFactor;
        std::array<int,2> m_originAtoms;
        std::array<int,3> m_basisAtoms;
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
        d->initializeDefault();
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

        d->initializeDefault();
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
    }

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
    /// \brief Molecule::internalOrigin
    /// \return
    ///
    /// returns the position of the molecule's internal origin.
    ///
    Eigen::Vector3d Molecule::internalOriginPosition() const
    {
    }

    ///
    /// \brief Molecule::internalBasis
    /// \return
    ///
    /// returns the basis vectors of the molecule's internal coordinate
    /// system as columns of the matrix.
    ///
    Eigen::Matrix3d Molecule::internalBasisVectors() const
    {
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
    }

    ///
    /// \brief Molecule::inertiaTensor
    /// \return
    ///
    /// returns the inertia tensor of the molecule.
    ///
    Eigen::Matrix3d Molecule::inertiaTensor() const
    {
    }

    ///
    /// \brief Molecule::covarianceMatrix
    /// \return
    ///
    /// returns the covariance matrix of the molecule.
    ///
    Eigen::Matrix3d Molecule::covarianceMatrix() const
    {
    }

    ///
    /// \brief Molecule::translate
    /// \param shiftVector
    ///
    /// Shifts the entire molecule by a given vector.
    ///
    void Molecule::translate(const Eigen::Vector3d &shiftVector)
    {
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
    }

    ///
    /// \brief Molecule::rotate
    /// \param axis
    /// \param angle
    ///
    /// rotates the molecule about the specified axis about its internal
    /// origin by the specified angle (in radians).
    ///
    void Molecule::rotate(const Eigen::Vector3d &axis, const double &angle)
    {
    }

    ///
    /// \brief Molecule::setOrigin
    /// \param newOrigin
    ///
    /// set the molecule's internal origin to \p newOrigin.
    ///
    void Molecule::setOrigin(const origin &newOrigin)
    {
    }

    ///
    /// \brief Molecule::setBasis
    /// \param newBasis
    ///
    /// set the molecule's internal coordinate system to \p newBasis.
    ///
    void Molecule::setBasis(const basis &newBasis)
    {
    }

    ///
    /// \brief Molecule::calcInertiaTensor
    /// \return
    ///
    /// calculate the inertia tensor of the molecule.
    ///
    Eigen::Matrix3d Molecule::calcInertiaTensor() const
    {
    }

    ///
    /// \brief Molecule::calcCovarianceMatrix
    /// \return
    ///
    /// calculate the covariance matrix of the molecule
    ///
    Eigen::Matrix3d Molecule::calcCovarianceMatrix() const
    {
    }

    ///
    /// \brief Molecule::calcInertiaEigenvalues
    /// \return
    ///
    /// calculate the eigenvalues of the molecule's inertia tensor
    ///
    Eigen::Vector3d Molecule::calcInertiaEigenvalues() const
    {
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
    }

    ///
    /// \brief Molecule::calcCovarianceEigenvalues
    /// \return
    ///
    /// calculate the eigenvalues of the molecule's covariance matrix.
    ///
    Eigen::Vector3d Molecule::calcCovarianceEigenvalues() const
    {
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
    }

} // namespace molconv
