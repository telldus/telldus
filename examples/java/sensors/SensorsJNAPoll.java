import java.io.*;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.ptr.*;

import java.util.Date;  

public class SensorsJNAPoll
{
	public interface CLibrary extends Library {
		
		static int TELLSTICK_TEMPERATURE = 1;
		static int TELLSTICK_HUMIDITY = 2;
		
		void tdInit();
		void tdClose();
		int tdTurnOn(int deviceId);
		int tdTurnOff(int deviceId);
		int tdSensor(byte[] protocol, int protocolLength, byte[] model, int modelLength, IntByReference id,IntByReference dataTypes);
		int tdSensorValue(byte[] protocol, byte[] model, int id, int dataType, byte[] value, int valueLength, IntByReference timestamp);
	}

	public static void main(String[] args)
	{
		CLibrary lib = (CLibrary)Native.loadLibrary("libtelldus-core.so.2", CLibrary.class);
		lib.tdInit();
		
		IntByReference id = new IntByReference();
		IntByReference dataTypes = new IntByReference();
		
		System.out.println("getting sensors");
		System.out.println("");
		
		byte protocol[] = new byte[20];
		byte model[] = new byte[20];
		
		//check every sensor
		while(lib.tdSensor(protocol, 20, model, 20, id, dataTypes) == 0){
			System.out.println("Sensor: " + Native.toString(protocol) + " " + Native.toString(model));
			
			byte value[] = new byte[20];
			IntByReference timestamp = new IntByReference();
			if((dataTypes.getValue() & CLibrary.TELLSTICK_TEMPERATURE) != 0){
				lib.tdSensorValue(protocol, model, id.getValue(), 1, value, 20, timestamp);
				long timestampvalue = (long)timestamp.getValue() * 1000;
				Date date = new Date(timestampvalue);
				System.out.println("Temperature: " + Native.toString(value) + "C, " + date.toString());
			}
			if((dataTypes.getValue() & CLibrary.TELLSTICK_HUMIDITY) != 0){
				lib.tdSensorValue(protocol, model, id.getValue(), 2, value, 20, timestamp);
				long timestampvalue = (long)timestamp.getValue() * 1000;
				Date date = new Date(timestampvalue);
				System.out.println("Humidity: " + Native.toString(value) + "%, " + date.toString());
			}
			System.out.println("");
		}
		lib.tdClose();
	}
	
}