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

#include "moleculegroup.h"

namespace molconv
{
    class MoleculeGroupPrivate
    {
    public:
        MoleculeGroupPrivate()
        {
        }
    };

    ///
    /// \brief MoleculeGroup::MoleculeGroup
    ///
    /// this is the default constructor
    ///
    MoleculeGroup::MoleculeGroup()
        : abstractMoleculeGroup()
        , d(new MoleculeGroupPrivate)
    {
    }

    ///
    /// \brief MoleculeGroup::MoleculeGroup
    /// \param name
    ///
    /// this constructor takes the group name as an argument
    ///
    MoleculeGroup::MoleculeGroup(const std::string &name)
        : abstractMoleculeGroup(name)
        , d(new MoleculeGroupPrivate)
    {
    }

    ///
    /// \brief MoleculeGroup::~MoleculeGroup
    ///
    /// this is the default destructor
    ///
    MoleculeGroup::~MoleculeGroup() {}



} // namespace molconv
