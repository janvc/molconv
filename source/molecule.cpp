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
#include<cmath>
#include<Eigen/Eigenvalues>
#include"molecule.h"
#include"utilities.h"


namespace molconv
{
	Molecule::Molecule(const chemkit::Molecule &base_molecule)
		: chemkit::Molecule(base_molecule)
		, number_of_atoms(this->size())
		, total_mass(this->mass())
		, center_of_mass(this->centerOfMass())
		, center_of_geometry(this->center())
	{
	}


	/*
	 * This function calculates the inertia tensor of the molecule. The elements of the inertia tensor
	 * are given by:
	 * 					J_ij = sum_k=1^N  m_k * ((r_k)^2 * delta_ij) - x_i * x_j
	 *
	 * This results in:
	 * 						 / y^2 + z^2    - x * y     - x * z  \
	 * 					J = |   - x * y    x^2 + z^2    - y * z   |
	 * 					     \  - x * z     - y * z    x^2 + y^2 /
	 *
	 * where the sum is implicitly included in the matrix elements.
	 */
	void Molecule::calc_inertia()
	{
		for (size_t alpha = 0; alpha < 3; alpha++)
		{
			for (size_t beta = 0; beta < 3; beta++)
			{
				this->inertia_tensor(alpha,beta) = 0.0;

				for (size_t atiter = 0; atiter < this->number_of_atoms; atiter++)
				{
					double factor = 0.0;

					if (alpha == beta)
						factor = (this->atom(atiter)->position() - this->center_of_mass)
						         .dot(this->atom(atiter)->position() - this->center_of_mass);

					factor -= (this->atom(atiter)->position() - this->center_of_mass)(alpha)
							* (this->atom(atiter)->position() - this->center_of_mass)(beta);

					this->inertia_tensor(alpha,beta) += this->atom(atiter)->mass() * factor;
				}
			}
		}
	}


	/*
	 * This function diagonalizes the inertia tensor using a routine from the EIGEN library.
	 * The eigenvectors of the inertia tensor are the principal axes of the molecule and these are
	 * used as the coordinate axes for the molecule fixed coordinate system. The eigenvalues of
	 * the inertia tensor are the moments of inertia along those axes.
	 */
	void Molecule::diag_inertia()
	{
		Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(this->inertia_tensor);

		if (solver.info() != Eigen::Success)
		{
			std::cerr << "The inertia tensor could not be diagonalized" << std::endl;
		}
		else
		{
			this->inertia_eigvals = solver.eigenvalues();
			this->inertia_eigvecs = solver.eigenvectors();
		}
	}

	void Molecule::calc_covar_mat()
	{
	}

