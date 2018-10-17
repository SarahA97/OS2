#include "tinbes.h"

#define TABLE_SIZE 25
#define RUNNING 'r'
#define PAUSED 'p'

string processName[TABLE_SIZE];
int id[TABLE_SIZE];
char state[TABLE_SIZE];
long addr[TABLE_SIZE];
int noOfProcesses = 0;
int idCounter = 0;

int newProcess(string name, long address) {
  processName[noOfProcesses]=name;
  id[noOfProcesses]=idCounter;
  addr[noOfProcesses]=address;
  state[noOfProcesses]=RUNNING;
  noOfProcesses++;
  return idCounter++;
}

int findProcess(int i) {
  for (int j = 0; j < noOfProcesses; j++) {
    if(id[j]=i){
      return j;
    }

  }
  return -1;
}

void removeProcess(int i) {
  int j = findProcess(i);
  noOfProcesses--;
  for (int j = i; j < noOfProcesses; j++) {
    if(i==id[j]){
      id[j]=NULL;
      processName[j]="";
      addr[j]=NULL;
      state[j]=NULL;
    }

  }
}

void executeProcesses() {
  long newAddr;
  for (int i = 0; i < noOfProcesses; i++) {
	cout << "state " <<state[i] << endl;
    if (state[i] == RUNNING) {
      newAddr = execute(addr[i]);
      cout << " addr " <<newAddr<<endl;
      if (newAddr == 0) {
        cout << "Process \"" << processName[i] << "\" has terminated." << endl;
        removeProcess(i--);
      } else {
        addr[i] = newAddr;
      }
    }
  }
}

void listProcesses() {
  for (int i = 0; i < noOfProcesses; i++) {
    cout << id[i] << ' ' << processName[i] << ' ' << state[i] << endl;
  }
}

void suspendProcess(int i) {
  int j = findProcess(i);
  if (j == -1) {
    cout << "Process does not exist." << endl;
    return;
  }
  if (state[j] == PAUSED) {
    cout << "Process already paused." << endl;
  }
  else {
    state[j]=PAUSED;

  }
}

void resumeProcess(int i) {
  int j = findProcess(i);
  if (j == -1) {
    cout << "Process does not exist." << endl;
    return;
  }
  if (state[j] == RUNNING) {
    cout << "Process already running." << endl;
  }
  else {
    state[j]=RUNNING;
  }
}

void killProcess(int i) {
  int j = findProcess(i);
  if (j == -1) {
    cout << "Process does not exist." << endl;
  } else {
    removeProcess(j);
  }
}

int main() {
  unblockCin();
  string command;
  while (true) {
    if (charactersAvailable()) {
      command = readLine();
      if (command == "RUN") {
        string name = readLine();
        long address = stoi(readLine());
        cout << newProcess(name, address) << endl;
      }
      if (command == "LIST") {
        listProcesses();
      }
      if (command == "SUSPEND") {
        int i = stoi(readLine());
        suspendProcess(i);
      }
      if (command == "RESUME") {
        int i = stoi(readLine());
        resumeProcess(i);
      }
      if (command == "KILL") {
        int i = stoi(readLine());
        killProcess(i);
      }
    }
    else {
      executeProcesses();
    }
  }
}
