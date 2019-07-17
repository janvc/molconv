/*
 * Copyright 2014 - 2019 Jan von Cosel & Sebastian Lenz
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


#include <iostream>
#include "test_molecule.h"

void TestMolecule::initTestCase()
{
//    molconv::Molecule mol;

    mol.addAtom("C");
    mol.addAtom("H");
    mol.addAtom("H");
    mol.addAtom("H");
    mol.addAtom("H");

    mol.atom(1)->setPosition(1.0, 1.0, 1.0);
    mol.atom(2)->setPosition(-1.0, -1.0, 1.0);
    mol.atom(3)->setPosition(-1.0, 1.0, -1.0);
    mol.atom(4)->setPosition(1.0, -1.0, -1.0);
}

void TestMolecule::testFoo()
{
//    molconv::Molecule mol;

    QVERIFY(mol.size() == 5);
}

void TestMolecule::testFoo2()
{
    std::cout << mol.center() << std::endl;
}

QTEST_APPLESS_MAIN(TestMolecule)
