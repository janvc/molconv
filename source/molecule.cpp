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
				  << std::setw(16) << std::fixed << atiter->get_x()
				  << std::setw(15) << std::fixed << atiter->get_y()
				  << std::setw(15) << std::fixed << atiter->get_z() << std::endl;
	}
	// calculate the molecular properties:
	this->calc_mass();
	std::cout << "Molecular mass: " << this->mass << std::endl;
	this->calc_com();
	std::cout << "Location of the center of mass: " << std::endl << this->center_of_mass << std::endl;

	this->internal_origin = this->center_of_mass;

	this->shift(- this->internal_origin);

	std::cout << "Atomic positions after shifting (internal positions):" << std::endl;
	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		std::cout.precision(5);
		std::cout << std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
				  << std::setw(16) << std::fixed << atiter->get_x()
				  << std::setw(15) << std::fixed << atiter->get_y()
				  << std::setw(15) << std::fixed << atiter->get_z() << std::endl;
	}

	this->calc_inertia();
	std::cout << "inertia tensor:" << std::endl << this->inertia_tensor << std::endl;
	this->diag_inertia();

	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		atiter->transform(this->internal_basis.transpose());
	}
	std::cout << "Atomic positions after transforming:" << std::endl;
	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		std::cout << atiter->get_atomicsymbol() << "  "
				  << atiter->get_x() << "  " << atiter->get_y() << "  " << atiter->get_z() << std::endl;
	}
}


void molecule::shift(Eigen::Vector3d shift_vector)
{
	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		atiter->shift(shift_vector);
	}
}


void molecule::print_stdout()
{
	std::cout << "The number of atoms is: " << this->number_of_atoms << std::endl;
	std::cout << this->comment_line << std::endl;
	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		std::cout.precision(5);
		std::cout << std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
				  << std::setw(16) << std::fixed << atiter->get_x()
				  << std::setw(15) << std::fixed << atiter->get_y()
				  << std::setw(15) << std::fixed << atiter->get_z() << std::endl;
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
		this->center_of_mass += atiter->get_position() * atiter->get_atomicmass();
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
					factor = atiter->get_position().dot(atiter->get_position());

				factor -= atiter->get_position()(alpha) * atiter->get_position()(beta);
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
		std::cout << "The moments of inertia are:" << std::endl
						  << this->inertia_moments << std::endl;
		std::cout << "Eigenvectors of inertia tensor:" << std::endl
				  << this->internal_basis << std::endl;
	}
}


void molecule::clean_up()
{
	// shift the molecule to the center of mass
	this->shift(-this->center_of_mass);
}


bool molecule::write_to_file(const char *outputfile)
{
	std::ofstream output(outputfile);

	output	<< this->number_of_atoms << std::endl
			<< "# File generated with molconv" << std::endl;

	for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
	{
		output.precision(5);
		output	<< std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
				<< std::setw(16) << std::fixed << atiter->get_x()
				<< std::setw(15) << std::fixed << atiter->get_y()
				<< std::setw(15) << std::fixed << atiter->get_z() << std::endl;
	}

    output.close();

	return true;
}


void atom::transform(Eigen::Matrix3d tmatrix)
{
	this->position = tmatrix * this->position;
}
