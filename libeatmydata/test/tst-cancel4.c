/* Copyright (C) 2002, 2003, 2004, 2006, 2007 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.
   (C) 2013 Stewart Smith.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* NOTE: Modified from original to use mkostemp() instead of relying on
   Makefile being present */

/* NOTE: this tests functionality beyond POSIX.  POSIX does not allow
   exit to be called more than once.  */

#include "config.h"

#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <sys/wait.h>

//#include "pthreadP.h"


/* Since STREAMS are not supported in the standard Linux kernel and
   there we don't advertise STREAMS as supported is no need to test
   the STREAMS related functions.  This affects
     getmsg()              getpmsg()          putmsg()
     putpmsg()

   lockf() and fcntl() are tested in tst-cancel16.

   pthread_join() is tested in tst-join5.

   pthread_testcancel()'s only purpose is to allow cancellation.  This
   is tested in several places.

   sem_wait() and sem_timedwait() are checked in tst-cancel1[2345] tests.

   mq_send(), mq_timedsend(), mq_receive() and mq_timedreceive() are checked
   in tst-mqueue8{,x} tests.

   aio_suspend() is tested in tst-cancel17.

   clock_nanosleep() is tested in tst-cancel18.
*/

/* Pipe descriptors.  */
static int fds[2];

/* Temporary file descriptor, to be closed after each round.  */
static int tempfd = -1;
static int tempfd2 = -1;
/* Name of temporary file to be removed after each round.  */
static char *tempfname;
/* Temporary message queue.  */
static int tempmsg = -1;

/* Often used barrier for two threads.  */
static pthread_barrier_t b2;


#ifndef IPC_ADDVAL
# define IPC_ADDVAL 0
#endif

/* Cleanup handling test.  */
static int cl_called;

static void
cl (void *arg)
{
  (void)arg;
  ++cl_called;
}



static void *
tf_fsync (void *arg)
{
  if (arg == NULL)
    // XXX If somebody can provide a portable test case in which fsync()
    // blocks we can enable this test to run in both rounds.
    abort ();

  char *fname= strdup("/tmp/eatmydataXXXXXX");

  tempfd = mkostemp(fname, O_RDONLY);
  if (tempfd == -1)
    {
      printf ("%s: cannot open %s\n", __func__, fname);
      exit (1);
    }

  int r = pthread_barrier_wait (&b2);
  if (r != 0 && r != PTHREAD_BARRIER_SERIAL_THREAD)
    {
      printf ("%s: barrier_wait failed\n", __func__);
      exit (1);
    }

  r = pthread_barrier_wait (&b2);
  if (r != 0 && r != PTHREAD_BARRIER_SERIAL_THREAD)
    {
      printf ("%s: 2nd barrier_wait failed\n", __func__);
      exit (1);
    }

  pthread_cleanup_push (cl, NULL);

  fsync (tempfd);

  pthread_cleanup_pop (0);

  printf ("%s: fsync returned\n", __func__);

  unlink(fname);
  free(fname);

  exit (1);
}


static void *
tf_fdatasync (void *arg)
{
  if (arg == NULL)
    // XXX If somebody can provide a portable test case in which fdatasync()
    // blocks we can enable this test to run in both rounds.
    abort ();

  char *fname= strdup("/tmp/eatmydataXXXXXX");

  tempfd = mkostemp(fname, O_RDONLY);
  if (tempfd == -1)
    {
      printf ("%s: cannot open %s\n", __func__, fname);
      exit (1);
    }

  int r = pthread_barrier_wait (&b2);
  if (r != 0 && r != PTHREAD_BARRIER_SERIAL_THREAD)
    {
      printf ("%s: barrier_wait failed\n", __func__);
      exit (1);
    }

  r = pthread_barrier_wait (&b2);
  if (r != 0 && r != PTHREAD_BARRIER_SERIAL_THREAD)
    {
      printf ("%s: 2nd barrier_wait failed\n", __func__);
      exit (1);
    }

  pthread_cleanup_push (cl, NULL);

  fdatasync (tempfd);

  pthread_cleanup_pop (0);

  printf ("%s: fdatasync returned\n", __func__);

  unlink(fname);
  free(fname);

  exit (1);
}


