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


#include <stdexcept>
#include <algorithm>
#include <Eigen/Eigenvalues>
#include <boost/make_shared.hpp>
#include "moleculebasis.h"
#include "system.h"


namespace molconv
{
    ///
    /// \brief System::System
    ///
    /// The default constructor of the System class
    ///
    void System::init()
    {
    }

    System::~System() {}

    ///
    /// \brief System::size
    /// \return size_t
    ///
    /// returns the number of molecules in the system
    ///
    size_t System::nMolecules() const
    {
        return m_molecules.size();
    }

    ///
    /// \brief System::nGroups
    /// \return
    ///
    /// returns the number of groups in the system
    ///
//    size_t System::nGroups() const
//    {
//        return m_groups.size();
//    }

    ///
    /// \brief System::Molecule
    /// \param index
    /// \return moleculePtr
    ///
    /// returns a shared pointer to the molecule at \p index
    ///
    moleculePtr System::getMolecule(const unsigned long index) const
    {
        return m_molecules.at(index);
    }

    ///
    /// \brief System::getGroup
    /// \param index
    /// \return
    ///
    /// returns a shared pointer to the group at \p index
    ///
//    groupPtr System::getGroup(const size_t index) const
//    {
//        if (index >= nGroups())
//            throw std::invalid_argument("index out of range.\n");

//        return m_groups.at(index);
//    }

    ///
    /// \brief System::MoleculeIndex
    /// \param theMolecule
    /// \return
    ///
    /// returns the position of the molecule in the vector
    ///
    size_t System::MoleculeIndex(const moleculePtr theMolecule)
    {
        size_t index = 0;

        for (size_t i = 0; i < nMolecules(); i++)
        {
            if (getMolecule(i) == theMolecule)
                index = i;
        }

        return index;
    }

    ///
    /// \brief System::GroupIndex
    /// \param theGroup
    /// \return
    ///
    /// returns the position of the group in the vector
    ///
//    size_t System::GroupIndex(const groupPtr &theGroup) const
//    {
//        size_t index = 0;

//        for (size_t i = 0; i < nGroups(); i++)
//        {
//            if (getGroup(i) == theGroup)
//                index = i;
//        }

//        return index;
//    }

    ///
    /// \brief System::addMolecule
    /// \param newMolecule
    ///
    /// adds a new molecule \p newMolecule to the system
    ///
    void System::addMolecule(const moleculePtr newMolecule)
    {
        m_molecules.insert(std::make_pair(newMolecule->molId(), newMolecule));
    }

    ///
    /// \brief System::removeMolecule
    /// \param index
    ///
    /// removes the molecule at index \p index
    ///
    void System::removeMolecule(const unsigned long key)
    {
        m_molecules.erase(key);
    }

    ///
    /// \brief System::addGroup
    /// \param newGroup
    ///
    /// adds a new group to the system
    ///
//    void System::addGroup(const groupPtr &newGroup)
//    {
//        m_groups.push_back(newGroup);
//    }

    ///
    /// \brief System::removeGroup
    /// \param index
    ///
    /// removes the group at index \p index
    ///
//    void System::removeGroup(const size_t index)
//    {
//        if (index >= nGroups())
//            throw std::invalid_argument("index out of range.\n");

//        m_groups.erase(m_groups.begin() + index);
//    }

    std::vector<unsigned long> System::getMolIDs() const
    {
        std::vector<unsigned long> result;

        for (auto const& element : m_molecules)
        {
            result.push_back(element.first);
        }

        return result;
    }

    ///
    /// \brief System::calculateRMSDbetween
    /// \param refMol
    /// \param otherMol
    /// \return
    ///
    /// calculate the RMSD between the reference molecule and the other molecule
    ///
    /// the RMSD value is calculated as
    ///
    ///            -------------------------
    ///           /  _N_                    |
    ///          / 1 \                     2
    /// RMSD =  / --- >  | r'       -  r  |
    ///       \/   N /      i(xyz)      i(xyz)
    ///              ---
    ///              i=1
    ///
    double System::calculateRMSDbetween(const unsigned long refMol, const unsigned long otherMol) const
    {
        moleculePtr refMolPtr = getMolecule(refMol);
        moleculePtr otherMolPtr = getMolecule(otherMol);

        if (refMolPtr->size() != otherMolPtr->size())
        {
            return -1.0;
        }

        double rmsd = 0.0;

        for (int i = 0; i < int(refMolPtr->size()); i++)
        {
            for (int j = 0; j < 3; j++)
            {
                rmsd += std::abs(double(otherMolPtr->atom(i)->position()(j))
                                 - double(refMolPtr->atom(i)->position()(j)))
                      * std::abs(double(otherMolPtr->atom(i)->position()(j))
                                 - double(refMolPtr->atom(i)->position()(j)));
            }
        }
        rmsd /= double(refMolPtr->size());
        rmsd = std::sqrt(rmsd);

        return rmsd;
    }

