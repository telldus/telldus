/* -------------------------------------------------------------------------------
 * TellStick.NET
 * -------------------------------------------------------------------------------
 * Created by Mikael Levén (http://labs.leven.se)
 * 
 * This piece of code is licensed as open source and can be freely used and 
 * redistributed in any type of project, both commercial and non-commercial, 
 * as long as this licensing policy is not altered. This means that any alterations 
 * of this code must also be licensed as open source and therefore cannot be 
 * protected or patented in any way. However, any software based on this code may 
 * still be copyrighted (and/or patented) and sold as propritary software as long as 
 * this code (and any alterations of it) can be freely used and modified by others.
 * 
 * Altough the code is licensed as open source you don't need to republish any 
 * changes made back to the community (it will be much appreciated tough).
 * 
 * The code comes without any kind of warrantys and the author cannot be held 
 * responsible for any kind of damage this software may (or may not) cause, no matter
 * if it's direct or indirect damage.
 * 
 * Please note: 
 * All TellStick core functionality is developed by Telldus Technologies
 * and as of now (2007-12-14) that code is also licensed as open source. This may 
 * however change in the future and I suggest that you check there website
 * to make sure the licensing terms are still the same: http://www.telldus.se/
 * The documentation and example code for Tellstic is available at their Wiki:
 * http://www.telldus.se/wiki/
 * 
 * -------------------------------------------------------------------------------
 * Description:
 * TellStick.NET is a Microsoft .NET wrapper for the native TellStick DLL. 
 * The wrapper also adds some functionality to make it easier to work with 
 * TellStick devices.
 * 
 * Author: Mikael Levén
 * Created: 2007-12-14
 * 
 * Dependencies:
 * This library depends on the native DLL for TellStick. To be able to actually 
 * switch lights on and off you also need the driver to be installed as well 
 * as an TellStick USB device.
 * 
 * Revisions:
-------------------------------------------------------------------------------
*/

using System.Collections.Generic;
using System.Runtime.InteropServices;

/// <summary>
/// TellStick.NET is a Microsoft .NET wrapper for the native TellStick DLL. 
/// The wrapper also adds some functionality to make it easier to work with TellStick devices.
/// </summary>
public sealed class TellStick
{
	private TellStick()
	{ }


	#region TellStick native DLL imports class
	public sealed class Native
	{
		private Native()
		{ }

		[DllImport("TellUsbD101.dll")]
		public static extern int devAddDevice();

		[DllImport("TellUsbD101.dll")]
		public static extern bool devTurnOn(int intDeviceId);

		[DllImport("TellUsbD101.dll")]
		public static extern bool devTurnOff(int intDeviceId);

		[DllImport("TellUsbD101.dll")]
		public static extern int devGetNumberOfDevices();

		[DllImport("TellUsbD101.dll")]
		public static extern string devGetName(int intDeviceId);

		[DllImport("TellUsbD101.dll")]
		public static extern bool devSetName(int intDeviceId, string chNewName);

		[DllImport("TellUsbD101.dll")]
		public static extern string devGetVendor(int intDeviceId);

		[DllImport("TellUsbD101.dll")]
		public static extern bool devSetVendor(int intDeviceId, string chNewName);

		[DllImport("TellUsbD101.dll")]
		public static extern string devGetModel(int intDeviceId);

		[DllImport("TellUsbD101.dll")]
		public static extern bool devSetModel(int intDeviceId, string chNewName);

		[DllImport("TellUsbD101.dll")]
		public static extern int[] devGetArguments(int intDeviceId);

		[DllImport("TellUsbD101.dll")]
		public static extern bool devSetArguments(int intDeviceId, int[] intArguments, int intNumberOfArguments);

		[DllImport("TellUsbD101.dll")]
		public static extern int devAddDeviceWithArguments(string strVendor, int[] intArguments, int intNumberOfArguments);

		[DllImport("TellUsbD101.dll")]
		public static extern bool devRemoveDevice(int intDeviceId);

		[DllImport("TellUsbD101.dll")]
		public static extern int devGetDeviceId(int intDeviceIndex);

	}
	#endregion

	/// <summary>
	/// Gets the number of devices registered
	/// </summary>
	/// <returns>Returns the number of devices as an integer</returns>
	/// <remarks>The native DLL is required for this methods to run. If it's missing a ModuleNotFoundException exception will be thrown.</remarks>
	public static int GetNumberOfDevices()
	{
		try
		{
			return Native.devGetNumberOfDevices();
		}
		catch (System.DllNotFoundException ex)
		{
			throw new ModuleNotFoundException("Could not find TellStick software. Please make sure it's installed before you run this application!", ex);
		}
		catch (System.Exception)
		{
			throw;
		}
		
	}

