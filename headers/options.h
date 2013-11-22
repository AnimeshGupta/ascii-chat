/*
 * Copyright (C) 2006 Christian Stigen Larsen, http://csl.sublevel3.org
 * Distributed under the GNU General Public License (GPL) v2.
 *
 * Project homepage on http://jp2a.sf.net
 *
 * $Id: options.h 480 2006-10-12 14:07:56Z cslsublevel3org $
 */

// see options.c
extern int verbose;
extern int auto_height;
extern int auto_width;
extern int width;
extern int height;
extern int use_border;
extern int invert;
extern int flipx;
extern int flipy;
extern int colorfill;
extern int convert_grayscale;
extern int debug;
extern int clearscr;
extern char ascii_palette[];
extern float redweight, greenweight, blueweight;
extern unsigned short int RED[256], GREEN[256], BLUE[256], GRAY[256];
extern const char *fileout;
extern int usecolors;

void parse_options(int, char **);
