#include <stdarg.h>
#include <string.h>

int sprintf(char * buf, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int i=vsprintf(buf, fmt, args);
	va_end(args);
	return i;
}

