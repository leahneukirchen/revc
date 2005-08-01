/* file-buffers.c: 
 *
 ****************************************************************
 * Copyright (C) 2004 Tom Lord
 * 
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */


#include "hackerlab/os/errno.h"
#include "hackerlab/vu/vu.h"
#include "hackerlab/properties/property.h"
#include "hackerlab/buffers/buffers.h"


/* __STDC__ prototypes for static functions */
static ssize_t buffer_file_names_property (void);
static void init_file_names_list (t_property_type * type,
                                  alloc_limits limits,
                                  ssize_t pos,
                                  void * mem);
static void uninit_file_names_list (t_property_type * type,
                                    alloc_limits limits,
                                    ssize_t pos,
                                    void * mem);
static t_oblist * buffer_names_list (ssize_t bset, ssize_t buffer);
static ssize_t buffer_file_stat_property (void);
static struct stat * buffer_last_stat (ssize_t, ssize_t buffer);



static ssize_t
buffer_file_names_property (void)
{
  static ssize_t answer = -1;

  if (answer < 0)
    {
      answer = property_global_id ((void *)buffer_file_names_property);
    }

  return answer;
}

static void
init_file_names_list (t_property_type * type,
                      alloc_limits limits,
                      ssize_t pos,
                      void * mem)
{
  t_oblist * list;

  list = (t_oblist *)mem;

  if (list)
    {
      init_oblist (list, limits, sizeof (t_uchar *), 0, 0);
    }
}


static void
uninit_file_names_list (t_property_type * type,
                        alloc_limits limits,
                        ssize_t pos,
                        void * mem)
{
  t_oblist * list;

  list = (t_oblist *)mem;
  
  if (list)
    {
      while (!oblist_is_empty (list, limits, sizeof (t_uchar *), 0, 0))
        {
          t_uchar ** box;

          box = (t_uchar **)oblist_burst (0, list, limits, sizeof (t_uchar *), 0, 0, 0, 1);
          if (box)
            lim_free (limits, (void *)*box);

          oblist_delete_n (list, limits, sizeof (t_uchar *), 0, 0, 0, 1);
        }
      
      uninit_oblist (list, limits, sizeof (t_uchar *), 0, 0);
    }
}

static t_property_fns buffer_file_names_property_fns =
{
  (t_uchar *)"buffer_file_names_property_type",
  sizeof (t_oblist),
  init_file_names_list,
  uninit_file_names_list,
};

static t_property_type buffer_file_names_property_type =
{
  &buffer_file_names_property_fns
};

static t_oblist *
buffer_names_list (ssize_t bset, ssize_t buffer)
{
  return (t_oblist *)buffer_session_property_ref (bset, sizeof (t_oblist), buffer, buffer_file_names_property(), &buffer_file_names_property_type);
}


t_uchar *
buf_file_name (ssize_t bset, ssize_t buffer)
{
  t_oblist * list;
  t_uchar ** box;

  list = buffer_names_list (bset, buffer);
  if (!list)
    return 0;

  box = (t_uchar **)oblist_burst (0, list, buffer_session_limits (bset), sizeof (t_uchar *), 0, 0, 0, 1);
  if (!box)
    return 0;
  else
    return * box;
}





static ssize_t
buffer_file_stat_property (void)
{
  static ssize_t answer = -1;

  if (answer < 0)
    {
      answer = property_global_id ((void *)buffer_file_stat_property);
    }

  return answer;
}


static struct stat *
buffer_last_stat (ssize_t bset, ssize_t buffer)
{
  return (struct stat *)buffer_session_property_ref (bset, sizeof (t_uchar *), buffer, buffer_file_stat_property(), 0);
}


ssize_t
buf_find_existing_file (int * errn,
                        ssize_t bset,
                        t_uchar * file)
{
  int my_errn;
  int fd;
  struct stat s;
  ssize_t room;
  ssize_t x;

  if (!errn)
    errn = &my_errn;

  fd = vu_open (errn, file, O_RDONLY, 0);
  if (fd < 0)
    {
      return -1;
    }

  if (0 > vu_fstat (errn, fd, &s))
    {
      vu_close (0, fd);
      return -1;
    }

  if (0 > vu_close (errn, fd))
    return -1;

  room = buffer_session_room (bset);
  for (x = 0; x < room; ++x)
    {
      struct stat * bufstat;

      bufstat = buffer_last_stat (bset, x);
      if (   bufstat
          && (s.st_ino == bufstat->st_ino)
          && (s.st_dev == bufstat->st_dev))
        {
          return x;
        }
    }

  if (errn)
    *errn = 0;
  return -1;
} 


ssize_t
buf_find_file (int * errn,
               ssize_t bset,
               t_uchar * file)
{
  int my_errn;
  int fd;
  struct stat s;
  ssize_t room;
  ssize_t x;
  ssize_t answer;
  t_uchar * name_for_buffer = file;

  if (!errn)
    errn = &my_errn;

  fd = vu_open (errn, file, O_RDONLY, 0);
  if (fd < 0)
    {
      return -1;
    }

  if (0 > vu_fstat (errn, fd, &s))
    {
      vu_close (0, fd);
      return -1;
    }

  room = buffer_session_room (bset);
  for (x = 0; x < room; ++x)
    {
      struct stat * bufstat;

      bufstat = buffer_last_stat (bset, x);
      if (   bufstat
          && (s.st_ino == bufstat->st_ino)
          && (s.st_dev == bufstat->st_dev))
        {
          if (0 > vu_close (errn, fd))
            return -1;
          return x;
        }
    }

  answer = buffer_session_index_create (bset, name_for_buffer);
  if (answer < 0)
    {
      vu_close (0, fd);
      return -1;
    }

  if (0 > buf_insert_from_fd (errn, 0, bset, answer, fd, 0, -1))
    {
      vu_close (0, fd);
      return -1;
    }

  {
    struct stat * saved_s;

    saved_s = buffer_last_stat (bset, answer);
    if (!saved_s)
      return -1;

    *saved_s = s;
  }

  if (0 > vu_close (errn, fd))
    return -1;

  if (errn)
    *errn = 0;

  return answer;
}




int
buf_insert_from_fd (int * errn,
                    ssize_t * amt_inserted_return,
                    ssize_t bset,
                    ssize_t buffer,
                    int fd,
                    ssize_t insert_pos,
                    ssize_t insert_amt)
{
  int my_errn;
  ssize_t amt_inserted;
  int at_eof;

  if (!errn)
    errn = &my_errn;

  amt_inserted = 0;
  at_eof = 0;
  
  while (   !at_eof
         && (   (insert_amt < 0)
             || (amt_inserted < insert_amt)))
    {
      t_uchar buf[16 * 1024];
      ssize_t amt_read;

      amt_read = vu_read (errn, fd, buf, sizeof (buf));
      if (amt_read < 0)
        {
          if ((*errn != EINTR) && (*errn != EAGAIN))
            return -1;
        }
      else if (amt_read == 0)
        {
          at_eof = 1;
        }
      else
        {
          if (0 > buf_insert_str_n (bset, buffer, insert_pos + amt_inserted, buf, amt_read))
            {
              *errn = 0;
              return -1;
            }
          amt_inserted += amt_read;
        }
    }

  if (amt_inserted_return)
    *amt_inserted_return = amt_inserted;

  return 0;
}





/* tag: Tom Lord Wed Oct 27 19:35:15 2004 (file-buffers.c)
 */
