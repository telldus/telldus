using System;
using System.IO;
using System.Data;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Resources;
using System.Reflection;

namespace DeviceScheduler
{
    public enum eLanguage
    { 
        Swedish=1053,
        English=1033
    }

    /// <summary>
    /// This class handles all localization such as retrieving specific strings and changing standard locale.
    /// </summary>
    class Localization
    {
        private static ResourceManager m_Mngr = null;
        private static Assembly m_Assembly = null;
        private static eLanguage m_SelectedLanguage = eLanguage.English;

        public static Assembly Assembly
        {
            get { return m_Assembly; }
            set { m_Assembly = value; }
        }

        public static void GetSelectedLanguage()
        {
           
            Microsoft.Win32.RegistryKey key = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(@"Software\Telldus");

            if (key != null)
            {

                if (key.GetValue("Language") != null)
                {
                    m_SelectedLanguage = (eLanguage)Enum.Parse(typeof(eLanguage), key.GetValue("Language").ToString());
                }
                key.Close();
            }
            
        }

        private static ResourceManager ResMngr
        {
            get
            {
                if (m_Mngr == null)
                    m_Mngr = new ResourceManager("DeviceScheduler.Languages.WinFormStrings", m_Assembly);

                return m_Mngr;
            }
        }

        public static string GetString(string name)
        {
            try
            {
                
                return ResMngr.GetString(name);

            }
            catch (Exception ex)
            {
                return "";
            }
        }


        public static void ChangeLanguage()
        {
            ChangeLanguage(m_SelectedLanguage);
        }

        public static void ChangeLanguage(eLanguage language)
        {
            m_SelectedLanguage = language;

            if (language == eLanguage.English)
            {
                System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-GB");
                System.Threading.Thread.CurrentThread.CurrentUICulture = new System.Globalization.CultureInfo("en-GB");//en
            }
            else if (language == eLanguage.Swedish)
            {
                System.Threading.Thread.CurrentThread.CurrentUICulture = new System.Globalization.CultureInfo("sv-SE");//sv
                System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("sv-SE");
            }
            
        }

       
    }
}
