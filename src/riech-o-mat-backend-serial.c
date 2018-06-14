#include <glib.h>
#include <locale.h>
#include "crom-serial.h"

static void
print_usage (const char *program)
{
	g_printerr ("Usage: %s valves_positions\n"
		    "E.g. %s 11000\n",
		    program, program);
}

int
main (int    argc,
      char **argv)
{
	const char *valves_positions;
	int fd;
	GError *error = NULL;

	setlocale (LC_ALL, "");

	if (argc != 2)
	{
		print_usage (argv[0]);
		return EXIT_FAILURE;
	}

	valves_positions = argv[1];
	crom_serial_is_valves_positions_valid (valves_positions, &error);
	if (error != NULL)
	{
		g_printerr ("%s\n\n", error->message);
		print_usage (argv[0]);
		g_clear_error (&error);
		return EXIT_FAILURE;
	}

	fd = crom_serial_open_serial_port ();
	if (fd == -1)
	{
		return EXIT_FAILURE;
	}

	crom_serial_send_valves_positions (fd, valves_positions);

	if (!crom_serial_close_serial_port (fd))
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
