/*
 * guzhoudiaoke@126.com
 * 2017-10-27
 */

#include "babyos.h"
#include "keyboard.h"
#include "x86.h"
#include "arch.h"

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::do_irq()
{
	uint8 scan_code = inb(0x60);				        /* read scan code */
	if (!m_queue.full())
	{
		m_queue.en_queue(scan_code);	
	}

	outb(0x20, 0x20);
}

void Keyboard::init()
{
    os()->get_console()->kprintf(RED, "Keyboard::init()\n");
	os()->get_arch()->get_8259a()->enable_irq(IRQ_KEYBOARD);		/* enable keyboard interrupt */
	m_queue.init();

	m_shift_l = 0;
	m_shift_r = 0;
	m_leading_e0 = 0;
}

int32 Keyboard::read()
{
	uint8 scan_code = 0, shift = 0;
	uint32 col = 0, key = 0;

	if (!m_queue.empty())
	{
		m_queue.de_queue(&scan_code);

		/* set m_leading_e0 */
		if (scan_code == 0xe0)
		{
			m_leading_e0 = 1;
			return 0;
		}
		
		/* key up */
		if (scan_code & 0x80)
		{
            key = keymap[scan_code & 0x7f][col];
            if (key == K_SHIFT_L) {
                m_shift_l = 0;
            }
            if (key == K_SHIFT_R) {
                m_shift_r = 0;
            }

			return 0;
		}

		shift = m_shift_l || m_shift_r;
		col = shift ? 1 : 0;
		if (m_leading_e0)
		{
			col = 2;
			m_leading_e0 = 0;
		}

		key = keymap[scan_code & 0x7f][col];
		if (key == K_SHIFT_L) {
			m_shift_l = 1;
        }
		if (key == K_SHIFT_R) {
			m_shift_r = 1;
        }
		
		shift = m_shift_l || m_shift_r;

        if (key == K_ENTER) {
            key = '\n';
        }
        else if (key == K_TAB) {
            key = '\t';
        }
        else if (key == K_BACKSPACE) {
            key = '\b';
        }
        else if (KEY_TYPE(key) != KT_ASCII) {
            return 0;
        }
        return key;
	}

    return 0;
}

