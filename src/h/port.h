// port.h - (规范化结构)
#ifndef __PORT_H
#define __PORT_H

// 用法:
// 	定义一个端口:
//		_DPa(framename)		// 端口开始
//			u8 a;		// 一字节变量
//			u16 b;		// 二字节变量
//			u32 c;		// 四字节变量
//		_DPz(framename)		// 端口结束
//
//	引用一个端口:
//		_framename	// 这里不需包装，只要定义符合标准，符号畅通便于移植即可
//

#include "frame.h"

//
// dox means docks. a port has many docks. a dock is a unit of
// a port. you can use docks to communicate with server.
// 
#define _DPa(x)		_DFa(x)	\
		u32 dox;
		
#define _DPz(x)		_DFz(x)

// check if a dock is vaild.
#define __PDOX(x, y)	((x)->dox >> (y) & 1)

#endif	// __FRAME_H
