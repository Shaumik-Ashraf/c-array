//array.h
//integer array library with bounds/error checking
//maybe make type generic later
//todo: make array_t* new_array_va(...) function later;

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

#ifndef _ARRAY_H_
#define _ARRAY_H_

#ifdef ARRAY_EXIT_ON_ERROR
	#define EXIT() exit(1)
#else
	#define EXIT()
#endif

#ifdef ARRAY_WARN_ON_ERROR
	#define WARN(M) fprintf(stderr, "Warning from %s line %s: %s\n", __file__, __line__, M)
#else
	#define WARN(M)
#endif

#define WARN_IF(C, M) (C ? WARN(M) : 0)
#define EXIT_IF(C) (C ? EXIT() : 0)
#define WARN_EXIT_IF(C, M) while(C) { WARN(M); EXIT(); break; }

typedef struct {
	int* array;
	int length;
	int capacity;
} array_t;

//initializes preallocated param ap for param size elements
void init_array(array_t* ap, int size);

//allocates, initializes, and returns new array_t
array_t* new_array(int size);

//resizes array to param new_size; may increase or decrease size
//returns 1 on success, 0 on failure
int array_resize(array_t* ap, int new_size);

//appends param x to end of array param ap
//inits array if necessary
//resizes array if necessary
//returns 1 on success, 0 on failure
int array_append(array_t* ap, int x);

//does linear search for param x in array at param ap
//returns index of x in found; returns -1 if not found
int array_lsearch(array_t* ap, int x);

//removes element at index param i from array at param ap
//if param i < 0, wrap once around array in reverse (i = length - i)
//if param p != NULL then sets *p to removed element
//returns 1 if success, 0 if fails (out of bounds)
int array_remove(array_t* ap, int i, int* p);

//inserts element to index param i in array at param ap
//if param i < 0, wrap once around array in reverse (i = length - i)
//inits array if necessary
//returns 1 if success, 0 if fails (out of bounds)
int array_insert(array_t* ap, int i, int x);

//returns newly allocated array splice from ap from start (incl.) to end (excl.) with step
//step may be negative; if step == 0 only element in return array is at start
//if end < 0, it is set to array length + 1 - end (wraps around in reverse)
//if start < 0, it is set to array length - start (wraps around in reverse)
array_t* array_splice(array_t* ap, int start, int end, int step);

//iterates thru all elements in array at param ap, and returns a new array containing those that satisfy (*boolean_lamdba)(int)
//boolean lamdba function pointer must take an parameter of same type as array, and return true if satisfied, false otherwise
//returns NULL if fails
array_t* array_select(array_t* ap, int (*boolean_lambda)(int iter));

//writes array at ap to buf upto a maximum of n characters
//printer is a function that takes 3 parameters respectively: buffer, length of buffer, and the value to write, returns bytes written
//start is prefixed to the array, delim is written between each element, and end is suffixed to the array 
int snprintfc_array(char* buf, int n, array_t* ap, const char* start, int (*printer)(char*,int,int), const char* delim, const char* end);

//prints array to stdout, upto param limit characters
void print_array(array_t* ap);

//frees array in struct a; sets length and capacity to 0
void clear_array(array_t* ap);

//frees array in struct at pointer and frees the struct itself
void destroy_array(array_t* ap);


#endif