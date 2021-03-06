/*
 * =====================================================================================
 *
 *       Filename:  pic.c
 *
 *    Description:  PIC 相关
 *
 *        Version:  1.0
 *        Created:  2014年11月06日 09时48分25秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Qianyi.lh (liuhuan), qianyi.lh@alibaba-inc.com
 *        Company:  Alibaba-Inc Aliyun
 *
 * =====================================================================================
 */

#include <common.h>

#include "pic.h"

#define IO_PIC1   (0x20)	  // Master (IRQs 0-7)
#define IO_PIC2   (0xA0)	  // Slave  (IRQs 8-15)

#define IO_PIC1C  (IO_PIC1+1)
#define IO_PIC2C  (IO_PIC2+1)

// 设置 8259A 芯片
void init_interrupt_chip(void)
{
        // 重新映射 IRQ 表
        // 两片级联的 Intel 8259A 芯片
        // 主片端口 0x20 0x21
        // 从片端口 0xA0 0xA1
        
        // 初始化主片、从片
        // 0001 0001
        outb(IO_PIC1, 0x11);
        outb(IO_PIC2, 0x11);

        // 设置主片 IRQ 从 0x20(32) 号中断开始
        outb(IO_PIC1C, 0x20);

        // 设置从片 IRQ 从 0x28(40) 号中断开始
        outb(IO_PIC2C, 0x28);
        
        // 设置主片 IR2 引脚连接从片
        outb(IO_PIC1C, 0x04);

        // 告诉从片输出引脚和主片 IR2 号相连
        outb(IO_PIC2C, 0x02);
        
        // 设置主片和从片按照 8086 的方式工作
        outb(IO_PIC1C, 0x01);
        outb(IO_PIC2C, 0x01);
        
        // 设置主从片允许中断
        outb(IO_PIC1C, 0x0);
        outb(IO_PIC2C, 0x0);
}

// 重设 8259A 芯片
void clear_interrupt_chip(uint32_t intr_no)
{
        // 发送中断结束信号给 PICs
        // 按照我们的设置，从 32 号中断起为用户自定义中断
        // 因为单片的 Intel 8259A 芯片只能处理 8 级中断
        // 故大于等于 40 的中断号是由从片处理的
        if (intr_no >= 40) {
                // 发送重设信号给从片
                outb(IO_PIC2, 0x20);
        }
        // 发送重设信号给主片
        outb(IO_PIC1, 0x20);
}

