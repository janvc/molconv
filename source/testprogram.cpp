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


#include<iostream>
#include<string>
#include<chemkit/moleculefile.h>
#include<boost/make_shared.hpp>
#include<QDebug>
#include"molconv-system/system.h"
#include"molconv-moleculegroup/moleculegroup.h"
#include"molconv-moleculegroup/moleculestack.h"
//#include"types.h"


int main(int argc, char *argv[])
{
    qDebug("starting the program");

    std::string firstFileName = argv[1];
    std::string secondFileName = argv[2];
    std::string outputFileName = argv[3];

    qDebug("creating and reading the molecule files");
    chemkit::MoleculeFile firstMolFile(firstFileName);
    firstMolFile.read();
    chemkit::MoleculeFile secondMolFile(secondFileName);
    secondMolFile.read();

    qDebug("creating the system");
    molconv::System theSystem;


    qDebug("adding the molecules to the system");
    theSystem.addMolecule(boost::make_shared<molconv::Molecule>(molconv::Molecule(firstMolFile.molecule())));
    theSystem.addMolecule(boost::make_shared<molconv::Molecule>(molconv::Molecule(secondMolFile.molecule())));


    std::cout << "the system contains " << theSystem.nMolecules() << " molecules.\n";

    qDebug("setting the origins and the bases of the molecules");
    theSystem.getMolecule(0)->setOrigin(molconv::kCenterOfMass);
    theSystem.getMolecule(1)->setOrigin(molconv::kCenterOfMass);
    theSystem.getMolecule(0)->setBasis(molconv::kInertiaVectors);
    theSystem.getMolecule(1)->setBasis(molconv::kInertiaVectors);

    qDebug("shifting the second molecule");
    theSystem.getMolecule(1)->translate(Eigen::Vector3d(0.0, 0.0, 3.0));

    qDebug("creating the stack");
    molconv::MoleculeStack theStack;

    qDebug("adding the first molecule to the stack");
    theStack.addMolecule(theSystem.getMolecule(0), molconv::kInertVLarge);
    qDebug("adding the second molecule to the stack");
    theStack.addMolecule(theSystem.getMolecule(1), molconv::kInertVLarge);

    qDebug("setting the distance to 1 angstrom");
    theStack.setPlaneDistance(1, 1.0);

    qDebug("creating a dummy molecule for the output file");
    chemkit::Molecule outputDummyMolecule = *(theSystem.getMolecule(0).get());

    qDebug("adding the atoms of the other molecule(s)");
    for (size_t i = 0; i < theSystem.getMolecule(1)->size(); i++)
    {
        outputDummyMolecule.addAtomCopy(theSystem.getMolecule(1)->atom(i));
    }

    qDebug("creating (and writing to) the output file");
    chemkit::MoleculeFile outputFile(outputFileName);
    outputFile.addMolecule(boost::make_shared<chemkit::Molecule>(outputDummyMolecule));
    outputFile.write();

    qDebug("ending the program");
    return 0;
}
