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
#include<iomanip>
#include<fstream>
#include<string>
#include<eigen3/Eigen/Eigenvalues>
#include"molecule.h"
#include"utilities.h"




molecule::molecule(const char *input_file)
{
	std::ifstream input(input_file);
	std::string line;


	// read the molecule from the input file:
	int lines_read = 0;
	while (getline(input, line))
	{
		lines_read++;
		std::istringstream iss(line);
		std::string atomsymbol_dummy;
		double dummy_x, dummy_y, dummy_z;
		switch (lines_read)
		{
		case 1:
			iss >> this->number_of_atoms;
			break;
		case 2:
			this->comment_line = line;
			break;
		default:
			iss >> atomsymbol_dummy >> dummy_x >> dummy_y >> dummy_z;

			int atomicnumber_dummy = symbol2number(atomsymbol_dummy);
			Eigen::Vector3d position_dummy(dummy_x, dummy_y, dummy_z);

			this->theatoms.push_back(atom(atomicnumber_dummy, position_dummy));
			break;
		}
	}
	std::cout << "Successfully read input file: " << input_file << std::endl;


	std::cout << "Atomic positions before shifting:" << std::endl;
	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		std::cout.precision(5);
		std::cout << std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
				  << std::setw(16) << std::fixed << atiter->get_int_x()
				  << std::setw(15) << std::fixed << atiter->get_int_y()
				  << std::setw(15) << std::fixed << atiter->get_int_z() << std::endl;
	}

	// calculate the molecular properties:
	this->calc_mass();
	this->calc_com();

	this->internal_origin = this->center_of_mass;

	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		atiter->shift(-(this->center_of_mass));
	}

	std::cout << "Atomic positions after shifting (internal positions):" << std::endl;
	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		std::cout.precision(5);
		std::cout << std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
				  << std::setw(16) << std::fixed << atiter->get_int_x()
				  << std::setw(15) << std::fixed << atiter->get_int_y()
				  << std::setw(15) << std::fixed << atiter->get_int_z() << std::endl;
	}

	// calculate and diagonalize the inertia tensor:
	this->calc_inertia();
	this->diag_inertia();

	// transform the atomic coordinates into the internal basis:
	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		atiter->transform(this->internal_basis.transpose());
	}

	std::cout << "Atomic positions in the internal basis after transforming:" << std::endl;
	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		std::cout.precision(5);
		std::cout << std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
				  << std::setw(16) << std::fixed << atiter->get_int_x()
				  << std::setw(15) << std::fixed << atiter->get_int_y()
				  << std::setw(15) << std::fixed << atiter->get_int_z() << std::endl;
	}

	std::cout << "Origin of the internal basis:" << std::endl << this->internal_origin << std::endl;
	std::cout << "Absolute positons:" << std::endl;
	this->print_stdout();
	this->show_info();
}


void molecule::shift(Eigen::Vector3d shift_vector)
{
	this->internal_origin += shift_vector;
}


void molecule::print_stdout()
{
	std::cout << "The number of atoms is: " << this->number_of_atoms << std::endl;
	std::cout << this->comment_line << std::endl;

	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		Eigen::Vector3d abs_pos = this->internal_origin + this->internal_basis * atiter->get_int_position();

		std::cout.precision(5);
		std::cout << std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
				  << std::setw(16) << std::fixed << abs_pos(0)
				  << std::setw(15) << std::fixed << abs_pos(1)
				  << std::setw(15) << std::fixed << abs_pos(2) << std::endl;
	}
}


std::string molecule::get_commentline()
{
	return this->comment_line;
}


void molecule::show_info()
{
	std::cout << "Molecular mass: " << std::endl << this->mass << " u" << std::endl;
	std::cout << std::endl;
	std::cout << "Center of mass: " << std::endl << this->center_of_mass << std::endl;
	std::cout << std::endl;
	std::cout << "Inertia Tensor: " << std::endl << this->inertia_tensor << std::endl;
}


void molecule::calc_mass()
{
	this->mass = 0.0;

	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		this->mass += atiter->get_atomicmass();
	}
}


void molecule::calc_com()
{
	this->center_of_mass << 0.0, 0.0, 0.0;

	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		this->center_of_mass += atiter->get_int_position() * atiter->get_atomicmass();
	}

	this->center_of_mass /= this->mass;
}


void molecule::calc_inertia()
{
	for (int alpha = 0; alpha < 3; alpha++)
	{
		for (int beta = 0; beta < 3; beta++)
		{
			this->inertia_tensor(alpha,beta) = 0.0;

			for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
			{
				double factor = 0.0;
				if (alpha == beta)
					factor = atiter->get_int_position().dot(atiter->get_int_position());

				factor -= atiter->get_int_position()(alpha) * atiter->get_int_position()(beta);
				this->inertia_tensor(alpha,beta) += atiter->get_atomicmass() * factor;
			}
		}
	}
}


void molecule::diag_inertia()
{
	Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(this->inertia_tensor);

	if (solver.info() != Eigen::Success)
	{
		std::cout << "The inertia tensor could not be diagonalized" << std::endl;
	}
	else
	{
		this->inertia_moments = solver.eigenvalues();
		this->internal_basis = solver.eigenvectors();
	}
}


bool molecule::write_to_file(const char *outputfile)
{
	std::ofstream output(outputfile);

	output	<< this->number_of_atoms << std::endl
			<< "# File generated with molconv" << std::endl;

	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		Eigen::Vector3d abs_pos = this->internal_origin + this->internal_basis * atiter->get_int_position();

		output.precision(5);
		output	<< std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
				<< std::setw(16) << std::fixed << abs_pos(0)
				<< std::setw(15) << std::fixed << abs_pos(1)
				<< std::setw(15) << std::fixed << abs_pos(2) << std::endl;
	}

    output.close();

	return true;
}


void molecule::clean_up()
{
	this->internal_origin << 0,0,0;
	this->internal_basis << 1,0,0,
			                0,1,0,
			                0,0,1;
}

