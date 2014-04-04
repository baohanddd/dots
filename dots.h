#ifndef __DOTS__
#define __DOTS__

#ifdef DOTS_EXPORTS
#define DOTS_API __declspec(dllexport) 
#else
#define DOTS_API __declspec(dllimport) 
#endif

extern DOTS_API void say_hello();

#endif	// __DOTS__