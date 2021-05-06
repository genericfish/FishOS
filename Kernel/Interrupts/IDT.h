#pragma once

void idt_set_gate(u8, u32, u16, u8);
void idt_install();
