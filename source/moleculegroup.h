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

#ifndef MOLECULEGROUP_H
#define MOLECULEGROUP_H

#include<boost/scoped_ptr.hpp>
#include"abstractmoleculegroup.h"

namespace molconv
{
    class MoleculeGroupPrivate;

    class MoleculeGroup : public abstractMoleculeGroup
    {
    public:
        MoleculeGroup();
        MoleculeGroup(const std::string &name);
        ~MoleculeGroup();

        void setPosition(const size_t index, const Eigen::Vector3d &newPosition);
        void setEulerTheta(const size_t index, const double newTheta);
        void setEulerPhi(const size_t index, const double newPhi);
        void setEulerPsi(const size_t index, const double newPsi);

    private:
        boost::scoped_ptr<MoleculeGroupPrivate> d;
    };

} // namespace molconv

#endif // MOLECULEGROUP_H
