% Must be: baud rate 9600, 8 data bits, 1 stop bit, no parity (all default
% values).
% Reduce timeout to 1 second. In riech-o-mat-backend-iowarrior.c (so in
% USB), timeout of 10 ms (but by serial port the value should probably be
% different).
serial_port = serial('/dev/ttyS0', ...
                     'Timeout', 1);
fopen(serial_port);
fprintf(serial_port, '\x55\x56\x00\x00\x00\x02\x01\xae');
fclose(serial_port);
