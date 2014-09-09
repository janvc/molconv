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
#include<chemkit/bondpredictor.h>
#include"molecule.h"
#include"utilities.h"


namespace molconv
{
    /*
     * This is just the default constructor that creates an empty molecule.
     */
    Molecule::Molecule()
        : chemkit::Molecule()
    {
    }


    /*
     * This constructor takes a chemkit::Molecule as argument and calculates the additional
     * stuff of the molconv::Molecule class / sets it to default values.
     */
    Molecule::Molecule(const chemkit::Molecule &base_molecule)
        : chemkit::Molecule(base_molecule)
        , number_of_atoms(this->size())
        , total_mass(this->mass())
        , center_of_mass(this->centerOfMass())
        , center_of_geometry(this->center())
        , internal_origin(Eigen::Vector3d::Zero())
        , internal_basis(Eigen::Matrix3d::Identity())
    {
        std::cout << "This is the first constructor of molconv::Molecule." << std::endl;

        chemkit::BondPredictor::predictBonds(this);

        this->calc_inertia();
        this->calc_covar_mat();

        this->diag_inertia();
        this->diag_covar_mat();
    }


    /*
     * Same as above, but with a pointer.
     */
    Molecule::Molecule(const boost::shared_ptr<chemkit::Molecule> &base_mol_ptr)
        : chemkit::Molecule(*(base_mol_ptr.get()))
        , number_of_atoms(this->size())
        , total_mass(this->mass())
        , center_of_mass(this->centerOfMass())
        , center_of_geometry(this->center())
    {
        std::cout << "This is the second constructor of molconv::Molecule." << std::endl;
        this->calc_inertia();
        this->calc_covar_mat();

        this->diag_inertia();
        this->diag_covar_mat();
    }

    /*
     * This function prints the inertia tensor as well as its eigenvalues and eigenvectors
     * to the standard output
     */
    void Molecule::show_inertia()
    {
        std::cout << "The inertia tensor:" << std::endl
                  << this->inertia_tensor << std::endl;

        std::cout << "The eigenvalues of the inertia tensor:" << std::endl
                  << this->inertia_eigvals << std::endl;

        std::cout << "The eigenvectors of the inertia tensor:" << std::endl
                  << this->inertia_eigvecs << std::endl;
    }


    /*
     * This function prints the covariance matrix as well as its eigenvalues and eigenvectors
     * to the standard output
     */
    void Molecule::show_covar()
    {
        std::cout << "The covariance_matrix:" << std::endl
                  << this->covar_mat << std::endl;

        std::cout << "The eigenvalues of the covariance matrix:" << std::endl
                  << this->covar_eigvals << std::endl;

        std::cout << "The eigenvectors of the covariance matrix:" << std::endl
                  << this->covar_eigvecs << std::endl;
    }


    /*
     * This function will rotate the molecule using a rotation matrix
     */
    void Molecule::rotate(Eigen::Matrix3d rot_mat)
    {

        std::cout << "using the matrix\n" << rot_mat << "\nto rotate the molecule.\n";

        for (size_t atiter = 0; atiter < this->number_of_atoms; atiter++)
        {
            Eigen::Vector3d new_pos = rot_mat * this->atom(atiter)->position();
            this->atom(atiter)->setPosition(new_pos);
        }
    }


    /*
     * This function will clean up the structure of the molecule, i.e. shift it
     * so that the origin of the internal basis equals the origin, and rotate
     * it so that the coordiante axes match the internal basis
     */
    void Molecule::clean_up(const molconv::configuration &config)
    {
        if (config.cleanup_wanted())
        {
            std::cout << "Cleaning up the molecule" << std::endl;
            std::cout << "shift vector:" << std::endl << this->center_of_geometry - this->internal_origin << std::endl;

            this->setCenter(this->center_of_geometry - this->internal_origin);

            this->rotate(this->internal_basis.transpose());
        }
    }

