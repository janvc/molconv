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

#include"molecule.h"
#include<QtGui>


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
	void add_molecule(boost::shared_ptr<molconv::Molecule> &molecule);

public slots:
	void openFile(const QString &filename);
	void openFile();

private:
	Ui::molconv_window *ui;
};

#endif /* MOLCONV_WINDOW_H_ */
