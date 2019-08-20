#include <stdio.h>

#include "config.h"
#include "util.h"

// ptr[0] will hold the total number of elements
struct array {
   int  pnt;                            // pointer on arr from 1 = ele[len]
   int  psz;                            // length of element
   int  pos;                            // starting position on origional array
}; struct array splt[1024];             // return element + max_size




char
*tdate
   (date)
     char date[];
{
  /* returns todays date */

  int hours, minutes, seconds, day, month, year;

  time_t now;

  time(&now);
  struct tm *local = localtime(&now);

  day    =  local->tm_mday;             // get day of month (1 to 31)
  month  =  local->tm_mon + 1;          // get month of year (0 to 11)
  year   =  local->tm_year + 1900;      // get year since 1900

  sprintf(date, "%02d/%02d/%d", month, day, year);

  return date;
}


short
pvars
   ( raddr, qstr, mthd)
     char *raddr;	// server address
     char *qstr;	// query string
     char *mthd;	// request method
{
  /* parses given eniron variables with expected, return bool */

  //if( strcmp(xarp, raddr) != 0 ) return 0;
  logfp(LFP, "strings");
  logfp(LFP, XSTR);
  logfp(LFP, qstr);

  if( strcmp(XSTR,  qstr) != 0 ) {
     if( strcmp(RSTR,  qstr) != 0 ) {
       logfp(LFP, "bad qstring");
       return 0;
     }
  }

  if( strcmp(XMTH,  mthd) != 0 ) {
//     if( strcmp(mthd,  "OPTIONS" ) != 0 ) {
       logfp(LFP, "bad method");
       logfp(LFP, mthd);
       return 0;
//     }
  }

  return 1;
}


char
**newdom
   (cur, tbl, usr, pwd, dom, eky, paid, aky, xtr, dte, sft, hrd)
    char **cur;                         // sql cursor query
     char *tbl;                         // table name
     char *usr;                         // user name
     char *pwd;                         // encrypted pwd
     char *dom;                         // domain name
     char *eky;				// email apiky
     char *paid;			// paid
     char *aky;                         // apikey
     char *xtr;                         // extra domains
     char *dte;                         // todays date
       int sft;                         // soft limmit - (current daily)
       int hrd;                         // hard limmit - (daily max)
{
  /* Generate NewDomain Cursor Query */

  size_t tln  =  strlen( tbl );
  size_t uln  =  strlen( usr );
  size_t pln  =  strlen( pwd );
  size_t dml  =  strlen( dom );
  size_t eln  =  strlen( eky );
  size_t kln  =  strlen( aky );
  size_t xln  =  strlen( xtr );
  size_t dln  =  strlen( dte );
  size_t mln  =  strlen( paid );


  size_t ttl = tln +uln +pln +dml +eln +kln +xln +dln +154;
  logfp(LFP, "reallocating cur");
  cur = realloc(NULL, ttl);

  sprintf(cur, "INSERT INTO %s VALUES('%s', '%s', '%s', '%s', '%s', '%s', %d, %d, '%s', '%s');", tbl, usr, pwd, dom, eky, paid, aky, sft, hrd, xtr, dte);

  logfp(LFP, "RETURNING CUR NEWDOM");
  //logfp(LFP, cur);
  return cur;
}



char
**newuser
    (cur, usr, pwd, dom, eky, paid, aky)
      char **cur;
      char *usr;
      char *pwd;
      char *dom;
      char *eky;
      char *paid;
      char *aky;
{
  /* initializes con, statment & adds user */

  char *xtr = "None";
  int sft   = 100;		// soft limmit
  int hrd   = 100;		// hard limmit


  char dte[12];			// time
  tdate(dte);

  logfp(LFP, "Adding New User");
  cur = newdom( cur, ZDMZ, usr, pwd, dom, eky, paid, aky, xtr, dte, sft, hrd);
  logfp(LFP, "RETURNING CUR NEWUSR");
  logfp(LFP, cur);

  return cur;
}





