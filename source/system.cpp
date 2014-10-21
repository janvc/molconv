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


#include<stdexcept>
#include<algorithm>
#include<boost/make_shared.hpp>
#include "system.h"


namespace molconv
{
    class SystemPrivate
    {
    public:
        std::vector<Molecule> m_molecules;
    };

    ///
    /// \brief System::System
    ///
    /// The default constructor of the System class
    ///
    System::System()
        : d(new SystemPrivate)
    {
    }

    ///
    /// \brief System::size
    /// \return
    ///
    /// returns the number of molecules in the system
    ///
    size_t System::size() const
    {
        return d->m_molecules.size();
    }

    ///
    /// \brief System::Molecule
    /// \param index
    /// \return
    ///
    /// returns a boost pointer to the molecule at \p index
    ///
    boost::shared_ptr<Molecule> System::getMolecule(const size_t index) const
    {
        if (index > size() || index < 0)
            throw std::invalid_argument("index out of range.\n");

        return boost::make_shared<Molecule>(d->m_molecules.at(index));
    }

    ///
    /// \brief System::removeMolecule
    /// \param index
    ///
    /// removes the molecule at index \p index
    ///
    void System::removeMolecule(const size_t index)
    {
        if (index > size() || index < 0)
            throw std::invalid_argument("index out of range.\n");

        d->m_molecules.erase(d->m_molecules.begin() + index);
    }

} // namespace molconv
