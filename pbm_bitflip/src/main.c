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

    Description: simple variation on pbm_copy, copies a pbm file's metadata
    and image data, and then flips every bit of the image data; i.e. if a
    bit is 0 it will be set to 1, and if it is 1 it will be set to 0. The
    purpose is to build on pbm_copy, by manipulating the image itself.
    Flipping a bit is one of the simplest ways to modify an image (or any data).
    The end result will be a noticeably different output image.
*/

#include "common.h"

/* libnetpbm include */
#include "netpbm/pam.h"

/* Constants */
static const char *p_out_filename = "bitflipped.pbm";

/* Standard includes */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Function prototypes */
void bitflip_tupplerow(tuple *p_tuple_row, const struct pam *p_in);
void pbm_bitflip(struct pam *p_in, struct pam *p_out);

/* Function definitions */

void bitflip_tupplerow(tuple *p_tuple_row, const struct pam *p_in)
{
    /* Outer loop, iterate through all tuples in a row */
    for(int i = 0; i < p_in->width; ++i)
    {
        /* Inner loop, iterate through all samples in a tuple */
        for (int j = 0; j < p_in->depth; ++j)
        {
            /* Perform bitflipping on indivudal sample */

            /* Find out number of relevant bits */
            uint8_t sample_bits;
            /* PBM (raw or otherwise) is monochrome, so uses only 1 bit (0 or 1).
             * Therefore, although nominally 1 byte is used, only 1 bit of it is
             * relevant
             */
            if(p_in->format == PBM_FORMAT || p_in->format == RPBM_FORMAT)
            {
                sample_bits = 1;
            }
            else
            {
                /* For all other formats, we're safe to flip every bit in a
                 * relevant byte
                 * */
                sample_bits = p_in->bytes_per_sample * 8;
            }
            /* Flip each relevant bit one at a time */
            for(int k = 0; k < sample_bits ; ++k)
            {
                    
                p_tuple_row[i][j] ^= 1 << k;
            }  
        }
    }

}

void pbm_bitflip(struct pam *p_in, struct pam *p_out)
{
    /* In order to bitflip , we need to know the following:
     * How many rows the image contains (height). How many tuples/columns
     * per row (width). Number of samples per row (depth). And finally the
     * number of bytes per sample.
     * Fortunately all of the above is contained in struc pam:
     * http://netpbm.sourceforge.net/doc/libnetpbm_ug.html#pamstruct
     */

    /* Allocate memory to hold PBM rows to a tuple pointer
     * tuple_row now effectively points to an 'array' of rows*/
    tuple *p_tuple_row = pnm_allocpamrow(p_in);

    /* Copy each row of input to the output */
    for( uint32_t row = 0; row < p_in->height; ++row)
    {
        /* Read a single row of the image as a 'tuple' */
        pnm_readpamrow(p_in, p_tuple_row);

        /* Pass row off to function that does bitflipping */
        bitflip_tupplerow(p_tuple_row, p_in);

        /* Write modified tuple row to output image */
        pnm_writepamrow(p_out, p_tuple_row);
    }

    /* Tuple pointer cleanup
     * Frees the memory allocated by pnm_allocpamrow */
    pnm_freepamrow(p_tuple_row);
}

/* A lot of the main() code from pbm_copy can be reused here for opening the input file and
 * prepping the output file. In this example the outputfile name is no longer a parameter.*/
int main(int argc, char *argv[])
{
    printf("Beginning pbm_bitflip\n");

    /* Initialise pm by passing the name of the program
     * Docs state pm_init() must be invoked by all programs using libnetpbm before
     * handling arguments.
     * See: http://netpbm.sourceforge.net/doc/libpm.html
    */
    pm_init(argv[0], 0);

    /* Get input filename from command line */
    if(argc < 2)
    {
        printf("Usage: pbm_bitflip input_filename\n");
        return -1;
    }
    char *p_in_filename = malloc(strlen(argv[1]) + 1);
    strcpy(p_in_filename, argv[1]);
    /* Open input file and read image metadata */
    printf("Reading pbm image from: %s\n", p_in_filename);
    struct pam in_pbm;
    bool read_successful = read_pbm_image(p_in_filename, &in_pbm);
    if(!read_successful)
    {
        printf("Read failed");
    }
    else
    {
        printf("Read successful\n");
        printf("Image height: %d, width: %d, depth: %d\n", in_pbm.height, in_pbm.width, in_pbm.depth);

        /* Prepare output file */
        printf("Preparing output file: %s\n", p_out_filename);
        struct pam out_pbm;
        bool prep_successful = prepare_pbm_copy(&in_pbm, &out_pbm, p_out_filename);

        if(!prep_successful)
        {
            printf("Output preparation failed\n");
        }
        else
        {
            printf("Output preparation successful\n");
            /* Do bitflipping */
            pbm_bitflip(&in_pbm, &out_pbm);
        }   
    }

    return 0;
}
