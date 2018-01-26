/*
    Copyright: Fraser Cadger (2017) <frasercadger@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Description: contains function declarations and common across all
    examples.
*/

/* libnetpbm include */
#include "netpbm/pam.h"

/* Standard includes */
#include <stdbool.h>

/* Function prototypes */
bool read_pbm_image(const char *filename, struct pam *p_input);
bool prepare_pbm_copy( struct pam *p_in_pbm, struct pam *p_out_pbm, const char *p_filename);
