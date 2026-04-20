/*Copyright (c) 2024-2026 MinervaWare LLC*/
/*Copyright (c) 2022-2024 Tristan Wellman*/
#include "util.h"

/*Allocator(s): For now I will just call standard C 
 * 				functions until I do like a bump allocator or something
 */
wAllocator wDefaultAllocator;

_W_HOT void *wDefaultAllocatorProc(int size, wAllocMode mode, 
		void *oldMem) {
	switch(mode) {
		case ALLOC: return malloc(size);
		case RESIZE: {
			WASSERT(oldMem!=NULL, "Passed NULL memory to resize!");
			return realloc(oldMem, size);
		}
		case FREE: {
			WASSERT(oldMem!=NULL, "Passed NULL memory to free!");
			free(oldMem); 
		}
	};
	return NULL;
}

_W_COLD void wInitDefaultAllocator() {
	wDefaultAllocator.allocProc = (WALLOCPROC)wDefaultAllocatorProc;
}

_W_COLD void wSetAllocator(WALLOCPROC proc) {
	wDefaultAllocator.allocProc = proc;
}

_W_HOT void *wAlloc(int _size) {
	return wDefaultAllocator.allocProc((_size), ALLOC, NULL);}
_W_HOT void *wResize(void *_oldMem, int _size) {
	return wDefaultAllocator.allocProc((_size), RESIZE, _oldMem);}
_W_HOT void wFree(void *_oldMem) {
	wDefaultAllocator.allocProc(0, FREE, _oldMem);}
