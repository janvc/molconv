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


#ifndef TEST_MOLCONV_WINDOW_H
#define TEST_MOLCONV_WINDOW_H

#include <QTest>

#include "source/molconv_window.h"

class TestMolconvWindow : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void test_add_molecule();
    void test_fresh_window_is_not_modified();
    void test_importing_molecule_sets_window_modified();
    void test_opening_molconv_file_does_not_modify_window();
    void test_saving_molconv_file_sets_window_not_modified();
    void test_adding_molecule_to_open_molconv_file_sets_window_modified();

private:
    boost::shared_ptr<chemkit::Molecule> m_cmol;
};

#endif // TEST_MOLCONV_WINDOW_H
