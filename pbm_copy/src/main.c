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

/* Constants */
static const char *p_default_out_file = "test.pbm";

/* TODO: Function prototypes */

/* Function definitions */

int main(int argc, char *argv[])
{
    printf("Beginning pbm_copy\n");

    /* Initialise pm by passing the name of the program
     * Docs state pm_init() must be invoked by all programs using libnetpbm before
     * handling arguments.
     * See: http://netpbm.sourceforge.net/doc/libpm.html
    */
    pm_init(argv[0], 0);

    /* Get input filename from command line */
    if(argc < 2)
    {
        printf("Usage: pbm_copy input_filename [output_filename]\n");
        return -1;
    }
    char *p_in_filename = malloc(strlen(argv[1]) + 1);
    p_in_filename = argv[1];

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
    /* Read the image header from file and initialise input structure
     * This is essentially the image's metadata.
     * For more information on struct pam see: http://netpbm.sourceforge.net/doc/libnetpbm_ug.html#pamstruct
     * For documentation on pnm_readpaminit see the libnetpbm manual
     */
    struct pam in_pbm;
    pnm_readpaminit(p_input_file, &in_pbm, PAM_STRUCT_SIZE(tuple_type));

    /* TODO: Print some information about the file */

    /* Prepare output file */
    printf("Preparing output file\n");

    /* Create output structure, copying in struct values
     * As we are doing a straight copy, the output file will
     * have similar metadata to the input file 
     */
    struct pam out_pbm;
    memcpy(&out_pbm, &in_pbm, PAM_STRUCT_SIZE(tuple_type));

    /* Open output file */
    FILE *p_output_file = fopen(p_out_filename, "w");
    if(p_output_file == NULL)
    {
        printf("Failed to open output file\n");
    }

    /* Set output struct's file pointer to our output file */
    out_pbm.file = p_output_file;

    printf("Writing output header\n");

    /* Copy image header to output file
     */
    pnm_writepaminit(&out_pbm);

    /* Allocate memory to hold PBM rows to a tuple pointer
     * tuple_row now effectively points to an 'array' of rows*/
    tuple *p_tuple_row = pnm_allocpamrow(&in_pbm);

    printf("Preparing to copy image from %s to %s...\n", p_in_filename, p_out_filename);

    /* Copy each row of input to the output */
    for( uint32_t row = 0; row < in_pbm.height; ++row)
    {
        /* Read a single row of the image as a 'tuple'
         * Then write the row to the out structure */
        pnm_readpamrow(&in_pbm, p_tuple_row);
        pnm_writepamrow(&out_pbm, p_tuple_row);
        /* Automatically 'seeks' the next row, so no need to manually increment pointer */
    }

    printf("Copy complete\nPerforming cleanup...\n");

    /* Tuple pointer cleanup
     * Frees the memory allocated by pnm_allocpamrow */
    pnm_freepamrow(p_tuple_row);

    /* Free memory allocated to filename pointers */
    free(p_in_filename);
    free(p_out_filename);

    return 0;
}

