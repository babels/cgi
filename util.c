#include <stdio.h>
#include "config.h"


typedef void (*fptr) ();


struct array {
   int  pnt;                    // pointer on arr from 1 = ele[len]
   int  psz;                    // length of element
   int  pos;                    // starting position on origional array
}; struct array splt[1024];     // return element + max_size



void
Fatal
   (err)
     char* err;
{
  logfp(LFP, "FATAL");
  logfp(LFP, err);
  exit(1);
}


void
logfp
   (fp, txt)
     char *fp;		// log path
     char *txt;		// text buffer
{
  /* writes text buffer to log path */

  FILE *fd;
  fd = fopen(fp, "a");
  fprintf(fd, "%s\n", txt);
  fclose(fd);
  return;
}





char*
concat
   (rbf,fp)
     char* rbf;
     char* fp;
{
   /* read text file to buffer for split*/

   int string_size, read_size;
   FILE *fd = fopen(fp, "r");

   if (fd) {

       fseek(fd, 0, SEEK_END);
       string_size = ftell(fd);
       rewind(fd);

       rbf = (char*) malloc(sizeof(char) * (string_size + 1) );
       read_size = fread(rbf, sizeof(char), string_size, fd);
       rbf[string_size] = '\0';

       if (string_size != read_size) {
           free(rbf);
           rbf = NULL;
       }

       fclose(fd);
    }

    return rbf;
}


void
loadar
   (wrds, lim)
     char* wrds;                        // character buffer
     char* lim;                         // delimiter
{
  /* load buffer to structure */

  int i  = 1;                           // element pointer
  int k  = 0;                           // character pointer
  int ll = strlen(lim);                 // delimiter length
  int wl = strlen(wrds);                // in buffer length

  char* nwrd = realloc(NULL,0);         // copy origional leaving intact
  nwrd = realloc(nwrd, wl);
  memmove(nwrd,wrds,wl);

  char* ele = strtok(nwrd, lim);


  while ( ele != NULL ) {

     int eln = strlen( ele );
     splt[i].pnt = i;                   // element pointer
     splt[i].psz = eln;                 // element length
     splt[i].pos = k;                   // element

  i++;                                  // math
  k = k + eln + ll;                     // position next pointer at end of last
  ele = strtok(NULL, lim);              // increment element
  }

  splt[0].pnt = i;                      // first element as max element count
  return;
}


void
split
  (pew, ptr, wrds)
     fptr  pew;                         // out function
     int   ptr;                         // in  index
     char* wrds;                        // in  buffer
{
  /* splits character buffer(wrds) by index(ptr) & pass' each element to the function pew */
  int i;
  int wl = strlen(wrds);
  char* nwrd = realloc(NULL, 0);        // initialize copy

  char* bff = realloc(NULL ,0);         // initialize return buuffer
  nwrd = realloc(nwrd, wl);             // copy origional leaving intact
  memmove(nwrd, wrds, wl);

  int rt;                               // element number
  rt = splt[ptr].pnt-1;
  int lst = splt[0].pnt-1;

  if ( ptr > lst )
     Fatal("index out of bounds");

  int tr = splt[0].pnt;                 // last element
  if ( ptr == 0 ) {                     // assumes we dont need to size just index
     for( i=1; i<tr; i++) {

       int ln = splt[i].psz;            // element length
       int ps = splt[i].pos;            // buffer position

       char* ele = realloc(NULL,0);
       ele = realloc( ele, ln );

       memmove(ele, nwrd+ps, ln);
       memmove(ele+ln, "\0", 1);

       pew(ele);
    }

    return;
  }


  if ( ptr < 0 )
    rt = lst + ptr + 1;                 // negetive index  lst +(-ptr) +1;

  if ( ptr == -1 )
    rt = lst;                           // last index

  int esz  =  splt[rt].psz;             // size of element at ptr
  int wrd  =  splt[rt].pos;             // character position on origional buffer
      bff  =  realloc(bff, esz);        // allocate memory to return buffer


  memmove(bff, nwrd+wrd, esz);          // move date into buffer
  memmove(bff+esz, "\0", 1);            // ensure nullterm, see todo

  pew(bff);
  return;
}


