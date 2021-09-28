#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.1415927
#endif

const double l0 = 0.25;
const double l1 = 0.2;
const double l2 = 0.2;
const double l3 = 0.15;

const double d1 = -0.04;
const double d2 = 0.04;
const double d3 = -0.04;
const double d4 = -0.04;


// allocating memory for a blank matrix
double** create_matrix()
{
	double** matrix = malloc(4*sizeof(double*));

	for (int i=0; i<4; i++)
	{
		matrix[i] = calloc(4,sizeof(double));
	}

	return matrix;
}

// copying a regular array style matrix over to an allocated matrix
void copy_matrix(double** target, double matrix[4][4])
{
	for (int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			target[i][j] = matrix[i][j];
		}
	}
}

// rotation matrix for X
double** create_Rx(double theta)
{
	double Rx[4][4] = 
	{
		{1, 0, 0, 0},
		{0, cos(theta), -1*sin(theta), 0},
		{0, sin(theta), cos(theta), 0},
		{0, 0, 0, 1}
	};

	double** rotmat = create_matrix();

	copy_matrix(rotmat, Rx);

	return rotmat;
}

// translation matrix for X
double** create_Dx(double length)
{
	double Dx[4][4] = 
	{
		{1, 0, 0, length},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	double** tranmat = create_matrix();

	copy_matrix(tranmat, Dx);

	return tranmat;
}

// rotation matrix for Y
double** create_Ry(double theta)
{
	double Ry[4][4] = 
	{
		{cos(theta), 0, sin(theta), 0},
		{0, 1, 0, 0},
		{-sin(theta), 0, cos(theta), 0},
		{0, 0, 0, 1}
	};

	double** rotmat = create_matrix();

	copy_matrix(rotmat, Ry);

	return rotmat;
}

// translation matrix for y
double** create_Dy(double length)
{
	double Dy[4][4] = 
	{
		{1, 0, 0, 0},
		{0, 1, 0, length},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	double** tranmat = create_matrix();

	copy_matrix(tranmat, Dy);

	return tranmat;
}

// rotation matrix for Z
double** create_Rz(double theta)
{
	double Rz[4][4] = 
	{
		{cos(theta), -1*sin(theta), 0, 0},
		{sin(theta), cos(theta), 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	double** rotmat = create_matrix();

	copy_matrix(rotmat, Rz);

	return rotmat;
}

// translation matrix for Z
double** create_Dz(double length)
{
	double Dz[4][4] = 
	{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, length},
		{0, 0, 0, 1}
	};

	double** tranmat = create_matrix();

	copy_matrix(tranmat, Dz);

	return tranmat;
}

// attempting to multiply matrices using memory allocated matrices
double** multiply_matrix(double** matrix1, double** matrix2)
{
	double** result = create_matrix();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}

	return result;
}

// forward kinematics function
void fwd_kin(double theta[6], double x[3])
{
	double** T;

	double** Rz_t0 = create_Rz(theta[0]);
	double** Dz_l0 = create_Dz(l0);
	double** Ry_t1 = create_Ry(theta[1]);
	double** Dy_d1 = create_Dy(d1);
	double** Dx_l1 = create_Dx(l1);
	double** Ry_t2 = create_Ry(theta[2]);
	double** Dy_d2 = create_Dy(d2);
	double** Dx_l2 = create_Dx(l2);
	double** Ry_t3 = create_Ry(theta[3]);
	double** Dy_d3 = create_Dy(d3);
	double** Dz_d4 = create_Dz(d4);
	double** Dx_l3 = create_Dx(l3);

	T = multiply_matrix(Rz_t0, Dz_l0);
	T = multiply_matrix(T, Ry_t1);
	T = multiply_matrix(T, Dy_d1);
	T = multiply_matrix(T, Dx_l1);
	T = multiply_matrix(T, Ry_t2);
	T = multiply_matrix(T, Dy_d2);
	T = multiply_matrix(T, Dx_l2);
	T = multiply_matrix(T, Ry_t3);
	T = multiply_matrix(T, Dy_d3);
	T = multiply_matrix(T, Dz_d4);
	T = multiply_matrix(T, Dx_l3);

	x[0] = T[0][3];
	x[1] = T[1][3];
	x[2] = T[2][3];
}


// inverse kinematics function
void inv_kin(double x[3], double theta[6])
{
	// code
}

