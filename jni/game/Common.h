/*
 * Common.h
 *
 *  Created on: 2013-4-18
 *      Author: zhaolei
 */
#include <android/log.h>
#include <sys/time.h>

#ifndef COMMON_H_
#define COMMON_H_
#define LOG(t,...)	__android_log_print(ANDROID_LOG_DEBUG,"gw_debug",t,__VA_ARGS__);

extern int screenW;
extern int screenH;
extern unsigned char Grid;

inline long getCurrentTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

inline int power2(int p) {
	int rval = 1;
	while (rval < p)
		rval <<= 1;
	return rval;
}
static int FPS=120;


class Common {
public:
	static const char* APK_PATH;
	Common();
	virtual ~Common();
};

#endif /* COMMON_H_ */
