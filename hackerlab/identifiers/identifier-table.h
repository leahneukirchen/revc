/* identifier-table.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__IDENTIFIER_TABLE_H
#define INCLUDE__LIBAWIKI__IDENTIFIER_TABLE_H


#include "hackerlab/obtables/obtable.h"
#include "hackerlab/oblists/oblist.h"


typedef struct identifier_table t_identifier_table;
typedef struct identifier_table_entry t_identifier_table_entry;


struct identifier_table
{
  t_obtable _kv;
  t_oblist _order;
};

struct identifier_table_entry
{
  struct obtable_elt _obtable_elt;
  const t_uchar * _key;
  t_uchar _value[1];
};

#define STATIC_DECL_IDENTIFIER_TABLE(STEM, TYPE) \
  static t_identifier_table * \
  STEM ## _table (void) \
  { \
    static t_identifier_table table; \
    static int initialized = -1; \
    if (initialized < 0) \
      { \
        initialized = init_identifier_table (&table, lim_use_malloc, sizeof (TYPE)); \
        if (initialized < 0) return 0; \
      } \
    return &table; \
  } \
  static TYPE * \
  STEM ## _find (t_uchar * key) \
  { \
    return (TYPE *)identifier_table_entry_find (STEM ## _table (), lim_use_malloc, sizeof (TYPE), key); \
  } \
  static TYPE * \
  STEM ## _ensure (t_uchar * key) \
  { \
    return (TYPE *)identifier_table_entry_ensure (STEM ## _table (), lim_use_malloc, sizeof (TYPE), key); \
  } \
  static ssize_t \
  STEM ## _index (t_uchar * key) \
  { \
    return identifier_table_entry_index (STEM ## _table (), lim_use_malloc, sizeof (TYPE), key); \
  } \
  static ssize_t \
  STEM ## _ensure_index (t_uchar * key) \
  { \
    if (!STEM ## _ensure (key)) \
      return -1; \
    else \
      return identifier_table_entry_index (STEM ## _table (), lim_use_malloc, sizeof (TYPE), key); \
  } \
  static TYPE * \
  STEM ## _n (t_uchar ** key_ret, ssize_t index) \
  { \
    return (TYPE *)identifier_table_entry_n (key_ret, STEM ## _table (), lim_use_malloc, sizeof (TYPE), index); \
  }




/* automatically generated __STDC__ prototypes */
extern int init_identifier_table (t_identifier_table * table,
                                  alloc_limits limits,
                                  ssize_t elt_size);
extern int uninit_identifier_table (t_identifier_table * table,
                                    alloc_limits limits,
                                    ssize_t elt_size);
extern ssize_t identifier_table_n_entries (t_identifier_table * table,
                                           alloc_limits limits,
                                           ssize_t elt_size);
extern ssize_t identifier_table_room (t_identifier_table * table,
                                      alloc_limits limits,
                                      ssize_t elt_size);
extern void * identifier_table_entry_n (const t_uchar ** key_return,
                                        t_identifier_table * table,
                                        alloc_limits limits,
                                        ssize_t elt_size,
                                        ssize_t n);
extern void * identifier_table_nth_entry (const t_uchar ** key_return,
                                          t_identifier_table * table,
                                          alloc_limits limits,
                                          ssize_t elt_size,
                                          ssize_t n);
extern ssize_t identifier_table_entry_index (t_identifier_table * table,
                                             alloc_limits limits,
                                             ssize_t elt_size,
                                             const t_uchar * key);
extern void * identifier_table_entry_find (t_identifier_table * table,
                                           alloc_limits limits,
                                           ssize_t elt_size,
                                           const t_uchar * key);
extern void * identifier_table_entry_ensure (t_identifier_table * table,
                                             alloc_limits limits,
                                             ssize_t elt_size,
                                             const t_uchar * key);
extern int identifier_table_entry_delete (t_identifier_table * table,
                                          alloc_limits limits,
                                          ssize_t elt_size,
                                          const t_uchar * key);
#endif  /* INCLUDE__LIBAWIKI__IDENTIFIER_TABLE_H */


/* tag: Tom Lord Thu Oct 28 11:31:23 2004 (libds/identifier-table.h)
 */
