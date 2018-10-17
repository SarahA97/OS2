#include <stdlib.h>
#include <string>
#include <iostream>
#include <wiringPi.h>

#define PIN 0

using namespace std;

char decodeMorse(string s) {
  const string morse("EISHVUF*ARL*WPJTNDBXKCYMGZQO");
  int p = -1;
  for (int i = 0; i < s.length(); i++) {
    if (s[i] == '-') p += 1 << (4 - i);
    else p++;
  }
  return morse[p];
}

void wait(int n) {
  delay(100 * n);
}

void dot() {
	// Reset LED to low
	cout << "."<<endl;
	digitalWrite(PIN, LOW);
	digitalWrite(PIN, HIGH);
	// Interval times 1 for scaling
	delay(100);
	digitalWrite(PIN, LOW);
	// Wait 1 times the interval
	wait(1);
}


void dash() {
	// Reset LED to low
	cout << "-";
	digitalWrite(PIN, LOW);
	digitalWrite(PIN, HIGH);
	delay(300);
	digitalWrite(PIN, LOW);
	wait(1);
}

void morse(char c) {
	switch (c)
	{
	case 'a':
		dot();
		dash();
		break;
	case 'b':
		dash();
		dot();
		dot();
		dot();
		break;
	case 'c':
		dash();
		dot();
		dash();
		dot();
		break;
	case 'd':
		dash();
		dot();
		dot();
		break;
	case 'e':
		dot();
		break;
	case 'f':
		dot();
		dot();
		dash();
		dot();
		break;
	case 'g':
		dash();
		dash();
		dot();
		break;
	case 'h':
		dot();
		dot();
		dot();
		dot();
		break;
	case 'i':
		dot();
		dot();
		break;
	case 'j':
		dot();
		dash();
		dash();
		dash();
		break;
	case 'k':
		dash();
		dot();
		dash();
		break;
	case 'l':
		dot();
		dash();
		dot();
		dot();
		break;
	case 'm':
		dash();
		dash();
		break;
	case 'n':
		dash();
		dot();
		break;
	case 'o':
		dash();
		dash();
		dash();
		break;
	case 'p':
		dot();
		dash();
		dash();
		dot();
		break;
	case 'q':
		dash();
		dash();
		dot();
		dash();
		break;
	case 'r':
		dot();
		dash();
		dot();
		break;
	case 's':
		dot();
		dot();
		dot();
		break;
	case 't':
		dash();
		break;
	case 'u':
		dot();
		dot();
		dash();
		break;
	case 'v':
		dot();
		dot();
		dot();
		dash();
		break;
	case 'w':
		dot();
		dash();
		dash();
		break;
	case 'x':
		dash();
		dot();
		dot();
		dash();
		break;
	case 'y':
		dash();
		dot();
		dash();
		dash();
		break;
	case 'z':
		dash();
		dash();
		dot();
		dot();
		break;
	case '0':
		dash();
		dash();
		dash();
		dash();
		dash();
		break;
	case '1':
		dot();
		dash();
		dash();
		dash();
		dash();
		break;
	case '2':
		dot();
		dot();
		dash();
		dash();
		dash();
		break;
	case '3':
		dot();
		dot();
		dot();
		dash();
		dash();
		break;
	case '4':
		dot();
		dot();
		dot();
		dot();
		dash();
		break;
	case '5':
		dot();
		dot();
		dot();
		dot();
		dot();
		break;
	case '6':
		dash();
		dot();
		dot();
		dot();
		dot();
		break;
	case '7':
		dash();
		dash();
		dot();
		dot();
		dot();
		break;
	case '8':
		dash();
		dash();
		dash();
		dot();
		dot();
		break;
	case '9':
		dash();
		dash();
		dash();
		dash();
		dot();
		break;
	case '.':
		dot();
		dash();
		dot();
		dash();
		dot();
		dash();
		break;
	case ',':
		dash();
		dash();
		dot();
		dot();
		dash();
		dash();
		break;
	case '?':
		dot();
		dot();
		dash();
		dash();
		dot();
		dot();
		break;
	case '\'':
		dot();
		dash();
		dash();
		dash();
		dash();
		dot();
		break;
	case '!':
		dash();
		dash();
		dash();
		dot();
		dot();
		dot();
		break;
	case '/':
		dash();
		dot();
		dot();
		dash();
		dot();
		break;
	case '(':
		dash();
		dot();
		dash();
		dash();
		dot();
		break;
	case ')':
		dash();
		dot();
		dash();
		dash();
		dot();
		dash();
		break;
	case '&':
		dot();
		dash();
		dot();
		dot();
		dot();
		break;
	case ':':
		dash();
		dash();
		dash();
		dot();
		dot();
		dot();
		break;
	case ';':
		dash();
		dot();
		dash();
		dot();
		dash();
		dot();
		break;
	case '=':
		dash();
		dot();
		dot();
		dot();
		dash();
		break;
	case '+':
		dot();
		dash();
		dot();
		dash();
		dot();
		break;
	case '-':
		dash();
		dot();
		dot();
		dot();
		dot();
		dash();
		break;
	case '"':
		dot();
		dash();
		dot();
		dot();
		dash();
		dot();
		break;
	case '@':
		dot();
		dash();
		dash();
		dot();
		dash();
		dot();
		break;
	case ' ':
		wait(4);
		break;
	default:
		break;
	}
}

int main() {
  if (wiringPiSetup() < 0) {
    cout << "Unable to setup wiring pi.";
    return 1;
  }
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
	delay(600);
  digitalWrite(PIN, LOW);
  string input;

  cout << "Please enter text:" << endl;
  while(1){
    getline(cin, input);
    for(int i = 0; i < input.length(); i = i + 1){
  	   if(input.at(i) == ' '){
         cout <<endl;
         input = "";
         wait(7);
      }
      else{
  	     morse(input.at(i));
      }
    }
  }
    return 0;
}
