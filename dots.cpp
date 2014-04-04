// dots.c : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

extern DOTS_API void say_hello() {
	printf("Hello from DLL !\n");
}


