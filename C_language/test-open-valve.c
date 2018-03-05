#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#define DEVICE_FILE "/dev/ttyS0"

/* Returns the file descriptor on success or -1 on error. */
int
open_serial_port (void)
{
	int fd;
	struct termios options;

	/* Can try also without O_NDELAY. */
	fd = open (DEVICE_FILE, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		perror ("Unable to open " DEVICE_FILE " - ");
	}
	else
	{
		fcntl (fd, F_SETFL, 0);
	}

	tcgetattr (fd, &options);
	if (cfgetospeed (&options) == B9600)
	{
		printf ("OK, 9600 baud\n");
	}
	else
	{
		printf ("KO\n");
	}


	return fd;
}

int
main (void)
{
	int fd;
	ssize_t n_bytes_written;
	int ret = EXIT_SUCCESS;

	fd = open_serial_port ();
	if (fd == -1)
	{
		return EXIT_FAILURE;
	}


	n_bytes_written = write (fd, "\x55\x56\x00\x00\x00\x02\x01\xae", 8);
	if (n_bytes_written < 0)
	{
		perror ("write() failed - ");
		ret = EXIT_FAILURE;
	}
	else if (n_bytes_written != 8)
	{
		fprintf (stderr,
			 "write() failed: %ld bytes written, should be 8.\n",
			 n_bytes_written);
		ret = EXIT_FAILURE;
	}

	if (close (fd) == -1)
	{
		perror ("close() failed - ");
		ret = EXIT_FAILURE;
	}

	return ret;
}
