#ifndef INTER_COMMANDS_H
#define INTER_COMMANDS_H

struct interrupt_frame;
void* isr80h_command10_echo(struct interrupt_frame* frame);

#endif