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

    Description: copies a source .pbm file into a new .pbm file using 
    libnetpbm functions. The purpose is to show how these functions are
    used to open the original, read in the data, generate an empty pbm
    + header, then fill it with the image data.
*/

/* libnetpbm include */
#include "netpbm/pam.h"

/* Standard includes */
#include <stdio.h>
#include <string.h>

/* TODO: Constants */
static const char *p_default_out_file = "test.pbm";

/* TODO: Function prototypes */

/* TODO: main() definition */
int main(int argc, char *argv[])
{
    printf("Beginning pbm_copy\n");

    /* Initialise pm by passing the name of the program 
       Docs state pm_init() must be invoked by all programs using libnetpbm before
       handling arguments.
       See: http://netpbm.sourceforge.net/doc/libpm.html
    */
    pm_init(argv[0], 0);

    /* Get input filename from command line */
    if(argc < 2)
    {
        printf("Usage: pbm_copy input_filename [output_filename]\n");
        return -1;
    }
    malloc(strlen(argv[1]) + 1);
    char *p_in_filename = argv[1];

    /* Get optional output filename, otherwise use default */
    char *p_out_filename;
    if(argc == 3)
    {
        malloc(strlen(argv[2]) + 1);
        strcpy(p_out_filename, argv[2]);
    }
    else
    {
        p_out_filename = malloc(strlen(p_default_out_file) + 1);
        strcpy(p_out_filename, p_default_out_file);
    }

    /* Open PBM image */
    FILE *p_input_file = fopen(p_in_filename, "r");
    if(p_input_file == NULL)
    {
        printf("Failed to open file\n");
        return -1;
    }
    else
    {
        printf("Successfuly opened %s\n", p_in_filename);
    }
    /* Read the image header from file and initialise input structure */
    struct pam in_pbm;
    pnm_readpaminit(p_input_file, &in_pbm, PAM_STRUCT_SIZE(tuple_type));

    /* Prepare output file */

    /* Create output structure, copying in structs values
     * This will create the header needed for the output file. */
    struct pam out_pbm;
    memcpy(&out_pbm, &in_pbm, PAM_STRUCT_SIZE(tuple_type));

    /* Open output file */
    FILE *p_output_file = fopen(p_out_filename, "w");
    if(p_output_file == NULL)
    {
        printf("Failed to open output file\n");
    }
    out_pbm.file = p_output_file;

    /* Copy image header to output file */
    pnm_writepaminit(&out_pbm);

    /* TODO: Copy actual image */


    /* Allocate memory to hold PBM row(s) to a tuple pointer
     * tuple_row now effectively points to an 'array' of rows*/
    tuple *p_tuple_row = pnm_allocpamrow(&in_pbm);
    
    /* Copy each row of input to the output */
    for( uint32_t row = 0; row < in_pbm.height; ++row)
    {
        pnm_readpamrow(&in_pbm, p_tuple_row);
        /* Automatically 'seeks' the next row, so no need to manually increment pointer */
        pnm_writepamrow(&out_pbm, p_tuple_row);
    }

    /* Tuple pointer cleanup */
    pnm_freepamrow(p_tuple_row);
    return 0;
}

/* TODO: Function definitions */
