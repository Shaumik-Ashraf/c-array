//array_test.c
//success!!

/* Quick reference:
	typedef struct {
		int* array;
		int length;
		int capacity;
	} array_t;

	void init_array(array_t* ap, int size);
	array_t* new_array(int size);
	int array_resize(array_t* ap, int new_size);
	int array_append(array_t* ap, int x);
	int array_lsearch(array_t* ap, int x);
	int array_remove(array_t* ap, int i, int* p);
	int array_insert(array_t* ap, int i, int x);
	array_t* array_splice(array_t* ap, int start, int end, int step);
	array_t* array_select(array_t* ap, int (*boolean_lambda)(int iter));
	int snprintfc_array(char* buf, int n, array_t* ap, const char* start, int (*printer)(char*,int,int), const char* delim, const char* end);
	void print_array(array_t* ap);
	void clear_array(array_t* ap);
	void destroy_array(array_t* ap);
 */

#include<stdio.h>
#include<assert.h>
#include<string.h>

#include"array.h"

static void debug_array(array_t* ap) {
	printf("DEBUG: ");
	print_array(ap);
	printf("(l=%i, c=%i)\n", ap->length, ap->capacity);
}

static void assert_array(array_t* ap) {
	assert(ap);
	assert(ap->array);
	assert(ap->capacity);
	assert(ap->length);
}

static int less_than_6(int x) {
	return( x < 6 );
}

static int my_printer(char* buf, int n, int x) {
	static int coord = -1;
	
	return( snprintf(buf, n, "x%i=%i", ++coord, x) );
}

int main(void) {
	
	printf("========Testing Array==========\n");
	
	array_t evens;
	char buf[256];
	
	//test init
	init_array(&evens, 5);
	assert(evens.array);
	assert(evens.capacity);
	
	//test first append
	array_append(&evens, 2);
	assert(evens.array[0]);
	assert(evens.length);
	assert(evens.capacity);
	
	//test print - success!
	//debug_array(&evens);
	
	//test clear
	clear_array(&evens);
	assert( evens.length == 0 );
	assert( evens.capacity == 0 );
	
	//test new
	array_t* primes = new_array(2);
	
	//test auto-init append
	array_append(primes, 2);
	assert_array(primes);
	assert(primes->array[0] == 2);
	debug_array(primes);
	
	//test multiple appends & auto resize
	array_append(primes, 3);
	array_append(primes, 5);
	array_append(primes, 7);
	array_append(primes, 8);
	array_append(primes, 11);
	assert_array(primes);
	debug_array(primes);
	
	//test lsearch, remove
	int x;
	int i = array_lsearch(primes, 8);
	assert(i);
	array_remove(primes, i, &x);
	assert_array(primes);
	assert(x==8);
	debug_array(primes);
	
	//test remove last
	array_remove(primes, -1, NULL);
	assert_array(primes);
	assert( array_lsearch(primes, 11) == -1 );
	debug_array(primes);
	
	//test remove wrap
	array_remove(primes, -2, &x);
	assert_array(primes);
	assert( array_lsearch(primes, x) == -1 );
	debug_array(primes);
	
	//test insert
	array_insert(primes, 2, 5);
	debug_array(primes);
	array_append(primes, 11);
	array_append(primes, 13);
	debug_array(primes);
	
	//test select
	array_t* pl6 = array_select(primes, less_than_6);
	assert_array(pl6);
	assert_array(primes);
	assert( array_lsearch(pl6, 7) == -1 );
	debug_array(pl6);
	debug_array(primes);
	
	//test splice
	array_t* middle = array_splice(primes, 2, 5, 1);
	assert_array(middle);
	assert_array(primes);
	debug_array(middle);
	debug_array(primes);
	
	//array splice with wrap/step
	printf("Should print reverse primes array:\n");
	array_t* reverse = array_splice(primes, -1, 0, -1);
	debug_array(reverse);
	debug_array(primes);

	//test snprintfc
	printf("Testing snprintfc: ");
	snprintfc_array(buf, 255, primes, "<", my_printer, "|", ">\n");
	//assert( strcmp("<x0=2|x1=3>\n",buf)==0 );
	printf("%s", buf);
	
	//test destroy
	destroy_array(middle);
	destroy_array(pl6);
	destroy_array(reverse);
	destroy_array(primes);
	
	printf("========Test Success!==========\n");	
	return(0);
	
}