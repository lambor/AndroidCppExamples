%module Unix

%{
#include <unistd.h>
#include "counter.h"
#include "immutable.h"
#include "enum.h"
#include "point.h"
%}

typedef unsigned int uid_t;
extern uid_t getuid(void);

//全局变量
extern int counter;
extern void inc_counter(void);

//常量
#define MAX_WIDTH 640
%constant int MAX_HEIGHT = 320;

//编译时常量
%javaconst(1);
#define MAX_WIDTH_COMPILE 640
%constant int MAX_HEIGHT_COMPILE = 320;
%javaconst(0);

//只读变量
%immutable;
extern int readOnly;
%mutable;
extern int readWrite;

//枚举
//匿名枚举
enum {ONE = 1, TWO = 2, THREE, FOUR};
//类型安全
enum NumbersSafe {ONE_SAFE = 1, TWO_SAFE = 2, THREE_SAFE, FOUR_SAFE};
//类型不安全
%include "enumtypeunsafe.swg"
enum NumbersUnSafe {ONE_UNSAFE = 1, TWO_UNSAFE = 2, THREE_UNSAFE, FOUR_UNSAFE};
//java 枚举
%include "enums.swg"
enum NumbersJava {ONE_JAVA = 1, TWO_JAVA = 2, THREE_JAVA, FOUR_JAVA};

//结构体
struct Point {
	int x;
	int y;
};