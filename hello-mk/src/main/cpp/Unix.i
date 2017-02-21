%module(directors="1") Unix

%{
#include <unistd.h>
#include "counter.h"
#include "immutable.h"
#include "enum.h"
#include "point.h"
#include "argument.h"
#include "class.h"

class AsyncUidProvider
{
public:
	AsyncUidProvider() {}

	virtual ~AsyncUidProvider() {}

	void get()
	{
		onUid(getuid());
	}

	virtual void onUid(uid_t uid)
	{
	}
};
%}

typedef unsigned int uid_t;

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

//指针 引用 和 值
void drawByPointer(struct Point* p);
void drawByReference(struct Point& p);
void drawByValue(struct Point p);

//默认参数
void func(int a,int b,int c=3);

//重载函数
void func(double d);
void func(int i);

//类
class A {
public:
    A();
    A(int value);
    ~A();

    void print();

    int value;
private:
    void reset();
};

//异常处理
%exception getuid {
	$action
	if (!result) {
		jclass clazz = jenv->FindClass("java/lang/NullPointerException");
        jenv->ThrowNew(clazz,"out of memory.");
        return $null;
	}
}
extern uid_t getuid(void);

//从原生代码调用java
%feature("director") AsyncUidProvider;
class AsyncUidProvider {
public:
	AsyncUidProvider();
	virtual ~AsyncUidProvider();

	void get();
	virtual void onUid(uid_t uid) = 0;
};

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("hellomk");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. \n" + e);
      System.exit(1);
    }
  }
%}