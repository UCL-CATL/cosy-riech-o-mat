% On Linux it's possible that this script needs to be run as root, otherwise it
% cannot create a lock file (even if the user is in the dialout group).

% Must be: baud rate 9600, 8 data bits, 1 stop bit, no parity (all default
% values).
% Reduce timeout to 1 second. In riech-o-mat-backend-iowarrior.c (so in
% USB), timeout of 10 ms (but by serial port the value should probably be
% different).
serial_port = serial('/dev/ttyS0', ...
                     'Timeout', 1);
fopen(serial_port);

% The numbers are written in decimal.
% Open channel 2.
% It works!
data = [85, 86, 0, 0, 0, 2, 1, 174];
fwrite(serial_port, data);

fclose(serial_port);
