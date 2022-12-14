# **C**





## 预处理（宏定义、文件包含、条件编译）

C语言在对程序进行编译时，会先根据预处理命令进行“预处理”，C语言的编译系统包括预处理、编译和链接等部分。



# **条件编译**

可以根据是否定义了一个变量来进行分支选择，一般用于调试。







## **例1：**

## #ifndef x	// if not define的简写

## #define x	代码段	#endif

**#ifndef x		//先测试x是否被宏定义过**

**#define x		//如果x没有被宏定义过，那么定义x，**

**程序段1			//并编译程序段1**

**#endif**

**程序段2			//如果x已经定义过了，则编译程序段2，“忽略”程序段1**

### 说明：条件指示符#idndef的最主要目的是防止头文件的重复包含和编译。

*条件编译当然也可以用条件语句来实现，但是用条件语句将会对整个源程序进行编译，生成的目标代码程序很长，而采用条件编译，则根据条件只编译其中的程序段1或者程序段2，生成的目标程序较短。如果选择的程序段很长，采用条件编译的方法是十分必要的。*





## #ifndef	标示1	//判断“标志1”是否定义，如果被定义则返回假，如果没有被定义则返回真。

## **例2:**

**语句1	#ifndef	标示1**

**语句2	#define	标示1**

**语句3	#endif**

**语句4	void mian(){	int i = 0;	}**

**…**

### 说明：如果标示1没被定义，则重新定义标示1，即执行语句2、语句3、语句4……；如果标示1已经被定义，则直接跳过语句2、语句3，直接执行语句4……。(#ifndf和#ifndf要一起使用，就像双引号一样，有头有尾)





## 例3：

## 头文件定义

​	***当两个.c文件都include了同一个.h头文件，两个.c文件要一同编译成一个可执行文件，如果没有使用#ifndef，那么会引起大量的声明冲突。***



**#ifndef <标识>**

**#define <标识>**

**……**

**……**

**#endif**

### <标识>在理论上来说可以是自有命名的，但是每个头文件的这个“标识”都应是唯一的，用文件名来命名是最好的。

#### *一般的标识命名规则：以头文件名全大写，前面加下划线，并把文件名中的“.”点也变成下划线，如：stdio.h*

**#ifndef	_STDIO_H**

**#define	_STDIO_H**

**……**

**……**

**#endif**

### 说明：

- ### 在C语言中，对同一个变量或者函数进行多次声明是不会报错的。

- 在C++语音中，#ifndef的作用域只是在当个文件夹中，所以如果H文件里定义了全局变量，即使采用#ifndef宏定义，一个c文件包含同一个h文件多次还是会出现全局变量重定义的错误。

- 使用#ifndef可以避免以下这种错误：

  - 如果在h文件中定义了全局变量，一个c文件包含同一个h文件多次，如果不加#ifndef宏定义，会出现变量重复定义的错误
  - 如果加了#ifndef，则不会出现这种错误



### **“条件编译”命令允许对程序中的内容选择性的编译，即可以根据一定的条件选择是否编译。**

### **条件编译的命令主要有以下几种：**

### 例1：

#ifndef	标识符

程序段1

#else

程序段2

#endif

### 说明：当“标识符”已经由#define定义过了，则编译“程序段2”，否则编译“程序段1”

### 如果不需要编译“程序段2”，则上述形式可以变换为形式2。

### 形式2：

#ifdef	标识符

程序段1

#endif



### 例子2:

#ifndef	标识符

#define	标识符

程序段1

#else

程序段2

#endif

### 说明：他的作用是当“标识符”没有由#define定义过。则编译“程序段1”。否则编译“程序段2”，同样当无“程序段2”时，则上述形式变换为：

#ifndef	标识符

#define	标识符

程序段	1

#endif



### 例子3：

#if	表达式

程序段1

#else

程序段2

#endif

### 说明：他的作用是，当“表达式”值为真时，编译“程序段1”，否则编译“程序段2”。同样，当无“程序段2”时，则上述形式变换为：

#if	表达式

程序段1

#endif



# **以上几种例子的条件编译与处理，结构都可以嵌套使用，当#else后嵌套#if时，可以使用预处理命令#elif，他相当于#else#if。在程序中使用条件预编译，主要是为了方便程序的调试和移植。**



参考：

https://www.cnblogs.com/mupanxi/p/5043707.html
