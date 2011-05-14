// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <tchar.h>

#include <vector>
#include <map>
#include <deque>
#include <algorithm>
#include <list>
#include <assert.h>

extern "C"{
#include "lua\lua.h"
//#include "lua\lstate.h"
#include "lua\lauxlib.h"
#include "lua\lualib.h"
}

// TODO: 在此处引用程序需要的其他头文件
