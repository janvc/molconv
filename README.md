molconv
=======

A tool for arranging and precisely positioning molecules in supermolecular
systems to perform electronic structure calculations.

Copyright and Warranty
----------------------

Copyright 2014 - 2019 Jan von Cosel & Sebastian Lenz

molconv is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

molconv is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

Description
-----------

The need for a tool like molconv came up when we tried to precisely arrange two
molecular structures for an electronic structure calculation in order to study
their interaction. None of the common tools was able to do the job properly so
we resorted to meticulous manual hacking of the xyz files.

We started the development of molconv to fill this gap and it also served as a
nice exercise since programming was fairly new to us.

Molconv would not have been possible without chemkit, a lightweight library for
molecular modeling and visualization, developed by Kyle Lutz, which we found on
github by accident (https://github.com/kylelutz/chemkit). This framework has
been invaluable for us, thank you very much Kyle!

Required packages for building chemkit and molconv on Ubuntu 18.04
------------------------------------------------------------------

To compile the chemkit library and molconv, you need to install the following
packages:

 g++
 cmake
 make
 libboost-system-dev
 libboost-filesystem-dev
 libboost-iostreams-dev
 libboost-program-options-dev
 libboost-thread-dev
 libboost-chrono-dev
 libeigen3-dev
 qtbase5-dev
 qttools5-dev

