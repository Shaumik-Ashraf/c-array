//array.c
//integer array library with bounds/error checking

/** testing success! - see array_test.c for test code
    see array.h for concise documentation!            */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"array.h"

//initializes preallocated param ap for param size elements
void init_array(array_t* ap, int size) {
	ap->array = malloc(size * sizeof(int));
	WARN_EXIT_IF(!ap->array, "malloc failed");
	ap->length = 0;
	ap->capacity = size;
}

//allocates, initializes, and returns new array_t
array_t* new_array(int size) {
	array_t* ret = malloc(sizeof(array_t));
	WARN_EXIT_IF(!ret, "malloc failed");
	
	init_array(ret, size);
	
	return(ret);
}

//resizes array to param new_size; may increase or decrease size
//returns 1 on success, 0 on failure
int array_resize(array_t* ap, int new_size) {
	void* tmp = realloc(ap->array, new_size * sizeof(int));
	WARN_EXIT_IF( !tmp, "realloc failed" );
	
	if( tmp ) {
		WARN_EXIT_IF( new_size < ap->length, "data leak from array resize");
		ap->array = tmp;
		ap->capacity = new_size;
		return(1);
	}
	return(0);
}

//appends param x to end of array param ap
//inits array if necessary
//resizes array if necessary
//returns 1 on success, 0 on failure
int array_append(array_t* ap, int x) {
	if( !ap->array ) {
		init_array(ap, 10);
	}
	
	if( ap->length == ap->capacity ) {
		int e = array_resize(ap, ap->capacity * 2);
		WARN_EXIT_IF(!e, "array resize for append failed");
		if( !e ) { return(0); }
	}
	
	ap->array[ ap->length++ ] = x;
	return(1);
}

//does linear search for param x in array at param ap
//returns index of x in found; returns -1 if not found
int array_lsearch(array_t* ap, int x) {
	int i;
	
	for(i=0; i<ap->length; i++) {
		if( ap->array[i] == x ) {
			return(i);
		}
	}
	
	return(-1);
}

//removes element at index param i from array at param ap
//if param i < 0, wrap once around array in reverse (i = length - i)
//if param p != NULL then sets *p to removed element
//returns 1 if success, 0 if fails (out of bounds)
int array_remove(array_t* ap, int i, int* p) {
	
	if( i < 0 ) {
		i = ap->length + i;
	}
	
	if( (i > ap->length) || (i < 0) ) {
		WARN("index out of bounds for array remove");
		EXIT();
		return(0);
	}
	
	if( p ) { *p = ap->array[i]; }
	
	if( i!=ap->length ) {
		int j;
		for(j=i; j<ap->length-1; j++) {
			ap->array[j] = ap->array[j+1];
		}
	}
	ap->length--;
	
	return(1);
}

//inserts element to index param i in array at param ap
//if param i < 0, wrap once around array in reverse (i = length - i)
//inits array if necessary
//returns 1 if success, 0 if fails (out of bounds)
int array_insert(array_t* ap, int i, int x) {	
	if( !ap->array ) {
		init_array(ap, 10);
	}
	
	if( i < 0 ) {
		i = ap->length + i;
	}
	
	if( i < 0 ) {
		WARN("index out of bounds for array insert");
		EXIT();
		return(0);
	}
	
	if( ap->length == ap->capacity ) {
		int e = array_resize(ap, ap->capacity * 2);
		WARN_EXIT_IF(!e, "array resize for insert failed");
		if( !e ) { return(0); }
	}
	
	int j;
	for(j=ap->length; j>i; j--) {
		ap->array[j] = ap->array[j-1];
	}
	
	ap->array[i] = x;
	ap->length++;
	
	return(1);
}

