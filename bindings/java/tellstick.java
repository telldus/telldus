//firstJNI.java

public class tellstick
{
	public final int TELLSTICK_TURNON  = 1;
	public final int TELLSTICK_TURNOFF = 2;
	public final int TELLSTICK_BELL    = 4;
	public final int TELLSTICK_DIM     = 16;

	public native boolean devTurnOn(int intDeviceId);
	public native boolean devTurnOff(int intDeviceId);
	public native boolean devBell(int intDeviceId);
	public native boolean devDim(int intDeviceId, int level);
	public native int devMethods(int intDeviceId);

	public native int devGetNumberOfDevices();
	public native int devGetDeviceId(int intDeviceIndex);

	public native String devGetName(int intDeviceId);

	static {
		System.loadLibrary("tellstickJNI");
	}


}

