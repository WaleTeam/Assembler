#ifndef _PROJECT_AS_EMU_H_INCLUDED_
#define _PROJECT_AS_EMU_H_INCLUDED_

typedef int emu_size_t;
typedef int emu_ssize_t;

void emu_init();
emu_ssize_t emu_read(int fh, void *buffer, emu_size_t size);
emu_ssize_t emu_write(int fh, const void *buffer, emu_size_t size);


extern int emu_stdin;
extern int emu_stdout;

#endif //_PROJECT_AS_EMU_H_INCLUDED_