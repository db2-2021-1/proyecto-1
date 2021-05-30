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

#pragma once

#include <stdbool.h>

#include "tree.h"

#ifdef __cplusplus
extern "C" {
#endif

#if __GNUC__ >= 11
__attribute__ ((malloc (sql_statement_tree_free, 1)))
#endif
sql_statement_tree* parse_str(const char* str);

void parse_str_init(const char* str);
void parse_str_free(void);



#if __GNUC__ >= 11
__attribute__ ((malloc (sql_statement_tree_free, 1)))
#endif
sql_statement_tree* parse_file(const char* file);

bool parse_file_init(const char* file);
void parse_file_free(void);


#ifdef __cplusplus
} // extern "C"
#endif
