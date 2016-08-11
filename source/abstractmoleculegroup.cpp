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


#include<algorithm>
#include<stdexcept>
#include "abstractmoleculegroup.h"


namespace molconv
{
    class abstractMoleculeGroupPrivate
    {
    public:
        std::string m_groupname;
        std::vector<boost::shared_ptr<Molecule> > m_molecules;
        std::vector<groupPtr> m_groups;
        groupPtr m_parentgroup;
    };

    ///
    /// \brief abstractMoleculeGroup::abstractMoleculeGroup
    ///
    /// the default constructor
    ///
    abstractMoleculeGroup::abstractMoleculeGroup()
        : d(new abstractMoleculeGroupPrivate)
    {
        d->m_groupname = "";
    }


    abstractMoleculeGroup::abstractMoleculeGroup(const std::string &name)
        : d(new abstractMoleculeGroupPrivate)
    {
        d->m_groupname = name;
    }

    ///
    /// \brief abstractMoleculeGroup::~abstractMoleculeGroup
    ///
    /// the default destructor
    ///
    abstractMoleculeGroup::~abstractMoleculeGroup() {}

    ///
    /// \brief abstractMoleculeGroup::nMolecules
    /// \return
    ///
    /// return the number of molecules in the group
    ///
    size_t abstractMoleculeGroup::nMolecules() const
    {
        return d->m_molecules.size();
    }

    ///
    /// \brief abstractMoleculeGroup::nGroups
    /// \return
    ///
    /// return the number of groups in the group
    ///
    size_t abstractMoleculeGroup::nGroups() const
    {
        return d->m_groups.size();
    }

    ///
    /// \brief abstractMoleculeGroup::name
    /// \return
    ///
    /// return the name of the group
    ///
    std::string abstractMoleculeGroup::name() const
    {
        return d->m_groupname;
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
    /// \brief abstractMoleculeGroup::checkIndex
    /// \param index
    ///
    /// this is a utility function to check if the index for a molecule is in range.
    ///
    void abstractMoleculeGroup::checkIndex(const size_t index) const
    {
        if (index > nMolecules())
            throw std::invalid_argument("Index out of range in abstractMoleculeGroup.\n");
    }

    ///
    /// \brief abstractMoleculeGroup::getMolecule
    /// \param index
    /// \return
    ///
    /// return a boost pointer to the molecule at \p index
    ///
    moleculePtr abstractMoleculeGroup::getMolecule(const size_t index) const
    {
        return d->m_molecules.at(index);
    }

    ///
    /// \brief abstractMoleculeGroup::getGroup
    /// \param index
    /// \return
    ///
    /// return a boost pointer to the group at \p index
    ///
    groupPtr abstractMoleculeGroup::getGroup(const size_t index) const
    {
        return d->m_groups.at(index);
    }

    ///
    /// \brief abstractMoleculeGroup::parent
    /// \return
    ///
    /// return a boost pointer to the parent of this group
    ///
    groupPtr abstractMoleculeGroup::parent() const
    {
        return d->m_parentgroup;
    }

    ///
    /// \brief abstractMoleculeGroup::setName
    /// \param newName
    ///
    /// set the name of the group to \p newName
    ///
    void abstractMoleculeGroup::setName(const std::string &newName)
    {
        d->m_groupname = newName;
    }

    ///
    /// \brief abstractMoleculeGroup::addMolecule
    /// \param newMolecule
    ///
    /// add a molecule to the group
    ///
    void abstractMoleculeGroup::addMolecule(const moleculePtr newMolecule)
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

    ///
    /// \brief abstractMoleculeGroup::addGroup
    /// \param newGroup
    ///
    /// add a group to the group
    ///
    void abstractMoleculeGroup::addGroup(const groupPtr &newGroup)
    {
        d->m_groups.push_back(newGroup);
    }

    ///
    /// \brief abstractMoleculeGroup::removeGroup
    /// \param index
    ///
    /// remove a group from the group
    ///
    void abstractMoleculeGroup::removeGroup(const size_t index)
    {
        checkIndex(index);

        d->m_groups.erase(d->m_groups.begin() + index);
    }

    ///
    /// \brief abstractMoleculeGroup::addToGroup
    /// \param newParentGroup
    ///
    /// set the parent of this group when it gets added to another group
    ///
    void abstractMoleculeGroup::addToGroup(const groupPtr &newParentGroup)
    {
        d->m_parentgroup = newParentGroup;
    }

} // namespace molconv
