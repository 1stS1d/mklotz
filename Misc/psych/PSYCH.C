/* PSYCH.C   M. W. Klotz   3/04

Calculate relative humidity and dew point from wet/dry bulb psychrometer data

*/

/* head..-------------------- header files -------------------------------- */

/* Unneeded headers can be left in place - the header files consist mainly of
'defines' for the compiler and do not add to the size of the executable code.
*/

/* turns off assert macros when not debugging */
#define NDEBUG

#include <assert.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <float.h>
#include <graph.h>
#include <malloc.h> 
#include <math.h>
#include <search.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* def..----------------------- defines -------------------------------------*/

/* type synonyms */
typedef double dbl;
typedef float real;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

/*
typ	format		pointer		size	range
0	c		char		1	
1	s		char		?
2	h(d,i)		char		1	-128 : 127
3	hu		uchar		1	0 : 255
4	d,i		int		2	-32768 : 32767
5	u		uint		2	0 : 65535
6	ld,li		long		4	-2,147,483,647 : 2,147,483,648
7	lu		ulong		4	0 : 4,294,967,295
8	e,E,f,F,g,G	float (real)	4	7 digit 10^+/-38
9	l(e,E,f,F,g,G)	double (dbl)	8	15 digit 10^+/-308
*/

/* macro declarations */
#define _in stdin			/* makes redirection easier */
#define _out stdout
#define pause getch()
#define pausem getkey("press a key\n")
#define beep printf ("\a")
#define bpause beep; pause
#define bpausem beep; pausem
#define quit beep; fcloseall(); exit(1)
#define print printf("\n")
#define FALSE 0
#define TRUE !FALSE
#define cls _clearscreen(0)

/* global utility functions (can return only one value)
   note: no space between function name and parameter list
*/
#define SQR(a) ((a)*(a))
#define CUBE(a) ((a)*(a)*(a))
#define FRAC(a) ((a)-floor(a))		/* or use modf function */
	/* see also split() in C:\CCC\GOODIES.TXT */
#define MAX(a,b) ((a)>=(b) ? (a) : (b))
#define MIN(a,b) ((a)<=(b) ? (a) : (b))
#define ABS(a) ((a)>=0 ? (a) : -(a))
#define DABS(a,b) (ABS((a)-(b)))
#define SGN(a) ((a)!=0 ? ((a)>0 ? 1 : -1) : 0)
#define ATOB(a,b) (pow((a),(b)))
#define LOG(a) (log(a))
#define LOG10(a) (log10(a))
#define SIND(a) sin((a)*RPD)
#define COSD(a) cos((a)*RPD)
#define TAND(a) tan((a)*RPD)
#define ASN(a) asin((ABS(a)<1) ? (a) : SGN(a))
#define ACS(a) acos((ABS(a)<1) ? (a) : SGN(a))
#define ASND(a) DPR*asin((ABS(a)<1) ? (a) : SGN(a))
#define ACSD(a) DPR*acos((ABS(a)<1) ? (a) : SGN(a))
#define ATAND(a) DPR*atan(a)
#define ATAN2(s,c) ((s!=0||c!=0) ? atan2((s),(c)) : 0)
#define ATAN2D(s,c) DPR*((s!=0||c!=0) ? atan2((s),(c)) : 0)
#define URAND(a,b) (((b)-(a))*(float)rand()/32767.+(a))
#define RSS2(a,b) sqrt((a)*(a)+(b)*(b))
#define RSS3(a,b,c) sqrt((a)*(a)+(b)*(b)+(c)*(c))
#define RND(a) floor ((a)+.5)

/* scan codes for keys */
/* #include <keycode.h> */

/* constant declarations - all 'defines' - no executable code */
#include <constant.h>

/* -------------------------- function prototypes ------------------------- */

/*
Minimize the number of these included since they contribute to exectuable
size.  Many of the more oft-used functions are now in MWK.LIB so they can
be called from source code without the need to include the include files.
*/

/*
global utility functions (now in MWK.LIB)
void delay (clock_t wait);
char getkey (char *prompt);
void ptext (char *text, int row, int col, int fgd, int bkg);
void textscreen (void);
void tone (int frequency, long duration);
void kbdset (uint kbtog);
*/
/* #include <util.h> */