    /*
     * This function will clean up the structure of the molecule, i.e. shift it
     * so that the origin of the internal basis equals the origin, and rotate
     * it so that the coordiante axes match the internal basis
     */
    void Molecule::clean_up()
    {
        std::cout << "Cleaning up the molecule" << std::endl;
        std::cout << "shift vector:" << std::endl << this->center_of_geometry - this->internal_origin << std::endl;

        this->setCenter(this->center_of_geometry - this->internal_origin);

        this->rotate(this->internal_basis.transpose());
    }

    /*
     * This function sets the internal basis of the molecule based on the information given
     * in the configuration
     */
    void Molecule::set_intbasis(const origin orig, const basis axes, const int orig_atom, const int basis_atom1, const int basis_atom2, const int basis_atom3)
    {
        switch (orig)
        {
            case ZERO:
                int_orig_type = 0;
                this->int_orig_atom = 0;
                this->internal_origin = Eigen::Vector3d::Zero();
                break;
            case COM:
                this->int_orig_type = 1;
                this->int_orig_atom = 0;
                this->internal_origin = this->center_of_mass;
                break;
            case COG:
                this->int_orig_type = 2;
                this->int_orig_atom = 0;
                this->internal_origin = this->center_of_geometry;
                break;
            case ATOM:
                this->int_orig_type = 3;
                this->int_orig_atom = orig_atom;
                this->internal_origin = this->atom(orig_atom)->position();
                break;
            default:
                std::cerr << "Serious ERROR in Molecule::set_intbasis while setting the internal origin" << std::endl;
                break;
        }

        switch (axes)
        {
            case IDENTITY:
                this->int_basis_type = 0;
                this->int_basis_atoms.push_back(0);
                this->int_basis_atoms.push_back(0);
                this->int_basis_atoms.push_back(0);
                this->internal_basis = Eigen::Matrix3d::Identity();
                break;
            case INERT:
                this->int_basis_type = 1;
                this->int_basis_atoms.push_back(0);
                this->int_basis_atoms.push_back(0);
                this->int_basis_atoms.push_back(0);
                this->internal_basis = this->inertia_eigvecs;
                break;
            case COVAR:
                this->int_basis_type = 2;
                this->int_basis_atoms.push_back(0);
                this->int_basis_atoms.push_back(0);
                this->int_basis_atoms.push_back(0);
                this->internal_basis = this->covar_eigvecs;
                break;
            case ATOMS:
            {
                Eigen::Vector3d column1 = this->atom(basis_atom2)->position()
                                        - this->atom(basis_atom1)->position();
                column1 /= column1.norm();

                Eigen::Vector3d column2 = this->atom(basis_atom3)->position()
                                        - this->atom(basis_atom1)->position();

                column2 -= (column1.dot(column2) * column1);
                column2 /= column2.norm();

                Eigen::Vector3d column3 = column1.cross(column2);
                column3 /= column3.norm();

                this->internal_basis.col(0) = column1;
                this->internal_basis.col(1) = column2;
                this->internal_basis.col(2) = column3;
                break;
            }
            default:
                std::cerr << "Serious ERROR in Molecule::set_intbasis while setting the internal origin" << std::endl;
                break;
        }

        std::cout << "internal origin:\n" << this->internal_origin << "\ninternal basis:\n" << this->internal_basis << std::endl;
        std::cout << "product of internal basis with itself:\n" << this->internal_basis.transpose() * this->internal_basis << std::endl;
    }

