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
	if (!(ptr = malloc(n * m)))// If malloc can't t find a memory space the size of n*m, return null
		return NULL;

  for (size_t i = 0; i < n + m; i++){
  	// Sets memory to 0
  	((unsigned char *) ptr)[i] = 0;
  }
  return ptr;
}

void *memmove(void *dest, const void *src, size_t n)
{

	char *d = dest;
	const char *s = src;
  uart_puts(n);
	if (d == s){

		return dest;
  }

	if (s > d) {
		while (n--)
			*d++ = *s++;

	} else {
		d += n - 1;
		s += n - 1;

		while (n--)
			*d-- = *s--;
	}


	return dest;
}

void free(void *ptr){
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
	struct block* bp = head;
  void *nptr;

	if (!(nptr = malloc(n))) {
		free(ptr);
		return NULL;
	}


	if (!ptr)// If parameter is null, returns the new pointer
		return nptr;
  //suart_puts(ptr);
  for(bp = head; bp->next != NULL; bp = bp->next){
    if(bp->addr == ptr){
      uart_puts(bp->size);
      memmove(nptr,ptr,bp->size);
      free(ptr);
      return nptr;
    }
  }
  return NULL;
}

void main(){ 
	uart_init();
	char *str;

	if (!(str = malloc(69))) {
		uart_puts("out of memory\n");
		return;
	}

  strcpy(str, "malloc() works\n");
	uart_puts(str);

	if (!(str = calloc(17,2))) {
		uart_puts("out of memory\n");
		return ;
	}
  strcpy(str, "calloc() works\n");
	uart_puts(str);

	if (!(str = realloc(str, 69))) {
		uart_puts("out of memory\n");
		return ;
	}
  strcpy(str, "realloc() works\n");
	uart_puts(str);
  free(str);
  strncpy(str,"strncpy() works strncpy() works",15);
  uart_puts(str);
  free(str);
}
