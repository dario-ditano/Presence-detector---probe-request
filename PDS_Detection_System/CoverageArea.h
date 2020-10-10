#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>

#include <mysqlx/xdevapi.h>

// Define Infinite (Using INT_MAX caused overflow problems) 
#define INF 10000 

/* Represent the position of an ESP */
struct Point
{
	double x;
	double y;
};
                                                                                          

/* Represent the Coverage Area of the involved ESP */
class CoverageArea
{
private:
	std::vector<Point> polygon;

	/* The function that returns true if line segment 'p1q1' and 'p2q2' intersect. */
	bool doIntersect(Point p1, Point q1, Point p2, Point q2);

	/* To find orientation of ordered triplet (p, q, r). The function returns following values:
	 0 --> p, q and r are co-linear
	 1 --> Clockwise
	 2 --> Counterclockwise */
	int orientation(Point p, Point q, Point r);

	/* Given three co-linear points p, q, r, the function checks if point q lies on line segment 'pr' */
	bool onSegment(Point p, Point q, Point r);

public:

	/*Collect the list of ESP from the DB to define the coverage area.*/
	CoverageArea();

	/* Returns true if the point p lies inside the polygon[] with n vertices */
	bool isInside(double x, double y);
};

