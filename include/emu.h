#ifndef _PROJECT_AS_EMU_H_INCLUDED_
#define _PROJECT_AS_EMU_H_INCLUDED_

typedef int emu_size_t;
typedef int emu_ssize_t;

void emu_init();
emu_ssize_t emu_read(int fh, void *buffer, emu_size_t size);
emu_ssize_t emu_write(int fh, const void *buffer, emu_size_t size);

void *emu_memcpy(void *dest, const void *src, emu_size_t n);
void *emu_memset( void * ptr, int value, emu_size_t num );

void *emu_malloc(emu_size_t size);
void *emu_realloc(void *ptr, emu_size_t size);
void emu_free(void *ptr);

void emu_log(char *log);


extern int emu_stdin;
extern int emu_stdout;

#endif //_PROJECT_AS_EMU_H_INCLUDED_