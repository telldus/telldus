/*tellstick.c*/
#include "tellstick.h"
#include <stdio.h>
#include <TellUsbD101.h>

JNIEXPORT jboolean JNICALL
Java_tellstick_devTurnOn(JNIEnv *, jobject, jint intDeviceId)
{
	return (jboolean) devTurnOn( (int)intDeviceId);
}


JNIEXPORT jboolean JNICALL
Java_tellstick_devTurnOff(JNIEnv *, jobject, jint intDeviceId)
{
	return (jboolean) devTurnOff( (int)intDeviceId);
}


JNIEXPORT jboolean JNICALL
Java_tellstick_devBell(JNIEnv *, jobject, jint intDeviceId)
{
	return (jboolean) devBell( (int)intDeviceId);
}


JNIEXPORT jboolean JNICALL
Java_tellstick_devDim(JNIEnv *, jobject, jint intDeviceId, jint level)
{
	return (jboolean) devDim( (int)intDeviceId,  (unsigned char) level);
}


JNIEXPORT jint JNICALL
Java_tellstick_devMethods(JNIEnv *, jobject, jint intDeviceId)
{
	return (jint) devMethods( (int)intDeviceId );
}


JNIEXPORT jint JNICALL
Java_tellstick_devGetNumberOfDevices(JNIEnv *, jobject)
{
	return (jint)devGetNumberOfDevices();
}


JNIEXPORT jint JNICALL
Java_tellstick_devGetDeviceId(JNIEnv *, jobject, jint intDeviceIndex)
{
	return (jint)devGetDeviceId( (int)intDeviceIndex );
}


JNIEXPORT jstring JNICALL
Java_tellstick_devGetName(JNIEnv *env, jobject, jint intDeviceId)
{
	const char *name = devGetName( (int)intDeviceId );
	return env->NewStringUTF(name);
}
