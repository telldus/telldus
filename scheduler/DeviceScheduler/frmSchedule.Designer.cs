namespace DeviceScheduler
{
    partial class frmSchedule
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmSchedule));
            this.cmdCancel = new System.Windows.Forms.Button();
            this.cmdOK = new System.Windows.Forms.Button();
            this.txtName = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.lvwDevices = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.label2 = new System.Windows.Forms.Label();
            this.txtDescription = new System.Windows.Forms.TextBox();
            this.cmdAdd = new System.Windows.Forms.Button();
            this.cmdRemove = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.dtpDate = new System.Windows.Forms.DateTimePicker();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.grpMonth = new System.Windows.Forms.GroupBox();
            this.nupMonth = new System.Windows.Forms.NumericUpDown();
            this.label12 = new System.Windows.Forms.Label();
            this.cboMontDec = new System.Windows.Forms.CheckBox();
            this.cboMonthNov = new System.Windows.Forms.CheckBox();
            this.cboMontOct = new System.Windows.Forms.CheckBox();
            this.cboMontSept = new System.Windows.Forms.CheckBox();
            this.cboMonthAug = new System.Windows.Forms.CheckBox();
            this.cboMonthJuly = new System.Windows.Forms.CheckBox();
            this.cboMonthJune = new System.Windows.Forms.CheckBox();
            this.cboMonthMay = new System.Windows.Forms.CheckBox();
            this.cboMonthApr = new System.Windows.Forms.CheckBox();
            this.cboMonthMar = new System.Windows.Forms.CheckBox();
            this.cboMonthFeb = new System.Windows.Forms.CheckBox();
            this.cboMonthJan = new System.Windows.Forms.CheckBox();
            this.cboMonthDays = new System.Windows.Forms.ComboBox();
            this.cboOrder = new System.Windows.Forms.ComboBox();
            this.rdbMonthThe = new System.Windows.Forms.RadioButton();
            this.rdbMonthDay = new System.Windows.Forms.RadioButton();
            this.label11 = new System.Windows.Forms.Label();
            this.grpOnce = new System.Windows.Forms.GroupBox();
            this.label14 = new System.Windows.Forms.Label();
            this.grpDay = new System.Windows.Forms.GroupBox();
            this.nupDay = new System.Windows.Forms.NumericUpDown();
            this.label8 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.dtpTime = new System.Windows.Forms.DateTimePicker();
            this.grpWeek = new System.Windows.Forms.GroupBox();
            this.nupWeek = new System.Windows.Forms.NumericUpDown();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.cboWeekSaturday = new System.Windows.Forms.CheckBox();
            this.cboWeekSunday = new System.Windows.Forms.CheckBox();
            this.cboWeekFriday = new System.Windows.Forms.CheckBox();
            this.cboWeekThursday = new System.Windows.Forms.CheckBox();
            this.cboWeekWednesday = new System.Windows.Forms.CheckBox();
            this.cboWeekTuesday = new System.Windows.Forms.CheckBox();
            this.cboWeekMonday = new System.Windows.Forms.CheckBox();
            this.cboSchedule = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label13 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.grpMonth.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nupMonth)).BeginInit();
            this.grpOnce.SuspendLayout();
            this.grpDay.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nupDay)).BeginInit();
            this.grpWeek.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nupWeek)).BeginInit();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // cmdCancel
            // 
            this.cmdCancel.AccessibleDescription = null;
            this.cmdCancel.AccessibleName = null;
            resources.ApplyResources(this.cmdCancel, "cmdCancel");
            this.cmdCancel.BackgroundImage = null;
            this.cmdCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cmdCancel.Font = null;
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.UseVisualStyleBackColor = true;
            this.cmdCancel.Click += new System.EventHandler(this.cmdCancel_Click);
            // 
            // cmdOK
            // 
            this.cmdOK.AccessibleDescription = null;
            this.cmdOK.AccessibleName = null;
            resources.ApplyResources(this.cmdOK, "cmdOK");
            this.cmdOK.BackgroundImage = null;
            this.cmdOK.Font = null;
            this.cmdOK.Name = "cmdOK";
            this.cmdOK.UseVisualStyleBackColor = true;
            this.cmdOK.Click += new System.EventHandler(this.cmdOK_Click);
            // 
            // txtName
            // 
            this.txtName.AccessibleDescription = null;
            this.txtName.AccessibleName = null;
            resources.ApplyResources(this.txtName, "txtName");
            this.txtName.BackgroundImage = null;
            this.txtName.Font = null;
            this.txtName.Name = "txtName";
            // 
            // label1
            // 
            this.label1.AccessibleDescription = null;
            this.label1.AccessibleName = null;
            resources.ApplyResources(this.label1, "label1");
            this.label1.Font = null;
            this.label1.Name = "label1";
            // 
            // lvwDevices
            // 
            this.lvwDevices.AccessibleDescription = null;
            this.lvwDevices.AccessibleName = null;
            resources.ApplyResources(this.lvwDevices, "lvwDevices");
            this.lvwDevices.BackgroundImage = null;
            this.lvwDevices.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.lvwDevices.Font = null;
            this.lvwDevices.Name = "lvwDevices";
            this.lvwDevices.UseCompatibleStateImageBehavior = false;
            this.lvwDevices.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            resources.ApplyResources(this.columnHeader1, "columnHeader1");
            // 
            // columnHeader2
            // 
            resources.ApplyResources(this.columnHeader2, "columnHeader2");
            // 
            // label2
            // 
            this.label2.AccessibleDescription = null;
            this.label2.AccessibleName = null;
            resources.ApplyResources(this.label2, "label2");
            this.label2.Font = null;
            this.label2.Name = "label2";
            // 
            // txtDescription
            // 
            this.txtDescription.AccessibleDescription = null;
            this.txtDescription.AccessibleName = null;
            resources.ApplyResources(this.txtDescription, "txtDescription");
            this.txtDescription.BackgroundImage = null;
            this.txtDescription.Font = null;
            this.txtDescription.Name = "txtDescription";
            // 
            // cmdAdd
            // 
            this.cmdAdd.AccessibleDescription = null;
            this.cmdAdd.AccessibleName = null;
            resources.ApplyResources(this.cmdAdd, "cmdAdd");
            this.cmdAdd.BackgroundImage = null;
            this.cmdAdd.Font = null;
            this.cmdAdd.Name = "cmdAdd";
            this.cmdAdd.UseVisualStyleBackColor = true;
            this.cmdAdd.Click += new System.EventHandler(this.cmdAdd_Click);
            // 
            // cmdRemove
            // 
            this.cmdRemove.AccessibleDescription = null;
            this.cmdRemove.AccessibleName = null;
            resources.ApplyResources(this.cmdRemove, "cmdRemove");
            this.cmdRemove.BackgroundImage = null;
            this.cmdRemove.Font = null;
            this.cmdRemove.Name = "cmdRemove";
            this.cmdRemove.UseVisualStyleBackColor = true;
            this.cmdRemove.Click += new System.EventHandler(this.cmdRemove_Click);
            // 
            // label4
            // 
            this.label4.AccessibleDescription = null;
            this.label4.AccessibleName = null;
            resources.ApplyResources(this.label4, "label4");
            this.label4.Font = null;
            this.label4.Name = "label4";
           
            // 
            // dtpDate
            // 
            this.dtpDate.AccessibleDescription = null;
            this.dtpDate.AccessibleName = null;
            resources.ApplyResources(this.dtpDate, "dtpDate");
            this.dtpDate.BackgroundImage = null;
            this.dtpDate.CalendarFont = null;
            this.dtpDate.CustomFormat = null;
            this.dtpDate.Font = null;
            this.dtpDate.Name = "dtpDate";
         
            // 
            // groupBox1
            // 
            this.groupBox1.AccessibleDescription = null;
            this.groupBox1.AccessibleName = null;
            resources.ApplyResources(this.groupBox1, "groupBox1");
            this.groupBox1.BackgroundImage = null;
            this.groupBox1.Controls.Add(this.txtDescription);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.txtName);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Font = null;
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.TabStop = false;
            // 
            // groupBox2
            // 
            this.groupBox2.AccessibleDescription = null;
            this.groupBox2.AccessibleName = null;
            resources.ApplyResources(this.groupBox2, "groupBox2");
            this.groupBox2.BackgroundImage = null;
            this.groupBox2.Controls.Add(this.lvwDevices);
            this.groupBox2.Controls.Add(this.cmdAdd);
            this.groupBox2.Controls.Add(this.cmdRemove);
            this.groupBox2.Font = null;
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.TabStop = false;
            // 
            // groupBox3
            // 
            this.groupBox3.AccessibleDescription = null;
            this.groupBox3.AccessibleName = null;
            resources.ApplyResources(this.groupBox3, "groupBox3");
            this.groupBox3.BackgroundImage = null;
            this.groupBox3.Controls.Add(this.grpMonth);
            this.groupBox3.Controls.Add(this.grpOnce);
            this.groupBox3.Controls.Add(this.grpDay);
            this.groupBox3.Controls.Add(this.dtpTime);
            this.groupBox3.Controls.Add(this.grpWeek);
            this.groupBox3.Controls.Add(this.cboSchedule);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.dtpDate);
            this.groupBox3.Font = null;
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.TabStop = false;
            // 
            // grpMonth
            // 
            this.grpMonth.AccessibleDescription = null;
            this.grpMonth.AccessibleName = null;
            resources.ApplyResources(this.grpMonth, "grpMonth");
            this.grpMonth.BackgroundImage = null;
            this.grpMonth.Controls.Add(this.nupMonth);
            this.grpMonth.Controls.Add(this.label12);
            this.grpMonth.Controls.Add(this.cboMontDec);
            this.grpMonth.Controls.Add(this.cboMonthNov);
            this.grpMonth.Controls.Add(this.cboMontOct);
            this.grpMonth.Controls.Add(this.cboMontSept);
            this.grpMonth.Controls.Add(this.cboMonthAug);
            this.grpMonth.Controls.Add(this.cboMonthJuly);
            this.grpMonth.Controls.Add(this.cboMonthJune);
            this.grpMonth.Controls.Add(this.cboMonthMay);
            this.grpMonth.Controls.Add(this.cboMonthApr);
            this.grpMonth.Controls.Add(this.cboMonthMar);
            this.grpMonth.Controls.Add(this.cboMonthFeb);
            this.grpMonth.Controls.Add(this.cboMonthJan);
            this.grpMonth.Controls.Add(this.cboMonthDays);
            this.grpMonth.Controls.Add(this.cboOrder);
            this.grpMonth.Controls.Add(this.rdbMonthThe);
            this.grpMonth.Controls.Add(this.rdbMonthDay);
            this.grpMonth.Controls.Add(this.label11);
            this.grpMonth.Font = null;
            this.grpMonth.Name = "grpMonth";
            this.grpMonth.TabStop = false;
            // 
            // nupMonth
            // 
            this.nupMonth.AccessibleDescription = null;
            this.nupMonth.AccessibleName = null;
            resources.ApplyResources(this.nupMonth, "nupMonth");
            this.nupMonth.Font = null;
            this.nupMonth.Maximum = new decimal(new int[] {
            30,
            0,
            0,
            0});
            this.nupMonth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nupMonth.Name = "nupMonth";
            this.nupMonth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // label12
            // 
            this.label12.AccessibleDescription = null;
            this.label12.AccessibleName = null;
            resources.ApplyResources(this.label12, "label12");
            this.label12.Font = null;
            this.label12.Name = "label12";
            // 
            // cboMontDec
            // 
            this.cboMontDec.AccessibleDescription = null;
            this.cboMontDec.AccessibleName = null;
            resources.ApplyResources(this.cboMontDec, "cboMontDec");
            this.cboMontDec.BackgroundImage = null;
            this.cboMontDec.Font = null;
            this.cboMontDec.Name = "cboMontDec";
            this.cboMontDec.UseVisualStyleBackColor = true;
            // 
            // cboMonthNov
            // 
            this.cboMonthNov.AccessibleDescription = null;
            this.cboMonthNov.AccessibleName = null;
            resources.ApplyResources(this.cboMonthNov, "cboMonthNov");
            this.cboMonthNov.BackgroundImage = null;
            this.cboMonthNov.Font = null;
            this.cboMonthNov.Name = "cboMonthNov";
            this.cboMonthNov.UseVisualStyleBackColor = true;
            // 
            // cboMontOct
            // 
            this.cboMontOct.AccessibleDescription = null;
            this.cboMontOct.AccessibleName = null;
            resources.ApplyResources(this.cboMontOct, "cboMontOct");
            this.cboMontOct.BackgroundImage = null;
            this.cboMontOct.Font = null;
            this.cboMontOct.Name = "cboMontOct";
            this.cboMontOct.UseVisualStyleBackColor = true;
            // 
            // cboMontSept
            // 
            this.cboMontSept.AccessibleDescription = null;
            this.cboMontSept.AccessibleName = null;
            resources.ApplyResources(this.cboMontSept, "cboMontSept");
            this.cboMontSept.BackgroundImage = null;
            this.cboMontSept.Font = null;
            this.cboMontSept.Name = "cboMontSept";
            this.cboMontSept.UseVisualStyleBackColor = true;
            // 
            // cboMonthAug
            // 
            this.cboMonthAug.AccessibleDescription = null;
            this.cboMonthAug.AccessibleName = null;
            resources.ApplyResources(this.cboMonthAug, "cboMonthAug");
            this.cboMonthAug.BackgroundImage = null;
            this.cboMonthAug.Font = null;
            this.cboMonthAug.Name = "cboMonthAug";
            this.cboMonthAug.UseVisualStyleBackColor = true;
            // 
            // cboMonthJuly
            // 
            this.cboMonthJuly.AccessibleDescription = null;
            this.cboMonthJuly.AccessibleName = null;
            resources.ApplyResources(this.cboMonthJuly, "cboMonthJuly");
            this.cboMonthJuly.BackgroundImage = null;
            this.cboMonthJuly.Font = null;
            this.cboMonthJuly.Name = "cboMonthJuly";
            this.cboMonthJuly.UseVisualStyleBackColor = true;
            // 
            // cboMonthJune
            // 
            this.cboMonthJune.AccessibleDescription = null;
            this.cboMonthJune.AccessibleName = null;
            resources.ApplyResources(this.cboMonthJune, "cboMonthJune");
            this.cboMonthJune.BackgroundImage = null;
            this.cboMonthJune.Font = null;
            this.cboMonthJune.Name = "cboMonthJune";
            this.cboMonthJune.UseVisualStyleBackColor = true;
            // 
            // cboMonthMay
            // 
            this.cboMonthMay.AccessibleDescription = null;
            this.cboMonthMay.AccessibleName = null;
            resources.ApplyResources(this.cboMonthMay, "cboMonthMay");
            this.cboMonthMay.BackgroundImage = null;
            this.cboMonthMay.Font = null;
            this.cboMonthMay.Name = "cboMonthMay";
            this.cboMonthMay.UseVisualStyleBackColor = true;
            // 
            // cboMonthApr
            // 
            this.cboMonthApr.AccessibleDescription = null;
            this.cboMonthApr.AccessibleName = null;
            resources.ApplyResources(this.cboMonthApr, "cboMonthApr");
            this.cboMonthApr.BackgroundImage = null;
            this.cboMonthApr.Font = null;
            this.cboMonthApr.Name = "cboMonthApr";
            this.cboMonthApr.UseVisualStyleBackColor = true;
            // 
            // cboMonthMar
            // 
            this.cboMonthMar.AccessibleDescription = null;
            this.cboMonthMar.AccessibleName = null;
            resources.ApplyResources(this.cboMonthMar, "cboMonthMar");
            this.cboMonthMar.BackgroundImage = null;
            this.cboMonthMar.Font = null;
            this.cboMonthMar.Name = "cboMonthMar";
            this.cboMonthMar.UseVisualStyleBackColor = true;
            // 
            // cboMonthFeb
            // 
            this.cboMonthFeb.AccessibleDescription = null;
            this.cboMonthFeb.AccessibleName = null;
            resources.ApplyResources(this.cboMonthFeb, "cboMonthFeb");
            this.cboMonthFeb.BackgroundImage = null;
            this.cboMonthFeb.Font = null;
            this.cboMonthFeb.Name = "cboMonthFeb";
            this.cboMonthFeb.UseVisualStyleBackColor = true;
            // 
            // cboMonthJan
            // 
            this.cboMonthJan.AccessibleDescription = null;
            this.cboMonthJan.AccessibleName = null;
            resources.ApplyResources(this.cboMonthJan, "cboMonthJan");
            this.cboMonthJan.BackgroundImage = null;
            this.cboMonthJan.Font = null;
            this.cboMonthJan.Name = "cboMonthJan";
            this.cboMonthJan.UseVisualStyleBackColor = true;
            // 
            // cboMonthDays
            // 
            this.cboMonthDays.AccessibleDescription = null;
            this.cboMonthDays.AccessibleName = null;
            resources.ApplyResources(this.cboMonthDays, "cboMonthDays");
            this.cboMonthDays.BackgroundImage = null;
            this.cboMonthDays.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboMonthDays.Font = null;
            this.cboMonthDays.FormattingEnabled = true;
            this.cboMonthDays.Items.AddRange(new object[] {
            resources.GetString("cboMonthDays.Items"),
            resources.GetString("cboMonthDays.Items1"),
            resources.GetString("cboMonthDays.Items2"),
            resources.GetString("cboMonthDays.Items3"),
            resources.GetString("cboMonthDays.Items4"),
            resources.GetString("cboMonthDays.Items5"),
            resources.GetString("cboMonthDays.Items6")});
            this.cboMonthDays.Name = "cboMonthDays";
            // 
            // cboOrder
            // 
            this.cboOrder.AccessibleDescription = null;
            this.cboOrder.AccessibleName = null;
            resources.ApplyResources(this.cboOrder, "cboOrder");
            this.cboOrder.BackgroundImage = null;
            this.cboOrder.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboOrder.Font = null;
            this.cboOrder.FormattingEnabled = true;
            this.cboOrder.Items.AddRange(new object[] {
            resources.GetString("cboOrder.Items"),
            resources.GetString("cboOrder.Items1"),
            resources.GetString("cboOrder.Items2"),
            resources.GetString("cboOrder.Items3"),
            resources.GetString("cboOrder.Items4")});
            this.cboOrder.Name = "cboOrder";
            // 
            // rdbMonthThe
            // 
            this.rdbMonthThe.AccessibleDescription = null;
            this.rdbMonthThe.AccessibleName = null;
            resources.ApplyResources(this.rdbMonthThe, "rdbMonthThe");
            this.rdbMonthThe.BackgroundImage = null;
            this.rdbMonthThe.Font = null;
            this.rdbMonthThe.Name = "rdbMonthThe";
            this.rdbMonthThe.UseVisualStyleBackColor = true;
            // 
            // rdbMonthDay
            // 
            this.rdbMonthDay.AccessibleDescription = null;
            this.rdbMonthDay.AccessibleName = null;
            resources.ApplyResources(this.rdbMonthDay, "rdbMonthDay");
            this.rdbMonthDay.BackgroundImage = null;
            this.rdbMonthDay.Checked = true;
            this.rdbMonthDay.Font = null;
            this.rdbMonthDay.Name = "rdbMonthDay";
            this.rdbMonthDay.TabStop = true;
            this.rdbMonthDay.UseVisualStyleBackColor = true;
            // 
            // label11
            // 
            this.label11.AccessibleDescription = null;
            this.label11.AccessibleName = null;
            resources.ApplyResources(this.label11, "label11");
            this.label11.Font = null;
            this.label11.Name = "label11";
            // 
            // grpOnce
            // 
            this.grpOnce.AccessibleDescription = null;
            this.grpOnce.AccessibleName = null;
            resources.ApplyResources(this.grpOnce, "grpOnce");
            this.grpOnce.BackgroundImage = null;
            this.grpOnce.Controls.Add(this.label14);
            this.grpOnce.Font = null;
            this.grpOnce.Name = "grpOnce";
            this.grpOnce.TabStop = false;
            // 
            // label14
            // 
            this.label14.AccessibleDescription = null;
            this.label14.AccessibleName = null;
            resources.ApplyResources(this.label14, "label14");
            this.label14.Font = null;
            this.label14.Name = "label14";
            // 
            // grpDay
            // 
            this.grpDay.AccessibleDescription = null;
            this.grpDay.AccessibleName = null;
            resources.ApplyResources(this.grpDay, "grpDay");
            this.grpDay.BackgroundImage = null;
            this.grpDay.Controls.Add(this.nupDay);
            this.grpDay.Controls.Add(this.label8);
            this.grpDay.Controls.Add(this.label5);
            this.grpDay.Font = null;
            this.grpDay.Name = "grpDay";
            this.grpDay.TabStop = false;
            // 
            // nupDay
            // 
            this.nupDay.AccessibleDescription = null;
            this.nupDay.AccessibleName = null;
            resources.ApplyResources(this.nupDay, "nupDay");
            this.nupDay.Font = null;
            this.nupDay.Maximum = new decimal(new int[] {
            7,
            0,
            0,
            0});
            this.nupDay.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nupDay.Name = "nupDay";
            this.nupDay.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // label8
            // 
            this.label8.AccessibleDescription = null;
            this.label8.AccessibleName = null;
            resources.ApplyResources(this.label8, "label8");
            this.label8.Font = null;
            this.label8.Name = "label8";
            // 
            // label5
            // 
            this.label5.AccessibleDescription = null;
            this.label5.AccessibleName = null;
            resources.ApplyResources(this.label5, "label5");
            this.label5.Font = null;
            this.label5.Name = "label5";
            // 
            // dtpTime
            // 
            this.dtpTime.AccessibleDescription = null;
            this.dtpTime.AccessibleName = null;
            resources.ApplyResources(this.dtpTime, "dtpTime");
            this.dtpTime.BackgroundImage = null;
            this.dtpTime.CalendarFont = null;
            this.dtpTime.Font = null;
            this.dtpTime.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dtpTime.Name = "dtpTime";
            this.dtpTime.ShowUpDown = true;
            // 
            // grpWeek
            // 
            this.grpWeek.AccessibleDescription = null;
            this.grpWeek.AccessibleName = null;
            resources.ApplyResources(this.grpWeek, "grpWeek");
            this.grpWeek.BackgroundImage = null;
            this.grpWeek.Controls.Add(this.nupWeek);
            this.grpWeek.Controls.Add(this.label9);
            this.grpWeek.Controls.Add(this.label10);
            this.grpWeek.Controls.Add(this.cboWeekSaturday);
            this.grpWeek.Controls.Add(this.cboWeekSunday);
            this.grpWeek.Controls.Add(this.cboWeekFriday);
            this.grpWeek.Controls.Add(this.cboWeekThursday);
            this.grpWeek.Controls.Add(this.cboWeekWednesday);
            this.grpWeek.Controls.Add(this.cboWeekTuesday);
            this.grpWeek.Controls.Add(this.cboWeekMonday);
            this.grpWeek.Font = null;
            this.grpWeek.Name = "grpWeek";
            this.grpWeek.TabStop = false;
            // 
            // nupWeek
            // 
            this.nupWeek.AccessibleDescription = null;
            this.nupWeek.AccessibleName = null;
            resources.ApplyResources(this.nupWeek, "nupWeek");
            this.nupWeek.Font = null;
            this.nupWeek.Maximum = new decimal(new int[] {
            52,
            0,
            0,
            0});
            this.nupWeek.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nupWeek.Name = "nupWeek";
            this.nupWeek.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // label9
            // 
            this.label9.AccessibleDescription = null;
            this.label9.AccessibleName = null;
            resources.ApplyResources(this.label9, "label9");
            this.label9.Font = null;
            this.label9.Name = "label9";
            // 
            // label10
            // 
            this.label10.AccessibleDescription = null;
            this.label10.AccessibleName = null;
            resources.ApplyResources(this.label10, "label10");
            this.label10.Font = null;
            this.label10.Name = "label10";
            // 
            // cboWeekSaturday
            // 
            this.cboWeekSaturday.AccessibleDescription = null;
            this.cboWeekSaturday.AccessibleName = null;
            resources.ApplyResources(this.cboWeekSaturday, "cboWeekSaturday");
            this.cboWeekSaturday.BackgroundImage = null;
            this.cboWeekSaturday.Font = null;
            this.cboWeekSaturday.Name = "cboWeekSaturday";
            this.cboWeekSaturday.UseVisualStyleBackColor = true;
            // 
            // cboWeekSunday
            // 
            this.cboWeekSunday.AccessibleDescription = null;
            this.cboWeekSunday.AccessibleName = null;
            resources.ApplyResources(this.cboWeekSunday, "cboWeekSunday");
            this.cboWeekSunday.BackgroundImage = null;
            this.cboWeekSunday.Font = null;
            this.cboWeekSunday.Name = "cboWeekSunday";
            this.cboWeekSunday.UseVisualStyleBackColor = true;
            // 
            // cboWeekFriday
            // 
            this.cboWeekFriday.AccessibleDescription = null;
            this.cboWeekFriday.AccessibleName = null;
            resources.ApplyResources(this.cboWeekFriday, "cboWeekFriday");
            this.cboWeekFriday.BackgroundImage = null;
            this.cboWeekFriday.Font = null;
            this.cboWeekFriday.Name = "cboWeekFriday";
            this.cboWeekFriday.UseVisualStyleBackColor = true;
            // 
            // cboWeekThursday
            // 
            this.cboWeekThursday.AccessibleDescription = null;
            this.cboWeekThursday.AccessibleName = null;
            resources.ApplyResources(this.cboWeekThursday, "cboWeekThursday");
            this.cboWeekThursday.BackgroundImage = null;
            this.cboWeekThursday.Font = null;
            this.cboWeekThursday.Name = "cboWeekThursday";
            this.cboWeekThursday.UseVisualStyleBackColor = true;
            // 
            // cboWeekWednesday
            // 
            this.cboWeekWednesday.AccessibleDescription = null;
            this.cboWeekWednesday.AccessibleName = null;
            resources.ApplyResources(this.cboWeekWednesday, "cboWeekWednesday");
            this.cboWeekWednesday.BackgroundImage = null;
            this.cboWeekWednesday.Font = null;
            this.cboWeekWednesday.Name = "cboWeekWednesday";
            this.cboWeekWednesday.UseVisualStyleBackColor = true;
            // 
            // cboWeekTuesday
            // 
            this.cboWeekTuesday.AccessibleDescription = null;
            this.cboWeekTuesday.AccessibleName = null;
            resources.ApplyResources(this.cboWeekTuesday, "cboWeekTuesday");
            this.cboWeekTuesday.BackgroundImage = null;
            this.cboWeekTuesday.Font = null;
            this.cboWeekTuesday.Name = "cboWeekTuesday";
            this.cboWeekTuesday.UseVisualStyleBackColor = true;
            // 
            // cboWeekMonday
            // 
            this.cboWeekMonday.AccessibleDescription = null;
            this.cboWeekMonday.AccessibleName = null;
            resources.ApplyResources(this.cboWeekMonday, "cboWeekMonday");
            this.cboWeekMonday.BackgroundImage = null;
            this.cboWeekMonday.Font = null;
            this.cboWeekMonday.Name = "cboWeekMonday";
            this.cboWeekMonday.UseVisualStyleBackColor = true;
            // 
            // cboSchedule
            // 
            this.cboSchedule.AccessibleDescription = null;
            this.cboSchedule.AccessibleName = null;
            resources.ApplyResources(this.cboSchedule, "cboSchedule");
            this.cboSchedule.BackgroundImage = null;
            this.cboSchedule.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboSchedule.Font = null;
            this.cboSchedule.FormattingEnabled = true;
            this.cboSchedule.Items.AddRange(new object[] {
            resources.GetString("cboSchedule.Items"),
            resources.GetString("cboSchedule.Items1"),
            resources.GetString("cboSchedule.Items2"),
            resources.GetString("cboSchedule.Items3")});
            this.cboSchedule.Name = "cboSchedule";
            this.cboSchedule.SelectedIndexChanged += new System.EventHandler(this.cboSchedule_SelectedIndexChanged);
            // 
            // label7
            // 
            this.label7.AccessibleDescription = null;
            this.label7.AccessibleName = null;
            resources.ApplyResources(this.label7, "label7");
            this.label7.Font = null;
            this.label7.Name = "label7";
            // 
            // label6
            // 
            this.label6.AccessibleDescription = null;
            this.label6.AccessibleName = null;
            resources.ApplyResources(this.label6, "label6");
            this.label6.Font = null;
            this.label6.Name = "label6";
            // 
            // tabControl1
            // 
            this.tabControl1.AccessibleDescription = null;
            this.tabControl1.AccessibleName = null;
            resources.ApplyResources(this.tabControl1, "tabControl1");
            this.tabControl1.BackgroundImage = null;
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Font = null;
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.AccessibleDescription = null;
            this.tabPage1.AccessibleName = null;
            resources.ApplyResources(this.tabPage1, "tabPage1");
            this.tabPage1.BackgroundImage = null;
            this.tabPage1.Controls.Add(this.groupBox1);
            this.tabPage1.Controls.Add(this.groupBox2);
            this.tabPage1.Font = null;
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // tabPage2
            // 
            this.tabPage2.AccessibleDescription = null;
            this.tabPage2.AccessibleName = null;
            resources.ApplyResources(this.tabPage2, "tabPage2");
            this.tabPage2.BackgroundImage = null;
            this.tabPage2.Controls.Add(this.groupBox3);
            this.tabPage2.Font = null;
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // panel1
            // 
            this.panel1.AccessibleDescription = null;
            this.panel1.AccessibleName = null;
            resources.ApplyResources(this.panel1, "panel1");
            this.panel1.BackColor = System.Drawing.Color.White;
            this.panel1.BackgroundImage = null;
            this.panel1.Controls.Add(this.label13);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.pictureBox1);
            this.panel1.Font = null;
            this.panel1.Name = "panel1";
            // 
            // label13
            // 
            this.label13.AccessibleDescription = null;
            this.label13.AccessibleName = null;
            resources.ApplyResources(this.label13, "label13");
            this.label13.Font = null;
            this.label13.Name = "label13";
            // 
            // label3
            // 
            this.label3.AccessibleDescription = null;
            this.label3.AccessibleName = null;
            resources.ApplyResources(this.label3, "label3");
            this.label3.Name = "label3";
            // 
            // pictureBox1
            // 
            this.pictureBox1.AccessibleDescription = null;
            this.pictureBox1.AccessibleName = null;
            resources.ApplyResources(this.pictureBox1, "pictureBox1");
            this.pictureBox1.BackgroundImage = null;
            this.pictureBox1.Font = null;
            this.pictureBox1.ImageLocation = null;
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.TabStop = false;
            // 
            // frmSchedule
            // 
            this.AcceptButton = this.cmdOK;
            this.AccessibleDescription = null;
            this.AccessibleName = null;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = null;
            this.CancelButton = this.cmdCancel;
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.cmdOK);
            this.Controls.Add(this.cmdCancel);
            this.Font = null;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = null;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmSchedule";
            this.Load += new System.EventHandler(this.frmSchedule_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.grpMonth.ResumeLayout(false);
            this.grpMonth.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nupMonth)).EndInit();
            this.grpOnce.ResumeLayout(false);
            this.grpOnce.PerformLayout();
            this.grpDay.ResumeLayout(false);
            this.grpDay.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nupDay)).EndInit();
            this.grpWeek.ResumeLayout(false);
            this.grpWeek.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nupWeek)).EndInit();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button cmdCancel;
        private System.Windows.Forms.Button cmdOK;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListView lvwDevices;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtDescription;
        private System.Windows.Forms.Button cmdAdd;
        private System.Windows.Forms.Button cmdRemove;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.DateTimePicker dtpDate;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.ComboBox cboSchedule;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.GroupBox grpDay;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox grpWeek;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.CheckBox cboWeekSaturday;
        private System.Windows.Forms.CheckBox cboWeekSunday;
        private System.Windows.Forms.CheckBox cboWeekFriday;
        private System.Windows.Forms.CheckBox cboWeekThursday;
        private System.Windows.Forms.CheckBox cboWeekWednesday;
        private System.Windows.Forms.CheckBox cboWeekTuesday;
        private System.Windows.Forms.CheckBox cboWeekMonday;
        private System.Windows.Forms.GroupBox grpMonth;
        private System.Windows.Forms.CheckBox cboMonthApr;
        private System.Windows.Forms.CheckBox cboMonthMar;
        private System.Windows.Forms.CheckBox cboMonthFeb;
        private System.Windows.Forms.CheckBox cboMonthJan;
        private System.Windows.Forms.ComboBox cboMonthDays;
        private System.Windows.Forms.ComboBox cboOrder;
        private System.Windows.Forms.RadioButton rdbMonthThe;
        private System.Windows.Forms.RadioButton rdbMonthDay;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.CheckBox cboMontDec;
        private System.Windows.Forms.CheckBox cboMonthNov;
        private System.Windows.Forms.CheckBox cboMontOct;
        private System.Windows.Forms.CheckBox cboMontSept;
        private System.Windows.Forms.CheckBox cboMonthAug;
        private System.Windows.Forms.CheckBox cboMonthJuly;
        private System.Windows.Forms.CheckBox cboMonthJune;
        private System.Windows.Forms.CheckBox cboMonthMay;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.NumericUpDown nupWeek;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.NumericUpDown nupDay;
        private System.Windows.Forms.NumericUpDown nupMonth;
        private System.Windows.Forms.DateTimePicker dtpTime;
        private System.Windows.Forms.GroupBox grpOnce;
        private System.Windows.Forms.Label label14;
    }
}