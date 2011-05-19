using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace SensorPollWin
{
	unsafe class Program
	{
		[DllImport("TelldusCore.dll")]
		public static extern int tdTurnOn(int deviceId);
		
		[DllImport("TelldusCore.dll")]
		public static extern int tdSensor(char* protocol, int protocolLength, char* model, int modelLength, int* id, int* dataTypes);
		
		[DllImport("TelldusCore.dll")]
		public static extern int tdSensorValue(char* protocol, char* model, int id, int dataType, char* value, int valueLength, int* timestamp);

		static unsafe void Main(string[] args)
		{
			int protocolstringsize = 20;
			int modelstringsize = 20;
			int valuestringsize = 20;
			char* protocol = (char*)Marshal.AllocHGlobal(Marshal.SystemDefaultCharSize * protocolstringsize);
			char* model = (char*)Marshal.AllocHGlobal(Marshal.SystemDefaultCharSize * modelstringsize);
			IntPtr id = Marshal.AllocHGlobal(sizeof(int));
			IntPtr dataType = Marshal.AllocHGlobal(sizeof(int));
			Console.WriteLine("getting sensors");

			while (tdSensor(protocol, protocolstringsize, model, modelstringsize, (int*)id, (int*)dataType) == 0)
			{
				Console.WriteLine("Sensor: " + System.Text.Encoding.UTF8.GetString(System.Text.Encoding.Unicode.GetBytes(new string(protocol))) + " " + System.Text.Encoding.UTF8.GetString(System.Text.Encoding.Unicode.GetBytes(new string(model))));

				char* value = (char*)Marshal.AllocHGlobal(Marshal.SystemDefaultCharSize * valuestringsize);
				IntPtr timestamp = Marshal.AllocHGlobal(sizeof(int));
				if ((Marshal.ReadIntPtr(dataType).ToInt32() & 1) != 0)
				{
					tdSensorValue(protocol, model, Marshal.ReadIntPtr(id).ToInt32(), 1, value, valuestringsize, (int*)timestamp);
					Console.WriteLine("Temperature: " + System.Text.Encoding.UTF8.GetString(System.Text.Encoding.Unicode.GetBytes(new string(value))) + "C, " + datify(Marshal.ReadIntPtr(timestamp).ToInt32()));
				}
				if ((Marshal.ReadIntPtr(dataType).ToInt32() & 2) != 0)
				{
					tdSensorValue(protocol, model, Marshal.ReadIntPtr(id).ToInt32(), 2, value, valuestringsize, (int*)timestamp);
					Console.WriteLine("Humidity: " + System.Text.Encoding.UTF8.GetString(System.Text.Encoding.Unicode.GetBytes(new string(value))) + "%, " + datify(Marshal.ReadIntPtr(timestamp).ToInt32()));
				}
				Console.WriteLine("");
				Marshal.FreeHGlobal((IntPtr)value);
				Marshal.FreeHGlobal(timestamp);
			}
			Marshal.FreeHGlobal((IntPtr)protocol);
			Marshal.FreeHGlobal((IntPtr)model);
			Marshal.FreeHGlobal(id);
			Marshal.FreeHGlobal(dataType);
		}

		private static string datify(int timestamp){
			System.DateTime dateTime = new System.DateTime(1970, 1, 1, 0, 0, 0, 0);
			dateTime = dateTime.AddSeconds(timestamp);
			dateTime = dateTime.ToLocalTime();
			return dateTime.ToString();
		}
	}
}