char
**puser
   (cur, wrds)
     char **cur;
     char *wrds;
{
  /* parse line for key val */

  char *user;		// user
  char *pwd;		// passwd
  char *dom;		// domain
  char *eky;		// email api ky
  char *paid;		// paid
  char *apk;		// webpage api ky

  logfp(LFP, "POST BUFFER");
  //logfp(LFP, wrds);

  void
  something
     (buf)
       char* buf;
  {
     /* fill key values from line of post json */
     // TODO: split then cmp so keys can also be values
     size_t tl;
     char *tik;
     logfp(LFP, "Toke");
     logfp(LFP, buf);
     //printf("buff %s\n",buf);
     if( strstr(buf, "user") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
//       logfp(LFP, tik);
       tl = strlen(tik);
       user = realloc(NULL, tl);
       memmove( user, tik, tl);
       memmove( user+tl, "\0", 1);

       logfp(LFP, "user");
       logfp(LFP, user);
     }

     if( strstr(buf, "pwd") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
//       logfp(LFP, tik);
       tl = strlen(tik);
       pwd = realloc(NULL, tl);
       memmove( pwd, tik, tl);
       memmove( pwd+tl, "\0", 1);

       logfp(LFP, "pwd");
       logfp(LFP, pwd);
     }

     if( strstr(buf, "domain") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       dom = realloc(NULL, tl);
       memmove( dom, tik, tl);
       memmove( dom+tl, "\0", 1);

       logfp(LFP, "dom");
       logfp(LFP, dom);
     }

     if( strstr(buf, "eky") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       eky = realloc(NULL, tl);
       memmove( eky, tik, tl);
       memmove( eky+tl, "\0", 1);

       logfp(LFP, "EKY");
       logfp(LFP, eky);
     }

     if( strstr(buf, "paid") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
//       logfp(LFP, tik);
       tl = strlen(tik);
       paid = realloc(NULL, tl);
       memmove( paid, tik, tl);
       memmove( paid+tl, "\0", 1);

       logfp(LFP, "paid");
       logfp(LFP, paid);
     }

     if( strstr(buf, "apky") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
//       logfp(LFP, tik);
       tl = strlen(tik);
       apk = realloc(NULL, tl);
       memmove( apk, tik, tl);
       memmove( apk+tl, "\0", 1);

       logfp(LFP, "apk");
       logfp(LFP, apk);
     }

     return;
  }


  /* main */
  char  *lim = ",";
  int   prog = something;       // function to pass to element(s)
  logfp(LFP, "splitting words");
  loadar(wrds, lim);            // in struct array

  int sze = splt[0].pnt-1;
  int i=1;

  while(i<=sze) {
     char *tik = realloc(NULL, 0);
     char *tok=NULL;
     tok = realloc(NULL ,sizeof(tok));                  // array element

     int tsz = splt[i].psz;             // token size
     int pos = splt[i].pos;             // array position
     int pnt = splt[i].pnt;             // buffer position

     tok = realloc(tok, tsz);

     memmove(tok, wrds+pos, tsz);
     memmove(tok+tsz, "\0", 1);

    // printf("size %d\nposition %d\ntoken %s\n", tsz,pos,tok);
     something(tok);

     memset(tok, '\0', sizeof(tok));
     // logfp(LFP, tok);
     free(tok);
     i++;
  }


  logfp(LFP, "words split");
/*  if( usr ) {
     if( pwd ) {
       if( dom ) {
          if( apk ) {
            if( eky ) {
               if( paid ) { */
                 // if all forms filled create user

                 cur = newuser(cur, user, pwd, dom, eky, paid, apk);
                 logfp(LFP, "RETURNING CUR PUSER");
                 logfp(LFP, cur);

                 return cur;
/*               }
            }
          }
       }
     }
  } */

  // missed some thing
/*  char *err = realloc(NULL,0);
  size_t lz = strlen( paid ) + strlen( eky ) + strlen( apk ) + strlen( dom ) + strlen( pwd ) + strlen( usr ) + 90;

  err = realloc(err, lz);
  sprintf(err, "usr %s : paid %s : eky %s : aky  %s : dom %s : pwd %s", usr, paid, eky, apk, dom);
  logfp(LFP, err); */

  return "error";
}


char
**lsev
   (cur, wrds)
     char **cur;
     char *wrds;
{
  /* List Events By Apiky */


  return cur;
}


