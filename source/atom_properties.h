/*
 * Copyright 2014 Jan von Cosel
 *
 * This file is part of molconv.
 *
 * molconv if free software: you can redistribute it and/or modify
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


#ifndef ATOM_PROPERTIES_H_
#define ATOM_PROPERTIES_H_


#include<string>


struct atom_properties
{
	std::string symbol;
	double mass;
};


const struct atom_properties atomprops[] =
{
		{ "H"  ,    1.00794   },
		{ "He" ,    4.002602  },
		{ "Li" ,    6.941     },
		{ "Be" ,    9.012182  },
		{ "B"  ,   10.811     },
		{ "C"  ,   12.011     },
		{ "N"  ,   14.00674   },
		{ "O"  ,   15.9994    },
		{ "F"  ,   18.9984032 },
		{ "Ne" ,   20.1797    },
		{ "Na" ,   22.989768  },
		{ "Mg" ,   24.305     },
		{ "Al" ,   26.981539  },
		{ "Si" ,   28.0855    },
		{ "P"  ,   30.973762  },
		{ "S"  ,   32.066     },
		{ "Cl" ,   35.4527    },
		{ "Ar" ,   39.948     },
		{ "K"  ,   39.0983    },
		{ "Ca" ,   40.078     },
		{ "Sc" ,   44.95591   },
		{ "Ti" ,   47.88      },
		{ "V"  ,   50.9415    },
		{ "Cr" ,   51.9961    },
		{ "Mn" ,   54.93805   },
		{ "Fe" ,   55.847     },
		{ "Co" ,   58.9332    },
		{ "Ni" ,   58.6934    },
		{ "Cu" ,   63.546     },
		{ "Zn" ,   65.39      },
		{ "Ga" ,   69.723     },
		{ "Ge" ,   72.61      },
		{ "As" ,   74.92159   },
		{ "Se" ,   78.96      },
		{ "Br" ,   79.904     },
		{ "Kr" ,   83.8       },
		{ "Rb" ,   85.4678    },
		{ "Sr" ,   87.62      },
		{ "Y"  ,   88.90585   },
		{ "Zr" ,   91.224     },
		{ "Nb" ,   92.90638   },
		{ "Mo" ,   95.94      },
		{ "Tc" ,   98.906252  },
		{ "Ru" ,  101.07      },
		{ "Rh" ,  102.9055    },
		{ "Pd" ,  106.42      },
		{ "Ag" ,  107.8682    },
		{ "Cd" ,  112.411     },
		{ "In" ,  114.818     },
		{ "Sn" ,  118.71      },
		{ "Sb" ,  121.757     },
		{ "Te" ,  127.6       },
		{ "I"  ,  126.90447   },
		{ "Xe" ,  131.29      }
};

#endif /* ATOM_PROPERTIES_H_ */
