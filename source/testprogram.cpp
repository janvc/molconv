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


#include<iostream>
#include<string>
#include<chemkit/moleculefile.h>
#include<boost/make_shared.hpp>
#include<QDebug>
#include"system.h"
//#include"types.h"


int main(int argc, char *argv[])
{
#ifdef HERTHA_DEBUG
    std::cout << "this is the main function.\n";
#endif
    std::string molFileName = argv[1];

    chemkit::MoleculeFile molFile(molFileName);
    molFile.read();

    molconv::System theSystem;

    molconv::Molecule theMolecule(*(molFile.molecule()));

    //theSystem.addMolecule(molFile.molecule());
    theSystem.addMolecule(boost::make_shared<molconv::Molecule>(theMolecule));
    //theSystem.addMolecule(boost::dynamic_pointer_cast<molconv::Molecule>(molFile.molecule()));

    std::cout << "the system contains " << theSystem.size() << " molecules.\n";

    theSystem.getMolecule(0)->setOrigin(molconv::kCenterOfMass);
    std::cout << std::endl << theSystem.getMolecule(0)->internalOriginPosition() << std::endl;

    molconv::Molecule testmol = *theSystem.getMolecule(0);
    testmol.setOrigin(molconv::kCenterOfMass);
    std::cout << std::endl << testmol.internalOriginPosition() << std::endl;


    theMolecule.setOrigin(molconv::kCenterOfMass);
    std::cout << std::endl << theMolecule.internalOriginPosition() << std::endl;

    qDebug() << "this is a qt debug message";

    return 0;
}