char
**gzid
   (cur, wrds)
     char **cur;
     char *wrds;
{
  /* Parse Apiky From Post Data */
  char *cr   =  realloc(NULL, 0);
  char *ur   =  realloc(NULL, 0);


  logfp(LFP, "Fuck Parsing Delete Buffers");
  logfp(LFP, wrds);
  void
  pbuf
     (buf)
       char* buf;
  {
     /* fill key values from line of post json */
     // TODO: split then cmp so keys can also be values

     //printf("buff %s\n",buf);
     logfp(LFP, "TOKS");
     logfp(LFP, buf);

     char *tik=NULL;
     size_t tl;

     if( strstr(buf, "apk") ) {
       logfp(LFP,"found apk");
       char *tok = realloc(NULL,0);
       tok  = realloc(tik, strlen( buf ) +1);
       memmove(tok, buf, strlen(buf));
       tik  =  strtok(tok, "\"");
       tik  =  strtok(NULL,"\"");
       tik  =  strtok(NULL,"\"");
       tik  =  strtok(NULL,"\"");

       tl   =  strlen(tik);
       ur  =  realloc(NULL, tl);

       memmove( ur, tik, tl);
       memmove( ur+tl, "\0", 1);

       logfp(LFP, "apk:");
       logfp(LFP, ur);
     }



     if( strstr(buf, "rid") ) {
       logfp(LFP,"found rid");

//       tl   = strlen( buf );
//       tik  =  realloc(NULL, buf+1);
//       memmove(tik, buf, tl);

       tik  =  strtok(buf, "\"");
       tik  =  strtok(NULL,"\"");
       tik  =  strtok(NULL,"\"");
       tik  =  strtok(NULL,"\"");
       tik  =  strtok(NULL,"\"");
      tik  =  strtok(NULL,"\"");
       tik  =  strtok(NULL,"\"");
       tik  =  strtok(NULL,"\"");


       tl   =  strlen(tik);
       cr  =  realloc(NULL, tl);

       memmove( cr, tik, tl);
       memmove( cr+tl, "\0", 1);

       logfp(LFP, "zid:");
       logfp(LFP, cr);
     }

  }
  pbuf(wrds);
  size_t tln = strlen( ur ) + strlen( cr ) + 2;
  cur = realloc(NULL, tln);
  sprintf(cur, "%s,%s", cr, ur);
  return cur;
}



char
**lsky
   (cur, wrds)
     char **cur;
     char *wrds;
{
  /* Parse Apiky From Post Data */
  char *apky   =  realloc(NULL, 0);

  logfp(LFP, "Fuck Parsing Buffers");

  void
  pbuf
     (buf)
       char* buf;
  {
     /* fill key values from line of post json */
     // TODO: split then cmp so keys can also be values

     //printf("buff %s\n",buf);
     logfp(LFP, "TOK");
     logfp(LFP, buf);

     char *tik=NULL;
     size_t tl;

     if( strstr(buf, "apky") ) {
       // TODO: COPY BUFF B4 TIK
/*       int bsz = strlen(buf);
       char *rbuf = realloc(NULL, 0);
       memmove(rbuf, buf, bsz); */
       tik  =  strtok(buf, "\"");
       tik  =  strtok(NULL,"\"");
       tik  =  strtok(NULL,"\"");
       tik  =  strtok(NULL,"\"");

       tl   =  strlen(tik);
       cur  =  realloc(NULL, tl);

       memmove( cur, tik, tl);
       memmove( cur+tl, "\0", 1);

       logfp(LFP, "apky:");
       logfp(LFP, cur);

     }

  }

  pbuf(wrds);
  return cur;
}


