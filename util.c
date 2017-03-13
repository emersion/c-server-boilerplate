#include <stdio.h>
#include <unistd.h>

ssize_t freadln(FILE *stream, char *buf, size_t max) {
	size_t n = 0;
	while (1) {
		if (n+1 >= max) {
			buf[n] = '\0';
			return n;
		}

		int ch = fgetc(stream);
		if (ch == EOF) {
			return n;
		} else if (ch < 0) {
			return ch;
		} else if (ch == '\r' || ch == '\n') {
			buf[n] = '\0';
			return n+1;
		} else {
			buf[n] = (char) ch;
			n++;
		}
	}
}
