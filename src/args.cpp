// Proyecto 1
// Copyright © 2021 otreblan
//
// proyecto-1 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// proyecto-1 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with proyecto-1.  If not, see <http://www.gnu.org/licenses/>.

#include <cstdlib>
#include <getopt.h>
#include <iostream>

#include "args.hpp"

void db2::args::parse(int argc, char* argv[])
{
	int c;
	static char shortopts[] = "hc:";
	static option options[] =
	{
		{"help",    no_argument,       nullptr, 'h'},
		{"command", required_argument, nullptr, 'c'},
	};

	while((c = getopt_long(argc, argv, shortopts, options, nullptr)) != -1)
	{
		switch(c)
		{
			case 'h':
				usage(EXIT_SUCCESS);
				break;

			case 'c':
				std::cout << "TODO: execute SQL expression: " << optarg << '\n';
				break;

			default:
				exit(EXIT_FAILURE);
		}
	}
}

void db2::args::usage(int exit_code)
{
	std::cout <<
		"Usage: proyecto-1 OPTION\n"
		"\t -h, --help            Show help and exit.\n"
		"\t -c, --command=COMMAND Execute SQL command.\n"
	;

	exit(exit_code);
}
