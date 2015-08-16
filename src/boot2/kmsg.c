#include <stdarg.h>

// a buffer used for store data
static char kmsg_heap[0x8000];
static unsigned kmsg_tail;

#define KMSG_CLIENT_MAX	64

static void (*kputs_callback[KMSG_CLIENT_MAX])(int, char *);
static int kputs_n_callback;

kmsg_puts_signin(cb)
	void (*cb)(int, char *);
{
	cb(kmsg_tail, kmsg_heap);
	kputs_callback[kputs_n_callback++] = cb;
}

kputs(s)
	char *s;
{
	int n;
	n = 0;
	while(*s) {
		if (*s == '\n') kmsg_heap[kmsg_tail + n++ & 0x7fff] = '\r';
		kmsg_heap[kmsg_tail + n++ & 0x7fff] = *s++;
	}
	int p = kmsg_tail;
	kmsg_tail = (kmsg_tail + n) & 0x7fff;
	int i;
	for(i=0; i<kputs_n_callback; ++i)
		kputs_callback[i](n, kmsg_heap + p);
	return 1;
}

printk(const char *fmt, ...)
{
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	int len=vsprintf(buf, fmt, args);
	va_end(args);
	if (kputs(buf)) return len;
	return 0;
}
