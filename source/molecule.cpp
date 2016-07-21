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
#include<QDebug>
#include "molecule.h"
#include "moleculelistitem.h"


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
            m_phi = 0;
            m_theta = 0;
            m_psi = 0;
        }

        origin m_origin;
        basis m_basis;

        double m_originFactor;
        std::array<int,2> m_originAtoms;
        std::array<int,3> m_basisAtoms;

        groupPtr m_group;
        std::vector<Eigen::Vector3d> m_intPos;

        boost::shared_ptr<MoleculeListItem> m_listItem;

        double m_phi;
        double m_theta;
        double m_psi;
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
        initIntPos();
    }


    Molecule::Molecule(const boost::shared_ptr<chemkit::Molecule> &BaseMolPtr)
        : chemkit::Molecule(*BaseMolPtr)
        , d(new MoleculePrivate)
    {
        qDebug() << "this is the third constructor of molconv::Molecule.";
        chemkit::BondPredictor::predictBonds(this);
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
        qDebug() << "this is the fourth constructor of molconv::Molecule.";
        d->m_origin = originalMolecule.internalOrigin();
        d->m_basis = originalMolecule.internalBasis();
        d->m_originFactor = originalMolecule.internalOriginFactor();
        d->m_originAtoms = originalMolecule.internalOriginAtoms();
        d->m_basisAtoms = originalMolecule.internalBasisAtoms();
        initIntPos();
    }

    ///
    /// \brief Molecule::~Molecule
    ///
    /// The default destructor
    ///
    Molecule::~Molecule() {}

    ///
    /// \brief Molecule::initIntPos
    ///
    /// Initialize the atomic positions w.r.t. the internal
    /// coordinate system of the molecule
    ///
    void Molecule::initIntPos()
    {
        Eigen::Matrix3d rotMat = internalBasisVectors();

        // determine the internal atomic positions:
        for (int i = 0; i < size(); i++)
        {
            Eigen::Vector3d origPos = atom(i)->position();
            Eigen::Vector3d intPos = rotMat.transpose() * (origPos - internalOriginPosition());
            d->m_intPos.push_back(intPos);
        }

        // determine the initial values of the euler angles:
        double theta = std::acos(double(rotMat(2,2)));
        double psi, phi;

        if (theta == 0.0 || theta == M_PI)
        {
            psi = std::acos(double(rotMat(0,0)));
            phi = 0.0;
        }
        else
        {
            phi = std::atan2(double(rotMat(0,2)),  double(rotMat(1,2)));
            psi = std::atan2(double(rotMat(2,0)), -double(rotMat(2,1)));
        }

        if (phi < 0.0)
            phi += 2.0 * M_PI;

        if (psi < 0.0)
            psi += 2.0 * M_PI;

        d->m_phi = phi;
        d->m_psi = psi;
        d->m_theta = theta;


//        Eigen::Matrix3d rotMat1 = internalBasisVectors();
//        Eigen::Matrix3d cMat = covarianceMatrix();


//        Eigen::Matrix3d iMat = inertiaTensor();
//        Eigen::Vector3d iVec = inertiaEigenvalues();
//        Eigen::Matrix3d iRot = inertiaEigenvectors();

//        std::cout << "inertia matrix:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(iMat(0,0))
//                  << std::setw(20) << double(iMat(0,1))
//                  << std::setw(20) << double(iMat(0,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(iMat(1,0))
//                  << std::setw(20) << double(iMat(1,1))
//                  << std::setw(20) << double(iMat(1,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(iMat(2,0))
//                  << std::setw(20) << double(iMat(2,1))
//                  << std::setw(20) << double(iMat(2,2)) << std::endl;

//        std::cout << "inertia moments:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(iVec(0))
//                  << std::setw(20) << double(iVec(1))
//                  << std::setw(20) << double(iVec(2)) << std::endl;

//        std::cout << "inertia eigenvectors:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(iRot(0,0))
//                  << std::setw(20) << double(iRot(0,1))
//                  << std::setw(20) << double(iRot(0,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(iRot(1,0))
//                  << std::setw(20) << double(iRot(1,1))
//                  << std::setw(20) << double(iRot(1,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(iRot(2,0))
//                  << std::setw(20) << double(iRot(2,1))
//                  << std::setw(20) << double(iRot(2,2)) << std::endl;

//        std::cout << "covariance matrix:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(cMat(0,0))
//                  << std::setw(20) << double(cMat(0,1))
//                  << std::setw(20) << double(cMat(0,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(cMat(1,0))
//                  << std::setw(20) << double(cMat(1,1))
//                  << std::setw(20) << double(cMat(1,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(cMat(2,0))
//                  << std::setw(20) << double(cMat(2,1))
//                  << std::setw(20) << double(cMat(2,2)) << std::endl;

//        std::cout << "transformation matrix:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(rotMat1(0,0))
//                  << std::setw(20) << double(rotMat1(0,1))
//                  << std::setw(20) << double(rotMat1(0,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(rotMat1(1,0))
//                  << std::setw(20) << double(rotMat1(1,1))
//                  << std::setw(20) << double(rotMat1(1,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(rotMat1(2,0))
//                  << std::setw(20) << double(rotMat1(2,1))
//                  << std::setw(20) << double(rotMat1(2,2)) << std::endl;

//        Eigen::Matrix3d rotMat3;
//        rotMat3.col(0) = rotMat1.col(2);
//        rotMat3.col(1) = rotMat1.col(1);
//        rotMat3.col(2) = rotMat1.col(0);

//        std::cout << "Determinant of the initial rotation matrix: " << double(rotMat3.determinant()) << std::endl;

//        Eigen::Matrix3d rotMat = rotMat3;
//        Eigen::Matrix3d rotMat2 = Eigen::Matrix3d::Zero();

//        if (std::abs(double(rotMat3.determinant()) + 1.0) < 1.0e-12)
//        {
//            std::cout << "the determinant is -1\n";
//            rotMat2.col(0) =  rotMat3.col(0);
//            rotMat2.col(1) = -rotMat3.col(1);
//            rotMat2.col(2) =  rotMat3.col(2);
//            rotMat = rotMat2;
//        }

//        Eigen::Matrix3d D1 = rotMat3.transpose() * cMat * rotMat3;
//        Eigen::Matrix3d D2 = rotMat.transpose() * cMat * rotMat;

//        std::cout << "the D1 matrix:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(D1(0,0))
//                  << std::setw(20) << double(D1(0,1))
//                  << std::setw(20) << double(D1(0,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(D1(1,0))
//                  << std::setw(20) << double(D1(1,1))
//                  << std::setw(20) << double(D1(1,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(D1(2,0))
//                  << std::setw(20) << double(D1(2,1))
//                  << std::setw(20) << double(D1(2,2)) << std::endl;

//        std::cout << "the D2 matrix:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(D2(0,0))
//                  << std::setw(20) << double(D2(0,1))
//                  << std::setw(20) << double(D2(0,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(D2(1,0))
//                  << std::setw(20) << double(D2(1,1))
//                  << std::setw(20) << double(D2(1,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(D2(2,0))
//                  << std::setw(20) << double(D2(2,1))
//                  << std::setw(20) << double(D2(2,2)) << std::endl;

//        std::cout << "the final rotation matrix:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(rotMat(0,0))
//                  << std::setw(20) << double(rotMat(0,1))
//                  << std::setw(20) << double(rotMat(0,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(rotMat(1,0))
//                  << std::setw(20) << double(rotMat(1,1))
//                  << std::setw(20) << double(rotMat(1,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(rotMat(2,0))
//                  << std::setw(20) << double(rotMat(2,1))
//                  << std::setw(20) << double(rotMat(2,2)) << std::endl;

//        // determine the initial values of the euler angles:
//        double theta = std::acos(double(rotMat(2,2)));
//        double phi = std::atan2(double(rotMat(0,2)),  double(rotMat(1,2)));
//        double psi = std::atan2(double(rotMat(2,0)), -double(rotMat(2,1)));

//        if (phi < 0.0)
//            phi += 2.0 * M_PI;

//        if (psi < 0.0)
//            psi += 2.0 * M_PI;

//        d->m_phi = phi;
//        d->m_psi = psi;
//        d->m_theta = theta;

//        std::cout << "Euler angles determined from this matrix:\n";
//        std::cout << "Theta = " << std::setw(20) << theta << std::setw(20) << theta * 180.0 / M_PI << std::endl;
//        std::cout << "Phi = " << std::setw(20) << phi << std::setw(20) << phi * 180.0 / M_PI << std::endl;
//        std::cout << "Psi = " << std::setw(20) << psi << std::setw(20) << psi * 180.0 / M_PI << std::endl;

//        // reconstruct the rotation matrix:
//        Eigen::Matrix3d analMat;
//        analMat(0,0) =  std::cos(psi) * std::cos(phi) - std::sin(psi) * std::cos(theta) * std::sin(phi);
//        analMat(0,1) =  std::sin(psi) * std::cos(phi) + std::cos(psi) * std::cos(theta) * std::sin(phi);
//        analMat(0,2) =  std::sin(theta) * std::sin(phi);
//        analMat(1,0) = -std::cos(psi) * std::sin(phi) - std::sin(psi) * std::cos(theta) * std::cos(phi);
//        analMat(1,1) = -std::sin(psi) * std::sin(phi) + std::cos(psi) * std::cos(theta) * std::cos(phi);
//        analMat(1,2) =  std::sin(theta) * std::cos(phi);
//        analMat(2,0) =  std::sin(psi) * std::sin(theta);
//        analMat(2,1) = -std::cos(psi) * std::sin(theta);
//        analMat(2,2) =  std::cos(theta);

//        std::cout << "reconstructed rotation matrix:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(analMat(0,0))
//                  << std::setw(20) << double(analMat(0,1))
//                  << std::setw(20) << double(analMat(0,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(analMat(1,0))
//                  << std::setw(20) << double(analMat(1,1))
//                  << std::setw(20) << double(analMat(1,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(analMat(2,0))
//                  << std::setw(20) << double(analMat(2,1))
//                  << std::setw(20) << double(analMat(2,2)) << std::endl;

//        std::cout << "Determinant of the reconstructed rotation matrix: " << analMat.determinant() << std::endl;

//        std::cout << "difference matrix:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double((rotMat - analMat)(0,0))
//                  << std::setw(20) << double((rotMat - analMat)(0,1))
//                  << std::setw(20) << double((rotMat - analMat)(0,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double((rotMat - analMat)(1,0))
//                  << std::setw(20) << double((rotMat - analMat)(1,1))
//                  << std::setw(20) << double((rotMat - analMat)(1,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double((rotMat - analMat)(2,0))
//                  << std::setw(20) << double((rotMat - analMat)(2,1))
//                  << std::setw(20) << double((rotMat - analMat)(2,2)) << std::endl;

//        std::cout << "quotient matrix:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(rotMat(0,0)) / double(analMat(0,0))
//                  << std::setw(20) << double(rotMat(0,1)) / double(analMat(0,1))
//                  << std::setw(20) << double(rotMat(0,2)) / double(analMat(0,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(rotMat(1,0)) / double(analMat(1,0))
//                  << std::setw(20) << double(rotMat(1,1)) / double(analMat(1,1))
//                  << std::setw(20) << double(rotMat(1,2)) / double(analMat(1,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(rotMat(2,0)) / double(analMat(2,0))
//                  << std::setw(20) << double(rotMat(2,1)) / double(analMat(2,1))
//                  << std::setw(20) << double(rotMat(2,2)) / double(analMat(2,2)) << std::endl;



//        // determine the internal atomic positions:
//        for (int i = 0; i < size(); i++)
//        {
//            Eigen::Vector3d origPos = atom(i)->position();
//            Eigen::Vector3d intPos = rotMat.transpose() * (origPos - internalOriginPosition());
//            d->m_intPos.push_back(intPos);
//        }

//        std::cout << "internal atomic positions:\n";
//        for (int i = 0; i < size(); i++)
//            std::cout << std::fixed
//                      << std::setw(12) << double(d->m_intPos.at(i)(0))
//                      << std::setw(12) << double(d->m_intPos.at(i)(1))
//                      << std::setw(12) << double(d->m_intPos.at(i)(2))
//                      << std::endl;

//        std::cout << "transformed positions using the exact transformation matrix:\n";
//        for (int i = 0; i < size(); i++)
//        {
//            Eigen::Vector3d newPos = rotMat * d->m_intPos.at(i);
//            std::cout << std::fixed
//                      << std::setw(12) << double(newPos(0))
//                      << std::setw(12) << double(newPos(1))
//                      << std::setw(12) << double(newPos(2))
//                      << std::endl;
//        }

//        std::cout << "transformed positions using the reconstructed transformation matrix:\n";
//        for (int i = 0; i < size(); i++)
//        {
//            Eigen::Vector3d newPos = analMat * d->m_intPos.at(i);
//            std::cout << std::fixed
//                      << std::setw(12) << double(newPos(0))
//                      << std::setw(12) << double(newPos(1))
//                      << std::setw(12) << double(newPos(2))
//                      << std::endl;
//        }
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
            basisVectors.col(0) = calcCovarianceEigenvectors().col(2);
            basisVectors.col(1) = calcCovarianceEigenvectors().col(1);
            basisVectors.col(2) = calcCovarianceEigenvectors().col(0);
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

        std::cout << "residual determinant: " << std::abs(double(basisVectors.determinant()) + 1.0) << " " << std::abs(double(basisVectors.determinant()) - 1.0) << std::endl;

        // if the determinant of the internal basis is -1, invert the
        // sign of the middle basis vector:
        if (std::abs(double(basisVectors.determinant()) + 1.0) < 1.0e-12)
            basisVectors.col(1) *= -1.0;

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
    /// \brief Molecule::phi
    /// \return
    ///
    /// calculate the euler angle phi from the current internal basis
    ///
    double Molecule::phi() const
    {
        //return asin(internalBasisVectors()(2,0) / sin(theta()));
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
        //return acos(internalBasisVectors()(2,2));
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
        //return asin(internalBasisVectors()(0,2) / sin(theta()));
        return d->m_psi;
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

        Eigen::Matrix3d rot;

        rot(0,0) =  cos(psi) * cos(phi) - cos(theta) * sin(phi) * sin(psi);
        rot(0,1) =  cos(psi) * sin(phi) + cos(theta) * cos(phi) * sin(psi);
        rot(0,2) =  sin(psi) * sin(theta);
        rot(1,0) = -sin(psi) * cos(phi) - cos(theta) * sin(phi) * cos(psi);
        rot(1,1) = -sin(psi) * sin(phi) + cos(theta) * cos(phi) * cos(psi);
        rot(1,2) =  cos(psi) * sin(theta);
        rot(2,0) =  sin(theta) * sin(phi);
        rot(2,1) = -sin(theta) * cos(phi);
        rot(2,2) =  cos(theta);

        for (int i = 0; i < size(); i++)
            atom(i)->setPosition(pos + rot * d->m_intPos.at(i));
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
        initIntPos();
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
//        std::cout << "the covarianve matrix:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(covarianceMatrix(0,0))
//                  << std::setw(20) << double(covarianceMatrix(0,1))
//                  << std::setw(20) << double(covarianceMatrix(0,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(covarianceMatrix(1,0))
//                  << std::setw(20) << double(covarianceMatrix(1,1))
//                  << std::setw(20) << double(covarianceMatrix(1,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(covarianceMatrix(2,0))
//                  << std::setw(20) << double(covarianceMatrix(2,1))
//                  << std::setw(20) << double(covarianceMatrix(2,2)) << std::endl;
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

//        Eigen::Vector3d tmpvec = solver.eigenvalues();
//        Eigen::Matrix3d rotMat = solver.eigenvectors();

//        std::cout << "eigenvalues of the covariance matrix:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(tmpvec(0))
//                  << std::setw(20) << double(tmpvec(1))
//                  << std::setw(20) << double(tmpvec(2)) << std::endl;
//        std::cout << "eigenvectors of the covarianve matrix:\n";
//        std::cout << std::scientific
//                  << std::setw(20) << double(rotMat(0,0))
//                  << std::setw(20) << double(rotMat(0,1))
//                  << std::setw(20) << double(rotMat(0,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(rotMat(1,0))
//                  << std::setw(20) << double(rotMat(1,1))
//                  << std::setw(20) << double(rotMat(1,2)) << std::endl;
//        std::cout << std::scientific
//                  << std::setw(20) << double(rotMat(2,0))
//                  << std::setw(20) << double(rotMat(2,1))
//                  << std::setw(20) << double(rotMat(2,2)) << std::endl;

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
