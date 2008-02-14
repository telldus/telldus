using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DeviceScheduler
{
    public partial class frmSchedule : Form
    {
        public frmSchedule(DataStorage data, TaskScheduler.ScheduledTasks tasks, DataStorage.JobRow job, bool newJob)
        {
            Localization.ChangeLanguage();
            InitializeComponent();
            m_Data = data;
            m_NewJob = newJob;
            m_Job = job;
            m_Tasks = tasks;
        }

        private DataStorage m_Data = null;
        private DataStorage.JobRow m_Job = null;
        private bool m_NewJob = false;
        private TaskScheduler.ScheduledTasks m_Tasks = null;
        private TaskScheduler.Task m_Task = null;

        private void cboSchedule_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (cboSchedule.SelectedIndex)
            {
                case 0: //Day
                    grpDay.Visible = true;
                    grpDay.Location = new Point(13, 66);
                    grpWeek.Visible = false;
                    grpOnce.Visible = false;
                    grpMonth.Visible = false;
                    break;

                case 1: //Week
                    grpDay.Visible = false;
                    grpWeek.Visible = true;
                    grpWeek.Location = new Point(13, 66);
                    grpOnce.Visible = false;
                    grpMonth.Visible = false;
                    break;

                case 2: //Month
                    grpDay.Visible = false;
                    grpWeek.Visible = false;
                    grpOnce.Visible = false;
                    grpMonth.Visible = true;
                    grpMonth.Location = new Point(13, 66);
                    break;

                case 3: //Once
                    grpDay.Visible = false;
                    grpWeek.Visible = false;
                    grpMonth.Visible = false;
                    grpOnce.Visible = true;
                    grpOnce.Location = new Point(13, 66);
                    break;
                    
            }
        }

        private void frmSchedule_Load(object sender, EventArgs e)
        {
            grpDay.Left = 13;
            grpDay.Top = 66;
            grpWeek.Left = 13;
            grpWeek.Top = 66;
            grpMonth.Left = 13;
            grpMonth.Top = 66;

            cboSchedule.SelectedIndex = 0;
            txtName.Focus();
            dtpDate.Format = DateTimePickerFormat.Custom; 
            dtpDate.CustomFormat = System.Globalization.CultureInfo.CurrentUICulture.DateTimeFormat.LongDatePattern; 

            ShowInformation();
        }

        private void ShowInformation()
        {
            if (m_NewJob) 
            {
                return; 
            }

            m_Task = m_Tasks.OpenTask(m_Job.Name);

            txtName.Text = m_Task.Name;
            txtDescription.Text = m_Task.Comment;

            //txtTime.Text = m_Task.Triggers[0].BeginDate.Hour.ToString() + ":" + m_Task.Triggers[0].BeginDate.Minute.ToString();
            
            dtpTime.Value = DateTime.Parse(m_Job.StartTime);
            dtpDate.Value = m_Task.Triggers[0].BeginDate;

            if (m_Task.Triggers[0] is TaskScheduler.DailyTrigger)
            {
                cboSchedule.SelectedIndex = 0;

                TaskScheduler.DailyTrigger dayTrigger = (TaskScheduler.DailyTrigger)m_Task.Triggers[0];
                nupDay.Value = Convert.ToDecimal(dayTrigger.DaysInterval);

            }
            else if (m_Task.Triggers[0] is TaskScheduler.WeeklyTrigger)
            {
                cboSchedule.SelectedIndex = 1;

                TaskScheduler.WeeklyTrigger weekTrigger = (TaskScheduler.WeeklyTrigger)m_Task.Triggers[0];
                string days = weekTrigger.WeekDays.ToString();
                
                cboWeekMonday.Checked = (days.IndexOf("Monday") > -1);
                cboWeekTuesday.Checked = (days.IndexOf("Tuesday") > -1);
                cboWeekWednesday.Checked = (days.IndexOf("Wednesday") > -1);
                cboWeekThursday.Checked = (days.IndexOf("Thursday") > -1);
                cboWeekFriday.Checked = (days.IndexOf("Friday") > -1);
                cboWeekSaturday.Checked = (days.IndexOf("Saturday") > -1);
                cboWeekSunday.Checked = (days.IndexOf("Sunday") > -1);

                nupWeek.Value = Convert.ToDecimal(weekTrigger.WeeksInterval);
                

            }
            else if (m_Task.Triggers[0] is TaskScheduler.MonthlyTrigger)
            {
                cboSchedule.SelectedIndex = 2;

                TaskScheduler.MonthlyTrigger monthTrigger = (TaskScheduler.MonthlyTrigger)m_Task.Triggers[0];
                string months = monthTrigger.Months.ToString();

                cboMonthJan.Checked = (months.IndexOf("January") > -1);
                cboMonthFeb.Checked = (months.IndexOf("February") > -1);
                cboMonthMar.Checked = (months.IndexOf("March") > -1);
                cboMonthApr.Checked = (months.IndexOf("Arpil") > -1);
                cboMonthMay.Checked = (months.IndexOf("May") > -1);
                cboMonthJune.Checked = (months.IndexOf("June") > -1);
                cboMonthJuly.Checked = (months.IndexOf("July") > -1);
                cboMonthAug.Checked = (months.IndexOf("August") > -1);
                cboMontSept.Checked = (months.IndexOf("September") > -1);
                cboMontOct.Checked = (months.IndexOf("October") > -1);
                cboMonthNov.Checked = (months.IndexOf("November") > -1);
                cboMontDec.Checked = (months.IndexOf("December") > -1);

                nupMonth.Value = Convert.ToInt32(monthTrigger.Days[0]);
            }
            else if (m_Task.Triggers[0] is TaskScheduler.RunOnceTrigger)
            {
                cboSchedule.SelectedIndex = 3;
            }

            foreach (DataStorage.JobActionsRow action in m_Job.GetJobActionsRows())
            {
                
                DeviceHandler.Device device= GetDevice(action.DeviceID);
                ListViewItem item = lvwDevices.Items.Add(device.Name);
                item.SubItems.Add(action.TurnOn ? Localization.GetString("uniton") : Localization.GetString("unitoff"));

                item.Tag = device;
            }

        }

        private DeviceHandler.Device GetDevice(int deviceID)
        {
            foreach (DeviceHandler.Device item in Program.DeviceHandler.Devices)
            {
                if (item.ID == deviceID)
                    return item;
            }

            return null;
        }

        private bool SetInformation()
        {
            try
            {

                if (lvwDevices.Items.Count == 0)
                {
                    MessageBox.Show(Localization.GetString("FailedToSelectUnits"), "", 
                        MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return false;
                }

                if (txtName.Text.Length == 0)
                {
                    MessageBox.Show(Localization.GetString("FailedToNameSchedule"), "", 
                        MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return false;
                }

                if (m_NewJob)
                {
                    frmAskUsername frm = new frmAskUsername();
                    DialogResult result=frm.ShowDialog(this);

                    if (result == DialogResult.Cancel)
                        return false;

                    m_Job = m_Data.Job.NewJobRow();

                    
                    string encrypted = EncryptDecrypt.Encrypt(frm.Password, "ThisIsNotParadise");

                    m_Job.Username = frm.Username;
                    m_Job.Password = encrypted;

                    frm.Close();
                }

                string password = EncryptDecrypt.Decrypt(m_Job.Password, "ThisIsNotParadise");

                //Set general information.
                m_Job.JobName = txtName.Text;
                m_Job.Name = txtName.Text;
                m_Job.Description = txtDescription.Text;

                if (!m_NewJob) //Clear previous actions and add new ones.
                {
                    foreach (DataStorage.JobActionsRow action in m_Job.GetJobActionsRows())
                    {
                        action.Delete();
                    }
                }
                m_Data.AcceptChanges();

                //Add actions.
                foreach (ListViewItem item in lvwDevices.Items)
                {
                    DataStorage.JobActionsRow action = m_Data.JobActions.NewJobActionsRow();
                    DeviceHandler.Device device = (DeviceHandler.Device)item.Tag;

                    action.DeviceID = device.ID;
                    action.JobID = m_Job.ID;

                    if (item.SubItems[1].Text == Localization.GetString("uniton"))
                        action.TurnOn = true;
                    else
                        action.TurnOn = false;

                    m_Data.JobActions.AddJobActionsRow(action);
                }

                TaskScheduler.Task task = null;

                //Create a new schedule in windows.
                if (m_Task != null)
                {
                    task = m_Task;
                }
                else
                {

                    task = m_Tasks.CreateTask(txtName.Text);
                }

                string path = System.IO.Path.GetDirectoryName(Application.ExecutablePath);
                path = System.IO.Path.Combine(path, "DeviceSchedulerAgent.exe");

                //Clear trigger.
                if(task.Triggers!=null)
                    task.Triggers.Clear();

                //Set general information.
                task.ApplicationName = path;
                task.Parameters = m_Job.ID.ToString();
                task.Comment = txtDescription.Text;

                //Set user account information.
                try
                {
                    if (password.Length == 0)
                        task.SetAccountInformation(m_Job.Username, null);
                    else
                        task.SetAccountInformation(m_Job.Username, password);
                }
                catch (Exception e)
                {
                    MessageBox.Show(Localization.GetString("FailedToSetPassword"),
                        Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                //Set Telldus as creator so we can recognize the task next time.
                task.Creator = "Telldus";

                m_Job.StartTime = dtpTime.Value.Hour.ToString() + ":" + dtpTime.Value.Minute.ToString();

                dtpTime.Value= DateTime.Parse(dtpDate.Value.ToShortDateString() + " " + dtpTime.Value.ToShortTimeString());

                DateTime time = dtpTime.Value;
                short hour = short.Parse(dtpTime.Value.Hour.ToString());
                short minute = short.Parse(dtpTime.Value.Minute.ToString());

                short interval = 1;

                switch (cboSchedule.SelectedIndex)
                {
                    case 0: //Dailytrigger
                        interval = short.Parse(nupDay.Value.ToString());
                        TaskScheduler.DailyTrigger dayTrigger = new TaskScheduler.DailyTrigger(hour, minute, interval);
                        dayTrigger.BeginDate = dtpDate.Value;
                        //trigger.BeginDate.AddHours(Convert.ToDouble(dtpTime.Value.Hour));
                        //trigger.BeginDate.AddMinutes(Convert.ToDouble(dtpTime.Value.Minute));
                        dayTrigger.Disabled = false;
                        task.Triggers.Add(dayTrigger);
                        break;

                    case 1: //Weeklytrigger
                        TaskScheduler.DaysOfTheWeek weekDays = 0;
                        interval = short.Parse(nupWeek.Value.ToString());

                        if (cboWeekMonday.Checked)
                            weekDays = weekDays | TaskScheduler.DaysOfTheWeek.Monday;

                        if (cboWeekTuesday.Checked)
                            weekDays = weekDays | TaskScheduler.DaysOfTheWeek.Tuesday;

                        if (cboWeekWednesday.Checked)
                            weekDays = weekDays | TaskScheduler.DaysOfTheWeek.Wednesday;

                        if (cboWeekThursday.Checked)
                            weekDays = weekDays | TaskScheduler.DaysOfTheWeek.Thursday;

                        if (cboWeekFriday.Checked)
                            weekDays = weekDays | TaskScheduler.DaysOfTheWeek.Friday;

                        if (cboWeekSaturday.Checked)
                            weekDays = weekDays | TaskScheduler.DaysOfTheWeek.Saturday;

                        if (cboWeekSunday.Checked)
                            weekDays = weekDays | TaskScheduler.DaysOfTheWeek.Sunday;

                        TaskScheduler.WeeklyTrigger weeklyTrigger = new TaskScheduler.WeeklyTrigger(hour, minute, weekDays, interval);
                        time = DateTime.Parse(time.ToShortDateString()+ " " + dtpTime.Value.Hour+":"+dtpTime.Value.Minute+":00" );
                        //weeklyTrigger.BeginDate = time;
                        //weeklyTrigger.BeginDate=weeklyTrigger.BeginDate.AddHours(Convert.ToDouble(dtpTime.Value.Hour));
                        //weeklyTrigger.BeginDate=weeklyTrigger.BeginDate.AddMinutes(Convert.ToDouble(dtpTime.Value.Minute));
                       task.Triggers.Add(weeklyTrigger);
                        break;

                    case 2: //Monthlytrigger
                        //interval = int.Parse(nupmo

                        TaskScheduler.MonthsOfTheYear months = 0;

                        if (cboMonthJan.Checked)
                            months = months | TaskScheduler.MonthsOfTheYear.January;

                        if (cboMonthFeb.Checked)
                            months = months | TaskScheduler.MonthsOfTheYear.February;

                        if (cboMonthMar.Checked)
                            months = months | TaskScheduler.MonthsOfTheYear.March;

                        if (cboMonthApr.Checked)
                            months = months | TaskScheduler.MonthsOfTheYear.April;

                        if (cboMonthMay.Checked)
                            months = months | TaskScheduler.MonthsOfTheYear.May;

                        if (cboMonthJune.Checked)
                            months = months | TaskScheduler.MonthsOfTheYear.June;

                        if (cboMonthJuly.Checked)
                            months = months | TaskScheduler.MonthsOfTheYear.July;

                        if (cboMonthAug.Checked)
                            months = months | TaskScheduler.MonthsOfTheYear.August;

                        if (cboMontSept.Checked)
                            months = months | TaskScheduler.MonthsOfTheYear.September;

                        if (cboMontOct.Checked)
                            months = months | TaskScheduler.MonthsOfTheYear.October;

                        if (cboMonthNov.Checked)
                            months = months | TaskScheduler.MonthsOfTheYear.November;

                        if (cboMontDec.Checked)
                            months = months | TaskScheduler.MonthsOfTheYear.December;

                        int[] days = new int[1] { int.Parse(nupMonth.Value.ToString()) };
                        TaskScheduler.MonthlyTrigger  monthlyTrigger = new TaskScheduler.MonthlyTrigger(hour, minute, days, months);
                        monthlyTrigger.BeginDate = dtpDate.Value;
                        //trigger.BeginDate.AddMinutes(Convert.ToDouble(dtpTime.Value.Minute));
                        task.Triggers.Add(monthlyTrigger);
                        break;

                    case 3:
                        TaskScheduler.RunOnceTrigger onceTrigger = new TaskScheduler.RunOnceTrigger(dtpDate.Value);
                        onceTrigger.StartHour = hour;
                        onceTrigger.StartMinute = minute;
                        task.Triggers.Add(onceTrigger);
                        break;
                }


                task.Triggers[0].BeginDate = time;

                task.Save();
                task.Close();

                if(m_NewJob)
                    m_Data.Job.AddJobRow(m_Job);

                m_Data.AcceptChanges();

                return true;

            }
            catch (Exception ex)
            {
                //ett ohanterat fel har uppstått i programmet.
                MessageBox.Show(Localization.GetString("msg1") + Environment.NewLine + ex.Message, 
                    Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Error);

                return false;
            }
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            if (SetInformation())
            {
                this.DialogResult = DialogResult.OK;
                this.Hide();
            }
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Hide();
        }

        private void cmdAdd_Click(object sender, EventArgs e)
        {
            frmScheduleAddDevice frm = new frmScheduleAddDevice();
            DialogResult result = frm.ShowDialog(this);


            if (result == DialogResult.OK)
            {
                
                ListViewItem item = lvwDevices.Items.Add(frm.SelectedDevice.Name);
                item.SubItems.Add(frm.SelectedDeviceAction == 0 ? Localization.GetString("uniton") : Localization.GetString("unitoff"));
                item.Tag = frm.SelectedDevice;
            }

            frm.Close();
        }

        
        private string GetActionName(int actionID)
        {
            return "";
        }

        private void cmdRemove_Click(object sender, EventArgs e)
        {
            if (lvwDevices.SelectedItems.Count == 0) { return; }

            //Vill du ta bort markerad enhet?
            DialogResult result = MessageBox.Show(Localization.GetString("msg3"), 
                Application.ProductName, MessageBoxButtons.YesNo, MessageBoxIcon.Question);

            if (result == DialogResult.Yes)
            {
                lvwDevices.Items.Remove(lvwDevices.SelectedItems[0]);
            }
        }


    }
}