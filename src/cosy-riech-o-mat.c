#include <glib.h>
#include <locale.h>

/* Beginning of a command line program to execute a punchcard file.
 * It was intended to use it to send the trigger out to the EEG from the
 * parallel port of the computer (doing it from riech-o-mat-backend would maybe
 * be more difficult or less ideal, since a delay is probably needed, so another
 * process would maybe need to be launched to not add the delay to
 * riech-o-mat-backend).
 * But we have canceled this idea, since it is possible to have a trigger out
 * unit directly connected to the Riech-O-Mat (with a parallel port too to have
 * the information of the opened channels).
 */

/* The minimum value for line_duration_ms (so in milliseconds). Maybe a too low
 * value can destroy the olfactometer, I don't know. In case of doubt, it's
 * better not to try :-)
 */
#define LINE_DURATION_MS_MIN (200)

static void
print_usage (const char *program)
{
	g_printerr ("Usage: %s <punchcard_file> <line_duration_ms>\n"
		    "E.g. %s punchcard.txt 1000\n",
		    program, program);
}

int
main (int    argc,
      char **argv)
{
	const char *punchcard_file;
	const char *line_duration_ms_str;
	guint64 line_duration_ms = 0;
	GError *error = NULL;

	setlocale (LC_ALL, "");

	if (argc != 3)
	{
		print_usage (argv[0]);
		return EXIT_FAILURE;
	}

	punchcard_file = argv[1];

	line_duration_ms_str = argv[2];
	g_ascii_string_to_unsigned (line_duration_ms_str,
				    10,
				    LINE_DURATION_MS_MIN,
				    G_MAXUINT64,
				    &line_duration_ms,
				    &error);
	if (error != NULL)
	{
		g_printerr ("Invalid line_duration_ms argument “%s”: %s\n",
			    line_duration_ms_str,
			    error->message);
		g_clear_error (&error);
		return EXIT_FAILURE;
	}

	g_print ("Will execute %s punchcard file "
		 "with %" G_GUINT64_FORMAT " ms for each line containing valve positions.\n",
		 punchcard_file,
		 line_duration_ms);

	return EXIT_SUCCESS;
}
