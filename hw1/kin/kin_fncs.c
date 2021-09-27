// #include <stdio.h>
// #include <math.h>

// #ifndef M_PI
// #define M_PI 3.1415927
// #endif


// fwd_kin(theta, x)
// double *theta;
// double x[3];
// {
// }


// inv_kin(x, theta)
// double *x;
// double theta[6];
// {
// }


//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415927
#endif


double **matrix_multiplication(double mat1[][4],double mat2[][4],double result[][4]){

int i = 0;
int j = 0;
int x = 0;

for(i=0;i<4;i++)
{
	for(j=0;j<4;j++)
	{
		result[i][j] = 0.0;
		for(x=0;x<4;x++)
		{
			(*(*(result + i) + j)) += (*(*(mat1 + i ) + x)) * (*(*(mat2 + x) + j));
		}
	}
}


}



int translation(double x,double y, double z,double D[][4])
{
	double D1[4][4] = {{1,0,0,x},{0,1,0,y},{0,0,1,z},{0,0,0,1}};
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			D[i][j] = D1[i][j];
		}
	}
	return 0;
}

int rotation_x(double theta,double R[][4])
{
	double Rx[4][4] = {{1,0,0,0},{0,cos(theta),-1*sin(theta),0},{0,sin(theta),cos(theta),0},{0,0,0,1}};
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			R[i][j] = Rx[i][j];
		}
	}
	return 0;
}

int rotation_y(double theta,double R[][4])
{
	double Ry[4][4] = {{cos(theta),0,sin(theta),0},{0,1,0,0},{-1*sin(theta),0,cos(theta),0},{0,0,0,1}};
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			R[i][j] = Ry[i][j];
		}
	}
	return 0;
}

int rotation_z(double theta,double R[][4])
{
	double Rz[4][4] = {{cos(theta),-1*sin(theta),0,0},{sin(theta),cos(theta),0,0},{0,0,1,0},{0,0,0,1}};
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			R[i][j] = Rz[i][j];
		}
	}
	return 0;
}


fwd_kin(theta, x)
double *theta;
double x[3];
{

	double length[] = {0.25,0.25,0.25,0.15};

	double Dzlo[4][4]; 
	translation(0,0,length[0],Dzlo);
	double Dxl1[4][4];
	translation(length[1],0,0,Dxl1);
	double Dxl2[4][4];
	translation(length[2],0,0,Dxl2);
	double Dxl3[4][4];
	translation(length[3],0,0,Dxl3);


	double d1 = 0.05;
	double d2 = 0.05;
	double Dyd1[4][4];
	double Dzd2[4][4];
	translation(0,d1,0,Dyd1);
	translation(0,0,d2,Dzd2);

	double Rzt0[4][4];
	double Ryt1[4][4];
	double Ryt2[4][4];
	double Ryt3[4][4];
	double Rxt4[4][4];
	rotation_z(theta[0],Rzt0);
	rotation_y(theta[1],Ryt1);
	rotation_y(theta[2],Ryt2);
	rotation_y(theta[3],Ryt3);
	rotation_y(theta[4],Rxt4);

	double _3T4_1[4][4],_3T4_2[4][4],_3T4[4][4];
	matrix_multiplication(Dxl3,Rxt4,_3T4_1);
	matrix_multiplication(Dzd2,_3T4_1,_3T4_2);
	matrix_multiplication(Dyd1,_3T4_2,_3T4);

	double _2T3_1[4][4],_2T3[4][4];
	matrix_multiplication(Dxl2,Ryt3,_2T3_1);
	matrix_multiplication(Dyd1,_2T3_1,_2T3);

	double _1T2[4][4],_bT0[4][4];
	matrix_multiplication(Dxl1,Ryt2,_1T2);

	matrix_multiplication(Dzlo,Rzt0,_bT0);

	double _R1[4][4],_R2[4][4],_R3[4][4],_R[4][4];
	matrix_multiplication(_2T3,_3T4,_R1);
	matrix_multiplication(_1T2,_R1,_R2);
	matrix_multiplication(Ryt1,_R2,_R3);
	matrix_multiplication(_bT0,_R3,_R);

	x[0] = _R[0][3];
	x[1] = _R[1][3];
	x[2] = _R[2][3];


}


inv_kin(x, theta)
double *x;
double theta[6];
{

	double _X = x[0];
	double _Y = x[1];
	double _Z = x[2];

	double l0 = 0.25;
	double l1 = 0.25;
	double l2 = 0.25;
	double l3 = 0.15;

	double d1 = 0.05;
	double d2 = 0.05;

	double powX = pow(_X,2);
	double powY = pow(_Y,2);
	double powD1 = pow(d1,2);

	double a = atan(_X/_Y);
	double R = sqrt(pow(_X,2)+pow(_Y,2));
	double theta0 = acos((2*d1)/R) - a;

	if(_X<0 && _Y<0)
	{
		theta0 = theta0 - M_PI;
	}
	else if(_Y < 0)
	{
		theta0 = theta0 - M_PI;
	}

	double powl1 = pow(l1,2);
	double powl2 = pow(l2,2);

	double X1 = (_X*cos(theta0)+_Y*sin(theta0))-d2;
	double Z1 = _Z-l0+l3;
	double r = pow(X1,2)+pow(Z1,2);
	double alpha = atan(Z1/X1);
	double beta = acos((powl1-powl2+r)/(2*l1*sqrt(r)));
	double theta1 = -1 * (beta + alpha);


	double gamma = acos((powl1+powl2-r)/(2*l1*l2));
	double theta2 = M_PI - gamma;

	double phy2 = M_PI/2 + alpha; 
	double phy1 = acos((powl2-powl1+r)/(2 * l2 * sqrt(r)));

	double theta3 = ( phy2 - phy1);

	double thetaD = 0.0;
	theta[0] = theta0;
	theta[1] = theta1;
	theta[2] = theta2;
	theta[3] = theta3;
	theta[4] = thetaD;
	theta[5] = thetaD;

}