    /*
     * This function sets the internal basis of the molecule based on the information given
     * in the configuration
     */
    void Molecule::set_intbasis(const configuration &config)
    {
        if (config.origin_exists() || config.axes_exist())  // settings are given
        {
            if (config.origin_exists())
            {
                switch (config.get_orig_type())
                {
                    case 1:
                        this->int_orig_type = 1;
                        this->int_orig_atom = 0;
                        this->internal_origin = this->center_of_mass;
                        break;
                    case 2:
                        this->int_orig_type = 2;
                        this->int_orig_atom = 0;
                        this->internal_origin = this->center_of_geometry;
                        break;
                    case 3:
                        this->int_orig_type = 3;
                        this->int_orig_atom = config.get_orig_atom();
                        this->internal_origin = this->atom(config.get_orig_atom())->position();
                        break;
                    default:
                        std::cerr << "Serious ERROR in Molecule::set_intbasis while setting the internal origin" << std::endl;
                        break;
                }
            }

            if (config.axes_exist())
            {
                switch (config.get_axes_type())
                {
                    case 1:
                        this->int_basis_type = 1;
                        this->int_basis_atoms.push_back(0);
                        this->int_basis_atoms.push_back(0);
                        this->int_basis_atoms.push_back(0);
                        this->internal_basis = this->inertia_eigvecs;
                        break;
                    case 2:
                        this->int_basis_type = 2;
                        this->int_basis_atoms.push_back(0);
                        this->int_basis_atoms.push_back(0);
                        this->int_basis_atoms.push_back(0);
                        this->internal_basis = this->covar_eigvecs;
                        break;
                    case 3:
                    {
                        Eigen::Vector3d column1 = this->atom(config.get_axes_atoms().at(1))->position()
                                                - this->atom(config.get_axes_atoms().at(0))->position();
                        column1 /= column1.norm();

                        Eigen::Vector3d column2 = this->atom(config.get_axes_atoms().at(2))->position()
                                                - this->atom(config.get_axes_atoms().at(0))->position();
                        column2 -= (column1.dot(column2) * column1);
                        column2 /= column2.norm();

                        Eigen::Vector3d column3 = column1.cross(column2);
                        column3 /= column3.norm();

                        this->internal_basis.col(0) = column1;
                        this->internal_basis.col(1) = column2;
                        this->internal_basis.col(2) = column3;
                        break;
                    }
                    default:
                        std::cerr << "Serious ERROR in Molecule::set_intbasis while setting the internal origin" << std::endl;
                        break;
                }
            }
        }
        else    // default: use zero as origin and identity matrix as basis vectors
        {
            this->int_orig_type = 0;
            this->int_orig_atom = 0;
            this->internal_origin = Eigen::Vector3d::Zero();
            this->int_basis_type = 0;
            this->int_basis_atoms.push_back(0);
            this->int_basis_atoms.push_back(0);
            this->int_basis_atoms.push_back(0);
            this->internal_basis = Eigen::Matrix3d::Identity();
        }

        std::cout << "internal origin:\n" << this->internal_origin << "\ninternal basis:\n" << this->internal_basis << std::endl;
        std::cout << "product of internal basis with itself:\n" << this->internal_basis.transpose() * this->internal_basis << std::endl;
    }

    /*
     * This function (re)calculates the geometric properties of the molecule.
     * It should be called e.g. after the coordinates have been changed.
     */
    void Molecule::update_geomprops()
    {
        this->center_of_mass = this->centerOfMass();
        this->center_of_geometry = this->center();

        this->calc_inertia();
        this->calc_covar_mat();

        this->diag_inertia();
        this->diag_covar_mat();
    }

