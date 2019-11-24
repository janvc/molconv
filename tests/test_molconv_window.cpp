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
    m_cmol = boost::shared_ptr<chemkit::Molecule>(new chemkit::Molecule);

    m_cmol->addAtom("C");
    m_cmol->addAtom("H");
    m_cmol->addAtom("H");
    m_cmol->addAtom("H");
    m_cmol->addAtom("H");
    m_cmol->atom(1)->setPosition(1.0, 1.0, 1.0);
    m_cmol->atom(2)->setPosition(-1.0, -1.0, 1.0);
    m_cmol->atom(3)->setPosition(-1.0, 1.0, -1.0);
    m_cmol->atom(4)->setPosition(1.0, -1.0, -1.0);
}

void TestMolconvWindow::test_add_molecule()
{
    MolconvWindow *win = new MolconvWindow(0);
    molconv::moleculePtr mol(new molconv::Molecule(m_cmol));

    win->add_molecule(mol);

    QCOMPARE(win->nMolecules(), 1);
}

void TestMolconvWindow::test_fresh_window_is_not_modified()
{
    MolconvWindow *win = new MolconvWindow(0);
    QVERIFY(!win->isWindowModified());
}

void TestMolconvWindow::test_fresh_window_is_untitled()
{
    MolconvWindow *win = new MolconvWindow(0);
    QString test = win->windowTitle();
    QString expectedTitle = tr("untitled[*] - molconv");

    QCOMPARE(test, expectedTitle);
}

void TestMolconvWindow::test_importing_molecule_sets_window_modified()
{
    MolconvWindow *win = new MolconvWindow(0);
    win->importFile("../../molconv/molecules/methane_C1.xyz", false);

    QVERIFY(win->isWindowModified());
}

void TestMolconvWindow::test_opening_molconv_file_does_not_modify_window()
{
    MolconvWindow *win = new MolconvWindow(0);
    win->readMolconvFile("../../molconv/molecules/example.mcv");
    QVERIFY(!win->isWindowModified());
}

void TestMolconvWindow::test_opening_molconv_file_sets_window_title()
{
    MolconvWindow *win = new MolconvWindow(0);
    win->readMolconvFile("../../molconv/molecules/example.mcv");
    QString test = win->windowTitle();
    QString expectedTitle = tr("example.mcv[*] - molconv");

    QCOMPARE(test, expectedTitle);
}

void TestMolconvWindow::test_saving_molconv_file_sets_window_not_modified()
{
    MolconvWindow *win = new MolconvWindow(0);

    win->importFile("../../molconv/molecules/methane_C1.xyz", false);
    win->writeMolconvFile("../../molconv/molecules/test.mcv");
    QVERIFY(!win->isWindowModified());
}

void TestMolconvWindow::test_saving_molconv_file_sets_window_title()
{
    MolconvWindow *win = new MolconvWindow(0);
    win->importFile("../../molconv/molecules/methane_C1.xyz", false);
    win->writeMolconvFile("../../molconv/molecules/test.mcv");
    QString test = win->windowTitle();
    QString expectedTitle = tr("test.mcv[*] - molconv");

    QCOMPARE(test, expectedTitle);
}

void TestMolconvWindow::test_adding_molecule_to_open_molconv_file_sets_window_modified()
{

    MolconvWindow *win = new MolconvWindow(0);
    win->readMolconvFile("../../molconv/molecules/example.mcv");
    win->importFile("../../molconv/molecules/methane_C1.xyz", false);

    QVERIFY(win->isWindowModified());
}

QTEST_MAIN(TestMolconvWindow)
