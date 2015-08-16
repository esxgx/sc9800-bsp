// frame.h - 框架(规范化结构)
#ifndef __FRAME_H
#define __FRAME_H

// 用法:
// 	定义一个框架:
//		_DFa(framename)		// 框架开始
//			u8 a;		// 一字节变量
//			u16 b;		// 二字节变量
//			u32 c;		// 四字节变量
//		_DFz(framename)		// 框架结束
//
//	引用一个框架:
//		_framename	// 这里不需包装，只要定义符合标准，符号畅通便于移植即可
//

#define _DFa(x)		typedef struct x {
#define _DFz(x)		} _ ## x;

#include "typedef.h"

#endif	// __FRAME_H