static void *
tf_msync (void *arg)
{
  if (arg == NULL)
    // XXX If somebody can provide a portable test case in which msync()
    // blocks we can enable this test to run in both rounds.
    abort ();

  char *fname= strdup("/tmp/eatmydataXXXXXX");

  tempfd = mkostemp(fname, O_RDONLY);
  if (tempfd == -1)
    {
      printf ("%s: cannot open %s\n", __func__, fname);
      exit (1);
    }

  void *p = mmap (NULL, 10, PROT_READ, MAP_SHARED, tempfd, 0);
  if (p == MAP_FAILED)
    {
      printf ("%s: mmap failed\n", __func__);
      exit (1);
    }

  int r = pthread_barrier_wait (&b2);
  if (r != 0 && r != PTHREAD_BARRIER_SERIAL_THREAD)
    {
      printf ("%s: barrier_wait failed\n", __func__);
      exit (1);
    }

  r = pthread_barrier_wait (&b2);
  if (r != 0 && r != PTHREAD_BARRIER_SERIAL_THREAD)
    {
      printf ("%s: 2nd barrier_wait failed\n", __func__);
      exit (1);
    }

  pthread_cleanup_push (cl, NULL);

  msync (p, 10, 0);

  pthread_cleanup_pop (0);

  printf ("%s: msync returned\n", __func__);

  unlink(fname);
  free(fname);

  exit (1);
}


static struct
{
  const char *name;
  void *(*tf) (void *);
  int nb;
  int only_early;
} tests[] =
{
#define ADD_TEST(name, nbar, early) { #name, tf_##name, nbar, early }
  ADD_TEST (fsync, 2, 1),
  ADD_TEST (fdatasync, 2, 1),
  ADD_TEST (msync, 2, 1),
};
#define ntest_tf (sizeof (tests) / sizeof (tests[0]))


