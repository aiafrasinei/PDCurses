/*
***************************************************************************
* This file comprises part of PDCurses. PDCurses is Public Domain software.
* You may use this code for whatever purposes you desire. This software
* is provided AS IS with NO WARRANTY whatsoever.
* Should this software be used in another application, an acknowledgement
* that PDCurses code is used would be appreciated, but is not mandatory.
*
* Any changes which you make to this software which may improve or enhance
* it, should be forwarded to the current maintainer for the benefit of 
* other users.
*
* The only restriction placed on this code is that no distribution of
* modified PDCurses code be made under the PDCurses name, by anyone
* other than the current maintainer.
* 
* See the file maintain.er for details of the current maintainer.
***************************************************************************
*/
#define  CURSES_LIBRARY 1
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include <curses.h>

/* undefine any macros for functions defined in this module */
#undef   scroll
#undef   scrl
#undef   wscrl

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#endif

#ifdef PDCDEBUG
char *rcsid_scroll = "$Id: scroll.c,v 1.6 2006/01/25 14:53:23 wmcbrine Exp $";
#endif

/*man-start*********************************************************************

  Name:                                                        scroll

  Synopsis:
	int scroll(WINDOW *win);
	int scrl(int n);
	int wscrl(WINDOW *win, int n);

  X/Open Description:
	scroll() causes the window to scroll up one line.  This involves 
	moving the lines in the window data strcture.
 
	With the scrl() and wscrl() routines, for positive n scroll the 
	window up n lines (line i+n becomes i); otherwise scroll the 
	window down n lines.
 
	For these functions to work, scrolling must be enabled via 
	scrollok().
 
	Note that scrl() and scroll() may be macros.
 
	Note also that scrolling is not allowed if the supplied window 
	is a PAD.

  X/Open Return Value:
	All functions return OK on success and ERR on error.

  X/Open Errors:
	No errors are defined for this function.

  NOTE:
	The behaviour of Unix curses is to clear the line with a space
	and attributes of A_NORMAL. PDCurses clears the line with the
	window's current attributes (including current colour). To get
	the behaviour of PDCurses, #define PDCURSES_WCLR in curses.h or
	add -DPDCURSES_WCLR to the compile switches.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      scroll                                Y        Y       Y
      scrl                                  -        -      4.0
      wscrl                                 -        -      4.0

**man-end**********************************************************************/

/***********************************************************************/
#ifdef HAVE_PROTO
int PDC_CDECL scroll(WINDOW *win)
#else
int PDC_CDECL scroll(win)
WINDOW *win;
#endif
/***********************************************************************/
{
	PDC_LOG(("scroll() - called\n"));

	return wscrl(win, 1);
}

/***********************************************************************/
#ifdef HAVE_PROTO
int  PDC_CDECL wscrl(WINDOW *win, int n)
#else
int  PDC_CDECL wscrl(win, n)
WINDOW *win;
int n;
#endif
/***********************************************************************/
{
	int i, l;
	chtype blank, *ptr, *temp;

	/* Check if window scrolls. Valid for window AND pad */

	if ((win == (WINDOW *)NULL) || !win->_scroll)
		return ERR;

#if defined(PDCURSES_WCLR)
	blank = win->_blank | win->_attrs;
#else
	/* wrs (4/10/93) account for window background */

	blank = win->_bkgd;
#endif
	/* wrs -- 7/11/93 -- quick add to original scroll() routine to 
	   implement scrolling for a specified number of lines (not very 
	   efficient for more than 1 line) */

	if (n >= 0)
	{
		for (l = 0; l < n; l++) 
		{
			temp = win->_y[win->_tmarg];

			/* re-arrange line pointers */

			for (i = win->_tmarg; i < win->_bmarg; i++)
			{
				win->_y[i] = win->_y[i + 1];
				win->_firstch[i] = 0;
				win->_lastch[i] = win->_maxx - 1;
			}

			/* make a blank line */

			for (ptr = temp; (ptr - temp) < win->_maxx; ptr++)
				*ptr = blank;

			win->_y[win->_bmarg] = temp;
			win->_firstch[win->_bmarg] = 0;
			win->_lastch[win->_bmarg] = win->_maxx - 1;
		}
	}
	else 
	{
		for (l = n; l < 0; l++)
		{
			temp = win->_y[win->_bmarg];

			/* re-arrange line pointers */

			for (i = win->_bmarg; i > win->_tmarg; i--)
			{
				win->_y[i] = win->_y[i - 1];
				win->_firstch[i] = 0;
				win->_lastch[i] = win->_maxx - 1;
			}

			/* make a blank line */

			for (ptr = temp; (ptr - temp) < win->_maxx; ptr++)
				*ptr = blank;

			win->_y[win->_tmarg] = temp;
			win->_firstch[win->_tmarg] = 0;
			win->_lastch[win->_tmarg] = win->_maxx - 1;
		}
	}

	PDC_sync(win);
	return OK;
}

/***********************************************************************/
#ifdef HAVE_PROTO
int PDC_CDECL scrl(int n)
#else
int PDC_CDECL scrl(n)
int n;
#endif
/***********************************************************************/
{
	PDC_LOG(("scrl() - called\n"));

	return wscrl(stdscr, n);
}
