/*----------------------------------------------------------------------
 *
 * alpha.c      - Alpha numeric utilities for skel
 *                Write/clear top line, read keyboard data.
 *
 * Copyright c 1985 The board of trustees of the Leland Stanford
 *                  Junior University. All rights reserved.
 * This subroutine may not be used outside of the SUPREM4 computer
 * program without the prior written consent of Stanford University.
 *
 * Original: CSR Feb85
 *---------------------------------------------------------------------*/

#include "alpha.h"		/* To keep definitions accurate */
#include "gplot.h"	/* For switching alpha/graphics mode. */
#include "general.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>


/*-----------------UERR-------------------------------------------------
 *You erred.
 *----------------------------------------------------------------------*/
void uerr (text)
    char *text;
{
    char ebuf[80];
    sprintf (ebuf,"*WARNING* %s\0",text);
    topl (ebuf);
    gatog();
}


/*-----------------TOPL-------------------------------------------------
 *Print something in the top-line buffer, clear if "something" is null.
 *Leaves terminal in alpha mode.
 *----------------------------------------------------------------------*/
static int 
    tpos, 	/*Current position in the buffer*/
    fresh;	/*Message has been read?        */
void topl (text)
    char *text;
{
    ggtoa();
  /*...Clear? */
    if (*text == '\0') {
	goto_ij (0, 0); 	al_cleol(); 	fflush(stdout);
	tpos = 0;		fresh = 0;
	return;
	}

  /*...If there is a message there that hasn't been read, wait. */
    if (tpos != 0 && fresh > 0) {
	goto_ij (0, tpos); printf(" --More-- "); 
	fflush (stdout);
	wait_more();
	}
    
  /*...OK write it. */
    goto_ij (0, 0); 		al_cleol();
    printf("%s",text);  	fflush (stdout);
    tpos = strlen (text); 	fresh = 1;
}

/*-----------------TOPL_UPD---------------------------------------------
 *If the stuff in the buffer is stale, erase it.
 *This should be called after every `user move', in practise after input.
 *----------------------------------------------------------------------*/
void topl_upd()
{
    if (--fresh < 0) topl("");
}

/*-----------------DREAD_TOPL-------------------------------------------
 *Read things on the top line.
 *----------------------------------------------------------------------*/
char *sread_topl (prompt)
    char *prompt;
{
#   define LLEN 80
    static char t[LLEN];	/* return pointer to static storage */
    char *c;

    /* Prompt, read string, strip eol, clean up display. */
    topl (prompt); 
    if (NULL == fgets (t, LLEN, stdin))
	{uerr ("End of input...quit."); allover();}
    for (c = t; *c != 0 && *c != '\n'; c++)
	;
    if (*c == '\n') *c = '\0';

    topl(""); 
    gatog();
    return (t);
}

double dread_topl (prompt)
    char *prompt;
{
    double t; char *s;

    s = sread_topl (prompt);
    if (sscanf(s, "%lf", &t) <= 0) 
	t = MAXFLOAT;
    return (t);
}

/*-----------------WAIT_MORE--------------------------------------------
 *Wait for the word to proceed
 *----------------------------------------------------------------------*/
void wait_more()
{
    char t[LLEN];
    fgets (t, LLEN, stdin);
}

