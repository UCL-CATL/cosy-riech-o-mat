#ifndef CROM_SERIAL_H
#define CROM_SERIAL_H

#include <glib.h>

G_BEGIN_DECLS

gboolean	crom_serial_is_valves_positions_valid	(const char  *valves_positions,
							 GError     **error);

int		crom_serial_open_serial_port		(void);

void		crom_serial_send_valves_positions	(int         fd,
							 const char *valves_positions);

gboolean	crom_serial_close_serial_port		(int fd);

G_END_DECLS

#endif /* CROM_SERIAL_H */
