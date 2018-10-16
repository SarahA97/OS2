#ifndef tinbes_h
#define tinbes_h

#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fcntl.h>

using namespace std;

void unblockCin();

string readLine();

bool charactersAvailable();

void storeByte(unsigned char b, long addr);

unsigned char recallByte(long addr);

long getMem();

long execute(long addr);

int writeByte(unsigned char b, long addr);

int readByte(long addr);

long getSize();

#endif
