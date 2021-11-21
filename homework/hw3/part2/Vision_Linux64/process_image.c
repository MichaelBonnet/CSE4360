// ================================== //
// === PART 2 - TEMPLATE MATCHING === //
// ================================== //

#include <stdio.h>
#include <math.h>
#include <X11/Xlib.h>

#define DIM 512

/******************************************************************/
/* This structure contains the coordinates of a box drawn with    */
/* the left mouse button on the image window.                     */
/* roi.x     , roi.y      - left upper corner's coordinates       */
/* roi.width , roi.height - width and height of the box           */
/******************************************************************/
extern XRectangle roi;

// ======================== //
// === Helper Functions === //
// ======================== //

// Make a template with the img
void create_template(unsigned char image[DIM][DIM], double template[roi.height][roi.width])
{
    for (int i = 0; i < roi.height; i++)
    {
        for (int j = 0; j < roi.width; j++)
        {
            template[i][j] = (double)image[roi.x + i][roi.y + j];
        }
    }
}

// Get the region within the image to be convoluted with the template
void clip_region(unsigned char image[DIM][DIM], double region[roi.height][roi.width], int x, int y, int row, int col)
{
    for (int i = 0; i < roi.height; i++)
    {
        for (int j = 0; j < roi.width; j++)
        {
            if (x + i >= row || y + j >= col)
            {
                region[i][j] = 0.0;
            }
            else
            {
                region[i][j] = image[x + i][y + j];
            }
        }
    }
}

// Get the mean of the matrix
double get_mean( double matrix[roi.height][roi.width] ) 
{
    double sum = 0;
    int n = roi.height * roi.width;

    for (int i = 0; i < roi.height; i++)
    {
        for (int j = 0; j < roi.width; j++)
        {
            sum += matrix[i][j];
        }
    }

    return (sum / n);
}

// Get the standard deviation of the matrix
double get_stdev( double matrix[roi.height][roi.width] )
{
    double temp[roi.height][roi.width];
    double mean = get_mean(matrix);

    for (int i = 0; i < roi.height; i++)
    {
        for (int j = 0; j < roi.width; j++)
        {
            temp[i][j] = pow(matrix[i][j] - mean, 2);
        }
    }

    return sqrt(get_mean(temp));
}

// Convolute the region with the template
double convolute(double region[roi.height][roi.width], double template[roi.height][roi.width])
{
    double template_stdev = get_stdev(template);
    double template_mean = get_mean(template);

    double region_stdev = get_stdev(region);
    double region_mean = get_mean(region);

    double total_pixels = roi.width * roi.height;
    double sum = 0.0;

    for (int i = 0; i < roi.height; i++)
    {
        for (int j = 0; j < roi.width; j++)
        {
            sum += (region[i][j] - region_mean) * (template[i][j] - template_mean) * (1.0 / (region_stdev * template_stdev));
        }
    }

    // double res = sum / total_pixels;
    // return res;
    return (sum / total_pixels);
    return 0.0;
}

// With the template, convolute the clipped region to fill out the correlation matrix
void get_norm_cross_correlation(unsigned char image[DIM][DIM], double template[roi.height][roi.width], int row, int col, double corr_matrix[DIM][DIM])
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            double region[roi.height][roi.width];
            clip_region(image, region, i, j, row, col);
            corr_matrix[i][j] = convolute(region, template);
        }
    }
}

// Normalize the templated image
void normalize(double image[DIM][DIM], unsigned char nrml_img[DIM][DIM], int row, int col)
{
    double min = 9999.9;
    double max = -9999.9;

    // Get the minimum and the maximum
    for (int i = 0; i < row; i++)      // iterate through every row
    {
        for (int j = 0; j < col; j++)  // iterate through every column
        {
            if (image[i][j] < min)     // find the minimum
            {
                min = image[i][j];
            }
            if (image[i][j] > max)     // find the maximum
            {
                max = image[i][j];
            }
        }
    }

    // Using minimum and maximum,
    // Normalize the image to get edges
    for (int i = 0; i < row; i++)      // iterate through every row
    {
        for (int j = 0; j < col; j++)  // iterate through every column
        {
            if (min != max)
            { // match found
                // normalize value
                nrml_img[i][j] = (unsigned char)((image[i][j] - min) * 255 / (max - min));
            }
            else
            { // no match found
                nrml_img[i][j] = 0; // record a zero
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
    int row = size[0]; // total rows
    int col = size[1]; // total columns

    double template[roi.height][roi.width]; // the template to match
    create_template(image, template);

    double corr_matrix[DIM][DIM];           // pre-normalization correlation matrix
    get_norm_cross_correlation(image, template, row, col, corr_matrix);  // get the normalization matrix

    // Normalize with the correlation matrix
    normalize(corr_matrix, proc_img, row, col);
}
