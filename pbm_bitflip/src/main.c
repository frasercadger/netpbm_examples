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
static const char *p_default_out_file = "bitflipped.pbm";

/* Standard includes */
#include <stdbool.h>
#include <stdio.h>

/* Function prototypes */
void pbm_bitflip(struct pam *p_in, struct pam *p_out);

/* Function definitions */
void pbm_bitflip(struct pam *p_in, struct pam *p_out)
{
}

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
    }

    return 0;
}