    /*
     * This function calculates the inertia tensor of the molecule. The elements of the inertia tensor
     * are given by:
     *                     J_ij = sum_k=1^N  m_k * ((r_k)^2 * delta_ij) - x_i * x_j
     *
     * This results in:
     *                          / y^2 + z^2    - x * y     - x * z  \
     *                     J = |   - x * y    x^2 + z^2    - y * z   |
     *                          \  - x * z     - y * z    x^2 + y^2 /
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


    /*
     * This function calculates the covariance matrix of the molecule. The variance is a measure of the
     * spread of the atoms in the respective direction. The covariance matrix is calculated as:
     *
     *                 C_ij = 1/N * sum_k=1^N  (x_i - u_i) * (x_j - u_j)
     *
     * or in matrix form:
     *                             /     (x - u_x)^2      (x - u_x)*(y - u_y)  (x - u_x)*(y - u_y) \
     *                 C = 1/N *  |  (x - u_x)*(y - u_y)      (y - u_y)^2      (y - u_y)*(z - u_z)  |
     *                             \ (x - u_x)*(y - u_y)  (y - u_y)*(z - u_z)      (z - u_z)^2     /
     *
     * where u_x, u_y, u_z are the positions of the GEOMETRIC center of the molecule (i.e. the center
     * of geometry, see respective function above).
     */
    void Molecule::calc_covar_mat()
    {
        for (size_t i = 0; i < 3; i++)
        {
            for (size_t j = 0; j < 3; j++)
            {
                this->covar_mat(i,j) = 0.0;

                for (size_t atiter = 0; atiter < this->number_of_atoms; atiter++)
                {
                    this->covar_mat(i,j) += ((this->atom(atiter)->position()(i) - this->center_of_geometry(i))
                                           * (this->atom(atiter)->position()(j) - this->center_of_geometry(j)));
                }
                this->covar_mat(i,j) /= this->number_of_atoms;
            }
        }
    }


    /*
     * This function diagonalizes the covariance matrix. The eigenvectors of the covariance matrix are
     * the directions with the minimum and maximum variance, respectively, while the corresponding
     * eigenvalues are the variances along these directions. For example, the eigenvector corresponding
     * to the lowest variance stands perpendicular to the least squares plane of the molecule.
     */
    void Molecule::diag_covar_mat()
    {
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(this->covar_mat);

        if (solver.info() != Eigen::Success)
        {
            std::cerr << "The covariance matrix could not be diagonalized" << std::endl;
        }
        else
        {
            this->covar_eigvals = solver.eigenvalues();
            this->covar_eigvecs = solver.eigenvectors();
        }
    }



/*
 * This function calculates the elements of the transformation matrix from the eulerian angles
 * describing the rotations from the external coordinate system to the internal coordinate system.
 * the definition of the eulerian angles is adapted from: H. Goldstein, "classical mechanics".
 */
void Molecule::euler2trans()
{
    this->internal_basis(0,0) =  cos(this->euler_psi) * cos(this->euler_phi)
                                 - cos(this->euler_theta) * cos(this->euler_phi) * cos(this->euler_psi);
    this->internal_basis(0,1) =  cos(this->euler_psi) * sin(this->euler_phi)
                                 + cos(this->euler_theta) * cos(this->euler_phi) * sin(this->euler_psi);
    this->internal_basis(0,2) =  sin(this->euler_psi) * sin(this->euler_theta);
    this->internal_basis(1,0) = -sin(this->euler_psi) * cos(this->euler_phi)
                                 - cos(this->euler_theta) * sin(this->euler_phi) * cos(this->euler_psi);
    this->internal_basis(1,1) = -sin(this->euler_psi) * sin(this->euler_phi)
                                 + cos(this->euler_theta) * cos(this->euler_phi) * cos(this->euler_psi);
    this->internal_basis(1,2) =  cos(this->euler_psi) * sin(this->euler_theta);
    this->internal_basis(2,0) =  sin(this->euler_theta) * sin(this->euler_phi);
    this->internal_basis(2,1) = -sin(this->euler_theta) * cos(this->euler_phi);
    this->internal_basis(2,2) =  cos(this->euler_theta);

    this->euler_theta = 1.0;
}


/*
 * This function calculates the eulerian angles describing the rotations from the external (global) coordinate
 * system to the internal coordinate system from a given transformation matrix performing these rotations.
 */
void Molecule::trans2euler()
{
    double abs_l = sqrt(pow(this->internal_basis(2,0), 2) + pow(this->internal_basis(2,1), 2));

    this->euler_theta = acos(this->internal_basis(2,2));
    this->euler_phi = acos(-this->internal_basis(2,1)/abs_l);
    this->euler_psi = acos((this->internal_basis(0,1)*this->internal_basis(2,0)-this->internal_basis(0,0)*this->internal_basis(2,1))/abs_l);
}

}
