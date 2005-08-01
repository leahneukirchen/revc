/* symbol-table.h:
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef INCLUDE__LIBAWIKI__SYMBOL_TABLE_H
#define INCLUDE__LIBAWIKI__SYMBOL_TABLE_H


#include "hackerlab/obtables/obtable.h"
#include "hackerlab/oblists/oblist.h"


typedef struct symbol_table t_symbol_table;
typedef struct symbol_table_entry t_symbol_table_entry;


struct symbol_table
{
  t_obtable _kv;
  t_oblist _order;
};

struct symbol_table_entry
{
  struct obtable_elt _obtable_elt;
  t_uchar * _key;
  t_uchar _value[1];
};

#define STATIC_DECL_SYMBOL_TABLE(STEM, TYPE) \
  static t_symbol_table * \
  STEM ## _table (void) \
  { \
    static t_symbol_table table; \
    static int initialized = -1; \
    if (initialized < 0) \
      { \
        initialized = init_symbol_table (&table, lim_use_malloc, sizeof (TYPE)); \
        if (initialized < 0) return 0; \
      } \
    return &table; \
  } \
  static TYPE * \
  STEM ## _find (const t_uchar * const key) \
  { \
    return (TYPE *)symbol_table_entry_find (STEM ## _table (), lim_use_malloc, sizeof (TYPE), key); \
  } \
  static TYPE * \
  STEM ## _ensure (const t_uchar * const key) \
  { \
    return (TYPE *)symbol_table_entry_ensure (STEM ## _table (), lim_use_malloc, sizeof (TYPE), key); \
  } \
  static ssize_t \
  STEM ## _index (const t_uchar * const key) \
  { \
    return symbol_table_entry_index (STEM ## _table (), lim_use_malloc, sizeof (TYPE), key); \
  } \
  static ssize_t \
  STEM ## _ensure_index (const t_uchar * const key) \
  { \
    if (!STEM ## _ensure (key)) \
      return -1; \
    else \
      return symbol_table_entry_index (STEM ## _table (), lim_use_malloc, sizeof (TYPE), key); \
  } \
  static TYPE * \
  STEM ## _n (const t_uchar ** const key_ret, ssize_t const index) \
  { \
    return (TYPE *)symbol_table_entry_n (key_ret, STEM ## _table (), lim_use_malloc, sizeof (TYPE), index); \
  }


/* automatically generated __STDC__ prototypes */
extern int init_symbol_table (t_symbol_table * const table,
                              alloc_limits const limits,
                              ssize_t const elt_size);
extern int uninit_symbol_table (t_symbol_table * const table,
                                alloc_limits const limits,
                                ssize_t const elt_size);
extern ssize_t symbol_table_n_entries (t_symbol_table * const table,
                                       alloc_limits const limits,
                                       ssize_t const elt_size);
extern ssize_t symbol_table_room (t_symbol_table * const table,
                                  alloc_limits const limits,
                                  ssize_t const elt_size);
extern void * symbol_table_entry_n (const t_uchar ** const key_return,
                                    t_symbol_table * const table,
                                    alloc_limits const limits,
                                    ssize_t const elt_size,
                                    ssize_t const n);
extern void * symbol_table_nth_entry (const t_uchar ** const key_return,
                                      t_symbol_table * const table,
                                      alloc_limits const limits,
                                      ssize_t const elt_size,
                                      ssize_t const n);
extern ssize_t symbol_table_entry_index (t_symbol_table * const table,
                                         alloc_limits const limits,
                                         ssize_t const elt_size,
                                         const t_uchar * const key);
extern void * symbol_table_entry_find (t_symbol_table * const table,
                                       alloc_limits const limits,
                                       ssize_t const elt_size,
                                       const t_uchar * const key);
extern void * symbol_table_entry_ensure (t_symbol_table * const table,
                                         alloc_limits const limits,
                                         ssize_t const elt_size,
                                         const t_uchar * const key);
extern int symbol_table_entry_delete (t_symbol_table * const table,
                                      alloc_limits const limits,
                                      ssize_t const elt_size,
                                      const t_uchar * const key);
#endif  /* INCLUDE__LIBAWIKI__SYMBOL_TABLE_H */


/* tag: Tom Lord Wed Oct 27 11:22:41 2004 (symbol-table.h)
 */
