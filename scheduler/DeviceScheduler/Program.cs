using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace DeviceScheduler
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new frmMain());
        }
        private static DeviceHandler.Handler m_Handler = new DeviceHandler.Handler();

        public static DeviceHandler.Handler DeviceHandler
        {
            get 
            {
                if (m_Handler == null)
                {
                    m_Handler = new DeviceHandler.Handler();
                    
                }
                
                return m_Handler;
            }
            set
            {
                m_Handler = value;
            }
        }
    }
}