#include <stdio.h>
#include <mysql.h>

#include "config.h"



MYSQL
*mycon
   (con, mydb)
     MYSQL *con;		// mysql socket
     char *mydb;		// database name
{
  char *err;
  con = mysql_init(NULL);

  if (con == NULL)
  {

      err = mysql_error(con);
      logfp(LFP, err);
      //fprintf(stderr, "%s\n", err);
      exit(1);
  }

  if (mysql_real_connect(con, MYDOM, MYUSR, MYPWD,
          MYDB, 0, NULL, 0) == NULL)
  {
      err = mysql_error(con);
      //fprintf(stderr, "%s\n", err);
      logfp(LFP, err);
      mysql_close(con);
      exit(1);
  }

  return con;
}




void
finish_with_error
   (con)
     MYSQL *con;
{
  /* print sql error and exit */

  fprintf(stderr, "%s\n", mysql_error(con));

  mysql_close(con);

  exit(1);
}


void
curdom
  (cur)
     char *cur;
{
  /* executes sql statment */

  MYSQL *con;
  con = mycon(con, MYDB);

  logfp(LFP, cur);

  if (mysql_query(con, cur)) {
      finish_with_error(con);
  }

  mysql_close(con);
  return;
}


char
**curdim
  (cur, ret)
     char *cur;
     char **ret;
{
  /* executes sql statment */

  MYSQL *con;
  MYSQL_ROW row;
  con = mycon(con, MYDB);
  logfp(LFP, cur);

  if (mysql_query(con, cur)) {
      finish_with_error(con);
  }

  MYSQL_RES *result = mysql_store_result(con);
  while ((row = mysql_fetch_row(result)))
  {
     logfp(LFP, "ROW");
     char *line = row[0];				// line
     logfp(LFP, line);
     size_t lsz = strlen( line );
     ret = realloc(NULL, 47);
     memmove(ret,line,47);
  }
  mysql_close(con);
  return ret;
}


void
droptbl
   (tbl)
     char *tbl;
{
  /* drops table */

  char **cur = "";
  char *p1   = "DROP TABLE IF EXISTS";

  size_t pl  =  strlen( p1 );
  size_t tl  =  strlen( tbl );

  size_t ql  =  pl + tl + 1;

  cur = realloc( NULL, ql );
  sprintf( cur, "%s %s;", p1, tbl );
  curdom( cur );

  return;
}




void
initbl
   (tbl, qry)
     char *tbl;		// sql table name
     char *qry;		// sql cursor query
{
  /* creates new tables */

  char *q1    =  "CREATE TABLE IF NOT EXISTS";
  size_t l1   =  strlen( q1 ) + strlen( qry ) + strlen(tbl) +100;

  char **cur  =  "";
  cur         =  realloc(NULL, l1);		// initialize cursor

  sprintf( cur,  "%s %s%s", q1, tbl, qry );	// move it all to cur

  curdom(cur);					// execute query

  return;
}


char
*lsrem
   (eky, mjson)
     char *eky;
     char *mjson;
{
  MYSQL *con = mysql_init(NULL);
  con = mycon(con, MYDB);

  if (con == NULL) {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }

  if( strlen( eky ) != 47 ) {
     logfp(LFP, "bad eky length!");
     exit(1);
  }

  char *req = realloc(NULL, 79);
  sprintf( req, "SELECT * FROM zevt WHERE eky='%s';", eky);
  logfp(LFP, "REQ");
  logfp(LFP, req);

  if (mysql_query(con, req) ) {
      finish_with_error(con);
  }

  MYSQL_RES *result = mysql_store_result(con);

  if (result == NULL) {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  char **json = realloc(NULL, 0);

  int i = 0;
  while (( row   =  mysql_fetch_row(result) ))
  {
      char *zid   =  row[0];		// uid
      char *usr   =  row[2];            // user
      char *ttl   =  row[3];            // title
      char *dsc   =  row[4];            // description
      char *msg   =  row[5];            // message
      char *loc   =  row[6];            // location
      char *stme  =  row[7];            // start time
      char *etme  =  row[8];            // end time
      char *utc   =  row[9];            // utc

      char *lks   =  row[11];            // links
      char *mps   =  row[10];             // mps
      char *mer   =  row[i];

      char *ner = realloc(NULL, 2);
      sprintf(ner, "%d", i);
      logfp(LFP, "ROW");
      logfp(LFP, ner);
      logfp(LFP, mer);


      size_t xln  =  strlen( usr ) + strlen( ttl ) + strlen( dsc ) + strlen( msg ) + strlen( loc ) + strlen( stme ) + strlen( etme) +\
                     strlen( utc ) + strlen(mps) + strlen( lks ) + strlen( zid ) + 125;
      size_t jln  =  strlen( json );
      size_t tln  =  xln + jln +1;

      char *line  =  realloc(NULL, xln);
      char *njson =  realloc(NULL, tln);

      sprintf(line, "\{\ \"usr\": \"%s\", \"ttl\": \"%s\", \"dsc\": \"%s\", \"msg\": \"%s\", \"loc\": \"%s\", \"stme\": \"%s\", \"etme\": \"%s\", \"utc\": \"%s\", \"mps\": \"%s\",  \"zid\": \"%s\", \"lks\": \"%s\"},",
                   usr, ttl, dsc, msg, loc, stme, etme, utc, mps, zid, lks);

      size_t lln = strlen( line );

      memmove(njson, json, jln);
      memmove(njson +jln, line, lln);

      json = realloc(NULL, tln);
      memmove(json, njson, tln);
      i++;
     // i = i +1;
  }

  int er = strlen(json) -1;
  mjson  = realloc(NULL, er +4);
  memmove(mjson, "\{\ \{", 3);

  memmove(mjson+3, json +1, er);

  memmove(mjson+strlen(mjson)-1,"\ \}",2);


  mysql_free_result(result);
  mysql_close(con);

  logfp(LFP,"json");
  logfp(LFP, mjson);

  return mjson;
}


void
initbls()
{
  /* initializes known tables */

  logfp(LFP, "Initializing Tables");
  char *usrq  =  "(user VARCHAR(191) NOT NULL UNIQUE KEY, pwd VARCHAR(191) NOT NULL, domain VARCHAR(191) NOT NULL UNIQUE, eky VARCHAR(64) NOT NULL UNIQUE, paid VARCHAR(12) NOT NULL, apky VARCHAR(64) NOT NULL UNIQUE, slmt INT NOT NULL, hlmt INT NOT NULL, xtradom TEXT, today VARCHAR(10) NOT NULL, PRIMARY KEY ix_length_user (user(191)))  ENGINE=InnoDB;";
  char *zevt  =  "(id MEDIUMINT NOT NULL AUTO_INCREMENT, eky VARCHAR(191) NOT NULL, user VARCHAR(191) NOT NULL, ttl TEXT(1024) NOT NULL, dsc TEXT(8148) NOT NULL, msg TEXT(8148), loc VARCHAR(191), stme VARCHAR(16), etme VARCHAR(16), utc VARCHAR(191), mps VARCHAR(191), lks TEXT(9001), PRIMARY KEY (id))     ENGINE=InnoDB;";

  initbl( ZDMZ, usrq);				// user table
  initbl( ZEVT, zevt);				// event table

  return;
}

void
droptbls() {
  /* drops tables */
  logfp(LFP, "Deleting Tables");

  droptbl(ZDMZ);
  droptbl(ZEVT);

  return;
}




void
reinittbls() {
  /* ddrop then reinitialize tables */

  droptbls();
  initbls();

  return;
}
