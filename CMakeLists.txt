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

cmake_minimum_required(VERSION 3.13.0)

project(proyecto-1
	VERSION 0.2.3
	DESCRIPTION "Proyecto 1"
	HOMEPAGE_URL "https://github.com/db2-2021-1/proyecto-1"
)

# Default flags
if(UNIX)
	if(NOT (DEFINED ENV{CFLAGS} OR CMAKE_C_FLAGS))
		set(CMAKE_C_FLAGS "-Wall -Wextra -g")
	endif()
	if(NOT (DEFINED ENV{CXXFLAGS} OR CMAKE_CXX_FLAGS))
		set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")
	endif()
endif()

# Without this, paths are not relative in the sources list
cmake_policy(SET CMP0076 NEW)

include(GNUInstallDirs)
include(FindPkgConfig)
include(CPack)

# Packages
find_package(FLEX)
find_package(BISON)

add_subdirectory(src) # Sources list
add_subdirectory(pkg) # Packaging

# C++ version
set_target_properties(${PROJECT_NAME}
	PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		CXX_STANDARD 17
)

# Shared libraries
pkg_check_modules(shared
	REQUIRED IMPORTED_TARGET
		readline
)

# Header only libraries
pkg_check_modules(header_only
	REQUIRED IMPORTED_TARGET
		RapidJSON
)

# Linking
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		PkgConfig::shared

	INTERFACE
		PkgConfig::header_only
)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
		RAPIDJSON_HAS_STDSTRING
)

# Install target
install(TARGETS ${PROJECT_NAME}
	DESTINATION ${CMAKE_INSTALL_BINDIR}
)
