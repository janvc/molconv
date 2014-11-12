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
#include"moleculegroup.h"
#include"moleculestack.h"
//#include"types.h"


int main(int argc, char *argv[])
{
    qDebug("starting the program");

    std::string firstFileName = argv[1];
    std::string secondFileName = argv[2];

    qDebug("creating and reading the molecule files");
    chemkit::MoleculeFile firstMolFile(firstFileName);
    firstMolFile.read();
    chemkit::MoleculeFile secondMolFile(secondFileName);
    secondMolFile.read();

    qDebug("creating the system");
    molconv::System theSystem;


    qDebug("trying a new thing");
    theSystem.addMolecule(boost::make_shared<molconv::Molecule>(molconv::Molecule(firstMolFile.molecule())));
    theSystem.addMolecule(boost::make_shared<molconv::Molecule>(molconv::Molecule(secondMolFile.molecule())));


    std::cout << "the system contains " << theSystem.size() << " molecules.\n";

    qDebug("setting the origins of the molecules");
    theSystem.getMolecule(0)->setOrigin(molconv::kCenterOfGeometry);
    theSystem.getMolecule(1)->setOrigin(molconv::kCenterOfMass);

    qDebug("writing out the origin positions");
    std::cout << std::endl << theSystem.getMolecule(0)->internalOriginPosition() << std::endl;
    std::cout << std::endl << theSystem.getMolecule(1)->internalOriginPosition() << std::endl;

    qDebug("creating the group");
    molconv::MoleculeGroup theGroup;

    qDebug("adding the molecules to the group");
    theGroup.addMolecule(theSystem.getMolecule(0));
    theGroup.addMolecule(theSystem.getMolecule(1));

    std::cout << "the group contains " << theGroup.size() << " molecules.\n";

    qDebug("writing out the distance vector");
    std::cout << std::endl << theGroup.DistanceVector(0, 1) << std::endl;

    qDebug("and the distance");
    std::cout << std::endl << theGroup.Distance(0, 1) << std::endl;

    qDebug("removing the molecules");
    theGroup.removeMolecule(0);
    theGroup.removeMolecule(0);

    std::cout << "the group contains " << theGroup.size() << " molecules.\n";

    qDebug("creating a stack");
    molconv::MoleculeStack theStack;

    std::cout << theStack.size() << std::endl;

    qDebug("adding the first molecule to the stack");
    theStack.addMolecule(theSystem.getMolecule(0), molconv::kInertVLarge);
    qDebug("adding the second molecule to the stack");
    theStack.addMolecule(theSystem.getMolecule(1), molconv::kCovarVMedium);

    qDebug("ending the program");
    return 0;
}
