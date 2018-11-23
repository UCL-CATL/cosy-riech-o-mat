Riech-O-Mat olfactometer - UCLouvain report
===========================================

Open/close valves from the computer
-----------------------------------

We intend to use the olfactometer with other devices, with some code in Matlab
to control the devices and synchronize them (Matlab, or other programming
languages).

We have ordered the serial communication unit. With the serial port of the
computer, send some binary data to open/close the valves. But after some tests
and some code written, we came to the conclusion that it was unreliable, or it
would need more code to make it reliable. Basically with serial communication
there is no checks to see if the message was well received (and re-send the
message if not, etc). The checks must be done ourselves, with code.

That's why we have changed our plans and we have ordered the normal unit for
the communication with the PC, with a USB port and
[IO-Warrior](https://www.codemercs.com/en/io). There are several advantages:
- The software is already written by the manufacturer.
- With IO-Warrior and USB, it's the USB protocol itself that checks whether the
  message is well received, re-send the message if not, etc. So it is more
  reliable, and the code is simpler. Welcome to the 21st century.

With Matlab it is easily possible to use the serial port, but it is not the
case for USB/IO-Warrior. The plan is to call the ZeroMQ library to the rescue.
We already have developed a basic ZeroMQ binding for Matlab:
[cosy-zeromq-matlab](https://github.com/UCLouvain-IoNS-CATL/cosy-zeromq-matlab).
By using a simple Request-Reply pattern: the replier is implemented in C (for
example), and we make requests from Matlab (just sending the sequence of five
0's and 1's).

Trigger out to the EEG
----------------------

We initially didn't know, but it's possible to order a Riech-O-Mat unit for the
trigger out. In our case it's for the EEG. We have ordered a parallel port
(DB-25) with the five first data pins connected to the five channels (so
including the airflow valve, we didn't want to exclude possibilities). The data
pin is at 5V when the valve is open, at 0V when the valve is closed. It's as
simple as that. With the five data pins of the parallel port, we can
distinguish the channels in the output of the EEG, so it is better than having
a simple single trigger (e.g. with a BNC port).

So, no need to send the trigger from the computer (it would have been more
complicated: a delay would probably be needed, supposing that the delay is not
variable).

Note: the EEG operates at 1kHz, so the data pin needs to be at 5V during at
least 1ms (if it is less than 1ms the EEG might not see the trigger). The
valves are opened during much more than 1ms (e.g. 333ms or 1s), so no problem.
Also, the EEG recognizes the trigger on the rising edge.

Fork of upstream code
---------------------

The upstream riech-o-mat code is
[here](https://github.com/sommeru/riech-o-mat). Our fork is
[here](https://github.com/UCLouvain-IoNS-CATL/riech-o-mat) on the “uclouvain”
branch. See the commits for more details.

Computer installation
---------------------

The upstream riech-o-mat code runs on Linux, so we use Linux too.

We have automated almost all the installation with Kickstart and Ansible, see
[cosy-linux-automation](https://github.com/UCLouvain-IoNS-CATL/cosy-linux-automation).
