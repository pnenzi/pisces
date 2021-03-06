/*----------------------------------------------------------------------
**  Copyright 1986 by
**  The Board of Trustees of the Leland Stanford Junior University
**  All rights reserved.
**
**  This routine may not be used without the prior written consent of
**  the Board of Trustees of the Leland Stanford University.
**----------------------------------------------------------------------
**/


/*
 * Wed Jan 31 13:47:57 PST 1990 (dredge--stanford)
 *ff
 * "symbl2" : symbol drawing routine. Will draw one or more characters
 *	from a given starting point at a given angle and size. Or will
 *	draw a sigle centered symbol (for marking plot points).
 *
 * calling sequence:
 *	symbl2(x0, y0, cbuf, cnt, hsize, wsize, angle, hornt, wornt)
 *
 * where:
 *	x0	- (float) Starting point in x.
 *	y0	- (float) Starting point in y.
 *	cbuf	- (char []) String of characters to draw.
 *	cnt	- (int) Number of character is 'cbuf'
 *	hsize	- (float) Height in plot units (ie: inches) for each characer.
 *	wsize	- (float) Width in plot units (ie: inches) for each characer.
 *	angle	- (float) Angle (degrees) at which to draw the string of
 *		  characters (from x,y to the horizontally to the right
 *		  is 0 degrees).
 *	hornt	- (float) Height orientation of character string.
 *		  Range: 0 <= hornt <= 1.0
 *		  0.0 : x,y is bottom of symbol string.
 *		  1.0 : x,y is top of symbol string.
 *	wornt   - (float) Width orientation of character string.
 *		  Range: 0 <= wornt <= 1.0
 *		  0.0 : x,y is left of symbol string.
 *		  1.0 : x,y is right of symbol string.
 *
 * notes:
 *	> This routine replaces symb2(). There are now separate size params
 *	  for character height and width.
 *	> This routine makes use of 'gplot' transformation features.
 *	> This version uses character definition vectors generated by
 *	  the 'mksym' program.
 *	> Current centerable symbols are:
 *		0	- a square
 *		1	- a circle
 *		2	- a triangle
 *		3	- a plus
 *		4	- an X
 *		5	- a diamond
 *		6	- an up arrow
 *		7	- a closed top X
 *		8	- a 'Z'
 *		9	- a 'Y'
 *		10	- a curved edged square
 *		11	- an asterisk
 *		12	- an hourglass
 *		13	- an apostrophe
 *		14	- a star of david
 *		15	- a clover leaf
 *	> Giving a count less than zero will print abs(cnt) characters
 *	  but will return the pen to the starting location instead of
 *	  leaving it where it finished.  This is mostly used for drawing
 *	  single centerable symbols.
 *
 * see also:
 *	mksym(1), gplot(3), plotcap(5)
 *of
 * written: Michael Eldredge (nov 83)
 * mod # 1: Michael Eldredge (dec 85)
 *	Changed the character size parameter to separate size parameters for
 *	height and width so that those that plot microns by concentration
 *	can get centerable symbols that are "unscaled". e.g.:
 *		gscale(depth_fact, conc_fact ) ;
 *		gplot(depth, conc) ;
 *		symbl2(depth, conc, "\01", 1, 1./conc_fact, 1./depth_fact,
 *			angle, .5, .5) ;
 * modified: Michael Eldredge (mar 87) added G_MARK calls.
 * modified: Michael Eldredge (jan 90) -cnt returns to start point.
 */

#include <gplot.h>
#include <gpmarks.h>
#include "symb2.h"

static struct _symb2 *curfont = &Symb2Fonts;     /* default font set. */
static int getvects(float *x, float *y, int *pen, char cvect);


/* "symbl2": Draw the given character buffer.*/
int symbl2(double x0, double y0, char cbuf[], int cnt, double hsize, double wsize, double angle, double hornt, double wornt)
	{

 	float   xstart, ystart;       /* start of each character */
	float   xplot , yplot ;       /* points to plot, post rotate */
	float   xvect , yvect ;       /* values from vector array    */
	int     pen;
	float   xscale, yscale ;
	float   Xinc;                 /* after each character, bumb by Xinc */
	int     ipnt;                 /* vector pointer */
	int	doretn = 0 ;		/* return to the starting point? */
	int     i;

	/* start */
	/* set up */
	if (cnt == 0 || hsize <= 0.0 || wsize <= 0.0)
		return;    /* don't bother !! */

	if (cnt < 0) {
		cnt = -cnt ;
		doretn = 1 ;
		}

	/* MARK */
	gplot2(G_MARK, GM_STRING, 0., 0.) ;

	xscale  = wsize / curfont->sym_wid;	/* point width divided by size*/
	yscale  = hsize / curfont->sym_hit;	/* point width divided by size*/
	Xinc    = wsize ;			/* */

	/* set up starting location */
	xstart  = x0 - (float)cnt * wsize * wornt; /* adjust center of string */
	ystart  = y0 - hsize * hornt;	 	/* adjust center of height */

	/* easy way to rotate etc. */
	grotat(x0, y0, angle);

	/* step through each character */
	for (i=0; i<cnt; i++) {
		ipnt = curfont->sym_locs[(int) *cbuf++];  /* point to vectors */

		while (getvects(&xvect,&yvect,&pen, curfont->sym_vect[ipnt++]) )
			{
			yvect -= curfont->sym_dis;    /* for distender */

			xplot = xstart + (xvect * xscale);
			yplot = ystart + (yvect * yscale);

			gplot(xplot, yplot, pen);
			}

		xstart += Xinc;                  /* next char start loc */
		}

	/* unrotate it */
	grotat(x0, y0, -angle);

	/* if requested go back to the start */
	if (doretn) gmove(x0,y0) ;

	/* MARK */
	gplot2(G_MARK, -GM_STRING, 0., 0.) ;
	}


static int getvects(float *x, float *y, int *pen, char cvect)
	{

	int    v;

	v = cvect & 0377;        /* to stop sign extention */

	if (v == 255) return (0);   /* done, no draw */

	if (v >= 100) { *pen = G_MOVE; v -= 100; }
	else            *pen = G_DRAW;

	*x = (float) (v % 10);
	*y = (float) (v / 10);
     
	return (1);                /* got good stuff */
	}
