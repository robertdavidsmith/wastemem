# Wastemem

A simple C program that just allocates memory, writes junk to it, then sleeps.

Useful for examining how the amount of free memory affects the performance of other applications.

Example usage (waste 2GiB in 128MiB chunks for one hour):
```
git clone https://github.com/robertdavidsmith/wastemem
cd wastemem
gcc -o wastemem wastemem.c
./wastemem 2048 134217728 3600
```