char
**pevt
   (cur, wrds)
     char **cur;
     char *wrds;
{
  /* parse buffer for event fields */

  char *eky   =  realloc(NULL, 0);		// email apiky
  char *user  =  realloc(NULL, 0);		// user email
  char *ttl   =  realloc(NULL, 0);		// event title
  char *dsc   =  realloc(NULL, 0);		// description
  char *msg   =  realloc(NULL, 0);		// messages
  char *loc   =  realloc(NULL, 0);		// location
  char *stme  =  realloc(NULL, 0);		// start time
  char *etme  =  realloc(NULL, 0);		// end time
  char *utc   =  realloc(NULL, 0);		// time zone
  char *mps   =  realloc(NULL, 0);		// google maps
  char *lks   =  realloc(NULL, 0);		// links

  logfp(LFP, "Fuck Parsing Buffers");

  void
  pbuf
     (buf)
       char* buf;
  {
     /* fill key values from line of post json */
     // TODO: split then cmp so keys can also be values

     //printf("buff %s\n",buf);
     logfp(LFP, "TOK");
     logfp(LFP, buf);
     char *tik=NULL;
     size_t tl;
     if( strstr(buf, "eky") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
//       logfp(LFP, tik);
       tl = strlen(tik);
       eky = realloc(NULL, tl);
       memmove( eky, tik, tl);
       memmove( eky+tl, "\0", 1);

       logfp(LFP, "EKY");
       logfp(LFP, eky);
//       free(tik);

//       eky +9;
//       eky[strlen(eky)-1] = 0;
//       return;
     }

     if( strstr(buf, "user") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       user = realloc(NULL, tl);
       memmove( user, tik, tl);
       memmove( user+tl, "\0", 1);

       logfp(LFP, "user");
       logfp(LFP, user);
//       free(tik);

     }

     if( strstr(buf, "ttl") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       ttl = realloc(NULL, tl);
       memmove( ttl, tik, tl);
       memmove( ttl+tl, "\0", 1);

       logfp(LFP, "title");
       logfp(LFP, ttl);
     }

     if( strstr(buf, "dsc") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       dsc = realloc(NULL, tl);
       memmove( dsc, tik, tl);
       memmove( dsc+tl, "\0", 1);

       logfp(LFP, "dsc");
       logfp(LFP, dsc);
     }

     if( strstr(buf, "msg") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       msg = realloc(NULL, tl);
       memmove( msg, tik, tl);
       memmove( msg+tl, "\0", 1);

       logfp(LFP, "msg");
       logfp(LFP, msg);

     }

     if( strstr(buf, "loc") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       loc = realloc(NULL, tl);
       memmove( loc, tik, tl);
       memmove( loc+tl, "\0", 1);

       logfp(LFP, "location");
       logfp(LFP, loc);
     }

     if( strstr(buf, "stme") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       stme = realloc(NULL, tl);
       memmove( stme, tik, tl);
       memmove( stme+tl, "\0", 1);

       logfp(LFP, "stme");
       logfp(LFP, stme);
     }

     if( strstr(buf, "etme") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       etme = realloc(NULL, tl);
       memmove( etme, tik, tl);
       memmove( etme+tl, "\0", 1);

       logfp(LFP, "endtime");
       logfp(LFP, etme);

     }

     if( strstr(buf, "utc") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       utc = realloc(NULL, tl);
       memmove( utc, tik, tl);
       memmove( utc+tl, "\0", 1);

       logfp(LFP, "utc");
       logfp(LFP, utc);

     }

     if( strstr(buf, "mps") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       mps = realloc(NULL, tl);
       memmove( mps, tik, tl);
       memmove( mps+tl, "\0", 1);

       logfp(LFP, "maps");
       logfp(LFP, mps);
     }

     if( strstr(buf, "lks") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       lks = realloc(NULL, tl);
       memmove( lks, tik, tl);
       memmove( lks+tl, "\0", 1);

       logfp(LFP, "links");
       logfp(LFP, lks);

     }

  }


  char  *lim = ",";
  int   prog = pbuf;       		// function to pass to element(s)
  logfp(LFP, "loading words");		//

  loadar(wrds, lim);            	// in struct array


  logfp(LFP, "splitting words");

  // char *wrds = "I am the very model of a modern major general";
  // int   prog = something;               // function to pass to element(s)
  // loadar(wrds);                         // in struct array

  int sze = splt[0].pnt-1;
  int i=1;

  while(i<=sze) {
     char *tik = realloc(NULL, 0);
     char *tok=NULL;
     tok = realloc(NULL ,sizeof(tok));                  // array element

     int tsz = splt[i].psz;             // token size
     int pos = splt[i].pos;             // array position
     int pnt = splt[i].pnt;             // buffer position

     tok = realloc(tok, tsz +1);

     memmove(tok, wrds+pos, tsz);
     memmove(tok+tsz, "\0", 1);

    // printf("size %d\nposition %d\ntoken %s\n", tsz,pos,tok);
     pbuf(tok);

     memset(tok, '\0', sizeof(tok));
     logfp(LFP, tok);
     free(tok);
     i++;
  }


  //split(prog,  0, wrds);
  logfp(LFP,"finished parsing");

  size_t eln = strlen( eky );
  size_t uln = strlen( user );
  size_t tln = strlen( ttl );
  size_t dln = strlen( dsc );
  size_t mln = strlen( msg );
  size_t lln = strlen( loc );
  size_t sln = strlen( stme );
  size_t fln = strlen( etme );
  size_t cln = strlen( utc );
  size_t pln = strlen( mps );
  size_t kln = strlen( lks );
  size_t tbl = strlen( ZEVT );

  size_t xln = eln +uln +tln +dln +mln +lln +sln +fln +cln +pln +kln +161;  // cursor length

  cur = realloc(NULL, xln);
  logfp(LFP, "EKY");
  logfp(LFP, eky);
  sprintf(cur, "INSERT INTO %s (eky,user,ttl,dsc,msg,loc,stme,etme,utc,mps,lks) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');", ZEVT, eky, user, ttl, dsc, msg, loc, stme, etme, utc, mps, lks);
  logfp(LFP, "CUR");
  logfp(LFP, cur);
  //printf("some words\nkey %s\nuser %\nttl %s\ndsc %s\nmsg %s\nstme %s\nstme %s\nutc %s\nloc %s\nmps %s\nlks %s\n", eky, user, ttl, dsc, msg, stme, etme, utc, loc, mps, lks);

  return cur;
}

