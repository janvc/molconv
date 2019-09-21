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


#include "test_molconv_window.h"

void TestMolconvWindow::initTestCase()
{
}

void TestMolconvWindow::test_add_molecule()
{
    MolconvWindow *win = new MolconvWindow(0);
    molconv::moleculePtr mol(new molconv::Molecule);

    mol->addAtom("C");
    mol->addAtom("H");
    mol->addAtom("H");
    mol->addAtom("H");
    mol->addAtom("H");
    mol->atom(1)->setPosition(1.0, 1.0, 1.0);
    mol->atom(2)->setPosition(-1.0, -1.0, 1.0);
    mol->atom(3)->setPosition(-1.0, 1.0, -1.0);
    mol->atom(4)->setPosition(1.0, -1.0, -1.0);

    win->add_molecule(mol);

    QCOMPARE(win->nMolecules(), 1);
}

void TestMolconvWindow::test_fresh_window_is_not_modified()
{
    MolconvWindow *win = new MolconvWindow(0);
    QVERIFY(!win->isWindowModified());
}

void TestMolconvWindow::test_new_molecule_sets_window_modified()
{
    MolconvWindow *win = new MolconvWindow(0);
    molconv::moleculePtr mol(new molconv::Molecule);

    mol->addAtom("C");
    mol->addAtom("H");
    mol->addAtom("H");
    mol->addAtom("H");
    mol->addAtom("H");
    mol->atom(1)->setPosition(1.0, 1.0, 1.0);
    mol->atom(2)->setPosition(-1.0, -1.0, 1.0);
    mol->atom(3)->setPosition(-1.0, 1.0, -1.0);
    mol->atom(4)->setPosition(1.0, -1.0, -1.0);

    win->add_molecule(mol);

    QVERIFY(win->isWindowModified());
}

void TestMolconvWindow::test_opening_molconv_file_does_not_modify_window()
{
    MolconvWindow *win = new MolconvWindow(0);
    win->readMolconvFile("../../molconv/molecules/example.mcv");
    QVERIFY(!win->isWindowModified());
}

void TestMolconvWindow::test_saving_molconv_file_sets_window_not_modified()
{
    MolconvWindow *win = new MolconvWindow(0);
    molconv::moleculePtr mol(new molconv::Molecule);

    mol->addAtom("C");
    mol->addAtom("H");
    mol->addAtom("H");
    mol->addAtom("H");
    mol->addAtom("H");
    mol->atom(1)->setPosition(1.0, 1.0, 1.0);
    mol->atom(2)->setPosition(-1.0, -1.0, 1.0);
    mol->atom(3)->setPosition(-1.0, 1.0, -1.0);
    mol->atom(4)->setPosition(1.0, -1.0, -1.0);

    win->add_molecule(mol);

    win->writeMolconvFile("../../molconv/molecules/test.mcv");
    QVERIFY(!win->isWindowModified());
}

void TestMolconvWindow::test_adding_molecule_to_open_molconv_file_sets_window_modified()
{

    MolconvWindow *win = new MolconvWindow(0);
    win->readMolconvFile("../../molconv/molecules/example.mcv");

    molconv::moleculePtr mol(new molconv::Molecule);

    mol->addAtom("C");
    mol->addAtom("H");
    mol->addAtom("H");
    mol->addAtom("H");
    mol->addAtom("H");
    mol->atom(1)->setPosition(1.0, 1.0, 1.0);
    mol->atom(2)->setPosition(-1.0, -1.0, 1.0);
    mol->atom(3)->setPosition(-1.0, 1.0, -1.0);
    mol->atom(4)->setPosition(1.0, -1.0, -1.0);

    win->add_molecule(mol);

    QVERIFY(win->isWindowModified());
}

QTEST_MAIN(TestMolconvWindow)
