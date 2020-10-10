
/**
 * Copyright (c) 2019, HypnoProject
 * COVERAGE AREA
 *
 * Bonafede Giacomo
 * Ditano Dario
 * Poppa Emanuel
 *
 */

 /****************************
 * INCLUDE, DEFINE, MACRO    *
 ****************************/
/* _______________________________________________  INCLUDE  ________________________________________________________ */

#include "stdafx.h"
#include "CoverageArea.h"

/*****************************************************
* CONSTANTS, STRUCTS, GLOBAL VARIABLES AND FUNCTIONS *
******************************************************/
/* ______________________________________________  FUNCTIONS ________________________________________________________ */
/*
	Collect the list of ESP from the DB to define the coverage area.
*/
/*Collect the list of ESP from the DB to define the coverage area.*/
CoverageArea::CoverageArea()
{
	try {
		mysqlx::Session session("localhost", 33060, "pds_user", "password");                                             /* Connect with the DB */
		try {
			mysqlx::Schema myDb = session.getSchema("pds_db");                                                           /* Get DB schema */
			mysqlx::Table espTable = myDb.getTable("ESP");                                                               /* Get ESP table */

			mysqlx::RowResult esp_coordinates = espTable.select("x", "y").execute();                                     /* Get the coordinates of the ESP */

			for (mysqlx::Row row : esp_coordinates.fetchAll()) {
				Point p = { row[0], row[1] };
				polygon.push_back(p);                                                                                    /* Insert the ESP point in the CoverageArea polygon */
			}
		}
		catch (std::exception& err) {
			std::cout << "CoverageArea(): The following error occurred: " << err.what() << std::endl;
			exit(1);
		}
	}
	catch (std::exception& err) {
		std::cout << "CoverageArea(): The database session could not be opened: " << err.what() << std::endl;
		exit(1);
	}
}


/* Given three co-linear points p, q, r, the function checks if point q lies on line segment 'pr' */
bool CoverageArea::onSegment(Point p, Point q, Point r)
{
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		return true;
	return false;
}

/* In two dimensions, given an ordered set of three or more connected vertices (points) which forms a simple polygon,
 * the orientation of the resulting polygon is directly related to the sign of the angle at any vertex of the convex hull of the polygon,
 * In computations, the sign of the smaller angle formed by a pair of vectors is typically determined by the sign of the cross product of the vectors.
 * The latter one may be calculated as the sign of the determinant of their orientation matrix.
 * If the determinant is negative, then the polygon is oriented clockwise. If the determinant is positive, the polygon is oriented counterclockwise.
 * The determinant is non-zero if points A, B, and C are non-collinear.
 *
 * (https://www.geeksforgeeks.org/orientation-3-ordered-points/)
 * To find orientation of ordered triplet (p, q, r). The function returns following values:
	 0 --> p, q and r are co-linear
	 1 --> Clockwise
	 2 --> Counterclockwise */
int CoverageArea::orientation(Point p, Point q, Point r)
{
	int val = (int)((q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y));

	if (val == 0) return 0;                                                                                              /* co- linear */
	return (val > 0) ? 1 : 2;                                                                                            /* clock or counterclock wise */
}

/* https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
 * The function that returns true if line segment 'p1q1' and 'p2q2' intersect. */
bool CoverageArea::doIntersect(Point p1, Point q1, Point p2, Point q2)
{
	int o1 = orientation(p1, q1, p2);                                                                                    /* Find the four orientations needed for general and special cases */
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	/* ------- General case ------- */
	if (o1 != o2 && o3 != o4)
		return true;

	/* ------ Special Cases ------- */
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;                                                                   /* p1, q1 and p2 are co-linear and p2 lies on segment p1q1 */
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;                                                                   /* p1, q1 and p2 are co-linear and q2 lies on segment p1q1 */
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;                                                                   /* p2, q2 and p1 are co-linear and p1 lies on segment p2q2 */
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;                                                                   /* p2, q2 and q1 are co-linear and q1 lies on segment p2q2 */

	return false;                                                                                                        /* Doesn't fall in any of the above cases */
}

/* https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
 * Returns true if the point p lies inside the polygon[] with n vertices */
bool CoverageArea::isInside(double x, double y)
{
	int n = polygon.size();
	Point p = { x, y };

	if (n < 2)                                                                                                           /* if the number of vertices is less then 2, the problem is not valid. */
		return true;                                                                                                     /* Return true for debugging purposes. */

	else if (n == 2)                                                                                                     /* if the number of vertices is equal to 2, the point is surely co-linear to the segment */
		return onSegment(polygon[0], p, polygon[1]);                                                                     /* hence we only need to check whether the point is on the segment. */

	Point extreme = { INF, p.y };                                                                                        /* Create a point for line segment from p to infinite */
	int count = 0, i = 0;                                                                                                /* Count intersections of the above line with sides of polygon */

	do
	{
		int next = (i + 1) % n;                                                                                          /* Check if the line segment from 'p' to 'extreme' */
		if (doIntersect(polygon[i], polygon[next], p, extreme))                                                          /* intersects with the line segment from 'polygon[i]' to 'polygon[next]' */
		{
			if (orientation(polygon[i], p, polygon[next]) == 0)                                                          /* If the point 'p' is co-linear with line segment 'i-next', then check if it lies on segment. */
				return onSegment(polygon[i], p, polygon[next]);                                                          /* If it lies, return true, otherwise false */
			count++;
		}
		i = next;
	} while (i != 0);
	return count & 1;
}

