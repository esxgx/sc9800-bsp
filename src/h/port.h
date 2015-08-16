// port.h - (�淶���ṹ)
#ifndef __PORT_H
#define __PORT_H

// �÷�:
// 	����һ���˿�:
//		_DPa(framename)		// �˿ڿ�ʼ
//			u8 a;		// һ�ֽڱ���
//			u16 b;		// ���ֽڱ���
//			u32 c;		// ���ֽڱ���
//		_DPz(framename)		// �˿ڽ���
//
//	����һ���˿�:
//		_framename	// ���ﲻ���װ��ֻҪ������ϱ�׼�����ų�ͨ������ֲ����
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
