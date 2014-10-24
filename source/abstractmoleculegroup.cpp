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
        if (index > d->m_molecules.size())
            throw std::invalid_argument("Index out of range.\n");

        d->m_molecules.erase(d->m_molecules.begin() + index);
    }

} // namespace molconv
