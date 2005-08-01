/* awiki.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "awiki/libawiki-nodes/unparsed.h"
#include "awiki/libawiki-nodes/unparsed-html.h"
#include "awiki/libawiki-nodes/paper.h"
#include "awiki/libawiki-nodes/paper-html.h"
#include "awiki/libawiki-nodes/title.h"
#include "awiki/libawiki-nodes/title-html.h"
#include "awiki/libawiki-nodes/authors.h"
#include "awiki/libawiki-nodes/authors-html.h"
#include "awiki/libawiki-nodes/abstract.h"
#include "awiki/libawiki-nodes/abstract-html.h"
#include "awiki/libawiki-nodes/styled-text.h"
#include "awiki/libawiki-nodes/styled-text-html.h"
#include "awiki/libawiki-nodes/styled-lines.h"
#include "awiki/libawiki-nodes/styled-lines-html.h"
#include "awiki/libawiki-nodes/paragraph.h"
#include "awiki/libawiki-nodes/paragraph-html.h"
#include "awiki/libawiki-nodes/essay.h"
#include "awiki/libawiki-nodes/essay-html.h"
#include "awiki/libawiki-nodes/outline.h"
#include "awiki/libawiki-nodes/outline-html.h"
#include "awiki/libawiki-nodes/tty.h"
#include "awiki/libawiki-nodes/tty-html.h"
#include "awiki/libawiki-nodes/cartouche.h"
#include "awiki/libawiki-nodes/cartouche-html.h"
#include "awiki/libawiki-nodes/null.h"
#include "awiki/libawiki-nodes/null-html.h"
#include "awiki/libawiki-nodes/saying.h"
#include "awiki/libawiki-nodes/saying-html.h"
#include "awiki/libawiki-nodes/blockquote.h"
#include "awiki/libawiki-nodes/blockquote-html.h"
#include "awiki/libawiki/awiki.h"



int
init_awiki (void)
{
  if (0 > awiki_init_node_unparsed ())
    return -1;

  if (0 > awiki_init_node_unparsed_html ())
    return -1;

  if (0 > awiki_init_node_paper ())
    return -1;

  if (0 > awiki_init_node_paper_html ())
    return -1;

  if (0 > awiki_init_node_title ())
    return -1;

  if (0 > awiki_init_node_title_html ())
    return -1;

  if (0 > awiki_init_node_authors ())
    return -1;

  if (0 > awiki_init_node_authors_html ())
    return -1;

  if (0 > awiki_init_node_abstract ())
    return -1;

  if (0 > awiki_init_node_abstract_html ())
    return -1;

  if (0 > awiki_init_node_styled_text ())
    return -1;

  if (0 > awiki_init_node_styled_text_html ())
    return -1;

  if (0 > awiki_init_node_styled_lines ())
    return -1;

  if (0 > awiki_init_node_styled_lines_html ())
    return -1;

  if (0 > awiki_init_node_paragraph ())
    return -1;

  if (0 > awiki_init_node_paragraph_html ())
    return -1;

  if (0 > awiki_init_node_essay ())
    return -1;

  if (0 > awiki_init_node_essay_html ())
    return -1;

  if (0 > awiki_init_node_outline ())
    return -1;

  if (0 > awiki_init_node_outline_html ())
    return -1;

  if (0 > awiki_init_node_tty ())
    return -1;

  if (0 > awiki_init_node_tty_html ())
    return -1;

  if (0 > awiki_init_node_null ())
    return -1;

  if (0 > awiki_init_node_null_html ())
    return -1;

  if (0 > awiki_init_node_cartouche ())
    return -1;
	  
  if (0 > awiki_init_node_cartouche_html ())
    return -1;

  if (0 > awiki_init_node_saying ())
    return -1;
	  
  if (0 > awiki_init_node_saying_html ())
    return -1;

  if (0 > awiki_init_node_blockquote ())
    return -1;
	  
  if (0 > awiki_init_node_blockquote_html ())
    return -1;

  return 0;
}




/* arch-tag: Tom Lord Sun Nov 21 11:12:45 2004 (libawiki/awiki.c)
 */
