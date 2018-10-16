#include "tinbes.h"

#define MEM 512
#define SIZE 1024

void unblockCin() {
  int flags = fcntl(0, F_GETFL, 0) | O_NONBLOCK;
  fcntl(0, F_SETFL, flags);
}

string readLine() {
  string myString;
  int c;
  while ((c = getchar()) != '\n') {
    if (c != EOF) myString += (char)c;
  }
  return myString;
}

bool charactersAvailable() {
  return cin.rdbuf()->sgetc() != -1;
}

unsigned char memory[MEM];
unsigned char drive[SIZE];

void storeByte(unsigned char b, long addr) {
  if (addr >= 0 && addr < MEM) memory[addr] = b;
}

unsigned char recallByte(long addr) {
  // Returns unsigned char on succes, 0 otherwise
  if (addr >= 0 && addr < MEM) return memory[addr];
  return 0;
}

long getMem() {
  return MEM;
}

long execute(long addr) {
  usleep(100000);
  if (rand() % 100) return addr + rand() % 9 + 1;
  else return 0;
}

int writeByte(unsigned char b, long addr) {
  // Returns 0 on succes, -1 otherwise
  if (addr < 0 || addr >= SIZE || b < 0 || b > 255) return -1;
  drive[addr] = b;
  return 0;
}

int readByte(long addr) {
  // Returns unsigned char on succes, -1 otherwise
  if (addr < 0 || addr >= SIZE) return -1;
  return (int)drive[addr];
}

long getSize() {
  return SIZE;
}
