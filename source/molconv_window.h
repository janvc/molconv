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

#ifndef MOLCONV_WINDOW_H_
#define MOLCONV_WINDOW_H_


#include<QtGui>
#ifndef Q_MOC_RUN
    #include<chemkit/moleculefile.h>
    #include<chemkit/graphicsmoleculeitem.h>
#endif
#include"molecule.h"
#include"open_molecule_dialog.h"


namespace Ui
{
    class molconv_window;
}

class molconv_window : public QMainWindow
{
    Q_OBJECT

public:
    molconv_window(QMainWindow *parent = 0);
    ~molconv_window();
    void add_molecule();
    void add_molecule(chemkit::Molecule temp_mol);
    void clean_up(const int mol_nr, const molconv::configuration &config);
    void set_intbasis(const int mol_nr, const molconv::configuration &config);

public slots:
    void openFile(const QString &filename);
    void saveFile(const QString &filename);
    void saveFile();
    void closeFile();
    void quit();
    void get_molecule_Dialog();
    void openDialog();

private slots:

private:
    Ui::molconv_window *ui;
    chemkit::MoleculeFile *the_molfile;
    std::vector<molconv::Molecule> the_molecule_objects;
    std::vector<boost::shared_ptr<molconv::Molecule> > the_molecule_pointers;
    chemkit::GraphicsMoleculeItem *the_graph_item;
    open_molecule_dialog *open_dialog;
};

#endif /* MOLCONV_WINDOW_H_ */
