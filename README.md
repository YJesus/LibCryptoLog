# LibCryptoLog
To encrypt log files on the fly from Apache, Nginx, Postfix and more 

There is a common problem when you need to encrypt log files from programs developed by others and lack this feature. You can try to re-adapt the software but, in big projects like Apache, Nginx, Postfix,... is hard to accomplish

libCryptoLog tries to achieve the mission with a different paradigm that could be useful in very wide environments

libCryptoLog hooks fprintf() and write() functions and makes the encryption on the fly, so log files are always encrypted

In short:

1- libCryptoLog hooks fprintf() and write()

2- Takes the argument about to print

3- Send this argument to an external program (helper)

4- The helper transform text (encrypts, change, beautifies or whatever)

5- The returned data from helper is saved in log file

So you can write your own helper to implement encryption, send to another host via ssh or whatever you want.

Example: http://www.security-projects.com/?libCryptoLog___Apache_example