static int
do_test (void)
{
  int val;
  socklen_t len;

  if (socketpair (AF_UNIX, SOCK_STREAM, PF_UNIX, fds) != 0)
    {
      perror ("socketpair");
      exit (1);
    }

  val = 1;
  len = sizeof(val);
  setsockopt (fds[1], SOL_SOCKET, SO_SNDBUF, &val, sizeof(val));
  if (getsockopt (fds[1], SOL_SOCKET, SO_SNDBUF, &val, &len) < 0)
    {
      perror ("getsockopt");
      exit (1);
    }
  setsockopt (fds[1], SOL_SOCKET, SO_SNDBUF, &val, sizeof(val));

  int result = 0;
  size_t cnt;
  for (cnt = 0; cnt < ntest_tf; ++cnt)
    {
      if (tests[cnt].only_early)
	continue;

      if (pthread_barrier_init (&b2, NULL, tests[cnt].nb) != 0)
	{
	  puts ("b2 init failed");
	  exit (1);
	}

      /* Reset the counter for the cleanup handler.  */
      cl_called = 0;

      pthread_t th;
      if (pthread_create (&th, NULL, tests[cnt].tf, NULL) != 0)
	{
	  printf ("create for '%s' test failed\n", tests[cnt].name);
	  result = 1;
	  continue;
	}

      int r = pthread_barrier_wait (&b2);
      if (r != 0 && r != PTHREAD_BARRIER_SERIAL_THREAD)
	{
	  printf ("%s: barrier_wait failed\n", __func__);
	  result = 1;
	  continue;
	}

      struct timespec  ts = { .tv_sec = 0, .tv_nsec = 100000000 };
      while (nanosleep (&ts, &ts) != 0)
	continue;

      if (pthread_cancel (th) != 0)
	{
	  printf ("cancel for '%s' failed\n", tests[cnt].name);
	  result = 1;
	  continue;
	}

      void *status;
      if (pthread_join (th, &status) != 0)
	{
	  printf ("join for '%s' failed\n", tests[cnt].name);
	  result = 1;
	  continue;
	}
      if (status != PTHREAD_CANCELED)
	{
	  printf ("thread for '%s' not canceled\n", tests[cnt].name);
	  result = 1;
	  continue;
	}

      if (pthread_barrier_destroy (&b2) != 0)
	{
	  puts ("barrier_destroy failed");
	  result = 1;
	  continue;
	}

      if (cl_called == 0)
	{
	  printf ("cleanup handler not called for '%s'\n", tests[cnt].name);
	  result = 1;
	  continue;
	}
      if (cl_called > 1)
	{
	  printf ("cleanup handler called more than once for '%s'\n",
		  tests[cnt].name);
	  result = 1;
	  continue;
	}

      printf ("in-time cancel test of '%s' successful\n", tests[cnt].name);

      if (tempfd != -1)
	{
	  close (tempfd);
	  tempfd = -1;
	}
      if (tempfd2 != -1)
	{
	  close (tempfd2);
	  tempfd2 = -1;
	}
      if (tempfname != NULL)
	{
	  unlink (tempfname);
	  free (tempfname);
	  tempfname = NULL;
	}
      if (tempmsg != -1)
	{
	  msgctl (tempmsg, IPC_RMID, NULL);
	  tempmsg = -1;
	}
    }

  for (cnt = 0; cnt < ntest_tf; ++cnt)
    {
      if (pthread_barrier_init (&b2, NULL, tests[cnt].nb) != 0)
	{
	  puts ("b2 init failed");
	  exit (1);
	}

      /* Reset the counter for the cleanup handler.  */
      cl_called = 0;

      pthread_t th;
      if (pthread_create (&th, NULL, tests[cnt].tf, (void *) 1l) != 0)
	{
	  printf ("create for '%s' test failed\n", tests[cnt].name);
	  result = 1;
	  continue;
	}

      int r = pthread_barrier_wait (&b2);
      if (r != 0 && r != PTHREAD_BARRIER_SERIAL_THREAD)
	{
	  printf ("%s: barrier_wait failed\n", __func__);
	  result = 1;
	  continue;
	}

      if (pthread_cancel (th) != 0)
	{
	  printf ("cancel for '%s' failed\n", tests[cnt].name);
	  result = 1;
	  continue;
	}

      r = pthread_barrier_wait (&b2);
      if (r != 0 && r != PTHREAD_BARRIER_SERIAL_THREAD)
	{
	  printf ("%s: barrier_wait failed\n", __func__);
	  result = 1;
	  continue;
	}

      void *status;
      if (pthread_join (th, &status) != 0)
	{
	  printf ("join for '%s' failed\n", tests[cnt].name);
	  result = 1;
	  continue;
	}
      if (status != PTHREAD_CANCELED)
	{
	  printf ("thread for '%s' not canceled\n", tests[cnt].name);
	  result = 1;
	  continue;
	}

      if (pthread_barrier_destroy (&b2) != 0)
	{
	  puts ("barrier_destroy failed");
	  result = 1;
	  continue;
	}

      if (cl_called == 0)
	{
	  printf ("cleanup handler not called for '%s'\n", tests[cnt].name);
	  result = 2;
	  continue;
	}
      if (cl_called > 1)
	{
	  printf ("cleanup handler called more than once for '%s'\n",
		  tests[cnt].name);
	  result = 1;
	  continue;
	}

      printf ("early cancel test of '%s' successful\n", tests[cnt].name);

      if (tempfd != -1)
	{
	  close (tempfd);
	  tempfd = -1;
	}
      if (tempfd2 != -1)
	{
	  close (tempfd2);
	  tempfd2 = -1;
	}
      if (tempfname != NULL)
	{
	  unlink (tempfname);
	  free (tempfname);
	  tempfname = NULL;
	}
      if (tempmsg != -1)
	{
	  msgctl (tempmsg, IPC_RMID, NULL);
	  tempmsg = -1;
	}
    }

  return result;
}

#define TIMEOUT 60
//#define TEST_FUNCTION do_test ()

int main(void)
{
	return do_test();
}
//#include "test-skeleton.c"
