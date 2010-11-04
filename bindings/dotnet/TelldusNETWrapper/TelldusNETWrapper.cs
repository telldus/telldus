using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace TelldusWrapper
{
	public sealed class TelldusNETWrapper	//don't allow inheritance
	{
		int lastEventID = 0;
		Dictionary<int, Object> callbackFunctionReferenceList = new Dictionary<int, Object>();	//this prevents errors in "unknown module" due to callback functions being garbage collected since there are no reference to them

		//TODO: reference the correct DLL
		//TODO: compile towards correct platform (x86/64?) "Any" working?
		//TODO: kommentera... både wrapper och exempel...
		//TODO: kommentera i början av filen...
		//error handling? Or just throw it upwards?
		//close och init (om den skulle göras alls) görs här i wrappern, de exporteras inte vidare... eller ska de? Inte heller connected/disconnected tellstick
		//TODO, use getNumberOfDevices as an example, outside the wrapper, service not running = -6, handle this here or later? Negative values = error
		//throwing System.DllNotFoundException if dll's not ready... Handle here or in program? Might aswell keep that error message
			

		//Device methods
		public const int TELLSTICK_TURNON = 1;
		public const int TELLSTICK_TURNOFF = 2;
		public const int TELLSTICK_BELL = 4;
		public const int TELLSTICK_TOGGLE = 8;
		public const int TELLSTICK_DIM = 16;
		public const int TELLSTICK_LEARN = 32;

		//Error codes
		public const int TELLSTICK_SUCCESS = 0;
		public const int TELLSTICK_ERROR_NOT_FOUND = -1;
		public const int TELLSTICK_ERROR_PERMISSION_DENIED = -2;
		public const int TELLSTICK_ERROR_DEVICE_NOT_FOUND = -3;
		public const int TELLSTICK_ERROR_METHOD_NOT_SUPPORTED = -4;
		public const int TELLSTICK_ERROR_COMMUNICATION = -5;
		public const int TELLSTICK_ERROR_CONNECTING_SERVICE = -6;
		public const int TELLSTICK_ERROR_UNKNOWN_RESPONSE = -7;
		public const int TELLSTICK_ERROR_UNKNOWN = -99;

		//Device typedef
		public const int TELLSTICK_TYPE_DEVICE = 1;
		public const int TELLSTICK_TYPE_GROUP = 2;

		//Device changes
		public const int TELLSTICK_DEVICE_ADDED = 1;
		public const int TELLSTICK_DEVICE_CHANGED = 2;
		public const int TELLSTICK_DEVICE_REMOVED = 3;
		public const int TELLSTICK_DEVICE_STATE_CHANGED = 4;

		//Change types
		public const int TELLSTICK_CHANGE_NAME = 1;
		public const int TELLSTICK_CHANGE_PROTOCOL = 2;
		public const int TELLSTICK_CHANGE_MODEL = 3;


		//variables for event callback (e.g. turnon, turnoff)
		public delegate int EventCallbackFunction(int deviceId, int method, string data, int callbackId, Object obj);
		Dictionary<int, EventCallbackFunction> eventList = new Dictionary<int, EventCallbackFunction>();
		GCHandle eventContextHandle;
		int registeredEventFunctionId = -1;

		//variables for device event callback (e.g. change of name/protocol)
		public delegate int DeviceChangeEventCallbackFunction(int deviceId, int changeEvent, int changeType, int callbackId, Object obj);
		Dictionary<int, DeviceChangeEventCallbackFunction> deviceChangeEventList = new Dictionary<int, DeviceChangeEventCallbackFunction>();
		GCHandle deviceChangeEventContextHandle;
		int registeredDeviceChangeEventFunctionId = -1;

		//variables for raw controller listening callback (e.g. Tellstick Duo receives data)
		public delegate int RawListeningCallbackFunction(string data, int controllerId, int callbackId, Object obj);
		Dictionary<int, RawListeningCallbackFunction> rawListenerList = new Dictionary<int, RawListeningCallbackFunction>();
		GCHandle rawListenerContextHandle;
		int registeredRawListenerFunctionId = -1;


		public TelldusNETWrapper()
		{
		}

		~TelldusNETWrapper()
		{
			//clean up
			if (registeredEventFunctionId != -1)
			{
				UnmanagedImport.tdUnregisterCallback(registeredEventFunctionId);
			}
			if (registeredDeviceChangeEventFunctionId != -1)
			{
				UnmanagedImport.tdUnregisterCallback(registeredDeviceChangeEventFunctionId);
			}
			if (registeredRawListenerFunctionId != -1)
			{
				UnmanagedImport.tdUnregisterCallback(registeredRawListenerFunctionId);
			}
			UnmanagedImport.tdClose();	//Close the library and clean up the cache it uses.
			if (eventContextHandle.IsAllocated)
			{
				eventContextHandle.Free();
			}
			if (deviceChangeEventContextHandle.IsAllocated)
			{
				deviceChangeEventContextHandle.Free();
			}
			if (rawListenerContextHandle.IsAllocated)
			{
				rawListenerContextHandle.Free();
			}
		}

		/// <summary>
		/// Imported functions from TelldusCore.dll
		/// </summary>
		private sealed class UnmanagedImport
		{
			private UnmanagedImport() { }		//cannot instantiate this class

			#region Unmanaged Code
			[DllImport("TelldusCore.dll")]
			public static extern int tdGetNumberOfDevices();

			[DllImport("TelldusCore.dll")]
			public static extern int tdGetDeviceId(int value);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern char* tdGetName(int deviceId);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern char* tdGetProtocol(int deviceId);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern char* tdGetModel(int deviceId);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern char* tdGetDeviceParameter(int deviceId, char* name, char* defaultValue);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern bool tdSetName(int deviceId, char* name);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern bool tdSetProtocol(int deviceId, char* protocol);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern bool tdSetModel(int deviceId, char* model);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern bool tdSetDeviceParameter(int deviceId, char* name, char* value);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern int tdAddDevice();

			[DllImport("TelldusCore.dll")]
			public static unsafe extern bool tdRemoveDevice(int deviceId);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern int tdMethods(int deviceId, int methodsSupported);

			[DllImport("TelldusCore.dll")]
			public static extern int tdTurnOn(int deviceId);

			[DllImport("TelldusCore.dll")]
			public static extern int tdTurnOff(int deviceId);

			[DllImport("TelldusCore.dll")]
			public static extern int tdBell(int deviceId);

			[DllImport("TelldusCore.dll")]
			public static extern int tdDim(int deviceId, char level);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern char* tdGetErrorString(int errorNo);

			[DllImport("TelldusCore.dll")]
			public static extern void tdClose();

			[DllImport("TelldusCore.dll")]
			public static extern void tdInit();

			[DllImport("TelldusCore.dll")]
			public static unsafe extern int tdRegisterDeviceEvent(Delegate eventFunction, void* context);

			[DllImport("TelldusCore.dll")]
			public static extern int tdLastSentCommand(int deviceId, int methods);

			[DllImport("TelldusCore.dll")]
			public static extern int tdGetDeviceType(int deviceId);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern int tdSendRawCommand(char* command, int reserved);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern int tdRegisterRawDeviceEvent(Delegate rawListeningFunction, void* context);

			[DllImport("TelldusCore.dll")]
			public static extern int tdLearn(int deviceId);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern char* tdLastSentValue(int deviceId);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern void tdReleaseString(char* value);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern int tdUnregisterCallback(int eventId);

			[DllImport("TelldusCore.dll")]
			public static unsafe extern int tdRegisterDeviceChangeEvent(Delegate deviceEventFunction, void* context);

			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public unsafe delegate int EventFunctionDelegate(int deviceId, int method, char* data, int callbackId, void* context);

			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public unsafe delegate int DeviceChangeEventFunctionDelegate(int deviceId, int changeEvent, int changeType, int callbackId, void* context);

			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public unsafe delegate int RawListeningDelegate(char* data, int controllerId, int callbackId, void* context);

			#endregion
		}

		#region Public Methods

		/// <summary>
		/// Add a new device to the global database of devices. This function must be called first before
		/// any call to tdSetName(), tdSetProtocol() and similar functions.
 		/// </summary>
		/// <returns> The device id for the newly created device. If the creation fails it returnes a 
		/// negative value.</returns>
		public static int tdAddDevice()
		{
			return UnmanagedImport.tdAddDevice();
		}

		/// <summary>
		///  Sends bell command to devices supporting this.
		///  Make sure the device supports this by calling tdMethods() before any calls to this function.
		/// </summary>
		/// <param name="deviceId">Id of device to sound the bell on</param>
		/// <returns>Success or error code</returns>
		public static int tdBell(int deviceId)
		{
			return UnmanagedImport.tdBell(deviceId);
		}

		/// <summary>
		/// Dims a device.
		/// Make sure the device supports this by calling tdMethods() before any calls to this function.
		/// </summary>
		/// <param name="deviceId">The device id to dim</param>
		/// <param name="level">The level the device should dim to. This value should be 0-255</param>
		/// <returns>Success or error code</returns>
		public static int tdDim(int deviceId, char level)
		{
			return UnmanagedImport.tdDim(deviceId, level);
		}

		/// <summary>
		///  This function returns the unique id of a device with a specific index.
		///  To get all the id numbers you should loop over all the devices, use tdGetNumberOfDevices() to get the number of devices
		/// </summary>
		/// <param name="order">Device index</param>
		/// <returns>Id of device at that position</returns>
		public static int tdGetDeviceId(int order)
		{
			return UnmanagedImport.tdGetDeviceId(order);
		}

		/// <summary>
		/// Get a protocol specific device parameter
		/// </summary>
		/// <param name="deviceId">The id of the device to query</param>
		/// <param name="name">The name of the parameter to query</param>
		/// <param name="defaultValue">A defaultValue to return if the current parameter hasn't previously been set</param>
		/// <returns>Any protocol specific parameter specified by "name"</returns>
		public static unsafe string tdGetDeviceParameter(int deviceId, string name, string defaultValue)
		{
			return getString(UnmanagedImport.tdGetDeviceParameter(deviceId, stringToChar(name), stringToChar(defaultValue)));
		}

		/// <summary>
		/// Returns which type the device is
		/// </summary>
		/// <param name="deviceId">Id of the device</param>
		/// <returns>TELLSTICK_TYPE_DEVICE or TELLSTICK_TYPE_GROUP</returns>
		public static int tdGetDeviceType(int deviceId)
		{
			return UnmanagedImport.tdGetDeviceType(deviceId);
		}

		/// <summary>
		/// Get a human readable string from an error code returned from a function in telldus-core
		/// </summary>
		/// <param name="errorNo">The error code to translate</param>
		/// <returns>A string ready to show to the user.
		/// TELLSTICK_SUCCESS
		/// TELLSTICK_ERROR_NOT_FOUND
		/// TELLSTICK_ERROR_PERMISSION_DENIED
		/// TELLSTICK_ERROR_DEVICE_NOT_FOUND
		/// TELLSTICK_ERROR_METHOD_NOT_SUPPORTED
		/// TELLSTICK_ERROR_COMMUNICATION
		/// TELLSTICK_ERROR_CONNECTING_SERVICE
		/// TELLSTICK_ERROR_UNKNOWN_RESPONSE
		/// TELLSTICK_ERROR_UNKNOWN
		/// </returns>
		public static unsafe string tdGetErrorString(int errorNo)
		{
			return getString(UnmanagedImport.tdGetErrorString(errorNo));
		}

		/// <summary>
		/// Query a device for it’s name
		/// </summary>
		/// <param name="deviceId">Id of device to query</param>
		/// <returns>The name of the device or an empty string if the device is not found</returns>
		public static unsafe string tdGetName(int deviceId)
		{
			return getString(UnmanagedImport.tdGetName(deviceId));
		}

		/// <summary>
		///  Query a device for which methods it supports. By supplying the methods you support 
		///  the library could remap the methods a device support for better fit the application.
		/// </summary>
		/// <param name="deviceId">Id of device to query</param>
		/// <param name="methodsSupported">Methods that the client application supports</param>
		/// <returns>Supported (both by client application and by device) method-flags OR'ed into an integer
		/// TELLSTICK_TURNON
		/// TELLSTICK_TURNOFF
		/// TELLSTICK_BELL
		/// TELLSTICK_TOGGLE
		/// TELLSTICK_DIM
		/// TELLSTICK_LEARN
		/// </returns>
		public static int tdMethods(int deviceId, int methodsSupported)
		{
			return UnmanagedImport.tdMethods(deviceId, methodsSupported);
		}

		/// <summary>
		/// Query a device for it’s model
		/// </summary>
		/// <param name="deviceId">Id of device to query</param>
		/// <returns>The model for a device. Not all protocols use this.</returns>
		public static unsafe string tdGetModel(int deviceId)
		{
			return getString(UnmanagedImport.tdGetModel(deviceId));
		}

		/// <summary>
		/// This function returns the number of devices configured
		/// </summary>
		/// <returns>The total number of devices configured</returns>
		public static int tdGetNumberOfDevices()
		{
			return UnmanagedImport.tdGetNumberOfDevices();
		}

		/// <summary>
		/// Query a device for it’s protocol
		/// </summary>
		/// <param name="deviceId">Id of device to query</param>
		/// <returns>The protocol for a device.</returns>
		public static unsafe string tdGetProtocol(int deviceId)
		{
			return getString(UnmanagedImport.tdGetProtocol(deviceId));
		}

		/// <summary>
		/// Returns the last sent command to a specific device
		/// </summary>
		/// <param name="deviceId">Id of device to query</param>
		/// <param name="methods">The methods supported by the client. See tdMethods() for more information.</param>
		/// <returns>The last sent command as integer, for example TELLSTICK_TURNON or TELLSTICK_TURNOFF</returns>
		public static int tdLastSentCommand(int deviceId, int methods)
		{
			return UnmanagedImport.tdLastSentCommand(deviceId, methods);
		}

		/// <summary>
		/// If the last sent command it TELLSTICK_DIM this returns the dimmed value
		/// </summary>
		/// <param name="deviceId">Id of device to query</param>
		/// <returns>The the value as a human readable string, for example "128" for 50%</returns>
		public static unsafe string tdLastSentValue(int deviceId)
		{
			return getString(UnmanagedImport.tdLastSentValue(deviceId));
		}

		/// <summary>
		/// Sends a special learn command to some devices that need a special learn-command to be used from TellStick
		/// Make sure the device supports this by calling tdMethods() before any calls to this function.
		/// </summary>
		/// <param name="deviceId">Id of device to learn</param>
		/// <returns>Success or error code</returns>
		public static int tdLearn(int deviceId)
		{
			return UnmanagedImport.tdLearn(deviceId);
		}

		/// <summary>
		/// Register a callback event function to be called when a device event (e.g. turn on, turn off) occurs
		/// </summary>
		/// <param name="eventFunc">Callback function to be called</param>
		/// <param name="obj">Context object that will be echoed back when function is called. Only the object when the first function is registered will be used. Set to null if not used.</param>
		/// <returns>Callback event id</returns>
		public unsafe int tdRegisterDeviceEvent(EventCallbackFunction eventFunc, Object obj)
		{
			int returnValue = 0;
			
			if (eventList.Count == 0)
			{
				//first added, register with dll too
				//only the context object of the first event will be registered
				UnmanagedImport.EventFunctionDelegate eventFunctionDelegate = new UnmanagedImport.EventFunctionDelegate(eventFunction);
				if (obj != null)
				{
					eventContextHandle = GCHandle.Alloc(obj);
					registeredEventFunctionId = UnmanagedImport.tdRegisterDeviceEvent(eventFunctionDelegate, (void*)GCHandle.ToIntPtr(eventContextHandle));	//context here or above?
				}
				else
				{
					registeredEventFunctionId = UnmanagedImport.tdRegisterDeviceEvent(eventFunctionDelegate, (void*)null);	//context here or above?
				}
				callbackFunctionReferenceList.Add(registeredEventFunctionId, eventFunctionDelegate);
			}

			++lastEventID;
			returnValue = lastEventID;
			eventList.Add(returnValue, eventFunc);
			
			return returnValue;
		}

		/// <summary>
		/// Register a callback device change event function to be called when a device change event (e.g. change of name, device added) occurs
		/// </summary>
		/// <param name="eventFunc">Callback function to be called</param>
		/// <param name="obj">Context object that will be echoed back when function is called. Only the object when the first function is registered will be used. Set to null if not used.</param>
		/// <returns>Callback event id</returns>
		public unsafe int tdRegisterDeviceChangeEvent(DeviceChangeEventCallbackFunction deviceEventFunc, Object obj)
		{
			int returnValue = 0;
			if (deviceChangeEventList.Count == 0)
			{
				//first added, register with dll too
				//only the context object of the first event will be registered
				UnmanagedImport.DeviceChangeEventFunctionDelegate deviceChangeEventFunctionDelegate = new UnmanagedImport.DeviceChangeEventFunctionDelegate(deviceEventFunction);

				if (obj != null)
				{
					deviceChangeEventContextHandle = GCHandle.Alloc(obj);
					registeredDeviceChangeEventFunctionId = UnmanagedImport.tdRegisterDeviceChangeEvent(deviceChangeEventFunctionDelegate, (void*)GCHandle.ToIntPtr(deviceChangeEventContextHandle));
				}
				else
				{
					registeredDeviceChangeEventFunctionId = UnmanagedImport.tdRegisterDeviceChangeEvent(deviceChangeEventFunctionDelegate, (void*)null);
				}
				callbackFunctionReferenceList.Add(registeredDeviceChangeEventFunctionId, deviceChangeEventFunctionDelegate);

			}
			++lastEventID;
			returnValue = lastEventID;
			deviceChangeEventList.Add(returnValue, deviceEventFunc);

			return returnValue;
		}

		/// <summary>
		/// Register a callback listening function to be called when a listening event (e.g. data is received with Tellstick Duo) occurs
		/// </summary>
		/// <param name="eventFunc">Callback function to be called</param>
		/// <param name="obj">Context object that will be echoed back when function is called. Only the object when the first function is registered will be used. Set to null if not used.</param>
		/// <returns>Callback event id</returns>
		public unsafe int tdRegisterRawDeviceEvent(RawListeningCallbackFunction listeningFunc, Object obj)
		{
			int returnValue = 0;
			if (rawListenerList.Count == 0)
			{
				//first added, register with dll too
				//only the context object of the first event will be registered
				UnmanagedImport.RawListeningDelegate listeningFunctionDelegate = new UnmanagedImport.RawListeningDelegate(rawListeningFunction);

				if (obj != null)
				{
					rawListenerContextHandle = GCHandle.Alloc(obj);

					registeredRawListenerFunctionId = UnmanagedImport.tdRegisterRawDeviceEvent(listeningFunctionDelegate, (void*)GCHandle.ToIntPtr(rawListenerContextHandle));	//TODO context here or above?
				}
				else
				{
					registeredRawListenerFunctionId = UnmanagedImport.tdRegisterRawDeviceEvent(listeningFunctionDelegate, (void*)null);
				}
				callbackFunctionReferenceList.Add(registeredRawListenerFunctionId, listeningFunctionDelegate);
			}
			++lastEventID;
			returnValue = lastEventID;
			rawListenerList.Add(returnValue, listeningFunc);
			return returnValue;
		}

		/// <summary>
		/// Removes a device.
		/// </summary>
		/// <param name="deviceId">Id of device to remove</param>
		/// <returns>True on success, false otherwise</returns>
		public static bool tdRemoveDevice(int deviceId)
		{
			return UnmanagedImport.tdRemoveDevice(deviceId);
		}

		/// <summary>
		/// Send a raw command to TellStick. Please read the TellStick protocol definition on how the command should be constructed.
		/// </summary>
		/// <param name="command">The command for TellStick in its native format</param>
		/// <param name="reserved"></param>
		/// <returns>Success or error code</returns>
		public static unsafe int tdSendRawCommand(string command, int reserved)
		{
			char* commandChar = stringToChar(command);
			int returnValue = UnmanagedImport.tdSendRawCommand(commandChar, reserved);
			Marshal.FreeHGlobal((IntPtr)commandChar);
			return returnValue;
		}

		/// <summary>
		/// Sets a new protocol specific parameter. Please see the documentation of the protocols before setting any parameter.
		/// </summary>
		/// <param name="deviceId">Id of device to change</param>
		/// <param name="name">The name of the parameter to change</param>
		/// <param name="value">The new value for the parameter</param>
		/// <returns>Success or error code</returns>
		public static unsafe bool tdSetDeviceParameter(int deviceId, string name, string value)
		{
			char* nameChar = stringToChar(name);
			char* valueChar = stringToChar(value);
			bool returnValue = UnmanagedImport.tdSetDeviceParameter(deviceId, nameChar, valueChar);
			Marshal.FreeHGlobal((IntPtr)nameChar);
			Marshal.FreeHGlobal((IntPtr)valueChar);
			return returnValue;
		}

		/// <summary>
		///  Sets a new model for a device. Which model to set depends on the current protocol.
		/// </summary>
		/// <param name="deviceId">Id of device to change</param>
		/// <param name="model">The new model</param>
		/// <returns>Success or error code</returns>
		public static unsafe bool tdSetModel(int deviceId, string model)
		{
			char* modelChar = stringToChar(model);
			bool returnValue = UnmanagedImport.tdSetModel(deviceId, modelChar);
			Marshal.FreeHGlobal((IntPtr)modelChar);
			return returnValue;
		}

		/// <summary>
		/// Sets a new name for a device.
		/// </summary>
		/// <param name="deviceId">Id of device to change</param>
		/// <param name="name">The new name</param>
		/// <returns>Success or error code</returns>
		public static unsafe bool tdSetName(int deviceId, string name)
		{
			char* nameChar = stringToChar(name);
			bool returnValue = UnmanagedImport.tdSetName(deviceId, nameChar);
			Marshal.FreeHGlobal((IntPtr)nameChar);
			return returnValue;
		}

		/// <summary>
		/// This changes the current protocol used by a device. After changing the protocol, setting new parameters is required.
		/// </summary>
		/// <param name="deviceId">Id of device to change</param>
		/// <param name="protocol">The new protocol to use</param>
		/// <returns>Success or error code</returns>
		public static unsafe bool tdSetProtocol(int deviceId, string protocol)
		{
			char* protocolChar = stringToChar(protocol);
			bool returnValue = UnmanagedImport.tdSetProtocol(deviceId, protocolChar);
			Marshal.FreeHGlobal((IntPtr)protocolChar);
			return returnValue;
		}

		/// <summary>
		/// Turns a device on.
		/// Make sure the device supports this by calling tdMethods() before any calls to this function.
		/// </summary>
		/// <param name="deviceId">Id of device to turn on</param>
		/// <returns>Success or error code</returns>
		public static unsafe int tdTurnOn(int deviceId)
		{
			return UnmanagedImport.tdTurnOn(deviceId);
		}

		/// <summary>
		/// Turns a device off.
		/// Make sure the device supports this by calling tdMethods() before any calls to this function.
		/// </summary>
		/// <param name="deviceId">Id of device to turn off</param>
		/// <returns>Success or error code</returns>
		public static unsafe int tdTurnOff(int deviceId)
		{
			return UnmanagedImport.tdTurnOff(deviceId);
		}

		/// <summary>
		/// Unregister a callback function. All functions will be unregistered automatically when this objekt is deleted.
		/// </summary>
		/// <param name="eventId">Id of callback even to unregister</param>
		public void unregisterCallback(int eventId)
		{
			eventList.Remove(eventId);
			if (eventList.Count == 0)
			{
				//no more events in list
				UnmanagedImport.tdUnregisterCallback(registeredEventFunctionId);
				callbackFunctionReferenceList.Remove(registeredEventFunctionId);
				if (eventContextHandle.IsAllocated)
				{
					eventContextHandle.Free();
				}
			}

			deviceChangeEventList.Remove(eventId);
			if (deviceChangeEventList.Count == 0)
			{
				//no more events in list
				UnmanagedImport.tdUnregisterCallback(registeredDeviceChangeEventFunctionId);
				callbackFunctionReferenceList.Remove(registeredDeviceChangeEventFunctionId);
				if (deviceChangeEventContextHandle.IsAllocated)
				{
					deviceChangeEventContextHandle.Free();
				}
			}

			rawListenerList.Remove(eventId);
			if (rawListenerList.Count == 0)
			{
				//no more events in list
				UnmanagedImport.tdUnregisterCallback(registeredRawListenerFunctionId);
				callbackFunctionReferenceList.Remove(registeredRawListenerFunctionId);
				if (rawListenerContextHandle.IsAllocated)
				{
					rawListenerContextHandle.Free();
				}
			}
		}

		#endregion


		#region Private Methods

		/// <summary>
		/// Convert char* to correctly encoded string, clean up received char*
		/// </summary>
		/// <param name="input">Char* to convert</param>
		/// <returns>Converted string</returns>
		private static unsafe string getString(char* input)
		{
			return getString(input, true);
		}

		/// <summary>
		/// Convert char* to correctly encoded string, clean up of received char* is optional
		/// </summary>
		/// <param name="input">Char* to convert</param>
		/// <param name="release">Clean up char* or not (don't do that if it is still should be used somewhere)</param>
		/// <returns>Converted string</returns>
		private static unsafe string getString(char* input, bool release)
		{
			string returnString = System.Text.Encoding.UTF8.GetString(System.Text.Encoding.Unicode.GetBytes(new string(input)));
			if (release)
			{
				UnmanagedImport.tdReleaseString(input);
			}
			return returnString;
		}

		/// <summary>
		/// Convert string to correctly encoded char*
		/// </summary>
		/// <param name="input">String to convert</param>
		/// <returns>Converted char*</returns>
		private static unsafe char* stringToChar(string input)
		{
			input = String.Concat(input, char.MinValue);	//add null terminator
			byte[] inputByte = System.Text.Encoding.UTF8.GetBytes(input);
			int size = Marshal.SystemDefaultCharSize * inputByte.Length;
			IntPtr inputPointer = Marshal.AllocHGlobal(size);

			Marshal.Copy(inputByte, 0, inputPointer, inputByte.Length);
			return (char*)inputPointer;
		}

		/// <summary>
		/// Event function wrapper that will call all registered event functions with C#-arguments when
		/// this event function is called from telldus core, with C++-style arguments. This function will
		/// be registered with telldus core when the first event callback function is added.
		/// </summary>
		/// <param name="deviceId">Id of affected device</param>
		/// <param name="method">Method that was executed on device</param>
		/// <param name="data">Optional data that was sent to the method (e.g. dim value)</param>
		/// <param name="callbackId">Callback event id</param>
		/// <param name="context">Context (optional)</param>
		/// <returns>0</returns>
		private unsafe int eventFunction(int deviceId, int method, char* data, int callbackId, void* context)
		{
			foreach (EventCallbackFunction eventFunc in eventList.Values)
			{
				if (context != null)
				{
					GCHandle eventContextHandle = GCHandle.FromIntPtr((IntPtr)context);
					eventFunc(deviceId, method, getString(data, false), callbackId, (Object)eventContextHandle.Target);
				}
				else
				{
					eventFunc(deviceId, method, getString(data, false), callbackId, null);
				}
			}

			return 0;
		}

		/// <summary>
		/// Event function wrapper that will call all registered device change event functions with C#-arguments when
		/// this device change event function is called from telldus core, with C++-style arguments. This function will
		/// be registered with telldus core when the first device change event callback function is added.
		/// </summary>
		/// <param name="deviceId">Id of affected device</param>
		/// <param name="changeEvent">Method that was executed, TELLSTICK_DEVICE_ADDED, TELLSTICK_DEVICE_CHANGED, TELLSTICK_DEVICE_REMOVED, TELLSTICK_DEVICE_STATE_CHANGED</param>
		/// <param name="changeType">Type of change TELLSTICK_CHANGE_NAME, TELLSTICK_CHANGE_PROTOCOL, TELLSTICK_CHANGE_MODEL</param>
		/// <param name="callbackId">Callback event id</param>
		/// <param name="context">Context (optional)</param>
		/// <returns>0</returns>
		private unsafe int deviceEventFunction(int deviceId, int changeEvent, int changeType, int callbackId, void* context)
		{
			foreach (DeviceChangeEventCallbackFunction deviceEventFunc in deviceChangeEventList.Values)
			{
				if (context != null)
				{
					GCHandle deviceChangeEventContextHandle = GCHandle.FromIntPtr((IntPtr)context);
					deviceEventFunc(deviceId, changeEvent, changeType, callbackId, (Object)deviceChangeEventContextHandle.Target);
				}
				else
				{
					deviceEventFunc(deviceId, changeEvent, changeType, callbackId, null);
				}
			}

			return 0;
		}

		/// <summary>
		/// Event function wrapper that will call all registered listening functions with C#-arguments when
		/// this listening function is called from telldus core, with C++-style arguments. This function will
		/// be registered with telldus core when the first listening callback function is added.
		/// </summary>
		/// <param name="data">Received data</param>
		/// <param name="controllerId">Id of receiving controller</param>
		/// <param name="callbackId">Callback event id</param>
		/// <param name="context">Context (optional)</param>
		/// <returns>0</returns>
		private unsafe int rawListeningFunction(char* data, int controllerId, int callbackId, void* context)
		{
			foreach (RawListeningCallbackFunction rawListeningFunc in rawListenerList.Values)
			{
				if (context != null)
				{
					GCHandle rawListenerContextHandle = GCHandle.FromIntPtr((IntPtr)context);
					rawListeningFunc(getString(data, false), controllerId, callbackId, (Object)rawListenerContextHandle.Target);	//strings cannot be released here, since they are still in use by calling core (sent to other clients aswell), let the core take care of it with shared pointer
				}
				else
				{
					rawListeningFunc(getString(data, false), controllerId, callbackId, null);
				}
			}

			return 0;
		}

		#endregion
	}
}