//returns newly allocated array splice from ap from start (incl.) to end (excl.) with step
//step may be negative; if step == 0 only element in return array is at start
//if end < 0, it is set to array length + end (wraps around in reverse)
//if start < 0, it is set to array length + start (wraps around in reverse)
array_t* array_splice(array_t* ap, int start, int end, int step) {
	array_t* ret = new_array(10);
	int i;
	
	if( start < 0 ) {
		start += ap->length;
	}
	if( end < 0 ) {
		end += ap->length;
	}
	if( (start > ap->length) || (start < 0) ) {
		WARN("start index out of bounds for array splice");
		EXIT();
		return(NULL);
	}
	if( (end > ap->length) || (end < 0) ) {
		WARN("end index out of bounds for array splice");
		EXIT();
		return(NULL);
	}
	
	if( step == 0 ) {
		array_append(ret, ap->array[start]);
	}
	else if( step > 0 ) {
		for(i=start; i<=end; i+=step) {
			array_append(ret, ap->array[i]);
		}
	}
	else { //step < 0
		for(i=start; i>=end; i+=step) {
			array_append(ret, ap->array[i]);
		}
	}
	
	return(ret);
}

//iterates thru all elements in array at param ap, and returns a new array containing those that satisfy (*boolean_lamdba)(int)
//boolean lamdba function pointer must take an parameter of same type as array, and return true if satisfied, false otherwise
//returns NULL if fails
array_t* array_select(array_t* ap, int (*boolean_lambda)(int iter)) {
	array_t* ret = new_array(10);
	int i;
	
	for(i=0; i<ap->length; i++) {
		if( boolean_lambda(ap->array[i]) ) {
			if( !array_append(ret, ap->array[i]) ) {
				destroy_array(ret);
				return(NULL);
			}
		}
	}
	
	return(ret);
}

//writes array at ap to buf upto a maximum of n characters
//printer is a function that takes 3 parameters respectively: buffer, length of buffer, and the value to write, returns bytes written
//start is prefixed to the array, delim is written between each element, and end is suffixed to the array 
int snprintfc_array(char* buf, int n, array_t* ap, const char* start, int (*printer)(char*,int,int), const char* delim, const char* end) {
	int w = 0; //bytes written
	int i;
	
	strncpy(buf, start, n);
	w += strlen(start);
	WARN_IF(w >= n, "snprint_array exceeded buffer");
	if( w >= n ) { return w; }
	
	for(i=0; i+1<ap->length; i++) {
		w += printer(buf+w,n-w,ap->array[i]);
		WARN_IF(w >= n, "snprint_array exceeded buffer");
		if( w >= n ) { return w; }
		
		strncpy(buf+w, delim, n-w);
		w += strlen(delim);
		WARN_IF(w >= n, "snprint_array exceeded buffer");
		if( w >= n ) { return w; }
	}

	w += printer(buf+w,n-w,ap->array[i]);
	WARN_IF(w >= n, "snprint_array exceeded buffer");
	if( w >= n ) { return w; }
	
	strncpy(buf+w, end, n-w);
	w += strlen(end);
	WARN_IF(w >= n, "snprint_array exceeded buffer");
	if( w >= n ) { return w; }
	
	return(w);
}

//printer for snprint_array
static int int_printer(char* buf, int n, int x) {
	return snprintf(buf, n, "%i", x);
}

//prints array to stdout
void print_array(array_t* ap) {
	if( ap->length == 0 ) {
		printf("[]");
	}
	else if(ap->length == 1) {
		printf("[%i]", ap->array[0]);
	}
	else {
		int i;
		printf("[");
		for(i=0; i<ap->length-1; i++) {
			printf("%i, ", ap->array[i]);
		}
		printf("%i]", ap->array[i]);
	}
}

//frees array in struct a; sets length and capacity to 0
void clear_array(array_t* ap) {
	free(ap->array);
	ap->length = 0;
	ap->capacity = 0;
}

//frees array in struct at pointer and frees the struct itself
void destroy_array(array_t* ap) {
	free(ap->array);
	free(ap);
}