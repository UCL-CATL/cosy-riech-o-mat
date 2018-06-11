#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/ppdev.h>
#include <linux/parport.h>

#define DEVICE_FILE_PARALLEL_PORT "/dev/parport0"

int
open_parallel_port (void)
{
	int fd;
	int result;
	int mode;

	fd = open (DEVICE_FILE_PARALLEL_PORT, O_RDWR);
	if (fd == -1)
	{
		perror ("Unable to open parallel port “" DEVICE_FILE_PARALLEL_PORT "” - ");
		exit (EXIT_FAILURE);
	}

	result = ioctl (fd, PPEXCL);
	if (result == -1)
	{
		perror ("Unable to register parallel port exclusively - ");
		exit (EXIT_FAILURE);
	}

	result = ioctl (fd, PPCLAIM);
	if (result == -1)
	{
		perror ("Unable to claim parallel port - ");
		exit (EXIT_FAILURE);
	}

	mode = IEEE1284_MODE_BYTE;
	result = ioctl (fd, PPSETMODE, &mode);
	if (result == -1)
	{
		perror ("Unable to set parallel port mode - ");
		exit (EXIT_FAILURE);
	}

	return fd;
}

void
close_parallel_port (int fd)
{
	int result;

	result = ioctl (fd, PPRELEASE);
	if (result == -1)
	{
		perror ("Error when releasing parallel port - ");
		exit (EXIT_FAILURE);
	}

	if (close (fd) == -1)
	{
		perror ("Error when closing parallel port - ");
		exit (EXIT_FAILURE);
	}
}

int
main (void)
{
	int parallel_port_fd;
	unsigned char data;
	int result;
	int ret = EXIT_SUCCESS;

	setlocale (LC_ALL, "");

	parallel_port_fd = open_parallel_port ();

	/* Setting values to the data pins (pins 2 -> 9). */
	data = 0b00010101;
	result = ioctl (parallel_port_fd, PPWDATA, &data);
	if (result == -1)
	{
		perror ("Error when writing to the parallel port - ");
		ret = EXIT_FAILURE;
	}

	close_parallel_port (parallel_port_fd);

	return ret;
}