	void Molecule::diag_covar_mat()
	{
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//namespace molconv
//{
//	/*
//	 * The constructor of the molecule class. This constructor reads a (xyz-) molecular structure file
//	 * and sets up the internal coordinate representation of the molecular structure. It calculates and
//	 * diagonalizes the inertia tensor and performs a prinicpal axis transformation.
//	 */
//	molecule::molecule(const char *input_file, configuration *config)
//		:number_of_atoms(0),
//		 comment_line(""),
//		 mass(0.0),
//		 internal_origin(Eigen::Vector3d::Zero()),
//		 internal_basis(Eigen::Matrix3d::Identity())
//	{
//		std::ifstream input(input_file);
//		std::string line;
//
//
//		// read the molecule from the input file:
//		int lines_read = 0;
//		while (getline(input, line))
//		{
//			lines_read++;
//			std::istringstream iss(line);
//			std::string atomsymbol_dummy;
//			double dummy_x, dummy_y, dummy_z;
//			switch (lines_read)
//			{
//			case 1:
//				iss >> this->number_of_atoms;
//				break;
//			case 2:
//				this->comment_line = line;
//				break;
//			default:
//				iss >> atomsymbol_dummy >> dummy_x >> dummy_y >> dummy_z;
//
//				int atomicnumber_dummy = symbol2number(atomsymbol_dummy);
//				Eigen::Vector3d position_dummy(dummy_x, dummy_y, dummy_z);
//
//				this->theatoms.push_back(atom(atomicnumber_dummy, position_dummy));
//				break;
//			}
//		}
//		std::cout << "Successfully read input file: " << input_file << std::endl;
//
//
//		std::cout << "Atomic positions before shifting:" << std::endl;
//		for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
//		{
//			std::cout.precision(5);
//			std::cout << std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
//					  << std::setw(16) << std::fixed << atiter->get_int_x()
//					  << std::setw(15) << std::fixed << atiter->get_int_y()
//					  << std::setw(15) << std::fixed << atiter->get_int_z() << std::endl;
//		}
//
//		// calculate the molecular properties:
//		this->calc_mass();
//		this->calc_com();
//		this->calc_cog();
//		this->calc_inertia();
//		this->calc_covar_mat();
//
//		this->diag_inertia();
//		this->diag_covar_mat();
//
//		std::cout << " position of the center of mass:" << std::endl << this->center_of_mass << std::endl;
//		std::cout << " position of the center of geometry:" << std::endl << this->center_of_geometry << std::endl;
//
//		// set the internal origin according to the given configuration:
//		switch (config->get_orig_type())
//		{
//		case 1:
//			this->internal_origin = this->center_of_mass;
//			break;
//		case 2:
//			this->internal_origin = this->center_of_geometry;
//			break;
//		case 3:
//			this->internal_origin = this->theatoms.at(config->get_orig_atom()).get_int_position();
//			break;
//		}
//
//		for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
//		{
//			atiter->shift(-(this->internal_origin));
//		}
//
//		// set the internal basis according to the command options:
//		switch (config->get_axes_type())
//		{
//		case 1:
//			this->internal_basis = this->inertia_eigvecs;
//			break;
//		case 2:
//			this->internal_basis = this->covar_eigvec;
//			break;
//		}
//
//		std::cout << "Atomic positions after shifting (internal positions):" << std::endl;
//		for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
//		{
//			std::cout.precision(5);
//			std::cout << std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
//					  << std::setw(16) << std::fixed << atiter->get_int_x()
//					  << std::setw(15) << std::fixed << atiter->get_int_y()
//					  << std::setw(15) << std::fixed << atiter->get_int_z() << std::endl;
//		}
//
//		// calculate and diagonalize the inertia tensor:
//		this->calc_inertia();
//		this->diag_inertia();
//		std::cout << "Inertia Tensor: " << std::endl << this->inertia_tensor << std::endl;
//		std::cout << "Eigenvalues of the inertia Tensor:" << std::endl << this->inertia_moments << std::endl;
//		std::cout << "Eigenvectors of the inertia Tensor:" << std::endl << this->internal_basis << std::endl;
//
//		// transform the atomic coordinates into the internal basis:
//		for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
//		{
//			atiter->transform(this->internal_basis.transpose());
//		}
//
//		std::cout << "Atomic positions in the internal basis after transforming:" << std::endl;
//		for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
//		{
//			std::cout.precision(5);
//			std::cout << std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
//					  << std::setw(16) << std::fixed << atiter->get_int_x()
//					  << std::setw(15) << std::fixed << atiter->get_int_y()
//					  << std::setw(15) << std::fixed << atiter->get_int_z() << std::endl;
//		}
//
//		// calculate and diagonalize the covariance matrix:
//		this->calc_covar_mat();
//		this->diag_covar_mat();
//		std::cout << "Covariance Matrix:" << std::endl << this->covar_mat << std::endl;
//		std::cout << "Eigenvalues of the Covariance Matrix:" << std::endl << this->covar_eigval << std::endl;
//		std::cout << "Eigenvectors of the Covariance Matrix:" << std::endl << this->covar_eigvec << std::endl;
//		std::cout << "Eigenvectors of the Covariance Matrix in the internal basis:" << std::endl
//				  << this->internal_basis.transpose() * this->covar_eigvec << std::endl;
//
//		this->trans2euler();
//		std::cout << "Eulerian angles:" << std::endl << " Phi:    " << this->euler_phi << std::endl
//													 << " Psi:    " << this->euler_psi << std::endl
//													 << " Theta:  " << this->euler_theta << std::endl;
//
//	}
//
//
//	/*
//	 * This function translates the entire molecule about a specified shift vector
//	 */
//	void molecule::shift(Eigen::Vector3d shift_vector)
//	{
//		this->internal_origin += shift_vector;
//	}
//
//
//	/*
//	 * This function writes the molecular structure to the standard output stream, in the same format
//	 * as it would appear in an xyz-File. The coordinates are printed in the basis of the external space.
//	 */
//	void molecule::print_stdout()
//	{
//		std::cout << "The number of atoms is: " << this->number_of_atoms << std::endl;
//		std::cout << this->comment_line << std::endl;
//
//		for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
//		{
//			Eigen::Vector3d abs_pos = this->internal_origin + this->internal_basis * atiter->get_int_position();
//
//			std::cout.precision(5);
//			std::cout << std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
//					  << std::setw(16) << std::fixed << abs_pos(0)
//					  << std::setw(15) << std::fixed << abs_pos(1)
//					  << std::setw(15) << std::fixed << abs_pos(2) << std::endl;
//		}
//	}
//
//
//	/*
//	 * this function returns the comment-line from the xyz-file that was read.
//	 */
//	std::string molecule::get_commentline()
//	{
//		return this->comment_line;
//	}
//
//
//	/*
//	 * This function prints some information about the molecule to the standard output stream.
//	 */
//	void molecule::show_info()
//	{
//		std::cout << "Molecular mass: " << std::endl << this->mass << " u" << std::endl;
//		std::cout << std::endl;
//		std::cout << "Center of mass: " << std::endl << this->center_of_mass << std::endl;
//		std::cout << std::endl;
//		std::cout << "Inertia Tensor: " << std::endl << this->inertia_tensor << std::endl;
//		std::cout << "Eigenvalues of the inertia Tensor:" << std::endl << this->inertia_eigvals << std::endl;
//		std::cout << "Eigenvectors of the inertia Tensor:" << std::endl << this->internal_basis << std::endl;
//		std::cout << "Covariance Matrix:" << std::endl << this->covar_mat << std::endl;
//		std::cout << "Eigenvalues of the Covariance Matrix:" << std::endl << this->covar_eigval << std::endl;
//		std::cout << "Eigenvectors of the Covariance Matrix:" << std::endl << this->covar_eigvec << std::endl;
//	}
//
//
//	/*
//	 * This function calculates the total mass of the molecule.
//	 */
//	void molecule::calc_mass()
//	{
//		this->mass = 0.0;
//
//		for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
//		{
//			this->mass += atiter->get_atomicmass();
//		}
//	}
//
//
//	/*
//	 * This function calculates the position of the (mass-weighted) center-of-mass of the molecule.
//	 */
//	void molecule::calc_com()
//	{
//		this->center_of_mass << 0.0, 0.0, 0.0;
//
//		for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
//		{
//			this->center_of_mass += atiter->get_int_position() * atiter->get_atomicmass();
//		}
//
//		this->center_of_mass /= this->mass;
//	}
//
//
//	/*
//	 * This function calculates the position of the (non-mass-weighted) center-of-geometry of the
//	 * molecule, i.e. the average position of the atoms.
//	 */
//	void molecule::calc_cog()
//		{
//			this->center_of_geometry << 0.0, 0.0, 0.0;
//
//			for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
//			{
//				this->center_of_geometry += atiter->get_int_position();
//			}
//
//			this->center_of_geometry /= this->number_of_atoms;
//		}
//
//
//	/*
//	 * This function calculates the inertia tensor of the molecule. The elements of the inertia tensor
//	 * are given by:
//	 * 					J_ij = sum_k=1^N  m_k * ((r_k)^2 * delta_ij) - x_i * x_j
//	 *
//	 * This results in:
//	 * 						 / y^2 + z^2    - x * y     - x * z  \
//	 * 					J = |   - x * y    x^2 + z^2    - y * z   |
//	 * 					     \  - x * z     - y * z    x^2 + y^2 /
//	 *
//	 * where the sum is implicitly included in the matrix elements.
//	 */
//	void molecule::calc_inertia()
//	{
//		for (int alpha = 0; alpha < 3; alpha++)
//		{
//			for (int beta = 0; beta < 3; beta++)
//			{
//				this->inertia_tensor(alpha,beta) = 0.0;
//
//				for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
//				{
//					Eigen::Vector3d abs_pos = this->internal_origin + this->internal_basis * atiter->get_int_position();
//
//					double factor = 0.0;
//					if (alpha == beta)
//						factor = (abs_pos - this->center_of_mass).dot(abs_pos - this->center_of_mass);
//
//					factor -= (abs_pos - this->center_of_mass)(alpha) * (abs_pos - this->center_of_mass)(beta);
//					this->inertia_tensor(alpha,beta) += atiter->get_atomicmass() * factor;
//				}
//			}
//		}
//	}
//
//
//	/*
//	 * This function diagonalizes the inertia tensor using a routine from the EIGEN library.
//	 * The eigenvectors of the inertia tensor are the principal axes of the molecule and these are
//	 * used as the coordinate axes for the molecule fixed coordinate system. The eigenvalues of
//	 * the inertia tensor are the moments of inertia along those axes.
//	 */
//	void molecule::diag_inertia()
//	{
//		Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(this->inertia_tensor);
//
//		if (solver.info() != Eigen::Success)
//		{
//			std::cout << "The inertia tensor could not be diagonalized" << std::endl;
//		}
//		else
//		{
//			this->inertia_eigvals = solver.eigenvalues();
//			this->inertia_eigvecs = solver.eigenvectors();
//		}
//	}
//
//
//	/*
//	 * This function writes the molecular structure (in absolute coordinates) to a text file
//	 * using the xyz format.
//	 */
//	bool molecule::write_to_file(const char *outputfile)
//	{
//		std::ofstream output(outputfile);
//
//		output	<< this->number_of_atoms << std::endl
//				<< "# File generated with molconv" << std::endl;
//
//		for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
//		{
//			Eigen::Vector3d abs_pos = this->internal_origin + this->internal_basis * atiter->get_int_position();
//
//			output.precision(5);
//			output	<< std::left << std::setw(3) << atiter->get_atomicsymbol() << std::right
//					<< std::setw(16) << std::fixed << abs_pos(0)
//					<< std::setw(15) << std::fixed << abs_pos(1)
//					<< std::setw(15) << std::fixed << abs_pos(2) << std::endl;
//		}
//
//		output.close();
//
//		return true;
//	}
//
//
//	/*
//	 * This function shifts the origin of the internal coordinates into the origin of the external
//	 * space and sets the internal basis to the unit matrix. After cleaning up, the absolute coordinates
//	 * are equal to the internal coordinates.
//	 */
//	void molecule::clean_up()
//	{
//		this->internal_origin << 0,0,0;
//		this->internal_basis << 1,0,0,
//								0,1,0,
//								0,0,1;
//	}
//
//
//	/*
//	 * This function calculates the covariance matrix of the molecule. The variance is a measure of the
//	 * spread of the atoms in the respective direction. The covariance matrix is calculated as:
//	 *
//	 * 				C_ij = 1/N * sum_k=1^N  (x_i - u_i) * (x_j - u_j)
//	 *
//	 * or in matrix form:
//	 * 							/     (x - u_x)^2      (x - u_x)*(y - u_y)  (x - u_x)*(y - u_y) \
//	 * 				C = 1/N *  |  (x - u_x)*(y - u_y)      (y - u_y)^2      (y - u_y)*(z - u_z)  |
//	 * 							\ (x - u_x)*(y - u_y)  (y - u_y)*(z - u_z)      (z - u_z)^2     /
//	 *
//	 * where u_x, u_y, u_z are the positions of the GEOMETRIC center of the molecule (i.e. the center
//	 * of geometry, see respective function above).
//	 */
//	void molecule::calc_covar_mat()
//	{
//		for (int i = 0; i < 3; i++)
//		{
//			for (int j = 0; j < 3; j++)
//			{
//				this->covar_mat(i,j) = 0.0;
//				for (std::vector<atom>::iterator atiter = this->theatoms.begin(); atiter != this->theatoms.end(); atiter++)
//				{
//					Eigen::Vector3d abs_pos = this->internal_origin + this->internal_basis * atiter->get_int_position();
//
//					this->covar_mat(i,j) += ( (abs_pos(i) - this->center_of_geometry(i))
//							                * (abs_pos(j) - this->center_of_geometry(j)) );
//				}
//				this->covar_mat(i,j) /= this->number_of_atoms;
//			}
//		}
//	}
//
//
//	/*
//	 * This function diagonalizes the covariance matrix. The eigenvectors of the covariance matrix are
//	 * the directions with the minimum and maximum variance, respectively, while the corresponding
//	 * eigenvalues are the variances along these directions. For example, the eigenvector corresponding
//	 * to the lowest variance stands perpendicular to the least squares plane of the molecule.
//	 */
//	void molecule::diag_covar_mat()
//	{
//		Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(this->covar_mat);
//
//		if (solver.info() != Eigen::Success)
//		{
//			std::cout << "The covariance matrix could not be diagonalized" << std::endl;
//		}
//		else
//		{
//			this->covar_eigval = solver.eigenvalues();
//			this->covar_eigvec = solver.eigenvectors();
//		}
//
//	}
//
//
//	/*
//	 * This function calculates the elements of the transformation matrix from the eulerian angles
//	 * describing the rotations from the external coordinate system to the internal coordinate system.
//	 * the definition of the eulerian angles is adapted from: H. Goldstein, "classical mechanics".
//	 */
//	void molecule::euler2trans()
//	{
//		this->internal_basis(0,0) =  cos(this->euler_psi) * cos(this->euler_phi)
//									 - cos(this->euler_theta) * cos(this->euler_phi) * cos(this->euler_psi);
//		this->internal_basis(0,1) =  cos(this->euler_psi) * sin(this->euler_phi)
//									 + cos(this->euler_theta) * cos(this->euler_phi) * sin(this->euler_psi);
//		this->internal_basis(0,2) =  sin(this->euler_psi) * sin(this->euler_theta);
//		this->internal_basis(1,0) = -sin(this->euler_psi) * cos(this->euler_phi)
//									 - cos(this->euler_theta) * sin(this->euler_phi) * cos(this->euler_psi);
//		this->internal_basis(1,1) = -sin(this->euler_psi) * sin(this->euler_phi)
//									 + cos(this->euler_theta) * cos(this->euler_phi) * cos(this->euler_psi);
//		this->internal_basis(1,2) =  cos(this->euler_psi) * sin(this->euler_theta);
//		this->internal_basis(2,0) =  sin(this->euler_theta) * sin(this->euler_phi);
//		this->internal_basis(2,1) = -sin(this->euler_theta) * cos(this->euler_phi);
//		this->internal_basis(2,2) =  cos(this->euler_theta);
//	}
//
//
//	void molecule::trans2euler()
//	{
//		double abs_l = sqrt(pow(this->internal_basis(2,0), 2) + pow(this->internal_basis(2,1), 2));
//
//		this->euler_theta = acos(this->internal_basis(2,2));
//		this->euler_phi = acos(-this->internal_basis(2,1)/abs_l);
//		this->euler_psi = acos((this->internal_basis(0,1)*this->internal_basis(2,0)-this->internal_basis(0,0)*this->internal_basis(2,1))/abs_l);
//	}
//}
