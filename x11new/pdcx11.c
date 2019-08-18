/* PDCurses */

#include "pdcx11.h"

#include <errno.h>
#include <stdlib.h>

/*** Functions that are called by both processes ***/

unsigned char *Xcurscr;

int XCursesLINES = 24;
int XCursesCOLS = 80;

static void _dummy_function(void)
{
}

#ifdef PDCDEBUG
void XC_say(const char *msg)
{
    PDC_LOG(("%s:%s", XCLOGMSG, msg));
}
#endif

/*** Functions that are called by the "curses" process ***/

static int _setup_curses(void)
{
    int wait_value;

    XC_LOG(("_setup_curses called\n"));

    /* Set LINES and COLS now so that the size of the shared memory
       segment can be allocated */

    XCursesLINES = SP->lines;
    LINES = XCursesLINES - SP->linesrippedoff - SP->slklines;
    XCursesCOLS = COLS = SP->cols;

    atexit(XCursesExit);

    return OK;
}

int XCursesInitscr(int argc, char *argv[])
{
    int pid, rc;

    XC_LOG(("XCursesInitscr() - called\n"));

    rc = XCursesSetupX(argc, argv);
    rc = _setup_curses();

    return rc;
}

static void _cleanup_curses_process(int rc)
{
    PDC_LOG(("%s:_cleanup_curses_process() - called: %d\n", XCLOGMSG, rc));

    if (rc)
        _exit(rc);
}

void XCursesExitCursesProcess(int rc, char *msg)
{
    PDC_LOG(("%s:XCursesExitCursesProcess() - called: %d %s\n",
             XCLOGMSG, rc, msg));

    endwin();
    _cleanup_curses_process(rc);
}

void XCursesExit(void)
{
    static bool called = FALSE;

    XC_LOG(("XCursesExit() - called\n"));

    if (FALSE == called)
    {
        XC_exit_process(0, 0, "XCursesProcess requested to exit");
        _cleanup_curses_process(0);

        called = TRUE;
    }
}