char
**lseky
   ( cur )
      char **cur;		// apky
{
  /* converts apky to eky to remote events */
  short ksz = 64;
  if( strlen( cur ) == 64 ) {
     size_t xln = 33 + strlen( ZDMZ ) + 64;
     char *qry = realloc(NULL, xln);
     sprintf(qry, "SELECT eky from %s WHERE apky='%s';", ZDMZ, cur);
     //     memset(cur, '\0', xln);
     cur = realloc(NULL, xln);
     sprintf(cur, "%s", qry);
     logfp(LFP, "EKY");
     logfp(LFP, qry);
     logfp(LFP, cur);
  } else {
     logfp(LFP, "bad apikey length");
  }
  return cur;
}

char
**lsrvt
  ( cur )
     char **cur;
{
  /* return remote events from eky */
  short ksz = 32;
  if( strlen( cur ) == ksz ) {
     size_t xln = 29 + strlen( ZEVT ) + ksz;
     char *qry  = realloc(NULL, xln);

     sprintf(qry, "SELECT * from %s WHERE eky='%s';", ZEVT, cur);

     cur = realloc(NULL, xln);
     sprintf(cur, "%s", qry);

     logfp(LFP, "QRY");
  } else {

     logfp(LFP, "bad apikey length");
  }
  return cur;
}

char
**setk
  (cur, wrds)
     char **cur;
     char  *wrds;
{
  /* parse line for key val */

  char *user;           		// user
  char *apky;           		// passwd
  char *eky;            		// domain
  char *ki = realloc(NULL,0);		// event table query
  char *di = realloc(NULL,0);		// user table query

  void
  pline
     (buf)
       char *buf;
  {
     logfp(LFP, "TOK");
     logfp(LFP, buf);
     char *tik=NULL;
     size_t tl;


     if( strstr(buf, "user") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       user = realloc(NULL, tl);
       memmove( user, tik, tl);
       memmove( user+tl, "\0", 1);

       logfp(LFP, "EKY");
       logfp(LFP, eky);
     }


     if( strstr(buf, "apky") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       apky = realloc(NULL, tl);
       memmove( apky, tik, tl);
       memmove( apky+tl, "\0", 1);

       logfp(LFP, "EKY");
       logfp(LFP, eky);
     }

     if( strstr(buf, "eky") ) {
       tik = strtok(buf, "\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tik = strtok(NULL,"\"");
       tl = strlen(tik);
       eky = realloc(NULL, tl);
       memmove( eky, tik, tl);
       memmove( eky+tl, "\0", 1);

       logfp(LFP, "EKY");
       logfp(LFP, eky);
     }
  }


  /* main */
  char  *lim = ",";
  logfp(LFP, "splitting words");
  loadar(wrds, lim);            			// in struct array

  int sze = splt[0].pnt-1;
  int i=1;

  while(i<=sze) {
     char *tik = realloc(NULL, 0);
     char *tok=NULL;
     tok = realloc(NULL ,sizeof(tok));                  // array element

     int tsz = splt[i].psz;				// token size
     int pos = splt[i].pos;     		        // array position
     int pnt = splt[i].pnt;				// buffer position

     tok = realloc(tok, tsz);

     memmove(tok, wrds+pos, tsz);
     memmove(tok+tsz, "\0", 1);

     // printf("size %d\nposition %d\ntoken %s\n", tsz,pos,tok);
     pline(tok);

     memset(tok, '\0', sizeof(tok));
     // logfp(LFP, tok);
     free(tok);
     i++;
  }

  logfp(LFP, "words split");

  size_t uln = strlen( user );
  size_t aln = strlen( apky );
  size_t eln = strlen( eky  );
  size_t tln = strlen( ZDMZ );

  size_t xln = uln + aln + eln + tln + 44;		// user query length
  size_t kln = uln + eln + tln + 34;			// event query length
  size_t cln = xln + kln;				// combine length

  di   =  realloc(NULL, xln);
  ki   =  realloc(NULL, kln);
  cur  =  realloc(NULL, cln);

  sprintf(di, "UPDATE %s SET apky='%s', eky='%s' WHERE user='%s';|", ZDMZ, apky, eky, user);
  sprintf(ki, "UPDATE %s SET eky='%s' WHERE user='%s';", ZEVT, eky, user);
  sprintf(cur, "%s%s", di, ki);

  logfp(LFP, "CUR");
  logfp(LFP, cur);

  return cur;
}
