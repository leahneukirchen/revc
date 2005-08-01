/* parse.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/identifiers/identifier.h"
#include "hackerlab/identifiers/identifier-table.h"
#include "hackerlab/properties/property.h"
#include "awiki/libawiki/parse.h"


/* __STDC__ prototypes for static functions */



static t_identifier_table * id_table (void);
static t_awiki_node_type * id_type (const t_uchar * name_atom);



Identifier_safe_decl (unparsed, "unparsed");



int
awiki_register_type (const t_uchar * name_id,
                     t_awiki_node_type * type)
{
  t_awiki_node_type ** type_box;

  if (!name_id)
    return -1;

  if (id_type (name_id))
    return -1;

  type_box = identifier_table_entry_ensure (id_table (), lim_use_malloc, sizeof (t_awiki_node_type *), name_id);
  if (!type_box)
    return -1;

  *type_box = type;
  return 0;
}


t_awiki_node_type *
awiki_find_type (const t_uchar * name_id)
{
  t_awiki_node_type ** type_box;

  if (!name_id)
    return 0;

  type_box = (t_awiki_node_type **)identifier_table_entry_find (id_table (), lim_use_malloc, sizeof (t_awiki_node_type *), name_id);
  if (type_box)
    return *type_box;
  
  return awiki_find_type (Identifier_unparsed ());
}




int
awiki_parse_for_type (const t_uchar * name_atom,
                      ssize_t graph,
                      ssize_t node,
                      t_buffer_point * error_pt,
                      t_buffer_point * source_pt,
                      t_buffer_point * end_pt)
{
  t_awiki_node_type * node_type;

  if (!name_atom)
    return -1;

  node_type = awiki_find_type (name_atom);
  if (!node_type)
    return -1;

  if (!node_type->fns || !node_type->fns->parser)
    {
      return -1;
    }
  else
    {
      return node_type->fns->parser (node_type, graph, node, error_pt, source_pt, end_pt);
    }
}





static t_identifier_table *
id_table (void)
{
  static int initialized = 0;
  static t_identifier_table table;

  if (!initialized)
    {
      if (0 > init_identifier_table (&table, lim_use_malloc, sizeof (t_awiki_node_type *)))
        return 0;
      initialized = 1;
    }

  return &table;
}


static t_awiki_node_type *
id_type (const t_uchar * name_atom)
{
  return (t_awiki_node_type *)identifier_table_entry_find (id_table (), lim_use_malloc, sizeof (t_awiki_node_type *), name_atom);
}



/* arch-tag: Tom Lord Thu Oct 28 11:30:30 2004 (awiki-parse.c)
 */
