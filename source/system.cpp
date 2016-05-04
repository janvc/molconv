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


#include<stdexcept>
#include<algorithm>
#include<boost/make_shared.hpp>
#include<QDebug>
#include "system.h"


namespace molconv
{
    class SystemPrivate
    {
    public:
        std::vector<moleculePtr> m_molecules;
        std::vector<groupPtr> m_groups;
    };

    ///
    /// \brief System::System
    ///
    /// The default constructor of the System class
    ///
    System::System()
        : d(new SystemPrivate)
    {
        qDebug() << "this is the first constructor of molconv::System";
    }

    ///
    /// \brief System::~System
    ///
    /// The default destructor of the System class
    ///
    System::~System() {}

    ///
    /// \brief System::size
    /// \return size_t
    ///
    /// returns the number of molecules in the system
    ///
    size_t System::nMolecules() const
    {
        qDebug() << "entering System::nMolecules()";
        return d->m_molecules.size();
    }

    ///
    /// \brief System::nGroups
    /// \return
    ///
    /// returns the number of groups in the system
    ///
    size_t System::nGroups() const
    {
        qDebug("entering System::nGroups()");
        return d->m_groups.size();
    }

    ///
    /// \brief System::Molecule
    /// \param index
    /// \return moleculePtr
    ///
    /// returns a shared pointer to the molecule at \p index
    ///
    moleculePtr System::getMolecule(const size_t index) const
    {
        qDebug() << "entering System::getMolecule()";
        if (index >= nMolecules())
            throw std::invalid_argument("index out of range.\n");

        return d->m_molecules.at(index);
    }

    ///
    /// \brief System::getGroup
    /// \param index
    /// \return
    ///
    /// returns a shared pointer to the group at \p index
    ///
    groupPtr System::getGroup(const size_t index) const
    {
        qDebug("entering System::getGroup()");
        if (index >= nGroups())
            throw std::invalid_argument("index out of range.\n");

        return d->m_groups.at(index);
    }

    ///
    /// \brief System::MoleculeIndex
    /// \param theMolecule
    /// \return
    ///
    /// returns the position of the molecule in the vector
    ///
    size_t System::MoleculeIndex(const moleculePtr theMolecule)
    {
        qDebug("entering System::MoleculeIndex()");

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
    size_t System::GroupIndex(const groupPtr &theGroup) const
    {
        qDebug("entering System::GroupIndex()");

        size_t index = 0;

        for (size_t i = 0; i < nGroups(); i++)
        {
            if (getGroup(i) == theGroup)
                index = i;
        }

        return index;
    }

    ///
    /// \brief System::addMolecule
    /// \param newMolecule
    ///
    /// adds a new molecule \p newMolecule to the system
    ///
    void System::addMolecule(const moleculePtr newMolecule)
    {
        qDebug() << "entering System::addMolecule()";
        d->m_molecules.push_back(newMolecule);

        //QModelIndex index = createIndex(nMolecules(), 0, newMolecule.get());

        //emit dataChanged(index, index);
    }

    ///
    /// \brief System::removeMolecule
    /// \param index
    ///
    /// removes the molecule at index \p index
    ///
    void System::removeMolecule(const size_t index)
    {
        qDebug() << "entering System::removeMolecule()";
        if (index >= nMolecules())
            throw std::invalid_argument("index out of range.\n");

        d->m_molecules.erase(d->m_molecules.begin() + index);
    }

    ///
    /// \brief System::addGroup
    /// \param newGroup
    ///
    /// adds a new group to the system
    ///
    void System::addGroup(const groupPtr &newGroup)
    {
        qDebug("entering System::addGroup()");
        d->m_groups.push_back(newGroup);
    }

    ///
    /// \brief System::removeGroup
    /// \param index
    ///
    /// removes the group at index \p index
    ///
    void System::removeGroup(const size_t index)
    {
        qDebug("entering System::removeGroup()");

        if (index >= nGroups())
            throw std::invalid_argument("index out of range.\n");

        d->m_groups.erase(d->m_groups.begin() + index);
    }

} // namespace molconv
