// frame.h - ���(�淶���ṹ)
#ifndef __FRAME_H
#define __FRAME_H

// �÷�:
// 	����һ�����:
//		_DFa(framename)		// ��ܿ�ʼ
//			u8 a;		// һ�ֽڱ���
//			u16 b;		// ���ֽڱ���
//			u32 c;		// ���ֽڱ���
//		_DFz(framename)		// ��ܽ���
//
//	����һ�����:
//		_framename	// ���ﲻ���װ��ֻҪ������ϱ�׼�����ų�ͨ������ֲ����
//

#define _DFa(x)		typedef struct x {
#define _DFz(x)		} _ ## x;

#include "typedef.h"

#endif	// __FRAME_H
