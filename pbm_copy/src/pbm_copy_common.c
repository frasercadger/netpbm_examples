/*
    Copyright: Fraser Cadger (2018) <frasercadger@gmail.com>

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

    Description: contains common function definitions that may be of use
    in other examples.  
*/
#include "common.h"

/* libnetpbm include */
#include "netpbm/pam.h"

/* Standard includes */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Function defines */
bool read_pbm_image(const char *filename, struct pam *p_input)
{    
    /* Open image as regular file */
    FILE *p_input_file = fopen(filename, "r");
    if(p_input_file == NULL)
    {
        return false;
    }
    /* Read the image header from file and initialise input structure
     * This is essentially the image's metadata.
     * For more information on struct pam see: http://netpbm.sourceforge.net/doc/libnetpbm_ug.html#pamstruct
     * For documentation on pnm_readpaminit see the libnetpbm manual
     */
    pnm_readpaminit(p_input_file, p_input, PAM_STRUCT_SIZE(tuple_type));

    return true;
}

bool prepare_pbm_copy( struct pam *p_in_pbm, struct pam *p_out_pbm, const char *p_filename)
{
    /* Open output file */
    FILE *p_output_file = fopen(p_filename, "w");
    if(p_output_file == NULL)
    {
        printf("Failed to open output file\n");
        return -1;
    }

    /* Create output structure, copying in struct values
     * As we are doing a straight copy, the output file will
     * have similar metadata to the input file 
     */
    memcpy(p_out_pbm, p_in_pbm, PAM_STRUCT_SIZE(tuple_type));

    /* Set output struct's file pointer to our output file */
    p_out_pbm->file = p_output_file;
 
    /* Copy image header to output file
     */
    pnm_writepaminit(p_out_pbm);

    return true;
}
