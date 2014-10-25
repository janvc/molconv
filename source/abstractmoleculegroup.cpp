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


#include<algorithm>
#include<stdexcept>
#include "abstractmoleculegroup.h"


namespace molconv
{
    class abstractMoleculeGroupPrivate
    {
    public:
        std::vector<boost::shared_ptr<Molecule> > m_molecules;
    };

    ///
    /// \brief abstractMoleculeGroup::abstractMoleculeGroup
    ///
    /// the default constructor
    ///
    abstractMoleculeGroup::abstractMoleculeGroup()
        : d(new abstractMoleculeGroupPrivate)
    {
    }

    ///
    /// \brief abstractMoleculeGroup::size
    /// \return
    ///
    /// return the number of molecules in the group
    ///
    size_t abstractMoleculeGroup::size() const
    {
        return d->m_molecules.size();
    }

    ///
    /// \brief abstractMoleculeGroup::LineParallel
    /// \param firstMolecule
    /// \param secondMolecule
    /// \return
    ///
    /// returns true if the covariance eigenvectors of the molecules \p firstMolecule and \p secondMolecule
    /// corresponding to the largest variance are parallel.
    ///
    bool abstractMoleculeGroup::LineParallel(const size_t firstMolecule, const size_t secondMolecule) const
    {
        Eigen::Vector3d firstVector = getMolecule(firstMolecule)->covarianceEigenvectors().col(2);
        Eigen::Vector3d secondVector = getMolecule(secondMolecule)->covarianceEigenvectors().col(2);

        double scalarProduct = firstVector.dot(secondVector);

        if (scalarProduct - 1.0 < 1.0e-8)
            return true;
        else
            return false;
    }

    ///
    /// \brief abstractMoleculeGroup::PlaneParallel
    /// \param firstMolecule
    /// \param secondMolecule
    /// \return
    ///
    /// returns true if the least-squares planes of the molecules \p firstMolecule and \p secondMolecule
    /// (or equivalently, the covariance eigenvectors corresponding to the smallest variance) are parallel.
    ///
    bool abstractMoleculeGroup::PlaneParallel(const size_t firstMolecule, const size_t secondMolecule) const
    {
        Eigen::Vector3d firstVector = getMolecule(firstMolecule)->covarianceEigenvectors().col(0);
        Eigen::Vector3d secondVector = getMolecule(secondMolecule)->covarianceEigenvectors().col(0);

        double scalarProduct = firstVector.dot(secondVector);

        if (scalarProduct - 1.0 < 1.0e-8)
            return true;
        else
            return false;
    }

    ///
    /// \brief abstractMoleculeGroup::Distance
    /// \param firstMolecule
    /// \param secondMolecule
    /// \return
    ///
    /// return the distance between the internal origins of the two molecules
    ///
    double abstractMoleculeGroup::Distance(const size_t firstMolecule, const size_t secondMolecule) const
    {
        return DistanceVector(firstMolecule, secondMolecule).norm();
    }

    ///
    /// \brief abstractMoleculeGroup::DistanceVector
    /// \param firstMolecule
    /// \param secondMolecule
    /// \return
    ///
    /// return the distance vector from the first molecule's internal origin to the second
    /// molecule's internal origin
    ///
    Eigen::Vector3d abstractMoleculeGroup::DistanceVector(const size_t firstMolecule, const size_t secondMolecule) const
    {
        checkIndex(firstMolecule);
        checkIndex(secondMolecule);

        return getMolecule(secondMolecule)->internalOriginPosition() - getMolecule(firstMolecule)->internalOriginPosition();
    }

    ///
    /// \brief abstractMoleculeGroup::LineDistance
    /// \param firstMolecule
    /// \param secondMolecule
    /// \return
    ///
    /// return the orthogonal distance between the
    double abstractMoleculeGroup::LineDistance(const size_t firstMolecule, const size_t secondMolecule) const
    {
        return LineDistanceVector(firstMolecule, secondMolecule).norm();
    }

    ///
    /// \brief abstractMoleculeGroup::checkIndex
    /// \param index
    ///
    /// this is a utility function to check if the index for a molecule is in range.
    ///
    void abstractMoleculeGroup::checkIndex(const size_t index) const
    {
        if (index > size())
            throw std::invalid_argument("Index out of range in abstractMoleculeGroup.\n");
    }

    ///
    /// \brief abstractMoleculeGroup::getMolecule
    /// \param index
    /// \return
    ///
    /// return a boost pointer to the molecule at \p index
    ///
    boost::shared_ptr<Molecule> abstractMoleculeGroup::getMolecule(const size_t index) const
    {
        return d->m_molecules.at(index);
    }

    ///
    /// \brief abstractMoleculeGroup::addMolecule
    /// \param newMolecule
    ///
    /// add a molecule to the group
    ///
    void abstractMoleculeGroup::addMolecule(const boost::shared_ptr<Molecule> newMolecule)
    {
        d->m_molecules.push_back(newMolecule);
    }

    ///
    /// \brief abstractMoleculeGroup::removeMolecule
    /// \param index
    ///
    /// remove the molecule from the group by index
    ///
    void abstractMoleculeGroup::removeMolecule(const size_t index)
    {
        checkIndex(index);

        d->m_molecules.erase(d->m_molecules.begin() + index);
    }

} // namespace molconv
