#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "defs.h"

#define LENGTH(x) (sizeof(x) / sizeof(x[0]))

void quit(void);
void print(void);
void input(void);
int add(void);
void localRemove(void);
void change(void);
void sort(void);
void numSort(void);
void save(void);
void restore(void);

char *chomp(char *s, char c);

LinkedList *db;

LinkedList *
ll_create(void)
{
	LinkedList *l = malloc(sizeof(LinkedList)) ;
	l->first = malloc(sizeof(LinkedListEl)) ;
	l->first->next = 0 ;
	l->first->data = 0 ;
	l->len = 0 ;
	return l ;
}

void
ll_append(LinkedList *l, void *data)
{
	int i, len;
	len = l->len ;
	LinkedListEl *el = l->first ;
	for( i=0 ; i<len ; ++i)
		el = el->next ;
	el->next = malloc(sizeof(LinkedListEl)) ;
	el->next->data = data ;
	++l->len;
}

void
ll_push(LinkedList *l, void *data)
{
	LinkedListEl *el = l->first->next ;
	l->first->next = malloc(sizeof(LinkedListEl)) ;
	l->first->next->data = data ;
	l->first->next->next = el ;
	++l->len;
}

void *
ll_at(LinkedList *l, unsigned int n)
{
	unsigned int i;
	LinkedListEl *el;

	if(n >= l->len) return 0 ;

	el = l->first->next;
	for( i=0 ; i<n ; ++i )
		el = el->next ;

	return el->data ;
}

int
ll_remove(LinkedList *l, unsigned int n)
{
	int i;
	LinkedListEl *el, *buf;

	if(n>=l->len) return -1 ;

	el = l->first ;
	for( i=0 ; i<n ; ++i)
		el = el->next ;	

	buf = el->next ;
	el->next = el->next->next;
	free(buf);

	--l->len;
	return 0 ;
}

int
ll_insert(LinkedList *l, unsigned int n, void *data)
{
	unsigned int i;
	LinkedListEl *el, *buf;

	if(n > l->len) return -1 ;

	el = l->first ;
	for( i=0 ; i<n ; ++i)
		el = el->next ;

	buf = el->next ;
	el->next = malloc(sizeof(LinkedListEl)) ;
	el->next->data = data ;
	el->next->next = buf ;

	++l->len;
	return 0 ;
}

struct Computer {
	char Manufacture[26], CPUType[26];
	float ClockFreq;
	int Memory, HDDCapacity;
};

typedef struct Computer Computer;

/* Handler to make the program extendable. */
typedef void (*HndlFunc)(void);

struct HndlFuncStruct {
	HndlFunc Hndl;
	char *Desc;
} ;
typedef struct HndlFuncStruct HndlFuncStruct;

/* Handlers themselves. */
HndlFuncStruct handlers[] = {
	{quit, "Quit"},
	{print, "Print"},
	{input, "Input"},
	{add, "Add"},
	{localRemove, "Remove"},
	{change, "Change"},
	{sort, "Sort"},
	{numSort, "Digital sort"},
	{save, "Save"},
	{restore, "Restore"},
};

void
printComputer(Computer *c)
{
	printf(
		"Manufacture: %s\n"
		"CPU type: %s\n" 
		"Clock frequency: %f\n"
		"Memory: %d\n"
		"HDD capacity: %d\n",
		c->Manufacture, c->CPUType,
		c->ClockFreq, c->Memory, c->HDDCapacity
	);
}

void
print()
{
	int i;
	for(i=0 ; i<db->len ; ++i){
		puts("");
		printComputer((Computer *)ll_at(db, i));
	}
	puts("");
}

void
input()
{
	while(!add())
		;
}

char *
readString(char *prompt, char *buf, int siz)
{
	printf("%s", prompt);
	return chomp(fgets(buf, siz, stdin), '\n') ;
}


int
add()
{
	char buf[512];

	Computer *c = malloc(sizeof(*c)) ;

	readString("Manufacture: ", c->Manufacture, sizeof(c->Manufacture));
	if(!strcmp(c->Manufacture, "*")){
		return 1 ;
	}

	readString("CPU type: ", c->CPUType, sizeof(c->CPUType));

	c->ClockFreq =
		atof(readString("Clock frequency: ", buf, sizeof(buf))) ;

	c->Memory =
		atoi(readString("Memory: ", buf, sizeof(buf))) ;

	c->HDDCapacity =
		atoi(readString("HDD capacity: ", buf, sizeof(buf))) ;

	ll_push(db, c);
	return 0 ;
}

void
localRemove()
{
}

void
change()
{
}

void
sort()
{
}

void
numSort()
{
}

void
save()
{
}

void
restore()
{
}

void
quit(void)
{
	exit(0);
}

int
ckIn(int in)
{
	if(in < 0 || in > LENGTH(handlers))
		return 1 ;
	return 0 ;
}

void
printHndls(void)
{
	int i;
	puts("");
	for(i = 0 ; i<LENGTH(handlers) ; ++i){
		printf("%d. %s\n", i, handlers[i].Desc);
	}
}

int
hasNotDigits(char *s)
{
	if(!strlen(s))
		return 1 ;
	while(*s){
		if( !isdigit((int)*s) )
			return 1 ;
		++s;
	}
	return 0 ;
}

char *
chomp(char *s, char c)
{
	int len = strlen(s) ;
	if(s[len-1] == c)
		s[len-1] = 0 ;
	return s ;
}

void
mainLoop(void)
{
	int in, ret;
	char buf[512];
	while(1){
		printHndls();
		printf("> ");
		if(!fgets(buf, sizeof(buf), stdin))
			quit();
		chomp(buf, '\n');

		if(hasNotDigits(buf)){
			puts("Wrong input format");
			continue;
		}
		in = atoi(buf) ;

		if(ckIn(in)){
			puts("No such function");
			continue;
		}
		handlers[in].Hndl();
	}
}

int 
main(int argc, char *argv[])
{
	db = ll_create() ;
	mainLoop();
}
