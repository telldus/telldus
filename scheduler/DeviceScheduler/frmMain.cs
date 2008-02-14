using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;
using TaskScheduler;

namespace DeviceScheduler
{
    public partial class frmMain : Form
    {
        public frmMain()
        {
            Localization.GetSelectedLanguage();
            Localization.ChangeLanguage();
            Localization.Assembly = this.GetType().Assembly;
            InitializeComponent();
        }
        private TaskScheduler.ScheduledTasks m_Tasks = null;
        private DataStorage m_Data = null;

        private bool ShowSplash()
        {
            string[] cmds = Environment.GetCommandLineArgs();

            foreach (string cmd in cmds)
            {
                if (cmd.ToLower() == "/nosplash")
                    return false;
            }

            return true;
        }

       
        private void frmMain_Load(object sender, EventArgs e)
        {
            Application.ThreadException += new System.Threading.ThreadExceptionEventHandler(Application_ThreadException);
            
            frmSplash frm = null;
            bool splash = ShowSplash();

            if (splash)
            {
                frm = new frmSplash();
                frm.TopMost = true;
                frm.ShowDialog(this);
            }

           

            SetupListViewDevices();
            m_Tasks = new ScheduledTasks();

            LoadData();
            LoadTasks();

            if (splash)
            {
                frm.TopMost = false;
                frm.Close();
            }

            if (IsPasswordPrincipalActive())
            {
                if (ShowAskSecurityPolicy())
                {
                    frmAskSecurityPolicy policy = new frmAskSecurityPolicy();
                    DialogResult result = policy.ShowDialog(this);

                    policy.Close();
                }
            }
        }

        private bool ShowAskSecurityPolicy()
        {
            RegistryKey key = Registry.CurrentUser.OpenSubKey(@"Software\Telldus\Scheduler\Settings");

            if (key != null)
            {
                if (key.GetValue("AskSecurityPolicy") != null)
                {
                    return Convert.ToBoolean(key.GetValue("AskSecurityPolicy"));
                    key.Close();
                }
            }

            return true;
        }

        //private void DisablePasswordPolicy()
        //{
        //    RegistryKey key = Registry.LocalMachine.OpenSubKey(@"SYSTEM\CurrentControlSet\Control\Lsa");
        //    key.SetValue("LimitBlankPasswordUse", 0);
        //    key.Close();

        //}

        private bool IsPasswordPrincipalActive()
        {
            RegistryKey key = Registry.LocalMachine.OpenSubKey(@"SYSTEM\CurrentControlSet\Control\Lsa");
            bool result = Convert.ToBoolean(key.GetValue("LimitBlankPasswordUse"));
            key.Close();

            return result;

        }

