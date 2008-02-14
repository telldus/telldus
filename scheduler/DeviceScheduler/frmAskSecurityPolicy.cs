using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DeviceScheduler
{
    public partial class frmAskSecurityPolicy : Form
    {
        public frmAskSecurityPolicy()
        {
            Localization.ChangeLanguage();
            InitializeComponent();
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.ProcessStartInfo info = new System.Diagnostics.ProcessStartInfo();

            info.FileName = linkLabel1.Tag.ToString();
            info.UseShellExecute = true;

            System.Diagnostics.Process.Start(info);
        }

        private void cmdYes_Click(object sender, EventArgs e)
        {
            SaveCheckbox();
            this.DialogResult = DialogResult.Yes;
            this.Hide();
        }

        private void SaveCheckbox()
        {
            Microsoft.Win32.RegistryKey key = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(@"Software\Telldus\Scheduler\Settings",true);
            key.SetValue("AskSecurityPolicy", !chkHide.Checked);
            key.Close();
        }

        private void cmdNo_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.No;
            this.Hide();
        }

        private void frmAskSecurityPolicy_Load(object sender, EventArgs e)
        {

        }
    }
}