    ///
    /// \brief System::alignMolecules
    /// \param refMol
    /// \param otherMol
    /// \return
    ///
    /// move the molecule \p otherMol to minimize the RMSD with \p refMol
    /// using the quaternion algorithm given in J. Comp. Chem. 25, 15 (2004)
    ///
    bool System::alignMolecules(const unsigned long refMol, const unsigned long otherMol) const
    {
        moleculePtr refMolPtr = getMolecule(refMol);
        moleculePtr otherMolPtr = getMolecule(otherMol);

        if (refMolPtr->size() != otherMolPtr->size())
        {
            return false;
        }

        int Natoms = refMolPtr->size();

        Eigen::Vector3d center = refMolPtr->center();
        Eigen::Vector3d shift = center - otherMolPtr->center();
        Eigen::Vector3d newOrigin = otherMolPtr->originPosition() + shift;

        otherMolPtr->moveFromParas(double(newOrigin(0)), double(newOrigin(1)), double(newOrigin(2)), 0.0, 0.0, 0.0);

        Eigen::MatrixXd Xr = Eigen::MatrixXd::Zero(3,Natoms);
        Eigen::MatrixXd Xo = Eigen::MatrixXd::Zero(3,Natoms);
        for (int i = 0; i < Natoms; i++)
        {
            Xr.col(i) = refMolPtr->atom(i)->position() - center;
            Xo.col(i) = otherMolPtr->atom(i)->position() - center;
        }

        Eigen::Matrix3d corr = Xo * Xr.transpose();

        // construct the quaternion matrix
        Eigen::Matrix4d F = Eigen::Matrix4d::Zero();
        F(0,0) =  corr(0,0) + corr(1,1) + corr(2,2);
        F(1,1) =  corr(0,0) - corr(1,1) - corr(2,2);
        F(2,2) = -corr(0,0) + corr(1,1) - corr(2,2);
        F(3,3) = -corr(0,0) - corr(1,1) + corr(2,2);
        F(0,1) =  corr(1,2) - corr(2,1);
        F(0,2) =  corr(2,0) - corr(0,2);
        F(0,3) =  corr(0,1) - corr(1,0);
        F(1,2) =  corr(0,1) + corr(1,0);
        F(1,3) =  corr(0,2) + corr(2,0);
        F(2,3) =  corr(1,2) + corr(2,1);
        F(1,0) = F(0,1);
        F(2,0) = F(0,2);
        F(3,0) = F(0,3);
        F(2,1) = F(1,2);
        F(3,1) = F(1,3);
        F(3,2) = F(2,3);

        Eigen::SelfAdjointEigenSolver<Eigen::Matrix4d> Feig(F);
        Eigen::Vector4d Feval = Feig.eigenvalues();
        Eigen::Matrix4d Fevec = Feig.eigenvectors();

        // the optimal rotation corresponds to either the first or the last eigenvector, depending on which eigenvalue is larger
        Eigen::Vector4d lQuart = std::abs(double(Feval(0))) > std::abs(double(Feval(3))) ? Fevec.block(0, 0, 4, 1) : Fevec.block(0, 3, 4, 1);

        Eigen::Matrix3d rotmat = Eigen::Matrix3d::Zero();
        rotmat(0,0) = lQuart(0) * lQuart(0) + lQuart(1) * lQuart(1) - lQuart(2) * lQuart(2) - lQuart(3) * lQuart(3);
        rotmat(1,1) = lQuart(0) * lQuart(0) - lQuart(1) * lQuart(1) + lQuart(2) * lQuart(2) - lQuart(3) * lQuart(3);
        rotmat(2,2) = lQuart(0) * lQuart(0) - lQuart(1) * lQuart(1) - lQuart(2) * lQuart(2) + lQuart(3) * lQuart(3);
        rotmat(0,1) = 2.0 * (lQuart(1) * lQuart(2) - lQuart(0) * lQuart(3));
        rotmat(0,2) = 2.0 * (lQuart(1) * lQuart(3) + lQuart(0) * lQuart(2));
        rotmat(1,2) = 2.0 * (lQuart(2) * lQuart(3) - lQuart(0) * lQuart(1));
        rotmat(1,0) = 2.0 * (lQuart(1) * lQuart(2) + lQuart(0) * lQuart(3));
        rotmat(2,0) = 2.0 * (lQuart(1) * lQuart(3) - lQuart(0) * lQuart(2));
        rotmat(2,1) = 2.0 * (lQuart(2) * lQuart(3) + lQuart(0) * lQuart(1));

        std::array<double,3> newEulers = molconv::MoleculeBasis::rot2euler(rotmat);
        double newPhi = newEulers[2];
        double newTheta = newEulers[1];
        double newPsi = newEulers[0];

        otherMolPtr->moveFromParas(double(newOrigin(0)), double(newOrigin(1)), double(newOrigin(2)), newPhi, newTheta, newPsi);

        return true;
    }

} // namespace molconv
