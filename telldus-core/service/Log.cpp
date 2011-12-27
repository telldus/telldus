#include "Log.h"
#include <stdarg.h>

#ifdef _LINUX
#include <syslog.h>
#endif

class Log::PrivateData {
public:
	PrivateData() : logOutput(Log::System) {}

	Log::LogOutput logOutput;

	static Log *instance;
};

Log *Log::PrivateData::instance = 0;

Log::Log()
	:d(new PrivateData)
{
#ifdef _LINUX
	setlogmask(LOG_UPTO(LOG_INFO));
	openlog("telldusd", LOG_CONS, LOG_USER);
#endif
}

Log::~Log() {
	delete d;
#ifdef _LINUX
	closelog();
#endif
}

void Log::debug(const char *fmt, ...) {
	Log *log = Log::instance();
	va_list ap;
	va_start(ap, fmt);
	log->message(Debug, fmt, ap);
	va_end(ap);
}

void Log::notice(const char *fmt, ...) {
	Log *log = Log::instance();
	va_list ap;
	va_start(ap, fmt);
	log->message(Notice, fmt, ap);
	va_end(ap);
}

void Log::warning(const char *fmt, ...) {
	Log *log = Log::instance();
	va_list ap;
	va_start(ap, fmt);
	log->message(Warning, fmt, ap);
	va_end(ap);
}

void Log::error(const char *fmt, ...) {
	Log *log = Log::instance();
	va_list ap;
	va_start(ap, fmt);
	log->message(Error, fmt, ap);
	va_end(ap);
}

void Log::setLogOutput(LogOutput logOutput) {
	Log *log = Log::instance();
	log->d->logOutput = logOutput;
}

void Log::message(Log::LogLevel logLevel, const char *format, va_list ap) const {
	if (d->logOutput == StdOut) {
		FILE *stream = stdout;
		if (logLevel == Warning || logLevel == Error) {
			stream = stderr;
		}
		vfprintf(stream, format, ap);
		fprintf(stream, "\n");
		fflush(stream);
	} else {
#ifdef _LINUX
		switch (logLevel) {
			case Debug:
				vsyslog(LOG_DEBUG, format, ap);
				break;
			case Notice:
				vsyslog(LOG_NOTICE, format, ap);
				break;
			case Warning:
				vsyslog(LOG_WARNING, format, ap);
				break;
			case Error:
				vsyslog(LOG_ERR, format, ap);
				break;
		}
#endif
	}
}

Log *Log::instance() {
	if (PrivateData::instance == 0) {
		PrivateData::instance = new Log();
	}
	return PrivateData::instance;
}
