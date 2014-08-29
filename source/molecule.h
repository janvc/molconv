/*
 * Copyright 2014 Jan von Cosel
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


#ifndef MOLECULE_H_
#define MOLECULE_H_

#ifndef Q_MOC_RUN
	#include<chemkit/molecule.h>
	#include<chemkit/atom.h>
#endif
#include<Eigen/Core>
#include"configuration.h"

namespace molconv
{
	class Molecule : public chemkit::Molecule
	{
	public:
		Molecule();
		Molecule(const chemkit::Molecule &base_molecule);
		Molecule(const boost::shared_ptr<chemkit::Molecule> &base_mol_ptr);
		void show_inertia();
		void show_covar();
		void rotate(Eigen::Matrix3d rot_mat);
		void clean_up(const molconv::configuration &config);
	private:
		size_t number_of_atoms;				// the number of atoms in the molecule
		double total_mass;					// total mass of the molecule
		Eigen::Vector3d center_of_mass;		// the center of mass of the molecule
		Eigen::Vector3d center_of_geometry;	// the center of geometry of the molecule
		Eigen::Matrix3d inertia_tensor;		// tensor of the moments of inertia
		Eigen::Vector3d inertia_eigvals;	// the eigenvalues of the inertia tensor
		Eigen::Matrix3d inertia_eigvecs;	// the eigenvectors of the inertia tensor (the principal axes)
		Eigen::Matrix3d covar_mat;			// the covariance matrix of the molecule
		Eigen::Vector3d covar_eigvals;		// eigenvalues of the covariance matrix
		Eigen::Matrix3d covar_eigvecs;		// eigenvectors of the covariance matrix

        double euler_phi;                   //
        double euler_theta;                 // | the eulerian angles specifying the orientation of the molecule
        double euler_psi;                   // /
        Eigen::Vector3d internal_origin;    // origin of the internal coordinate system
        Eigen::Matrix3d internal_basis;     // the basis vectors of the internal coordinate system in terms
                                            // of the global coordinates
        void trans2euler();                 // calculate the eulerian angles from the transformation matrix
        void euler2trans();                 // calculate the transformation matrix from the eulerian angles

		void calc_inertia();				// calculate the tensor of moments of inertia
		void diag_inertia();				// diagonalize the inertia tensor
		void calc_covar_mat();				// calculate the covariance matrix of the molecule
		void diag_covar_mat();				// diagonalize the covariance matrix
	};
}


//#include<vector>
//#include<string>
//#include<Eigen/Core>
//#include"atom.h"
//#include"configuration.h"
//
//
//namespace molconv
//{
//	class molecule
//	{
//	public:
//		molecule(const char *input_file, configuration *config);			// read a molecular structure from an xyz file
//		void shift(Eigen::Vector3d shift_vector);	// move the molecule
//		void print_stdout();						// print the structure to std out
//		std::string get_commentline();
//		void show_info();							// print info about the molecule to stdout
//		void clean_up();							// shift the internal origin to the com and make set the internal basis to unity
//		bool write_to_file(const char *outputfile);	// write the structure to an xyz-file
//	private:
//		int number_of_atoms;				// the number of atoms in the molecule
//		std::string comment_line;			// the comment line between the atom number and the coordinates
//
//		std::vector<atom> theatoms;			// the atoms
//
//		double mass;						// the total mass of the molecule
//
//		double euler_phi;					//
//		double euler_theta;					//  | the eulerian angles specifying the orientation of the molecule
//		double euler_psi;					// /
//		Eigen::Vector3d internal_origin;	// origin of the internal coordinate system in terms of the absolute coordinates
//		Eigen::Vector3d center_of_mass;		// position of the center of mass
//		Eigen::Vector3d center_of_geometry;	// like the center of mass, but without the mass weighting
//		Eigen::Vector3d inertia_eigvals;	// the moments of inertia along the principal axes
//		Eigen::Matrix3d inertia_eigvecs;	// the eigenvectors of the inertia tensor (the principal axes)
//		Eigen::Matrix3d inertia_tensor;		// tensor of the moments of inertia
//		Eigen::Matrix3d internal_basis;		// basis of the internal coordinates
//		Eigen::Matrix3d covar_mat;			// the covariance matrix of the molecule
//		Eigen::Vector3d covar_eigval;		// eigenvalues of the covariance matrix
//		Eigen::Matrix3d covar_eigvec;		// eigenvectors of the covariance matrix
//
//		void calc_mass();					// calculate the total mass of the molecule
//		void calc_com();					// calculate the center of mass
//		void calc_cog();					// calculate the center of geometry
//		void calc_inertia();				// calculate the tensor of moments of inertia
//		void diag_inertia();				// diagonalize the tensor of inertia
//		void calc_covar_mat();				// calculate the covariance matrix of the molecule
//		void diag_covar_mat();				// diagonalize the covariance matrix
//		void euler2trans();					// calculate the transformation matrix from the eulerian angles
//		void trans2euler();					// calculate the eulerian angles from the transformation matrix
//	};
//}


#endif /* MOLECULE_H_ */
