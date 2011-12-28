#ifndef LOG_H
#define LOG_H

#include <stdio.h>

class Log {
public:
	enum LogLevel { Debug, Notice, Warning, Error };
	enum LogOutput { StdOut, System };
	virtual ~Log();

	static void destroy();

	static void debug(const char *fmt, ...);
	static void notice(const char *fmt, ...);
	static void warning(const char *fmt, ...);
	static void error(const char *fmt, ...);

	static void setDebug();
	static void setLogOutput(LogOutput logOutput);

protected:
	Log();
	void message(LogLevel logLevel, const char *format, va_list ap) const;
	static Log *instance();

private:
	class PrivateData;
	PrivateData *d;
};


#endif //LOG_H
