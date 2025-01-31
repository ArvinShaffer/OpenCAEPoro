/*! \file    CornerGrid.hpp
 *  \brief   Declaration of classes related to the corner grid
 *  \author  Shizhe Li
 *  \date    Nov/16/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the OpenCAEPoro team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __CORNERGRID_HEADER__
#define __CORNERGRID_HEADER__


// Standard header files
#include <vector>
#include <math.h>
#include <stdlib.h>

// OpenCAEPoro header files
#include "OCPConst.hpp"

using namespace std;

const OCP_DBL	SMALL_REAL		= 1E-10;
const USI		MAX_NEIGHBOR	= 80;
const OCP_DBL TEENY = 1E-3;
const OCP_DBL SMALL = 1E-3;

class Point2D 
{
public:
	OCP_DBL	 x;
	OCP_DBL  y;
public:
	Point2D() = default;
	Point2D(OCP_DBL x0, OCP_DBL y0) :x(x0), y(y0) {};
};


class Point3D
{
public:
	OCP_DBL	 x;
	OCP_DBL  y;
	OCP_DBL  z;
public:
	Point3D() = default;
	Point3D(OCP_DBL x0, OCP_DBL y0, OCP_DBL z0) :x(x0), y(y0), z(z0) {};

	Point3D operator+(const Point3D& other) const;
	Point3D operator-(const Point3D& other) const;
	OCP_DBL operator*(const Point3D& other) const;
};

Point3D operator*(const Point3D& p, const OCP_DBL& a);
Point3D operator*(const OCP_DBL& a, const Point3D& p);
Point3D VectorProduct(const Point3D& p1, const Point3D& p2);


class Hexahedron
{
public:
	Point3D p0, p1, p2, p3, p4, p5, p6, p7;
};


class HexahedronFace
{
public:
	Point3D p0, p1, p2, p3;
};

class Matrix3
{
public:
	OCP_DBL M[3][3];
	Point3D operator*(const Point3D& v)const;
};


OCP_DBL VolumHexahedron(const Hexahedron& h);
Point3D CenterHexahedron(const Hexahedron& h);
Point3D VectorFace(const HexahedronFace& f);
Point3D CenterFace(const HexahedronFace& f);
Point2D CalCrossingPoint(const Point2D Line1[2], const Point2D Line2[2]);
OCP_DBL CalAreaNotQuadr(const HexahedronFace& FACE1, const HexahedronFace& FACE2);


class HalfConn {

public:
	OCP_DBL Ad_dd;
	Point3D d;
	OCP_USI neigh;
	USI directiontype;  // 1 - x, 2 - y, 3 - z, 4 - extension
};

class ConnGrid {

public:
	USI nConn, maxConn;
	vector<HalfConn> halfConn;
	void Allocate(const USI& max_neighbor);
	void AddHalfConn(const OCP_USI& n, const Point3D& area, const Point3D& d, const USI& direction);
};


class GeneralConnect {
public:
	OCP_USI begin, end;
	USI directiontype; // 1 - x, 2 - y, 3 - z, 4 - extension
	OCP_DBL Ad_dd_begin;
	OCP_DBL Ad_dd_end;
};




class COORD
{
	friend class Grid;
public:
	void Allocate(const USI& Nx, const USI& Ny, const USI& Nz);
	void InputData(const vector<OCP_DBL>& coord, const vector<OCP_DBL>& zcorn);
	bool InputCOORDDATA(const vector<OCP_DBL>& coord);
	bool InputZCORNDATA(const vector<OCP_DBL>& zcorn);
	void CalConn();

private:
	USI nx;
	USI ny;
	USI nz;
	OCP_DBL*** COORDDATA;
	OCP_DBL**** ZCORNDATA;
	Hexahedron*** cornerPoints;
	
	OCP_USI numGrid;
	OCP_USI numConn;
	OCP_USI numConnMax;
	vector<OCP_DBL> v;
	vector<OCP_DBL> depth;
	vector<OCP_DBL> dx;
	vector<OCP_DBL> dy;
	vector<OCP_DBL> dz;
	vector<Point3D> center;

	vector<GeneralConnect> connect;

};


#endif


/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Shizhe Li           Nov/16/2021      Create file                          */
/*----------------------------------------------------------------------------*/