using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DeviceScheduler
{
    public partial class frmAskUsername : Form
    {
        public frmAskUsername()
        {
            Localization.ChangeLanguage();
            InitializeComponent();
        }

        private void frmAskUsername_Load(object sender, EventArgs e)
        {
            txtUsername.Text = Environment.MachineName + @"\" + Environment.UserName;
        }

        public string Username
        {
            get { return txtUsername.Text; }
        }
        public string Password
        {
            get { return txtPassword.Text; }
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Hide();
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Hide();
        }

        private void txtUsername_TextChanged(object sender, EventArgs e)
        {
            cmdOK.Enabled=(txtUsername.Text.Length>0);
        }
    }
}