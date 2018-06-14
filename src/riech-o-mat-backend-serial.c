#include <glib.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define DEVICE_FILE "/dev/ttyS0"

static void
print_usage (const char *program)
{
	g_printerr ("Usage: %s valves_positions\n"
		    "E.g. %s 11000\n",
		    program, program);
}

static gboolean
is_valves_positions_input_valid (const char  *valves_positions,
				 GError     **error)
{
	gboolean basic_error = FALSE;
	int i;

	g_return_val_if_fail (valves_positions != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	if (strlen (valves_positions) != 5)
	{
		basic_error = TRUE;
		goto handle_basic_error;
	}

	for (i = 0; i < 5; i++)
	{
		if (valves_positions[i] != '0' &&
		    valves_positions[i] != '1')
		{
			basic_error = TRUE;
			goto handle_basic_error;
		}
	}

handle_basic_error:
	if (basic_error)
	{
		g_set_error (error,
			     G_NUMBER_PARSER_ERROR,
			     G_NUMBER_PARSER_ERROR_INVALID,
			     "“%s” is an invalid valves_positions, it must contain five 0's and 1's.",
			     valves_positions);
		return FALSE;
	}

	if (g_str_equal (valves_positions, "10000"))
	{
		g_set_error (error,
			     G_NUMBER_PARSER_ERROR,
			     G_NUMBER_PARSER_ERROR_INVALID,
			     "“10000” is an invalid valves_positions, because it opens *only* the airflow valve, "
			     "which can destroy the olfactometer because the pressure builds up.\n"
			     "So “10000” has not been executed.\n"
			     "See section 2. “Understanding your Riech-O-Mat” in the manual for more details.");
		return FALSE;
	}

	return TRUE;
}

/* Returns the file descriptor, or -1 on failure. */
static int
open_serial_port (void)
{
	int fd;

	fd = open (DEVICE_FILE, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		perror ("Unable to open " DEVICE_FILE " - ");
	}
	else
	{
		fcntl (fd, F_SETFL, 0);
	}

	return fd;
}

static gboolean
check_n_bytes_written (ssize_t n_bytes_written)
{
	if (n_bytes_written < 0)
	{
		perror ("write() failed - ");
		return FALSE;
	}
	else if (n_bytes_written != 8)
	{
		g_printerr ("write() failed: %ld bytes written, should be 8.\n",
			    n_bytes_written);
		return FALSE;
	}

	return TRUE;
}

/* Returns TRUE on success, FALSE otherwise. */
static gboolean
open_valve (int fd,
	    int valve_num)
{
	ssize_t n_bytes_written;

	switch (valve_num)
	{
		case 0:
			n_bytes_written = write (fd, "\x55\x56\x00\x00\x00\x01\x01\xad", 8);
			break;

		case 1:
			n_bytes_written = write (fd, "\x55\x56\x00\x00\x00\x02\x01\xae", 8);
			break;

		case 2:
			n_bytes_written = write (fd, "\x55\x56\x00\x00\x00\x03\x01\xaf", 8);
			break;

		case 3:
			n_bytes_written = write (fd, "\x55\x56\x00\x00\x00\x04\x01\xb0", 8);
			break;

		case 4:
			n_bytes_written = write (fd, "\x55\x56\x00\x00\x00\x05\x01\xb1", 8);
			break;

		default:
			g_assert_not_reached ();
	}

	return check_n_bytes_written (n_bytes_written);
}

/* Returns TRUE on success, FALSE otherwise. */
static gboolean
close_valve (int fd,
	     int valve_num)
{
	ssize_t n_bytes_written;

	switch (valve_num)
	{
		case 0:
			n_bytes_written = write (fd, "\x55\x56\x00\x00\x00\x01\x02\xae", 8);
			break;

		case 1:
			n_bytes_written = write (fd, "\x55\x56\x00\x00\x00\x02\x02\xaf", 8);
			break;

		case 2:
			n_bytes_written = write (fd, "\x55\x56\x00\x00\x00\x03\x02\xb0", 8);
			break;

		case 3:
			n_bytes_written = write (fd, "\x55\x56\x00\x00\x00\x04\x02\xb1", 8);
			break;

		case 4:
			n_bytes_written = write (fd, "\x55\x56\x00\x00\x00\x05\x02\xb2", 8);
			break;

		default:
			g_assert_not_reached ();
	}

	return check_n_bytes_written (n_bytes_written);
}

int
main (int    argc,
      char **argv)
{
	const char *valves_positions;
	int fd;
	int valve_num;
	GError *error = NULL;

	setlocale (LC_ALL, "");

	if (argc != 2)
	{
		print_usage (argv[0]);
		return EXIT_FAILURE;
	}

	valves_positions = argv[1];
	is_valves_positions_input_valid (valves_positions, &error);
	if (error != NULL)
	{
		g_printerr ("%s\n\n", error->message);
		print_usage (argv[0]);
		g_clear_error (&error);
		return EXIT_FAILURE;
	}

	fd = open_serial_port ();
	if (fd == -1)
	{
		return EXIT_FAILURE;
	}

	for (valve_num = 0; valve_num < 5; valve_num++)
	{
		if (valves_positions[valve_num] == '1')
		{
			open_valve (fd, valve_num);
		}
		else
		{
			close_valve (fd, valve_num);
		}
	}

	if (close (fd) == -1)
	{
		perror ("close() failed - ");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
