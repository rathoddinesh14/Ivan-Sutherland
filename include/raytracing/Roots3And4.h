/*
 *  Roots3And4.h
 *
 *  Utility functions to find cubic and quartic roots,
 *  coefficients are passed like this:
 *
 *      c[0] + c[1]*x + c[2]*x^2 + c[3]*x^3 + c[4]*x^4 = 0
 *
 *  The functions return the number of non-complex roots and
 *  put the values into the s array.
 *
 *  Author:         Jochen Schwarze (schwarze@isa.de)
 *
 *  Jan 26, 1990    Version for Graphics Gems
 *  Oct 11, 1990    Fixed sign problem for negative q's in SolveQuartic
 *  	    	    (reported by Mark Podlipec),
 *  	    	    Old-style function definitions,
 *  	    	    IsZero() as a macro
 *  Nov 23, 1990    Some systems do not declare acos() and cbrt() in
 *                  <math.h>, though the functions exist in the library.
 *                  If large coefficients are used, EQN_EPS should be
 *                  reduced considerably (e.g. to 1E-30), results will be
 *                  correct but multiple roots might be reported more
 *                  than once.
 */

#include    <math.h>
#include "include/common/math_utils.h"
#ifndef M_PI
#define M_PI          3.14159265358979323846
#endif
extern double   sqrt(), cbrt(), cos(), acos();

/* epsilon surrounding for near zero values */

#define     EQN_EPS     1e-9
#define	    IsZero(x)	((x) > -EQN_EPS && (x) < EQN_EPS)

#ifdef NOCBRT
#define     cbrt(x)     ((x) > 0.0 ? pow((double)(x), 1.0/3.0) : \
                          ((x) < 0.0 ? -pow((double)-(x), 1.0/3.0) : 0.0))
#endif

/**
 * @brief Get the Plane equation
 * c[3] - x, c[2] - y, c[1] - z, c[0] - d
 * @param c - coefficients of the equation
 * @param pt - point on the plane
 * @param DC - Direction cosines
 * @return void
 */
static void GetPlane(double c[4], Vector3f pt, Vector3f DC)
{
    c[3] = DC.x;
    c[2] = DC.y;
    c[1] = DC.z;
    c[0] = -(DC.x * pt.x + DC.y * pt.y + DC.z * pt.z);
}

/**
 * @brief function to check if a point is inside the triangle
 * 
 * @param A, B, C - vertices of the triangle, P - point to be checked
 * @return bool - true if the point is inside the triangle
 */
static bool IsInsideTriangle(Vector3f A, Vector3f B, Vector3f C, Vector3f P)
{
    A = A - P;
    B = B - P;
    C = C - P;

    // u - PBC, v - PCA, w - PAB

    Vector3f u = B.Cross(C);
    Vector3f v = C.Cross(A);
    Vector3f w = A.Cross(B);

    if (u.Dot(v) < 0.0)
        return false;
    if (u.Dot(w) < 0.0)
        return false;
    
    // all normals point in the same direction
    return true;
}

/**
 * @brief isovalue of a point using bilinear interpolation
 * 
 * @param A, B, C, D - vertices of the polygon, P - point to be checked
 * @param AVal, BVal, CVal, DVal - values of the vertices
 * @return float - isovalue of the point
 */
static float GetIsoValue(Vector3f A, Vector3f B, Vector3f C, Vector3f D, float AVal, float BVal, float CVal, float DVal, Vector3f P)
{
    float R1 = ((B.x - P.x) / (B.x - A.x)) * AVal + ((P.x - A.x) / (B.x - A.x)) * BVal;
    float R2 = ((C.x - P.x) / (C.x - D.x)) * DVal + ((P.x - D.x) / (C.x - D.x)) * CVal;
    return ((P.y - B.y)/(C.y - B.y)) * R2 + ((C.y - P.y)/(C.y - B.y)) * R1;
}


static int SolveCubic(double c[4], double s[3]){
    int     i, num;
    double  sub;
    double  A, B, C;
    double  sq_A, p, q;
    double  cb_p, D;

    /* normal form: x^3 + Ax^2 + Bx + C = 0 */

    A = c[ 2 ] / c[ 3 ];
    B = c[ 1 ] / c[ 3 ];
    C = c[ 0 ] / c[ 3 ];

    /*  substitute x = y - A/3 to eliminate quadric term:
	x^3 +px + q = 0 */

    sq_A = A * A;
    p = 1.0/3 * (- 1.0/3 * sq_A + B);
    q = 1.0/2 * (2.0/27 * A * sq_A - 1.0/3 * A * B + C);

    /* use Cardano's formula */

    cb_p = p * p * p;
    D = q * q + cb_p;

    if (IsZero(D))
    {
	if (IsZero(q)) /* one triple solution */
	{
	    s[ 0 ] = 0;
	    num = 1;
	}
	else /* one single and one double solution */
	{
	    double u = cbrt(-q);
	    s[ 0 ] = 2 * u;
	    s[ 1 ] = - u;
	    num = 2;
	}
    }
    else if (D < 0) /* Casus irreducibilis: three real solutions */
    {
	double phi = 1.0/3 * acos(-q / sqrt(-cb_p));
	double t = 2 * sqrt(-p);

	s[ 0 ] =   t * cos(phi);
	s[ 1 ] = - t * cos(phi + M_PI / 3);
	s[ 2 ] = - t * cos(phi - M_PI / 3);
	num = 3;
    }
    else /* one real solution */
    {
	double sqrt_D = sqrt(D);
	double u = cbrt(sqrt_D - q);
	double v = - cbrt(sqrt_D + q);

	s[ 0 ] = u + v;
	num = 1;
    }

    /* resubstitute */

    sub = 1.0/3 * A;

    for (i = 0; i < num; ++i)
	s[ i ] -= sub;

    return num;
}

static int SolveQuadric(double c[3], double s[2]){
    double p, q, D;

    /* normal form: x^2 + px + q = 0 */

    p = c[ 1 ] / (2 * c[ 2 ]);
    q = c[ 0 ] / c[ 2 ];

    D = p * p - q;

    if (IsZero(D))
    {
	s[ 0 ] = - p;
	return 1;
    }
    else if (D < 0)
    {
	return 0;
    }
    else if (D > 0)
    {
	double sqrt_D = sqrt(D);

	s[ 0 ] =   sqrt_D - p;
	s[ 1 ] = - sqrt_D - p;
	return 2;
    }
}