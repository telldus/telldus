using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.Serialization;

namespace DeviceHandler
{
    public class Handler
    {


        #region Unmanaged Code
        [DllImport("TelldusCore.dll")]
        private static extern bool tdTurnOn(int value);

        [DllImport("TelldusCore.dll")]
        private static extern bool tdTurnOff(int value);

        [DllImport("TelldusCore.dll")]
        private static extern bool tdBell(int value);

        [DllImport("TelldusCore.dll")]
        private static extern int tdGetDeviceId(int value);

        [DllImport("TelldusCore.dll")]
        private static unsafe extern char* tdGetName(int value);

        [DllImport("TelldusCore.dll")]
        private static unsafe extern char* tdGetProtocol(int value);

        [DllImport("TelldusCore.dll")]
        private static extern int tdGetNumberOfDevices();

        [DllImport("TelldusCore.dll")]
        private static extern int tdClose();

		[DllImport("TelldusCore.dll")]
		private static unsafe extern void tdReleaseString(char* value);
        #endregion

        private List<Device> m_Devices = new List<Device>();

        public Device Find(int deviceID)
        {
            foreach (Device item in m_Devices)
            {
                if (item.ID == deviceID)
                    return item;
            }

            return null;
        }

        /// <summary>
        /// Turns off a specific device.
        /// </summary>
        /// <param name="item">The item in question.</param>
        /// <returns>True if the command was successfull and false if not.</returns>
        public bool TurnOff(Device item)
        {
            return tdTurnOff(item.ID);
        }

        public bool TurnOffWithDelay(Device item)
        {
            bool result = tdTurnOff(item.ID);
            System.Threading.Thread.Sleep(1000);
            return result;
        }

        /// <summary>
        /// Turns on a specific device.
        /// </summary>
        /// <param name="item">The device in question.</param>
        /// <returns>True if the command was successfull.</returns>
        public bool TurnOn(Device item)
        {
            return tdTurnOn(item.ID);
        }


        public bool TurnOnWithDelay(Device item)
        {
            //Makesure that we halt for 1 second so that the Telldus stick have time to send the signal.
            bool result = tdTurnOn(item.ID);
            System.Threading.Thread.Sleep(1000);
            return result;
        }


        /// <summary>
        /// Sends the bell signal.
        /// </summary>
        /// <param name="item">The device in question.</param>
        /// <returns>True if the command was successfull.</returns>
        public bool Bell(Device item)
        {
            return tdBell(item.ID);
        }

        public bool BellWithDelay(Device item)
        {
            //Makesure that we halt for 1 second so that the Telldus stick have time to send the signal.
            bool result = tdBell(item.ID);
            System.Threading.Thread.Sleep(1000);
            return result;
        }

		/// <summary>
		/// Get name for device, as a string
		/// </summary>
		/// <param name="deviceId"></param>
		/// <returns></returns>
		private static unsafe string getName(int deviceId)
		{
			char* name = tdGetName(deviceId);

			string returnstring = System.Text.Encoding.UTF8.GetString(System.Text.Encoding.Unicode.GetBytes(new string(name)));

			tdReleaseString(name);
			return returnstring;

		}

		/// <summary>
		/// Get protocol for device, as a string
		/// </summary>
		/// <param name="deviceId"></param>
		/// <returns></returns>
		private static unsafe string getProtocol(int deviceId)
		{
			char* name = tdGetProtocol(deviceId);

			string returnstring = System.Text.Encoding.UTF8.GetString(System.Text.Encoding.Unicode.GetBytes(new string(name)));

			tdReleaseString(name);
			return returnstring;

		}

        /// <summary>
        /// Loads all known devices into a collection.
        /// </summary>
        /// <returns>True if the process was successfull.</returns>
        public bool Load()
        {
            try
            {
                m_Devices.Clear();

                int count = tdGetNumberOfDevices() - 1;
                for (int i = 0; i <= count; i++)
                {
                    //Collect information from the driver.
                    int deviceID = tdGetDeviceId(i);
					string deviceName = getName(deviceID);
					string deviceProtocol = getProtocol(deviceID);

                    m_Devices.Add(new Device(deviceID, deviceName, deviceProtocol));
                }

                return true;
            }
            catch
            {
                return false;
            }
        }

        public List<Device> Devices
        {
            get 
            {
                if (m_Devices.Count == 0)
                    Load();

                return m_Devices; 
            
            }
            set { m_Devices = value; }
        }
    }

    [Serializable]
    public class Device:ISerializable
    {
        private int m_ID = -1;
        private string m_Name = "";
        private string m_Protocol = "";

        public Device(SerializationInfo info,StreamingContext cntx)
        { 
            
        }

        public Device(int id, string name,string protocol)
        {
            m_ID = id;
            m_Name = name;
            m_Protocol = protocol;
        }

        public int ID
        {
            get { return m_ID; }
            set { m_ID = value; }
        }

        public string Name
        {
            get { return m_Name; }
            set { m_Name = value; }
        }

        public string Protocol
        {
            get { return m_Protocol; }
            set { m_Protocol = value; }
        }

        public override string ToString()
        {
            return m_Name;
        }

        #region ISerializable Members

        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            throw new Exception("The method or operation is not implemented.");
        }

        #endregion
    }
}
