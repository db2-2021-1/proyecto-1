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

find_package(GTest)

enable_testing()

set(TEST_BIN ${PROJECT_NAME}-test)

add_executable(${TEST_BIN}
	main.cpp
)

add_test(NAME ${TEST_BIN} COMMAND ${TEST_BIN})

# Shared libraries
pkg_check_modules(gtest_lib
	REQUIRED IMPORTED_TARGET
		gtest
)

target_link_libraries(${TEST_BIN}
	PUBLIC
		PkgConfig::gtest_lib
)