        /// <summary>
        /// Handles unexpected errors. All roads lead here...
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void Application_ThreadException(object sender, System.Threading.ThreadExceptionEventArgs e)
        {
            MessageBox.Show("Ett ohanterat fel har uppstått i programmet."+Environment.NewLine+e.Exception.Message,
                "", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void LoadData()
        {
            string path = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
            //string path = System.IO.Path.GetDirectoryName(Application.ExecutablePath);
            path = System.IO.Path.Combine(path, "Jobs.dat");

            m_Data = new DataStorage();

            if (System.IO.File.Exists(path))
            {
                m_Data.ReadXml(path, XmlReadMode.ReadSchema);
            }
        }

        private void SetupListViewDevices()
        {
            lvwCommon.Items.Clear();

            Program.DeviceHandler.Load();

            foreach(DeviceHandler.Device dev in Program.DeviceHandler.Devices)
            {
                ListViewItem item = lvwCommon.Items.Add(dev.Name,2);
                item.SubItems.Add(dev.ID.ToString());
                item.SubItems.Add(dev.Vendor);
                item.Tag = dev;
            }
        }

       

        private void mnuAbout_Click(object sender, EventArgs e)
        {
            frmAbout frm = new frmAbout();
            frm.ShowDialog(this);
        }

        private void cmdTurnOn_Click(object sender, EventArgs e)
        {

            if (lvwCommon.SelectedItems.Count == 1)
            {
                DeviceHandler.Device item = (DeviceHandler.Device)lvwCommon.SelectedItems[0].Tag;
                Program.DeviceHandler.TurnOn(item);
                return;
            }

            foreach (ListViewItem lvwItem in lvwCommon.SelectedItems)
            {
                DeviceHandler.Device item = (DeviceHandler.Device)lvwItem.Tag;
                Program.DeviceHandler.TurnOnWithDelay(item);
            }
        }

        private void cmdTurnOff_Click(object sender, EventArgs e)
        {
            if (lvwCommon.SelectedItems.Count == 1)
            {
                DeviceHandler.Device item = (DeviceHandler.Device)lvwCommon.SelectedItems[0].Tag;
                Program.DeviceHandler.TurnOff(item);
                return;
            }

            foreach (ListViewItem lvwItem in lvwCommon.SelectedItems)
            {
                DeviceHandler.Device item = (DeviceHandler.Device)lvwItem.Tag;
                Program.DeviceHandler.TurnOffWithDelay(item);
            }
        }

        private void ConfigUnits()
        {
            RegistryKey key = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Telldus");
            string path = key.GetValue("UninstallString").ToString();

            path = System.IO.Path.GetDirectoryName(path);

            Cursor = Cursors.WaitCursor;

            System.Diagnostics.Process proc = new System.Diagnostics.Process();
            string filename = System.IO.Path.Combine(path, "TelldusSetup.exe");
            proc.StartInfo.FileName = filename;
            proc.StartInfo.WorkingDirectory = path;
            proc.Start();


            Cursor = Cursors.Default;
        }

        private void toolManageDevices_Click(object sender, EventArgs e)
        {
            ConfigUnits();
        }

        private void LoadTasks()
        {
            
            //Clear listview of tasks
            lvwTasks.Items.Clear();

            foreach (string item in m_Tasks.GetTaskNames())
            {
                Task task = m_Tasks.OpenTask(item);
                if(task != null)
                {
                    if (task.Creator.ToLower().Contains("telldus"))
                    {
                        DataStorage.JobRow job = m_Data.Job.FindByID(Convert.ToInt32(task.Parameters));

                        ListViewItem lvwItem = lvwTasks.Items.Add(task.Name, 3);
                        lvwItem.SubItems.Add(job.StartTime.ToString());
                        lvwItem.SubItems.Add(task.Triggers[0].BeginDate.ToShortDateString());
                        lvwItem.Tag = task;

                    }
                    task.Close();
                }
            }
        }

        private void toolNew_Click(object sender, EventArgs e)
        {
            CreateSchedule();
        }

        private void CreateSchedule()
        {
            frmSchedule frm = new frmSchedule(m_Data, m_Tasks, null, true);
            DialogResult result = frm.ShowDialog(this);

            if (result == DialogResult.OK)
            {
                SaveDataStorage();
                LoadTasks();
            }
            frm.Close();
        }

        private void mnuExit_Click(object sender, EventArgs e)
        {
            SaveDataStorage();
            Application.Exit();
        }

        private void toolRemove_Click(object sender, EventArgs e)
        {
            RemoveSchedule();
        }

        private void RemoveSchedule()
        {
            if (lvwTasks.SelectedItems.Count == 0) { return; }

            //"Vill du ta bort markerad schemaläggning?"
            DialogResult result = MessageBox.Show(Localization.GetString("msg2"), 
                Application.ProductName, MessageBoxButtons.YesNo, MessageBoxIcon.Question);

            if (result == DialogResult.Yes)
            {

                foreach (ListViewItem item in lvwTasks.SelectedItems)
                {
                    Task task = m_Tasks.OpenTask(item.SubItems[0].Text);
                    int id = Convert.ToInt32(task.Parameters);
                    task.Close();

                    //First remove the task from windows scheduling
                    bool result2 = m_Tasks.DeleteTask(item.SubItems[0].Text);

                    //Second remove the entry from the Job table in the datastorage.
                    DataStorage.JobRow job = m_Data.Job.FindByID(id);
                    
                    //Third remove all JobActions
                    DataStorage.JobActionsRow[] actions = m_Data.JobActions.Select("JobID=" + job.ID.ToString()) as DataStorage.JobActionsRow[];

                    foreach (DataStorage.JobActionsRow action in actions)
                    {
                        action.Delete();
                    }

                    //Forth remove the job
                    if (job != null)
                        job.Delete();

                    //Fifth remove the entry from the listview.
                    lvwTasks.Items.Remove(item);
                }

                m_Data.AcceptChanges();
            }
        }

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            SaveDataStorage();
        }

        private void SaveDataStorage()
        {
            string path = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);

            //string path = System.IO.Path.GetDirectoryName(Application.ExecutablePath);
            path = System.IO.Path.Combine(path, "Jobs.dat");

            m_Data.WriteXml(path, XmlWriteMode.WriteSchema);
        }

        private void mnuRunTask_Click(object sender, EventArgs e)
        {
            if (lvwTasks.SelectedItems.Count == 0) { return; }
            Task item = m_Tasks.OpenTask(lvwTasks.SelectedItems[0].Text);
            
            item.Run();
        }

        private void toolEdit_Click(object sender, EventArgs e)
        {
            if (lvwTasks.SelectedItems.Count == 0) { return; }

            Task task = m_Tasks.OpenTask(lvwTasks.SelectedItems[0].Text);
            DataStorage.JobRow job = m_Data.Job.FindByID(Convert.ToInt32(task.Parameters));
            task.Close();

            frmSchedule frm = new frmSchedule(m_Data, m_Tasks, job, false);
            DialogResult result = frm.ShowDialog(this);

            if (result == DialogResult.OK)
            {
                //Reload all tasks
                LoadTasks();
            }

            frm.Close();

        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            CreateSchedule();
        }

        private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            ConfigUnits();
        }

        private void mnuWebsite_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process proc = new System.Diagnostics.Process();

            proc.StartInfo.FileName = "http://www.telldus.se";
            proc.StartInfo.UseShellExecute = true;
            proc.Start();
        }

        private void mnuNew_Click(object sender, EventArgs e)
        {
            CreateSchedule();
        }

        private void mnuRemove_Click(object sender, EventArgs e)
        {
            RemoveSchedule();
        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (tabControl1.SelectedIndex == 1)
                SetupListViewDevices();
        }
        
    }
}