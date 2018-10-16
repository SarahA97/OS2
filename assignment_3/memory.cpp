#include "tinbes.h"
#include <math.h>

#define TABLE_SIZE 25

long addr[TABLE_SIZE];
long length[TABLE_SIZE];
int noOfBlocks = 0;

void sort() {
  bool sorted = false;
  long tmp;
  while (!sorted) {
    sorted = true;
    for (int i = 0; i < noOfBlocks - 1; i++) {
      if (addr[i] > addr[i + 1]) {
        temp = addr[i];
        addr[i]=addr[i+1];
        addr[i+1]=temp;
        sorted = false;
      }
    }
  }
}

long findFreeSpace(long size) {
  sort();
  long prev = 0;
  for (int i = 0; i < noOfBlocks; i++) {
    if (addr[i] - prev > size) return prev; //TODO check this
    prev = addr[i];
  }
  if (getMem() - prev > size) return prev;
  return -1;
}

void allocate(long size) {
  int f = findFreeSpace(size);
  if (f == -1) {
    cout << "Not enough memory." << endl;
  }
  else {
    noOfBlocks++;
    addr[noOfBlocks]=f;
    length[noOfBlocks]=size;
    cout << f << endl;
  }
}

void deallocate(long a) {
  bool found = false;
  for (int i = 0; i < noOfBlocks; i++) {
    if (addr[i] == a) {
      found = true;
      noOfBlocks--;
    }
    if (found) {
      addr[i]=NULL;
      length[i]=NULL;
      break;
    }
  }
  if (!found) cout << "Address not allocated." << endl;
}

long freeMem() {
  long total = getMem();
  for (int i = 0; i < noOfBlocks; i++) {
    deallocate(i);
  }
  return total;
}

void storeChar(long a, string data) {
  storeByte(data[0], a);
}

void storeInt(long a, string data) {
  int i = stoi(data);
  storeByte((i & 0xFF00) >> 8, a);
  storeByte(i & 0xFF, a + 1);
}

void storeFloat(long a, string data) {
  float number = stof(data);
  unsigned char sign = 0;
  if (number < 0) {
    number = -number;
    sign = 128;
  }
  long integerPart = number;
  number -= integerPart;
  long fractionPart = 0;
  for (int i = 22; i >= 0; i--) {
    number *= 2;
    if (number >= 1) {
      fractionPart += (1l << i);
      number -= 1;
    }
  }
  unsigned char exponent = 127;
  while (integerPart != 1) {
    if (integerPart > 1) {
      fractionPart = ((integerPart & 1) << 22) + (fractionPart >> 1);
      integerPart = integerPart >> 1;
      exponent++;
    } else {
      integerPart = fractionPart >> 22;
      fractionPart = (fractionPart & 0x3FFFFF) << 1;
      number *= 2;
      if (number > 1) {
        fractionPart++;
        number -= 1;
      }
      exponent--;
    }
  }
  unsigned char b = sign + (exponent >> 1);
  storeByte(b, a);
  b = fractionPart >> 16;
  if (exponent & 1) b += 128;
  storeByte(b, a + 1);
  b = (fractionPart & 0xFF00) >> 8;
  storeByte(b, a + 2);
  b = fractionPart & 0xFF;
  storeByte(b, a + 3);
}

void storeString(long a, string data) {
  int i;
  for (i = 0; i < data.length(); i++) {
    if (data[i] == '\\' && i + 1 < data.length()) {
      a--;
      if (data[++i] == 'n') {
        storeByte('\n', a + i);
        continue;
      }
    }
    storeByte(data[i], a + i);
  }
  storeByte(0, a + i);
}

void store(long a, string type, string data) {
  long size;
  if (type == "CHAR" || type == "STRING") size = 1;
  if (type == "INT") size = 2;
  if (type == "FLOAT") size = 4;

  bool fits = false;
  for (int i = 0; i < noOfBlocks; i++) {
    if (a >= addr[i] && a + size <= addr[i] + length[i]) {
      fits = true;
      break;
    }
  }
  if (!fits) {
    cout << "Variable does not fit in allocated memory." << endl;
    return;
  }
  if (type == "CHAR") {
    storeChar(a, data);
  }
  if (type == "INT") {
    storeInt(a, data);
  }
  if (type == "FLOAT") {
    storeFloat(a, data);
  }
  if (type == "STRING") {
    storeString(a, data);
  }
}

string retrieveChar(long a) {
  string ret;
  int b = recallByte(a);
  // TO DO

  return ret;
}

string retrieveInt(long a) {
  int b1 = recallByte(a);
  int b2 = recallByte(a + 1);
  int i;

  // TO DO

  return to_string(i);
}

string retrieveFloat(long a) {
  unsigned char b = recallByte(a);
  float sign = 1;
  if (b & 128) sign = -1;
  int exponent = (b & 0x7F) << 1;
  b = recallByte(a + 1);
  if (b & 128) exponent++;
  exponent -= 127;
  long mantissa = ((long)b | 128) << 16;
  b = recallByte(a + 2);
  mantissa += (long)b << 8;
  b = recallByte(a + 3);
  mantissa += b;
  float number;

  // TO DO

  return to_string(number);
}

string retrieveString(long a) {
  string ret;
  char c;
  while ((c = (char)recallByte(a++)) != '\0') {

    // TO DO

  }
  return ret;
}

string retrieve(long a, string type) {
  long size;
  if (type == "CHAR" || type == "STRING") size = 1;
  if (type == "INT") size = 2;
  if (type == "FLOAT") size = 4;
  bool addressOK = false;
  for (int i = 0; i < noOfBlocks; i++) {
    if (a >= addr[i] && a + size <= addr[i] + length[i]) {
      addressOK = true;
      break;
    }
  }
  if (!addressOK) {
    return string("Address not in allocated memory.");
  }
  if (type == "CHAR") {
    return retrieveChar(a);
  }
  if (type == "INT") {
    return retrieveInt(a);
  }
  if (type == "FLOAT") {
    return retrieveFloat(a);
  }
  if (type == "STRING") {
    return retrieveString(a);
  }
  return string();
}

int main() {
  string command;
  while (true) {
    command = readLine();
    if (command == "ALLOC") {
      long i = stoi(readLine());
      allocate(i);
    }
    if (command == "DEALLOC") {
      long i = stoi(readLine());
      deallocate(i);
    }
    if (command == "FREEMEM") {
      cout << freeMem() << endl;
    }
    if (command == "SET") {
      long i = stoi(readLine());
      string type = readLine();
      string data = readLine();
      store(i, type, data);
    }
    if (command == "GET") {
      long i = stoi(readLine());
      string type = readLine();
      retrieve(i,type);
    }
  }
}