/*
generalized io functions (now in MWK.LIB)
void vin (char *prompt, void *x, char *format, dbl def, char *udef);
void vpr (char *label, void *x, char *format);
char cf (char *format);
int places (void *x, char *format);
void apr (char *label, void *x, char *format, int n1, int n2);
*/
/* #include <io.h> */

/*
simplified string tools (now in MWK.LIB)
char *sex (char *d, char *s, int p, int n);
char *sdel (char *s, char c);
char *ssi (char *s, char *p, char *r, int n);
void sdp (char *s);
*/
/* #include <str.h> */

/*
string manipulation
char *trim (char *s);
char *left (char *d, char *s, int num);
char *right (char *d, char *s, int num);
char *mid (char *d, char *s, int start, int num);
void strdelc (char *s, char c);
void strreplace (char *s, char new, char old);
char *strinsert (char *s, char insert);
char *strsub (char *s, char *pat, char *rep);
int stoint (char *s);
*/
/* #include <strings.h> */

/* data input routines */
/* #include <input.h> */

/* printing data */
/* #include <output.h> */

/* vector utilities */
/* #include <vector.h> */

/* graphics routines */
/* #include <graphics.h> */

/* file management routines */
/* #include <file.h> */

/* pmark..  functions peculiar to this program */
dbl dc (dbl f);
dbl df (dbl c);

/* gmark..-------------- global graphics variables ------------------------ */
/* some are contained in graphics.h */

/* vmark..------------ global variable declarations ----------------------- */
char fc;		// Farenheit/Celsius flag
dbl b=237.3;		// degC
dbl alt;		// altitude above sea level (m)
dbl tdb;		// measured dry bulb temperature (degC)
dbl twb;		// measured wet bulb temperature (degC)
dbl esdb;		// saturation vapor pressure at tdb (kPa)
dbl eswb;		// saturation vapor pressure at twb (kPa)
dbl ed;			// vapor pressure (kPa)
dbl p;			// air pressure (kPa)
dbl rh;			// relative humidity
dbl tdew;		// dewpoint temperature (degC)
/* smark..----------- global structure declarations ----------------------- */

/* main..================================================================== */

main (int argc, char *argv[], char *envp[] )

/* argc=count of command line arguments */
/* argv points to parameters, envp points to environment parameters */

{
int i,j,k;
char temp[4];
dbl a,t;

/* -------------------------- initializations ------------------------------ */
/* ------------------------------------------------------------------------- */

puts ("\nRELATIVE HUMIDITY FROM WET/DRY BULB PSYCHROMETER\n");

vin ("Elevation above sea level",&alt,"lf",970.,"ft"); alt*=0.3048;

puts ("Preferred temperature scale - Fahrenheit (default) or Celsius");
fc=getkey("[F], C ? "); if (fc != 'c') fc='f';

if (fc == 'f') {t=70.; strcpy (temp,"degF");} else {t=20.; strcpy (temp,"degC");}
vin ("Dry bulb temperature",&tdb,"lf",t,temp); if (fc == 'f') tdb=dc(tdb);

if (fc == 'f') {t=60.; strcpy (temp,"degF");} else {t=14.; strcpy (temp,"degC");}
vin ("Wet bulb temperature",&twb,"lf",t,temp); if (fc == 'f') twb=dc(twb);

p=101.325*exp(-0.0001184*alt);	// air pressure (kPa)

a=0.00066*(1.+0.00115*twb);	// conversion factor

esdb=exp((16.78*tdb-116.9)/(tdb+b));	// saturation vapor pressure @ tdb (kPa)
eswb=exp((16.78*twb-116.9)/(twb+b));	// saturation vapor pressure @ twb (kPa)

ed=eswb-a*p*(tdb-twb);		// vapor pressure (kPa)

rh=100.*ed/esdb;		// relative humidity (%)

printf ("Relative humidity = %.1lf %%\n",rh);

t=log(ed);
tdew=(116.9+b*t)/(16.78-t);

printf ("Dewpoint temperature = %.2lf degC = %.2lf degF\n",tdew,df(tdew));

/* post run processing */
mexit:
exit(0);			/* normal exit */
}
/* ======================================================================== */
/* dc..-------------------------------------------------------------------- */

dbl dc (dbl f)

{return (f-32.)/1.8;}
/* ------------------------------------------------------------------------ */
/* df..-------------------------------------------------------------------- */

dbl df (dbl c)

{return 1.8*c+32.;}
/* ------------------------------------------------------------------------ */
