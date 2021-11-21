// =============================== //
// === PART 1 - EDGE DETECTION === //
// =============================== //

#include <stdio.h>
#include <math.h>
#include <X11/Xlib.h>

#define DIM 512

/******************************************************************/
/* This structure contains the coordinates of a box drawn with    */
/* the left mouse button on the image window.                     */
/* roi.x , roi.y          - left upper corner's coordinates       */
/* roi.width , roi.height - width and height of the box           */
/******************************************************************/

extern XRectangle roi;

// ======================== //
// === Helper Functions === //
// ======================== //

// Apply edge filter to image
void apply_edge_filter(int filter[3][3], unsigned char img[DIM][DIM], int filter_img[DIM][DIM], int row, int col)
{
    int p_i, p_j;
    int v_i, v_j;

    for (int i = 0; i < row - 2; i++)      // iterate through every row
    {
        for (int j = 0; j < col - 2; j++)  // iterate through every column
        {
            filter_img[i][j] = 0;
            for (p_i = i, v_i = 0; v_i < 3; p_i++, v_i++)
            {
                for (p_j = j, v_j = 0; v_j < 3; p_j++, v_j++)
                {
                    filter_img[i][j] += img[p_i][p_j] * filter[v_i][v_j];
                }
            }
        }
    }
}

// Normalize the filtered image
void normalize(int filter_img[DIM][DIM], unsigned char proc_img[DIM][DIM], int row, int col)
{
    int min = 999;
    int max = -999;

    // Get the minimum and the maximum
    for (int i = 0; i < row; i++)        // iterate through each row
    {
        for (int j = 0; j < col; j++)    // iterate through each column
        {
            if (filter_img[i][j] < min)  // find minimum
            {
                min = filter_img[i][j];
            }
            if (filter_img[i][j] > max)  // find maximum
            {
                max = filter_img[i][j];
            }
        }
    }

    // Using minimum and maximum,
    // Normalize the image to get edges
    for (int i = 0; i < row; i++)        // iterate through each row
    {
        for (int j = 0; j < col; j++)    // iterate through each column
        {
            if (min != max)
            { // edge found
                // normalize image
                proc_img[i][j] = (unsigned char)((filter_img[i][j] - min) * 255 / (max - min));
            }
            else
            { // no edge found
                proc_img[i][j] = 0; // record a zero for no edges
            }
        }
    }
}


/******************************************************************/
/* Main processing routine. This is called upon pressing the      */
/* Process button of the interface.                               */
/* image      - the original greyscale image                      */
/* size       - the actual size of the image                      */
/* proc_image - the image representation resulting from the       */
/*              processing. This will be displayed upon return    */
/*              from this function.                               */
/******************************************************************/

void process_image(unsigned char image[DIM][DIM], int size[2], unsigned char proc_img[DIM][DIM])
{
    int row = size[0];
    int col = size[1];
    int filter_img[DIM][DIM];

    int horizontal_filter[3][3] = {  { -1, -2, -1},
                                     {  0,  0,  0},
                                     {  1,  2,  1}  };

    int vertical_filter[3][3]   = {  { -1,  0,  1},
                                     { -2,  0,  2},
                                     { -1,  0,  1}  };

    // Uncomment whichever one is being done then remake
    // apply_edge_filter(horizontal_filter, image, filter_img, row, col);
    apply_edge_filter(vertical_filter, image, filter_img, row, col);

    // Normalize with the edge filtering
    normalize(filter_img, proc_img, row - 2, col - 2);
}
