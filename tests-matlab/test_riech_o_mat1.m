% Test to write binary data to a file.

fd = fopen('test-output', 'w');

% Doesn't work, I don't know why. The last byte is not written correctly.
% The matlab doc about fprintf() doesn't explain how many characters need
% to follow \x.
%fprintf(fd, '\x55\x56\x00\x00\x00\x02\x01\xae');

% This works, the numbers are written in decimal.
% Open channel 2.
data = [85, 86, 0, 0, 0, 2, 1, 174];
fwrite(fd, data);

fclose(fd);
