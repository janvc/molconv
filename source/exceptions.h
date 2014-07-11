/*
 * Copyright 2014 Sebastian Lenz
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


#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_


#ifndef Q_MOC_RUN
	#include<string>
#endif
#include<exception>

namespace molconv
{
    namespace Err
    {
		class FileNotExist : public std::exception
		{
		public:
			FileNotExist(std::string filename);
            virtual ~FileNotExist() throw();
			std::string get_filename();						// returns filename which can't be open
		protected:
			std::string filename;
		};
	}
}


#endif /* EXCEPTIONS_H_ */
