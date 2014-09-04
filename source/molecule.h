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
        Molecule();                                                         // the default constructor
        Molecule(const chemkit::Molecule &base_molecule);                   // take a base molecule
        Molecule(const boost::shared_ptr<chemkit::Molecule> &base_mol_ptr); // same as above, but with pointer

        void show_inertia();                                    // show the inertia tensor
        void show_covar();                                      // show the covariance matrix
        void rotate(Eigen::Matrix3d rot_mat);                   // rotate the molecule about the origin with a rotation matrix
        void clean_up(const molconv::configuration &config);    // clean up the coordinates of the molecule
        void set_intbasis(const molconv::configuration &config);
    private:
        size_t number_of_atoms;             // the number of atoms in the molecule
        double total_mass;                  // total mass of the molecule
        Eigen::Vector3d center_of_mass;     // the center of mass of the molecule
        Eigen::Vector3d center_of_geometry; // the center of geometry of the molecule
        Eigen::Matrix3d inertia_tensor;     // tensor of the moments of inertia
        Eigen::Vector3d inertia_eigvals;    // the eigenvalues of the inertia tensor
        Eigen::Matrix3d inertia_eigvecs;    // the eigenvectors of the inertia tensor (the principal axes)
        Eigen::Matrix3d covar_mat;          // the covariance matrix of the molecule
        Eigen::Vector3d covar_eigvals;      // eigenvalues of the covariance matrix
        Eigen::Matrix3d covar_eigvecs;      // eigenvectors of the covariance matrix

        double euler_phi;                   //
        double euler_theta;                 // | the eulerian angles specifying the orientation of the molecule
        double euler_psi;                   // /
        Eigen::Vector3d internal_origin;    // origin of the internal coordinate system
        Eigen::Matrix3d internal_basis;     // the basis vectors of the internal coordinate system in terms
        size_t int_orig_type;               // internal origin: 1=com, 2=cog, 3=atom
        size_t int_orig_atom;               // atom that defines the internal origin (if set)
        size_t int_basis_type;              // internal basis: 1=inert, 2=covar, 3=atoms
        std::vector<size_t> int_basis_atoms;// atoms that define the internal basis (if set)
                                            // of the global coordinates
        void trans2euler();                 // calculate the eulerian angles from the transformation matrix
        void euler2trans();                 // calculate the transformation matrix from the eulerian angles

        void calc_inertia();    // calculate the tensor of moments of inertia
        void diag_inertia();    // diagonalize the inertia tensor
        void calc_covar_mat();  // calculate the covariance matrix of the molecule
        void diag_covar_mat();  // diagonalize the covariance matrix
        void update_geomprops();// update the geometrical properties (i.e. after an atomic coordinate change)
    };
}

#endif /* MOLECULE_H_ */
