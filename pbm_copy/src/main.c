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
    libnetpbm documentation can be found here: http://netpbm.sourceforge.net/doc/libnetpbm_image.html
*/

/* libnetpbm include */
#include "netpbm/pam.h"

/* Standard includes */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Constants */
static const char *p_default_out_file = "test.pbm";

/* TODO: Function prototypes */
static bool read_pbm_image(const char *filename, struct pam *p_input);
static bool copy_pbm_image(struct pam *p_input, struct pam *p_output);

/* Function definitions */

static bool read_pbm_image(const char *filename, struct pam *p_input)
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

static bool copy_pbm_image(struct pam *p_in_pbm, struct pam *p_out_pbm)
{
    /* Allocate memory to hold PBM rows to a tuple pointer
     * tuple_row now effectively points to an 'array' of rows*/
    tuple *p_tuple_row = pnm_allocpamrow(p_in_pbm);

    /* Copy each row of input to the output */
    for( uint32_t row = 0; row < p_in_pbm->height; ++row)
    {
        /* Read a single row of the image as a 'tuple'
         * Then write the row to the out structure */
        pnm_readpamrow(p_in_pbm, p_tuple_row);
        pnm_writepamrow(p_out_pbm, p_tuple_row);
        /* Automatically 'seeks' the next row, so no need to manually increment pointer */
    }

    /* Tuple pointer cleanup
     * Frees the memory allocated by pnm_allocpamrow */
    pnm_freepamrow(p_tuple_row);
}

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
    strcpy(p_in_filename, argv[1]);

    /* Open input file and read image metadata */
    struct pam in_pbm;
    bool read_successful = read_pbm_image(p_in_filename, &in_pbm);
    if(read_successful)
    {
        printf("Read successful\n");
        /* TODO: Print some information about the file */
    }
    else
    {
        printf("Read failed\n Terminating");
        free(p_in_filename);
        return -1;
    }
    
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

    /* Prepare output file */
    printf("Preparing output file: %s\n", p_out_filename);

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
 
    /* Copy image header to output file
     */
    printf("Writing output header\n");
    pnm_writepaminit(&out_pbm);

    printf("Preparing to copy image from %s to %s...\n", p_in_filename, p_out_filename);
    bool copy_successful = copy_pbm_image(&in_pbm, &out_pbm);
    if(copy_successful)
    {
        printf("Copy successful\n");
    }
    else
    {
        printf("Copy failed\n");
    }

    return 0;
}
