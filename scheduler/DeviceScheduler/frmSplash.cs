using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;

namespace DeviceScheduler
{
    public partial class frmSplash : Form
    {
        public frmSplash()
        {
            InitializeComponent();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            
            timer1.Enabled = false;
            Cursor = Cursors.Default;
            this.Hide();

        }

        private void frmSplash_Load(object sender, EventArgs e)
        {
            Cursor = Cursors.WaitCursor;
            lblVersion.Text = AssemblyVersion;
            timer1.Enabled = true;
        }
        public string AssemblyVersion
        {
            get
            {
                return Assembly.GetExecutingAssembly().GetName().Version.ToString();
            }
        }

    }
}