# Proyecto 1
# Copyright © 2021 otreblan
#
# proyecto-1 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# proyecto-1 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with proyecto-1.  If not, see <http://www.gnu.org/licenses/>.

BISON_TARGET(parser
	parser.y
	${CMAKE_CURRENT_BINARY_DIR}/parser.c
	DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
)

FLEX_TARGET(scanner
	lexer.l
	${CMAKE_CURRENT_BINARY_DIR}/lexer.c
)

ADD_FLEX_BISON_DEPENDENCY(scanner parser)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
	PRIVATE
	${BISON_parser_OUTPUTS}
	${FLEX_scanner_OUTPUTS}
		args.cpp
		main.cpp
		table.cpp
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		${FLEX_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}
)