	/// <summary>
	/// Gets the unique idetifier of the device
	/// </summary>
	/// <param name="deviceIndex">Index number of the device to get ID for</param>
	/// <returns>Returns the unique identifier as an integer</returns>
	/// <remarks>The native DLL is required for this methods to run. If it's missing a ModuleNotFoundException exception will be thrown.</remarks>
	public static int GetDeviceId(int deviceIndex)
	{
		// Retrieve and return the device unqiue identifier from the native method
		try
		{
			return Native.devGetDeviceId(deviceIndex);
		}
		catch (System.DllNotFoundException ex)
		{
			throw new ModuleNotFoundException("Could not find TellStick software. Please make sure it's installed before you run this application!", ex);
		}
		catch (System.Exception)
		{
			throw;
		}

	}


	/// <summary>
	/// Gets the user defined name of the device
	/// </summary>
	/// <param name="deviceId">Device unqiue identification</param>
	/// <returns>Returns the user defined device name as a string</returns>
	/// <remarks>The native DLL is required for this methods to run. If it's missing a ModuleNotFoundException exception will be thrown.</remarks>
	public static string GetName(int deviceId)
	{
		// Retrieve and return the device name from the native method
	    try
        {
            return Native.devGetName(deviceId);
        }
        catch (System.DllNotFoundException ex)
        {
            throw new ModuleNotFoundException("Could not find TellStick software. Please make sure it's installed before you run this application!", ex);
        }
        catch (System.Exception)
        {
            throw;
        }
    }


    /// <summary>
    /// Turns on the selected device
    /// </summary>
    /// <param name="deviceId">Device unqiue identification</param>
    /// <returns>Returns true if the device was succesfully turned on</returns>
	/// <remarks>The native DLL is required for this methods to run. If it's missing a ModuleNotFoundException exception will be thrown.</remarks>
	public static bool TurnOn(int deviceId)
    {
        // Send "on" signal
        try
        {
            return Native.devTurnOn(deviceId);
        }
        catch (System.DllNotFoundException ex)
        {
            throw new ModuleNotFoundException("Could not find TellStick software. Please make sure it's installed before you run this application!", ex);
        }
        catch (System.Exception)
        {
            throw;
        }
    }

    /// <summary>
    /// Turns off the selected device
    /// </summary>
    /// <param name="deviceId">Device unqiue identification</param>
    /// <returns>Returns true if the device was succesfully turned off</returns>
	/// <remarks>The native DLL is required for this methods to run. If it's missing a ModuleNotFoundException exception will be thrown.</remarks>
	public static bool TurnOff(int deviceId)
    {
        // Send "off" signal
        try
        {
            return Native.devTurnOff(deviceId);
        }
        catch (System.DllNotFoundException ex)
        {
            throw new ModuleNotFoundException("Could not find TellStick software. Please make sure it's installed before you run this application!", ex);
        }
        catch (System.Exception)
        {
            throw;
        }
    }

	/// <summary>
	/// Gets the vendors name
	/// </summary>
	/// <param name="deviceId"></param>
	/// <returns></returns>
	/// <remarks>The native DLL is required for this methods to run. If it's missing a ModuleNotFoundException exception will be thrown.</remarks>
	public static string GetVendor(int deviceId)
	{
		// Retrieve and return the vendor from the native method
		try
		{
			return Native.devGetVendor(deviceId);
		}
		catch (System.DllNotFoundException ex)
		{
			throw new ModuleNotFoundException("Could not find TellStick software. Please make sure it's installed before you run this application!", ex);
		}
		catch (System.Exception)
		{
			throw;
		}
	}



	#region "Support classes"

	public abstract class TellStickException : System.Exception
	{ 
		public TellStickException() : base()
		{ 
		}

		public TellStickException(string message)
			: base(message)
		{
		}

		public TellStickException(string message, System.Exception innerException)
			: base(message, innerException)
		{
		}

	}

	public class UnmanagedException : TellStickException
	{
		public UnmanagedException() : base()
		{ 
		}

		public UnmanagedException(string message)
			: base(message)
		{
		}

		public UnmanagedException(string message, System.Exception innerException)
			: base(message, innerException)
		{
		}

	}

	public class ModuleNotFoundException : TellStickException
	{
		public ModuleNotFoundException()
			: base("Could not find TellStick software. Please make sure it's installed before you run this application!")
		{ 
		
		}

		public ModuleNotFoundException(string message)
			: base(message)
		{

		}

		public ModuleNotFoundException(string message, System.Exception innerException)
			: base(message, innerException)
		{

		}
	}


	#endregion
}

