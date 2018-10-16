#include <stddef.h>
#include <stdint.h>

#include "uart.h"

#define MAX 64

static void *_end = (void *) 0x400000;

struct block
{
    void *addr;
    size_t size;
	struct block* next;
};

static struct block* head = NULL;

char* strcpy( char *dest, const char *src){
    char* tmp = dest;
    while((*dest++=*src++)!='\0');
    return tmp;
}

char* strncpy( char *dest, const char *src,int n){
    char* tmp = dest;
    int i = 0;
    while((*dest++=*src++)&&n>i++);
    return tmp;
}

void *malloc(size_t size){
	struct block* bp = head;
	if(head == NULL) {
		head = bp = _end;
		bp->addr = sizeof(struct block) + _end;
	}
  else {
		for(bp = head; bp->next != NULL; bp = bp->next);
		bp->next = bp->addr + bp->size;
		bp = bp->next;
		bp->addr = bp + sizeof(struct block);
	}
	bp->size = size;
	bp->next = NULL;
	return bp->addr;
}

void *calloc(size_t n, size_t m){
	void *ptr;
	if (!(ptr = malloc(n * m)))
		return NULL;
  return ptr;
}

void free(void *ptr){ //TODO verify
	struct block* bp = head;
	if(bp->next == NULL) {
		if(bp->addr == ptr) {
			bp = NULL;
		}
	} else {
		for(bp = head; bp->next->addr != ptr; bp = bp->next);
		if(bp->next->addr == ptr) {
			struct block* del = bp->next;
			if(del->next != NULL) {
				bp->next = del->next;
			}
			del = NULL;
		}
	}
}

void *realloc(void *ptr, size_t n){ //TODO verify
  void *nptr;
  size_t i;

	if (!(nptr = malloc(n))) {
		free(ptr);
		return NULL;
	}

	if (!ptr)
		return nptr;

  for(bp = head; bp->next != NULL; bp = bp->next){
    if(bp->addr == ptr){
      memmove(nptr,ptr,bp->size);
      free(ptr)
      return nptr;
    }
  }
  return NULL;
}

// A function to copy block of 'n' bytes from source
// address 'src' to destination address 'dest'.
void memmove(void *dest, void *src, size_t n){
  // Typecast src and dest addresses to (char *)
  char *csrc = (char *)src;
  char *cdest = (char *)dest;

  // Create a temporary array to hold data of src
  char *temp[n];
  // Copy data from csrc[] to temp[]
  for (int i=0; i<n; i++)
    temp[i] = csrc[i];

   // Copy data from temp[] to cdest[]
  for (int i=0; i<n; i++)
    cdest[i] = temp[i];
  //free temp[];
}
void main(){ //TODO add tests
	uart_init();
  char *startStr;
	char *str;

	strcpy(startStr, "Welcome\nEnter a command\n");
	uart_puts(startStr);

	uart_puts("Welcome\n");

	if (!(str = malloc(17))) {
		uart_puts("out of memory\n");
		return 0;
	}
  strcpy(str, "malloc() works!\n");
	uart_puts("before realloc(): ");
	uart_puts(str);

	/* Test realloc() */
	if (!(str = realloc(str, 69))) {
		uart_puts("out of memory\n");
		return 0;
	}

	uart_puts("after realloc(): ");
	uart_puts(str);

	if (!(str = calloc(17,2))) {
		uart_puts("out of memory\n");
		return 0;
	}
  free(str);
}
