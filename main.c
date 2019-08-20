#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "sesh.h"
#include "util.h"
#include "sql.h"

extern char **environ;


void
hdr
   (cln)
     int cln;		// content length
{
  /* prints header response */

  printf("Content-Length: %d\n");
  printf("Content-Type: text/html; charset=UTF-8\n\n");
}


void
cors
   (msg)
     char *msg;			// txt output
{
  /* forms cors response from msg */

  size_t tln  =  strlen( msg );				// message length

  printf("Content-Length: %d\n", tln);
  printf("Content-Type: application/json; charset=UTF-8\n");
  printf("Access-Control-Allow-Origin: *\n\n");
  printf("%s\n\n", msg);

}


char
*msg
   (qstr, ret)
     char *qstr;		// query string
     char ret[];		// complete msg
{
  /* returns main message */


  char *txt1  =  "<html><body>";
  char *txt2  =  "<br><br><center>HELLO THERE</center>";
  char *txt3  =  "</html></body>";

  int tln     =  strlen(txt1) + strlen(txt2) + strlen(txt3);		// text lengths
  int qln     =  strlen(qstr);						// variable string length
  int odx     =  6;							// newline plus null terminator
  int rln     =  tln + qln + odx;					// total content length

  ret         =  realloc( NULL, rln );
  sprintf( ret, "%s\n%s\n%s\n%s\n\n", txt1, txt2, qstr, txt3);

  return ret;
}


void
pmsg
   (qstr, quri, postdata)
     char *qstr;				// query string
     char *quri;				// query uri
     char *postdata;				// post data
 {
  /* prints header and message */
  logfp(LFP,"again data");
  logfp(LFP, postdata);

  void
  sndmsg
     (txt)
       char *txt;
  {
     /* sends header and message to stdout */

     int cln = strlen( txt );			// content length
     hdr(cln);					// print header
     printf("%s", txt);

  }

  char **cur = "";				// sql cursor query
  char *txt  = "Don't Talk to Strangers";	// bounce msg
  char **eky   = realloc(NULL, 0);


  logfp(LFP, "uri");
  logfp(LFP, quri);

  // MAIN
  if( strcmp("main",quri) == 0 ) {
     txt = msg( quri, txt );
     sndmsg(txt);				// send main response
     return;
  }

  // NEW USER
  if( strcmp(NUSR, quri) == 0 ) {

     txt = msg(NUSR, txt);			// concat newuser message
     sndmsg(txt);				// send user response

     logfp(LFP, "atteping to parse user fields");
     cur = puser(cur, postdata);		// parse user data

     logfp(LFP, "atteping to execute sql");
     logfp(LFP, cur);

     curdom(cur);				// execute sql

     free(postdata);				// free post data

     return;
  }

  // NEW APIKY
  if( strcmp(SETK, quri) == 0 ) {
     txt = "Recieved";
     sndmsg(txt);				// send tet response

     cur = setk(cur, postdata);			// concatonate sql query

     cur = strtok(cur, "|");			// user table update
     curdom(cur);

     cur = strtok(NULL,"|");			// event table update
     curdom(cur);
  }

  // NEW EVENT
  if( strcmp(NEVT, quri) == 0 ) {

     txt = msg(NEVT, txt);                      // concatonate sql query
     sndmsg(txt);                               // send user response

     logfp(LFP, "atteping to parse event fields");
     cur = pevt(cur, postdata);			// parse user data

     logfp(LFP, cur);
     logfp(LFP, "atteping to execute sql");

     curdom(cur);

     return;
  }

  // DELETE EVENTS
  if( strcmp(DLEV, quri) == 0 ) {
     char **zid = realloc(NULL, 0);

     logfp(LFP, "deleting event");

      zid = gzid( zid, postdata );
     sndmsg("sure man");

     return;
  }

  // LIST EVENTS
  if( strcmp(LSEV, quri) == 0 ) {
     char *mjson = realloc(NULL, 0);
     logfp(LFP, "Listing Events");

     size_t pln = strlen( postdata );

     char *nrp = realloc(NULL, pln+1);

     memmove(nrp, postdata, pln);

     cur  =  lsky( cur, nrp );					// parse apiky

      //cors(cur);						// print ky
     cur = lseky( cur );					// eky from apky

     eky = curdim(cur, eky);					// return response

     mjson = lsrem(eky, mjson);
     //cors(mjson);
     sndmsg(mjson);
     return;
  }

  // RESET TABLES
  if( strcmp(DDBS, quri) == 0 ) {
     txt = "Reinitializing Tables";
     sndmsg(txt);				// send reset respose

     logfp(LFP, "Reinitializing Tables");
     reinittbls();				// reset sql tables
     return;
  }

  sndmsg(txt);					// uknown page
  return;
}



void
main
   (void)
{
  /* them things */

  char *txt = "";		// response text

  // environ
  char *raddr    =  "";
  char *qstr     =  "";
  char *mthd     =  "";
  char *quri     =  "";
  char *len      =  "";
  char *postdata =  "";
  char *len_;


  raddr  =  getenv("REMOTE_ADDR");
  qstr   =  getenv("QUERY_STRING");
  mthd   =  getenv("REQUEST_METHOD");
  quri   =  getenv("REQUEST_URI");
  len_   =  getenv("CONTENT_LENGTH");


  // load any post data

  if( len_ ) {
     len       =  strtol(len_, NULL, 10);
     postdata  =  malloc(len + 1);

     fgets(postdata, len + 1, stdin);
     logfp(LFP, "we have post data");
     logfp(LFP, postdata);
  }


  // split uri from query
  logfp(LFP, "QURI");

  strtok(quri, "?");
  quri = basename( quri );

  logfp(LFP, quri);


  if ( raddr ) {
     if( qstr ) {
       if( quri )  {

          int vld = pvars( raddr, qstr, mthd );
          if( vld ) {
               pmsg( qstr, quri, postdata);
               logfp(LFP, "[+] Looks Good");
          } else {
               pmsg( "invalid" );
               logfp(LFP, "[!] Invalid Response");
          }
       }
     }
  }

  exit(0);
  return;
}
