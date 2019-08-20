void
initbl
   (con, tbl)
    MYSQL *con;
     char *tbl;
{
  /* creates new tables */

  char *q1   =  "CREATE TABLE IF NOT EXISTS";
  char *q2   =  "(user VARCHAR(255) NOT NULL UNIQUE KEY, pwd VARCHAR(255) NOT NULL, domain VARCHAR(255) NOT NULL UNIQUE, apky VARCHAR(64) NOT NULL UNIQUE, slmt INT NOT NULL, hlmt INT NOT NULL, xtradom TEXT, today VARCHAR(10) NOT NULL, PRIMARY KEY(user));";
  size_t l1  =  strlen( q1 ) + strlen( q2 );

  char **cur = "";
  cur = realloc(NULL, l1);

  sprintf( cur,  "%s %s%s", q1, tbl, q2 );

  curdom(con, cur);

  return;
}

