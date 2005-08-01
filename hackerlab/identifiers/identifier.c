/* identifier.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/char/str.h"
#include "hackerlab/oblists/oblist.h"
#include "hackerlab/identifiers/identifier.h"


/* __STDC__ prototypes for static functions */
static t_oblist * identifier_table (void);
static int name_cmp (void * vkey, void * velt, void * ign);



static t_oblist *
identifier_table (void)
{
  static int initialized = 0;
  static t_oblist table;

  if (!initialized)
    {
      if (0 > init_oblist (&table, lim_use_malloc, sizeof (t_uchar *), 0, 0))
        {
          return 0;
        }
      initialized = 1;
    }

  return &table;
}



const t_uchar *
intern_identifier (const t_uchar * const name)
{
  ssize_t found_at;
  ssize_t would_be_at;
  t_uchar ** place;

  found_at = oblist_find (&would_be_at, identifier_table (), lim_use_malloc, sizeof (t_uchar *), 0, 0, (void *)name, name_cmp, 0);

  if (found_at >= 0)
    {
      place = (t_uchar **)oblist_burst (0, identifier_table (), lim_use_malloc, sizeof (t_uchar *), 0, 0, found_at, 1);
    }
  else
    {
      t_uchar * saved_name;

      saved_name = str_save (lim_use_malloc, name);
      if (!saved_name)
        return 0;

      if (0 > oblist_insert_n (identifier_table (), lim_use_malloc, sizeof (t_uchar *), 0, 0, would_be_at, 1, 0))
        return 0;
      
      place = (t_uchar **)oblist_burst (0, identifier_table (), lim_use_malloc, sizeof (t_uchar *), 0, 0, would_be_at, 1);
      if (place)
        {
          *place = saved_name;
        }
    }


  if (!place)
    return 0;
  else
    return *place;
}


static int
name_cmp (void * vkey, void * velt, void * ign)
{
  t_uchar * key;
  t_uchar ** elt;

  key = (void *)vkey;
  elt = (void *)velt;

  if (!key && !elt)
    return 0;

  if (!key && (elt && *elt))
    return -1;

  if (key && (!elt || !*elt))
    return 1;
    
  return str_cmp (key, *elt);
}



/* tag: Tom Lord Wed Oct 27 23:05:15 2004 (identifier.c)
 */
