/*
    mtr  --  a network diagnostic tool
    Copyright (C) 1997,1998  Matt Kimball

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "mtr.h"
#include "display.h"
#ifdef CURSES
#include "mtr-curses.h"
#endif
#include "report.h"
#include "select.h"
#include "dns.h"
#ifdef IPINFO
#include "ipinfo.h"
#endif
#ifdef GRAPHCAIRO
#include "graphcairo-mtr.h"
#endif

extern int DisplayMode;

#ifdef SPLITMODE
#include "split.h"
#endif

void display_detect(int *argc, char ***argv) {
  DisplayMode = DisplayReport;
#ifdef CURSES
  DisplayMode = DisplayCurses;
#endif
}

void display_open(void) {
  switch(DisplayMode) {
  case DisplayReport:
    report_open();
    break;
#ifdef CURSES
  case DisplayCurses:
    mtr_curses_open();  
    break;
#endif
#ifdef SPLITMODE
  case DisplaySplit:
    split_open();
    break;
#endif
#ifdef GRAPHCAIRO
  case DisplayGraphCairo:
    if (!gc_open())
      exit(1);
    break;
#endif
  }
}

void display_close(time_t now) {
  switch(DisplayMode) {
  case DisplayReport:
    report_close();
    break;
#ifdef OUTPUT_FORMAT_TXT
  case DisplayTXT:
    txt_close();
    break;
#endif
#ifdef OUTPUT_FORMAT_XML
  case DisplayXML:
    xml_close();
    break;
#endif
#ifdef OUTPUT_FORMAT_CSV
  case DisplayCSV:
    csv_close(now);
    break;
#endif
#ifdef CURSES
  case DisplayCurses:
    mtr_curses_close();
    dns_close();
#ifdef IPINFO
    ii_close();
#endif
    break;
#endif
#ifdef SPLITMODE
  case DisplaySplit:
    split_close();
    break;
#endif
#ifdef GRAPHCAIRO
  case DisplayGraphCairo:
    gc_close();  
    break;
#endif
  }
}

void display_redraw(void) {
  switch(DisplayMode) {
#ifdef CURSES
  case DisplayCurses:
    mtr_curses_redraw();
    break;
#endif
#ifdef SPLITMODE
  case DisplaySplit:
    split_redraw();
    break;
#endif
#ifdef GRAPHCAIRO
  case DisplayGraphCairo:
    gc_redraw();
    break;
#endif
  }
}

int display_keyaction(void) {
  switch(DisplayMode) {
#ifdef CURSES
  case DisplayCurses:
    return mtr_curses_keyaction();
#endif
#ifdef SPLITMODE
  case DisplaySplit:
    return split_keyaction();
#endif
  }
  return 0;
}

void display_loop(void) {
  switch(DisplayMode) {
  case DisplayReport:
#ifdef OUTPUT_FORMAT_CSV
  case DisplayCSV:
#endif
#ifdef OUTPUT_FORMAT_RAW
  case DisplayRaw:
#endif
#ifdef OUTPUT_FORMAT_TXT
  case DisplayTXT:
#endif
#ifdef OUTPUT_FORMAT_XML
  case DisplayXML:
#endif
  case DisplaySplit:
  case DisplayCurses:
#ifdef GRAPHCAIRO
  case DisplayGraphCairo:
#endif
    select_loop();
    break;
  }
}

void display_clear(void) {
#ifdef CURSES
  if (DisplayMode == DisplayCurses)
    mtr_curses_clear();
#endif
}
