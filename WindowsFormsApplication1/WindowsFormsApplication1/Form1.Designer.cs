namespace WindowsFormsApplication1
{
    partial class Form1
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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.ColumnHeader Command;
            System.Windows.Forms.ColumnHeader columnHeader7;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.buttonExitApplication = new System.Windows.Forms.Button();
            this.listBoxPicRadioCommands = new System.Windows.Forms.ListBox();
            this.textBoxPic = new System.Windows.Forms.TextBox();
            this.labelPort1Number = new System.Windows.Forms.Label();
            this.SerialPortPic = new System.IO.Ports.SerialPort(this.components);
            this.GroupBoxPicComPortSettings = new System.Windows.Forms.GroupBox();
            this.buttonApplyCom1Settings = new System.Windows.Forms.Button();
            this.comboBoxPort1FlowControl = new System.Windows.Forms.ComboBox();
            this.comboBoxPort1StopBits = new System.Windows.Forms.ComboBox();
            this.comboBoxPort1BaudRate = new System.Windows.Forms.ComboBox();
            this.comboBoxPort1Parity = new System.Windows.Forms.ComboBox();
            this.comboBoxPort1DataBits = new System.Windows.Forms.ComboBox();
            this.comboBoxPort1Number = new System.Windows.Forms.ComboBox();
            this.labelComPort1Status = new System.Windows.Forms.Label();
            this.labelPort1FlowControl = new System.Windows.Forms.Label();
            this.labelPort1StopBits = new System.Windows.Forms.Label();
            this.labelPort1BaudRate = new System.Windows.Forms.Label();
            this.labelPort1Parity = new System.Windows.Forms.Label();
            this.labelPort1DataBits = new System.Windows.Forms.Label();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.PICtail = new System.Windows.Forms.TabPage();
            this.groupBoxPicCommandsBatches = new System.Windows.Forms.GroupBox();
            this.buttonPicCBEdit = new System.Windows.Forms.Button();
            this.buttonPicCBRemove = new System.Windows.Forms.Button();
            this.buttonPicCBAdd = new System.Windows.Forms.Button();
            this.listViewPicCommandsBatches = new System.Windows.Forms.ListView();
            this.BatchName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.BatchContent = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Semtech = new System.Windows.Forms.TabPage();
            this.groupBoxSemtechCommandsBatches = new System.Windows.Forms.GroupBox();
            this.buttonSemtechCBEdit = new System.Windows.Forms.Button();
            this.buttonSemtechCBRemove = new System.Windows.Forms.Button();
            this.buttonSemtechCBAdd = new System.Windows.Forms.Button();
            this.listViewSemtechCommandsBatches = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.textBoxSemtech = new System.Windows.Forms.TextBox();
            this.GroupBoxSemtechComPortSettings = new System.Windows.Forms.GroupBox();
            this.buttonApplyCom2Settings = new System.Windows.Forms.Button();
            this.comboBoxPort2FlowControl = new System.Windows.Forms.ComboBox();
            this.comboBoxPort2StopBits = new System.Windows.Forms.ComboBox();
            this.comboBoxPort2BaudRate = new System.Windows.Forms.ComboBox();
            this.comboBoxPort2Parity = new System.Windows.Forms.ComboBox();
            this.comboBoxPort2DataBits = new System.Windows.Forms.ComboBox();
            this.comboBoxPort2Number = new System.Windows.Forms.ComboBox();
            this.labelComPort2Status = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.listBoxSemtechRadioCommands = new System.Windows.Forms.ListBox();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.groupBoxUbiquitiCommandsBatches = new System.Windows.Forms.GroupBox();
            this.buttonUbiquitiCBEdit = new System.Windows.Forms.Button();
            this.buttonUbiquitiCBRemove = new System.Windows.Forms.Button();
            this.buttonUbiquitiCBAdd = new System.Windows.Forms.Button();
            this.listViewUbiquitiCommandsBatches = new System.Windows.Forms.ListView();
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.textBoxUbiquiti = new System.Windows.Forms.TextBox();
            this.GroupBoxUbiquitiComPortSettings = new System.Windows.Forms.GroupBox();
            this.buttonApplyCom3Settings = new System.Windows.Forms.Button();
            this.comboBoxPort3FlowControl = new System.Windows.Forms.ComboBox();
            this.comboBoxPort3StopBits = new System.Windows.Forms.ComboBox();
            this.comboBoxPort3BaudRate = new System.Windows.Forms.ComboBox();
            this.comboBoxPort3Parity = new System.Windows.Forms.ComboBox();
            this.comboBoxPort3DataBits = new System.Windows.Forms.ComboBox();
            this.comboBoxPort3Number = new System.Windows.Forms.ComboBox();
            this.labelComPort3Status = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.listViewUbiquitiRadioCommands = new System.Windows.Forms.ListView();
            this.CmdCode = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.CmdData = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.buttonSTHackedCBEdit = new System.Windows.Forms.Button();
            this.buttonSTHackedCBRemove = new System.Windows.Forms.Button();
            this.buttonSTHackedCBAdd = new System.Windows.Forms.Button();
            this.listViewSTHackedCommandsBatches = new System.Windows.Forms.ListView();
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.textBoxSTHacked = new System.Windows.Forms.TextBox();
            this.GroupBoxSTHackedComPortSettings = new System.Windows.Forms.GroupBox();
            this.buttonApplyCom4Settings = new System.Windows.Forms.Button();
            this.comboBoxPort4FlowControl = new System.Windows.Forms.ComboBox();
            this.comboBoxPort4StopBits = new System.Windows.Forms.ComboBox();
            this.comboBoxPort4BaudRate = new System.Windows.Forms.ComboBox();
            this.comboBoxPort4Parity = new System.Windows.Forms.ComboBox();
            this.comboBoxPort4DataBits = new System.Windows.Forms.ComboBox();
            this.comboBoxPort4Number = new System.Windows.Forms.ComboBox();
            this.labelComPort4Status = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.listViewSTHackedRadioCommands = new System.Windows.Forms.ListView();
            this.columnHeader8 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader9 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.SerialPortSemtech = new System.IO.Ports.SerialPort(this.components);
            this.textBoxGeneralData = new System.Windows.Forms.TextBox();
            this.SerialPortUbiquiti = new System.IO.Ports.SerialPort(this.components);
            this.SerialPortSTHacked = new System.IO.Ports.SerialPort(this.components);
            Command = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.GroupBoxPicComPortSettings.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.PICtail.SuspendLayout();
            this.groupBoxPicCommandsBatches.SuspendLayout();
            this.Semtech.SuspendLayout();
            this.groupBoxSemtechCommandsBatches.SuspendLayout();
            this.GroupBoxSemtechComPortSettings.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.groupBoxUbiquitiCommandsBatches.SuspendLayout();
            this.GroupBoxUbiquitiComPortSettings.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.GroupBoxSTHackedComPortSettings.SuspendLayout();
            this.SuspendLayout();
            // 
            // Command
            // 
            Command.Tag = "";
            Command.Text = "Command";
            Command.Width = 300;
            // 
            // columnHeader7
            // 
            columnHeader7.Tag = "";
            columnHeader7.Text = "Command";
            columnHeader7.Width = 300;
            // 
            // buttonExitApplication
            // 
            this.buttonExitApplication.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonExitApplication.FlatAppearance.BorderSize = 3;
            this.buttonExitApplication.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonExitApplication.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonExitApplication.Location = new System.Drawing.Point(853, 466);
            this.buttonExitApplication.Margin = new System.Windows.Forms.Padding(4);
            this.buttonExitApplication.Name = "buttonExitApplication";
            this.buttonExitApplication.Size = new System.Drawing.Size(150, 38);
            this.buttonExitApplication.TabIndex = 0;
            this.buttonExitApplication.Text = "Close";
            this.buttonExitApplication.UseVisualStyleBackColor = false;
            this.buttonExitApplication.Click += new System.EventHandler(this.buttonExitApplication_Click);
            // 
            // listBoxPicRadioCommands
            // 
            this.listBoxPicRadioCommands.BackColor = System.Drawing.SystemColors.MenuBar;
            this.listBoxPicRadioCommands.FormattingEnabled = true;
            this.listBoxPicRadioCommands.HorizontalScrollbar = true;
            this.listBoxPicRadioCommands.ItemHeight = 16;
            this.listBoxPicRadioCommands.Location = new System.Drawing.Point(325, 15);
            this.listBoxPicRadioCommands.Name = "listBoxPicRadioCommands";
            this.listBoxPicRadioCommands.Size = new System.Drawing.Size(245, 324);
            this.listBoxPicRadioCommands.TabIndex = 1;
            this.listBoxPicRadioCommands.DoubleClick += new System.EventHandler(this.listBoxPicRadioCommands_DoubleClick);
            // 
            // textBoxPic
            // 
            this.textBoxPic.BackColor = System.Drawing.SystemColors.InactiveCaptionText;
            this.textBoxPic.Cursor = System.Windows.Forms.Cursors.Default;
            this.textBoxPic.ForeColor = System.Drawing.Color.White;
            this.textBoxPic.Location = new System.Drawing.Point(20, 15);
            this.textBoxPic.Multiline = true;
            this.textBoxPic.Name = "textBoxPic";
            this.textBoxPic.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxPic.Size = new System.Drawing.Size(291, 324);
            this.textBoxPic.TabIndex = 2;
            this.textBoxPic.WordWrap = false;
            this.textBoxPic.TextChanged += new System.EventHandler(this.textBoxPic_TextChanged);
            // 
            // labelPort1Number
            // 
            this.labelPort1Number.AutoSize = true;
            this.labelPort1Number.BackColor = System.Drawing.Color.Transparent;
            this.labelPort1Number.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelPort1Number.Location = new System.Drawing.Point(58, 40);
            this.labelPort1Number.Name = "labelPort1Number";
            this.labelPort1Number.Size = new System.Drawing.Size(54, 19);
            this.labelPort1Number.TabIndex = 3;
            this.labelPort1Number.Text = "Port #";
            this.labelPort1Number.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // GroupBoxPicComPortSettings
            // 
            this.GroupBoxPicComPortSettings.BackColor = System.Drawing.Color.LightGray;
            this.GroupBoxPicComPortSettings.Controls.Add(this.buttonApplyCom1Settings);
            this.GroupBoxPicComPortSettings.Controls.Add(this.comboBoxPort1FlowControl);
            this.GroupBoxPicComPortSettings.Controls.Add(this.comboBoxPort1StopBits);
            this.GroupBoxPicComPortSettings.Controls.Add(this.comboBoxPort1BaudRate);
            this.GroupBoxPicComPortSettings.Controls.Add(this.comboBoxPort1Parity);
            this.GroupBoxPicComPortSettings.Controls.Add(this.comboBoxPort1DataBits);
            this.GroupBoxPicComPortSettings.Controls.Add(this.comboBoxPort1Number);
            this.GroupBoxPicComPortSettings.Controls.Add(this.labelComPort1Status);
            this.GroupBoxPicComPortSettings.Controls.Add(this.labelPort1FlowControl);
            this.GroupBoxPicComPortSettings.Controls.Add(this.labelPort1StopBits);
            this.GroupBoxPicComPortSettings.Controls.Add(this.labelPort1BaudRate);
            this.GroupBoxPicComPortSettings.Controls.Add(this.labelPort1Parity);
            this.GroupBoxPicComPortSettings.Controls.Add(this.labelPort1DataBits);
            this.GroupBoxPicComPortSettings.Controls.Add(this.labelPort1Number);
            this.GroupBoxPicComPortSettings.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.GroupBoxPicComPortSettings.Location = new System.Drawing.Point(830, 15);
            this.GroupBoxPicComPortSettings.Name = "GroupBoxPicComPortSettings";
            this.GroupBoxPicComPortSettings.Size = new System.Drawing.Size(260, 324);
            this.GroupBoxPicComPortSettings.TabIndex = 4;
            this.GroupBoxPicComPortSettings.TabStop = false;
            this.GroupBoxPicComPortSettings.Text = "Com Port Settings";
            // 
            // buttonApplyCom1Settings
            // 
            this.buttonApplyCom1Settings.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonApplyCom1Settings.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonApplyCom1Settings.FlatAppearance.BorderSize = 3;
            this.buttonApplyCom1Settings.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonApplyCom1Settings.Location = new System.Drawing.Point(179, 282);
            this.buttonApplyCom1Settings.Name = "buttonApplyCom1Settings";
            this.buttonApplyCom1Settings.Size = new System.Drawing.Size(70, 26);
            this.buttonApplyCom1Settings.TabIndex = 5;
            this.buttonApplyCom1Settings.Text = "Apply";
            this.buttonApplyCom1Settings.UseVisualStyleBackColor = false;
            this.buttonApplyCom1Settings.Click += new System.EventHandler(this.buttonApplyCom1Settings_Click);
            // 
            // comboBoxPort1FlowControl
            // 
            this.comboBoxPort1FlowControl.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort1FlowControl.FormattingEnabled = true;
            this.comboBoxPort1FlowControl.Location = new System.Drawing.Point(128, 232);
            this.comboBoxPort1FlowControl.Name = "comboBoxPort1FlowControl";
            this.comboBoxPort1FlowControl.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort1FlowControl.TabIndex = 4;
            // 
            // comboBoxPort1StopBits
            // 
            this.comboBoxPort1StopBits.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort1StopBits.FormattingEnabled = true;
            this.comboBoxPort1StopBits.Location = new System.Drawing.Point(128, 154);
            this.comboBoxPort1StopBits.Name = "comboBoxPort1StopBits";
            this.comboBoxPort1StopBits.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort1StopBits.TabIndex = 4;
            // 
            // comboBoxPort1BaudRate
            // 
            this.comboBoxPort1BaudRate.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort1BaudRate.FormattingEnabled = true;
            this.comboBoxPort1BaudRate.Location = new System.Drawing.Point(128, 77);
            this.comboBoxPort1BaudRate.Name = "comboBoxPort1BaudRate";
            this.comboBoxPort1BaudRate.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort1BaudRate.TabIndex = 4;
            // 
            // comboBoxPort1Parity
            // 
            this.comboBoxPort1Parity.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort1Parity.FormattingEnabled = true;
            this.comboBoxPort1Parity.Location = new System.Drawing.Point(128, 193);
            this.comboBoxPort1Parity.Name = "comboBoxPort1Parity";
            this.comboBoxPort1Parity.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort1Parity.TabIndex = 4;
            // 
            // comboBoxPort1DataBits
            // 
            this.comboBoxPort1DataBits.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort1DataBits.FormattingEnabled = true;
            this.comboBoxPort1DataBits.Location = new System.Drawing.Point(128, 115);
            this.comboBoxPort1DataBits.Name = "comboBoxPort1DataBits";
            this.comboBoxPort1DataBits.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort1DataBits.TabIndex = 4;
            // 
            // comboBoxPort1Number
            // 
            this.comboBoxPort1Number.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort1Number.FormattingEnabled = true;
            this.comboBoxPort1Number.Location = new System.Drawing.Point(128, 40);
            this.comboBoxPort1Number.Name = "comboBoxPort1Number";
            this.comboBoxPort1Number.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort1Number.TabIndex = 4;
            // 
            // labelComPort1Status
            // 
            this.labelComPort1Status.AutoSize = true;
            this.labelComPort1Status.BackColor = System.Drawing.SystemColors.InfoText;
            this.labelComPort1Status.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelComPort1Status.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.labelComPort1Status.Location = new System.Drawing.Point(13, 287);
            this.labelComPort1Status.Name = "labelComPort1Status";
            this.labelComPort1Status.Size = new System.Drawing.Size(127, 19);
            this.labelComPort1Status.TabIndex = 3;
            this.labelComPort1Status.Text = "Port not opened";
            // 
            // labelPort1FlowControl
            // 
            this.labelPort1FlowControl.AutoSize = true;
            this.labelPort1FlowControl.BackColor = System.Drawing.Color.Transparent;
            this.labelPort1FlowControl.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelPort1FlowControl.ForeColor = System.Drawing.Color.Black;
            this.labelPort1FlowControl.Location = new System.Drawing.Point(13, 232);
            this.labelPort1FlowControl.Name = "labelPort1FlowControl";
            this.labelPort1FlowControl.Size = new System.Drawing.Size(99, 19);
            this.labelPort1FlowControl.TabIndex = 3;
            this.labelPort1FlowControl.Text = "Flow Control";
            this.labelPort1FlowControl.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // labelPort1StopBits
            // 
            this.labelPort1StopBits.AutoSize = true;
            this.labelPort1StopBits.BackColor = System.Drawing.Color.Transparent;
            this.labelPort1StopBits.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelPort1StopBits.Location = new System.Drawing.Point(37, 154);
            this.labelPort1StopBits.Name = "labelPort1StopBits";
            this.labelPort1StopBits.Size = new System.Drawing.Size(75, 19);
            this.labelPort1StopBits.TabIndex = 3;
            this.labelPort1StopBits.Text = "Stop Bits";
            this.labelPort1StopBits.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // labelPort1BaudRate
            // 
            this.labelPort1BaudRate.AutoSize = true;
            this.labelPort1BaudRate.BackColor = System.Drawing.Color.Transparent;
            this.labelPort1BaudRate.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelPort1BaudRate.Location = new System.Drawing.Point(26, 77);
            this.labelPort1BaudRate.Name = "labelPort1BaudRate";
            this.labelPort1BaudRate.Size = new System.Drawing.Size(86, 19);
            this.labelPort1BaudRate.TabIndex = 3;
            this.labelPort1BaudRate.Text = "Baud Rate";
            this.labelPort1BaudRate.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // labelPort1Parity
            // 
            this.labelPort1Parity.AutoSize = true;
            this.labelPort1Parity.BackColor = System.Drawing.Color.Transparent;
            this.labelPort1Parity.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelPort1Parity.Location = new System.Drawing.Point(60, 193);
            this.labelPort1Parity.Name = "labelPort1Parity";
            this.labelPort1Parity.Size = new System.Drawing.Size(52, 19);
            this.labelPort1Parity.TabIndex = 3;
            this.labelPort1Parity.Text = "Parity";
            this.labelPort1Parity.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // labelPort1DataBits
            // 
            this.labelPort1DataBits.AutoSize = true;
            this.labelPort1DataBits.BackColor = System.Drawing.Color.Transparent;
            this.labelPort1DataBits.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelPort1DataBits.Location = new System.Drawing.Point(37, 115);
            this.labelPort1DataBits.Name = "labelPort1DataBits";
            this.labelPort1DataBits.Size = new System.Drawing.Size(75, 19);
            this.labelPort1DataBits.TabIndex = 3;
            this.labelPort1DataBits.Text = "Data bits";
            this.labelPort1DataBits.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.PICtail);
            this.tabControl1.Controls.Add(this.Semtech);
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(5, 5);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1108, 386);
            this.tabControl1.TabIndex = 5;
            // 
            // PICtail
            // 
            this.PICtail.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("PICtail.BackgroundImage")));
            this.PICtail.Controls.Add(this.groupBoxPicCommandsBatches);
            this.PICtail.Controls.Add(this.textBoxPic);
            this.PICtail.Controls.Add(this.GroupBoxPicComPortSettings);
            this.PICtail.Controls.Add(this.listBoxPicRadioCommands);
            this.PICtail.Location = new System.Drawing.Point(4, 25);
            this.PICtail.Name = "PICtail";
            this.PICtail.Padding = new System.Windows.Forms.Padding(3);
            this.PICtail.Size = new System.Drawing.Size(1100, 357);
            this.PICtail.TabIndex = 0;
            this.PICtail.Text = "PICtail";
            this.PICtail.UseVisualStyleBackColor = true;
            // 
            // groupBoxPicCommandsBatches
            // 
            this.groupBoxPicCommandsBatches.BackColor = System.Drawing.Color.LightGray;
            this.groupBoxPicCommandsBatches.Controls.Add(this.buttonPicCBEdit);
            this.groupBoxPicCommandsBatches.Controls.Add(this.buttonPicCBRemove);
            this.groupBoxPicCommandsBatches.Controls.Add(this.buttonPicCBAdd);
            this.groupBoxPicCommandsBatches.Controls.Add(this.listViewPicCommandsBatches);
            this.groupBoxPicCommandsBatches.Location = new System.Drawing.Point(585, 15);
            this.groupBoxPicCommandsBatches.Name = "groupBoxPicCommandsBatches";
            this.groupBoxPicCommandsBatches.Size = new System.Drawing.Size(230, 324);
            this.groupBoxPicCommandsBatches.TabIndex = 6;
            this.groupBoxPicCommandsBatches.TabStop = false;
            this.groupBoxPicCommandsBatches.Text = "Commands Batches";
            // 
            // buttonPicCBEdit
            // 
            this.buttonPicCBEdit.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonPicCBEdit.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonPicCBEdit.FlatAppearance.BorderSize = 3;
            this.buttonPicCBEdit.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonPicCBEdit.Location = new System.Drawing.Point(164, 287);
            this.buttonPicCBEdit.Name = "buttonPicCBEdit";
            this.buttonPicCBEdit.Size = new System.Drawing.Size(56, 26);
            this.buttonPicCBEdit.TabIndex = 7;
            this.buttonPicCBEdit.Text = "Edit";
            this.buttonPicCBEdit.UseVisualStyleBackColor = false;
            this.buttonPicCBEdit.Click += new System.EventHandler(this.buttonPicCBEdit_Click);
            // 
            // buttonPicCBRemove
            // 
            this.buttonPicCBRemove.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonPicCBRemove.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonPicCBRemove.FlatAppearance.BorderSize = 3;
            this.buttonPicCBRemove.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonPicCBRemove.Location = new System.Drawing.Point(74, 287);
            this.buttonPicCBRemove.Name = "buttonPicCBRemove";
            this.buttonPicCBRemove.Size = new System.Drawing.Size(84, 26);
            this.buttonPicCBRemove.TabIndex = 7;
            this.buttonPicCBRemove.Text = "Remove";
            this.buttonPicCBRemove.UseVisualStyleBackColor = false;
            this.buttonPicCBRemove.Click += new System.EventHandler(this.buttonPicCBRemove_Click);
            // 
            // buttonPicCBAdd
            // 
            this.buttonPicCBAdd.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonPicCBAdd.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonPicCBAdd.FlatAppearance.BorderSize = 3;
            this.buttonPicCBAdd.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonPicCBAdd.Location = new System.Drawing.Point(10, 287);
            this.buttonPicCBAdd.Name = "buttonPicCBAdd";
            this.buttonPicCBAdd.Size = new System.Drawing.Size(58, 26);
            this.buttonPicCBAdd.TabIndex = 6;
            this.buttonPicCBAdd.Text = "Add";
            this.buttonPicCBAdd.UseVisualStyleBackColor = false;
            this.buttonPicCBAdd.Click += new System.EventHandler(this.buttonPicCBAdd_Click);
            // 
            // listViewPicCommandsBatches
            // 
            this.listViewPicCommandsBatches.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.listViewPicCommandsBatches.BackColor = System.Drawing.SystemColors.Info;
            this.listViewPicCommandsBatches.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.BatchName,
            this.BatchContent});
            this.listViewPicCommandsBatches.GridLines = true;
            this.listViewPicCommandsBatches.HideSelection = false;
            this.listViewPicCommandsBatches.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.listViewPicCommandsBatches.LabelWrap = false;
            this.listViewPicCommandsBatches.Location = new System.Drawing.Point(10, 22);
            this.listViewPicCommandsBatches.MultiSelect = false;
            this.listViewPicCommandsBatches.Name = "listViewPicCommandsBatches";
            this.listViewPicCommandsBatches.Size = new System.Drawing.Size(210, 251);
            this.listViewPicCommandsBatches.TabIndex = 5;
            this.listViewPicCommandsBatches.UseCompatibleStateImageBehavior = false;
            this.listViewPicCommandsBatches.View = System.Windows.Forms.View.List;
            this.listViewPicCommandsBatches.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listViewPicCommandsBatches_DoubleClick);
            // 
            // BatchName
            // 
            this.BatchName.Tag = "";
            this.BatchName.Width = 200;
            // 
            // Semtech
            // 
            this.Semtech.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("Semtech.BackgroundImage")));
            this.Semtech.Controls.Add(this.groupBoxSemtechCommandsBatches);
            this.Semtech.Controls.Add(this.textBoxSemtech);
            this.Semtech.Controls.Add(this.GroupBoxSemtechComPortSettings);
            this.Semtech.Controls.Add(this.listBoxSemtechRadioCommands);
            this.Semtech.Location = new System.Drawing.Point(4, 25);
            this.Semtech.Name = "Semtech";
            this.Semtech.Padding = new System.Windows.Forms.Padding(3);
            this.Semtech.Size = new System.Drawing.Size(1100, 357);
            this.Semtech.TabIndex = 1;
            this.Semtech.Text = "Semtech";
            this.Semtech.UseVisualStyleBackColor = true;
            // 
            // groupBoxSemtechCommandsBatches
            // 
            this.groupBoxSemtechCommandsBatches.BackColor = System.Drawing.Color.LightGray;
            this.groupBoxSemtechCommandsBatches.Controls.Add(this.buttonSemtechCBEdit);
            this.groupBoxSemtechCommandsBatches.Controls.Add(this.buttonSemtechCBRemove);
            this.groupBoxSemtechCommandsBatches.Controls.Add(this.buttonSemtechCBAdd);
            this.groupBoxSemtechCommandsBatches.Controls.Add(this.listViewSemtechCommandsBatches);
            this.groupBoxSemtechCommandsBatches.Location = new System.Drawing.Point(585, 15);
            this.groupBoxSemtechCommandsBatches.Name = "groupBoxSemtechCommandsBatches";
            this.groupBoxSemtechCommandsBatches.Size = new System.Drawing.Size(230, 324);
            this.groupBoxSemtechCommandsBatches.TabIndex = 8;
            this.groupBoxSemtechCommandsBatches.TabStop = false;
            this.groupBoxSemtechCommandsBatches.Text = "Commands Batches";
            // 
            // buttonSemtechCBEdit
            // 
            this.buttonSemtechCBEdit.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonSemtechCBEdit.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonSemtechCBEdit.FlatAppearance.BorderSize = 3;
            this.buttonSemtechCBEdit.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonSemtechCBEdit.Location = new System.Drawing.Point(164, 287);
            this.buttonSemtechCBEdit.Name = "buttonSemtechCBEdit";
            this.buttonSemtechCBEdit.Size = new System.Drawing.Size(56, 26);
            this.buttonSemtechCBEdit.TabIndex = 7;
            this.buttonSemtechCBEdit.Text = "Edit";
            this.buttonSemtechCBEdit.UseVisualStyleBackColor = false;
            this.buttonSemtechCBEdit.Click += new System.EventHandler(this.buttonSemtechCBEdit_Click);
            // 
            // buttonSemtechCBRemove
            // 
            this.buttonSemtechCBRemove.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonSemtechCBRemove.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonSemtechCBRemove.FlatAppearance.BorderSize = 3;
            this.buttonSemtechCBRemove.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonSemtechCBRemove.Location = new System.Drawing.Point(74, 287);
            this.buttonSemtechCBRemove.Name = "buttonSemtechCBRemove";
            this.buttonSemtechCBRemove.Size = new System.Drawing.Size(84, 26);
            this.buttonSemtechCBRemove.TabIndex = 7;
            this.buttonSemtechCBRemove.Text = "Remove";
            this.buttonSemtechCBRemove.UseVisualStyleBackColor = false;
            this.buttonSemtechCBRemove.Click += new System.EventHandler(this.buttonSemtechCBRemove_Click);
            // 
            // buttonSemtechCBAdd
            // 
            this.buttonSemtechCBAdd.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonSemtechCBAdd.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonSemtechCBAdd.FlatAppearance.BorderSize = 3;
            this.buttonSemtechCBAdd.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonSemtechCBAdd.Location = new System.Drawing.Point(10, 287);
            this.buttonSemtechCBAdd.Name = "buttonSemtechCBAdd";
            this.buttonSemtechCBAdd.Size = new System.Drawing.Size(58, 26);
            this.buttonSemtechCBAdd.TabIndex = 6;
            this.buttonSemtechCBAdd.Text = "Add";
            this.buttonSemtechCBAdd.UseVisualStyleBackColor = false;
            this.buttonSemtechCBAdd.Click += new System.EventHandler(this.buttonSemtechCBAdd_Click);
            // 
            // listViewSemtechCommandsBatches
            // 
            this.listViewSemtechCommandsBatches.BackColor = System.Drawing.SystemColors.Info;
            this.listViewSemtechCommandsBatches.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.listViewSemtechCommandsBatches.GridLines = true;
            this.listViewSemtechCommandsBatches.HideSelection = false;
            this.listViewSemtechCommandsBatches.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.listViewSemtechCommandsBatches.LabelWrap = false;
            this.listViewSemtechCommandsBatches.Location = new System.Drawing.Point(11, 22);
            this.listViewSemtechCommandsBatches.MultiSelect = false;
            this.listViewSemtechCommandsBatches.Name = "listViewSemtechCommandsBatches";
            this.listViewSemtechCommandsBatches.Size = new System.Drawing.Size(210, 251);
            this.listViewSemtechCommandsBatches.TabIndex = 5;
            this.listViewSemtechCommandsBatches.UseCompatibleStateImageBehavior = false;
            this.listViewSemtechCommandsBatches.View = System.Windows.Forms.View.List;
            this.listViewSemtechCommandsBatches.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listViewSemtechCommandsBatches_DoubleClick);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Tag = "";
            this.columnHeader1.Width = 200;
            // 
            // textBoxSemtech
            // 
            this.textBoxSemtech.BackColor = System.Drawing.SystemColors.InactiveCaptionText;
            this.textBoxSemtech.Cursor = System.Windows.Forms.Cursors.Default;
            this.textBoxSemtech.ForeColor = System.Drawing.Color.White;
            this.textBoxSemtech.Location = new System.Drawing.Point(20, 15);
            this.textBoxSemtech.Multiline = true;
            this.textBoxSemtech.Name = "textBoxSemtech";
            this.textBoxSemtech.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxSemtech.Size = new System.Drawing.Size(291, 324);
            this.textBoxSemtech.TabIndex = 6;
            this.textBoxSemtech.WordWrap = false;
            // 
            // GroupBoxSemtechComPortSettings
            // 
            this.GroupBoxSemtechComPortSettings.BackColor = System.Drawing.Color.LightGray;
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.buttonApplyCom2Settings);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.comboBoxPort2FlowControl);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.comboBoxPort2StopBits);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.comboBoxPort2BaudRate);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.comboBoxPort2Parity);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.comboBoxPort2DataBits);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.comboBoxPort2Number);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.labelComPort2Status);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.label2);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.label3);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.label4);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.label5);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.label6);
            this.GroupBoxSemtechComPortSettings.Controls.Add(this.label7);
            this.GroupBoxSemtechComPortSettings.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.GroupBoxSemtechComPortSettings.Location = new System.Drawing.Point(830, 15);
            this.GroupBoxSemtechComPortSettings.Name = "GroupBoxSemtechComPortSettings";
            this.GroupBoxSemtechComPortSettings.Size = new System.Drawing.Size(260, 324);
            this.GroupBoxSemtechComPortSettings.TabIndex = 7;
            this.GroupBoxSemtechComPortSettings.TabStop = false;
            this.GroupBoxSemtechComPortSettings.Text = "Com Port Settings";
            // 
            // buttonApplyCom2Settings
            // 
            this.buttonApplyCom2Settings.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonApplyCom2Settings.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonApplyCom2Settings.FlatAppearance.BorderSize = 3;
            this.buttonApplyCom2Settings.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonApplyCom2Settings.Location = new System.Drawing.Point(179, 282);
            this.buttonApplyCom2Settings.Name = "buttonApplyCom2Settings";
            this.buttonApplyCom2Settings.Size = new System.Drawing.Size(70, 26);
            this.buttonApplyCom2Settings.TabIndex = 5;
            this.buttonApplyCom2Settings.Text = "Apply";
            this.buttonApplyCom2Settings.UseVisualStyleBackColor = false;
            this.buttonApplyCom2Settings.Click += new System.EventHandler(this.buttonApplyCom2Settings_Click);
            // 
            // comboBoxPort2FlowControl
            // 
            this.comboBoxPort2FlowControl.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort2FlowControl.FormattingEnabled = true;
            this.comboBoxPort2FlowControl.Location = new System.Drawing.Point(128, 232);
            this.comboBoxPort2FlowControl.Name = "comboBoxPort2FlowControl";
            this.comboBoxPort2FlowControl.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort2FlowControl.TabIndex = 4;
            // 
            // comboBoxPort2StopBits
            // 
            this.comboBoxPort2StopBits.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort2StopBits.FormattingEnabled = true;
            this.comboBoxPort2StopBits.Location = new System.Drawing.Point(128, 154);
            this.comboBoxPort2StopBits.Name = "comboBoxPort2StopBits";
            this.comboBoxPort2StopBits.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort2StopBits.TabIndex = 4;
            // 
            // comboBoxPort2BaudRate
            // 
            this.comboBoxPort2BaudRate.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort2BaudRate.FormattingEnabled = true;
            this.comboBoxPort2BaudRate.Location = new System.Drawing.Point(128, 77);
            this.comboBoxPort2BaudRate.Name = "comboBoxPort2BaudRate";
            this.comboBoxPort2BaudRate.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort2BaudRate.TabIndex = 4;
            // 
            // comboBoxPort2Parity
            // 
            this.comboBoxPort2Parity.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort2Parity.FormattingEnabled = true;
            this.comboBoxPort2Parity.Location = new System.Drawing.Point(128, 193);
            this.comboBoxPort2Parity.Name = "comboBoxPort2Parity";
            this.comboBoxPort2Parity.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort2Parity.TabIndex = 4;
            // 
            // comboBoxPort2DataBits
            // 
            this.comboBoxPort2DataBits.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort2DataBits.FormattingEnabled = true;
            this.comboBoxPort2DataBits.Location = new System.Drawing.Point(128, 115);
            this.comboBoxPort2DataBits.Name = "comboBoxPort2DataBits";
            this.comboBoxPort2DataBits.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort2DataBits.TabIndex = 4;
            // 
            // comboBoxPort2Number
            // 
            this.comboBoxPort2Number.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort2Number.FormattingEnabled = true;
            this.comboBoxPort2Number.Location = new System.Drawing.Point(128, 40);
            this.comboBoxPort2Number.Name = "comboBoxPort2Number";
            this.comboBoxPort2Number.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort2Number.TabIndex = 4;
            // 
            // labelComPort2Status
            // 
            this.labelComPort2Status.AutoSize = true;
            this.labelComPort2Status.BackColor = System.Drawing.SystemColors.InfoText;
            this.labelComPort2Status.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelComPort2Status.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.labelComPort2Status.Location = new System.Drawing.Point(13, 287);
            this.labelComPort2Status.Name = "labelComPort2Status";
            this.labelComPort2Status.Size = new System.Drawing.Size(127, 19);
            this.labelComPort2Status.TabIndex = 3;
            this.labelComPort2Status.Text = "Port not opened";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.Transparent;
            this.label2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label2.ForeColor = System.Drawing.Color.Black;
            this.label2.Location = new System.Drawing.Point(13, 232);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(99, 19);
            this.label2.TabIndex = 3;
            this.label2.Text = "Flow Control";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.Transparent;
            this.label3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label3.Location = new System.Drawing.Point(37, 154);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(75, 19);
            this.label3.TabIndex = 3;
            this.label3.Text = "Stop Bits";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.BackColor = System.Drawing.Color.Transparent;
            this.label4.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label4.Location = new System.Drawing.Point(26, 77);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(86, 19);
            this.label4.TabIndex = 3;
            this.label4.Text = "Baud Rate";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.BackColor = System.Drawing.Color.Transparent;
            this.label5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label5.Location = new System.Drawing.Point(60, 193);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(52, 19);
            this.label5.TabIndex = 3;
            this.label5.Text = "Parity";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.BackColor = System.Drawing.Color.Transparent;
            this.label6.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label6.Location = new System.Drawing.Point(37, 115);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(75, 19);
            this.label6.TabIndex = 3;
            this.label6.Text = "Data bits";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.BackColor = System.Drawing.Color.Transparent;
            this.label7.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label7.Location = new System.Drawing.Point(58, 40);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(54, 19);
            this.label7.TabIndex = 3;
            this.label7.Text = "Port #";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // listBoxSemtechRadioCommands
            // 
            this.listBoxSemtechRadioCommands.BackColor = System.Drawing.SystemColors.MenuBar;
            this.listBoxSemtechRadioCommands.FormattingEnabled = true;
            this.listBoxSemtechRadioCommands.ItemHeight = 16;
            this.listBoxSemtechRadioCommands.Location = new System.Drawing.Point(325, 15);
            this.listBoxSemtechRadioCommands.Name = "listBoxSemtechRadioCommands";
            this.listBoxSemtechRadioCommands.Size = new System.Drawing.Size(245, 324);
            this.listBoxSemtechRadioCommands.TabIndex = 5;
            this.listBoxSemtechRadioCommands.DoubleClick += new System.EventHandler(this.listBoxSemtechRadioCommands_DoubleClick);
            // 
            // tabPage1
            // 
            this.tabPage1.BackgroundImage = global::WindowsFormsApplication1.Properties.Resources.networkArchictecture;
            this.tabPage1.Controls.Add(this.groupBoxUbiquitiCommandsBatches);
            this.tabPage1.Controls.Add(this.textBoxUbiquiti);
            this.tabPage1.Controls.Add(this.GroupBoxUbiquitiComPortSettings);
            this.tabPage1.Controls.Add(this.listViewUbiquitiRadioCommands);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(1100, 357);
            this.tabPage1.TabIndex = 2;
            this.tabPage1.Text = "Ubiquiti";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // groupBoxUbiquitiCommandsBatches
            // 
            this.groupBoxUbiquitiCommandsBatches.BackColor = System.Drawing.Color.LightGray;
            this.groupBoxUbiquitiCommandsBatches.Controls.Add(this.buttonUbiquitiCBEdit);
            this.groupBoxUbiquitiCommandsBatches.Controls.Add(this.buttonUbiquitiCBRemove);
            this.groupBoxUbiquitiCommandsBatches.Controls.Add(this.buttonUbiquitiCBAdd);
            this.groupBoxUbiquitiCommandsBatches.Controls.Add(this.listViewUbiquitiCommandsBatches);
            this.groupBoxUbiquitiCommandsBatches.Location = new System.Drawing.Point(585, 15);
            this.groupBoxUbiquitiCommandsBatches.Name = "groupBoxUbiquitiCommandsBatches";
            this.groupBoxUbiquitiCommandsBatches.Size = new System.Drawing.Size(230, 324);
            this.groupBoxUbiquitiCommandsBatches.TabIndex = 12;
            this.groupBoxUbiquitiCommandsBatches.TabStop = false;
            this.groupBoxUbiquitiCommandsBatches.Text = "Commands Batches";
            // 
            // buttonUbiquitiCBEdit
            // 
            this.buttonUbiquitiCBEdit.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonUbiquitiCBEdit.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonUbiquitiCBEdit.FlatAppearance.BorderSize = 3;
            this.buttonUbiquitiCBEdit.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonUbiquitiCBEdit.Location = new System.Drawing.Point(164, 287);
            this.buttonUbiquitiCBEdit.Name = "buttonUbiquitiCBEdit";
            this.buttonUbiquitiCBEdit.Size = new System.Drawing.Size(56, 26);
            this.buttonUbiquitiCBEdit.TabIndex = 7;
            this.buttonUbiquitiCBEdit.Text = "Edit";
            this.buttonUbiquitiCBEdit.UseVisualStyleBackColor = false;
            this.buttonUbiquitiCBEdit.Click += new System.EventHandler(this.buttonUbiquitiCBEdit_Click);
            // 
            // buttonUbiquitiCBRemove
            // 
            this.buttonUbiquitiCBRemove.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonUbiquitiCBRemove.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonUbiquitiCBRemove.FlatAppearance.BorderSize = 3;
            this.buttonUbiquitiCBRemove.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonUbiquitiCBRemove.Location = new System.Drawing.Point(74, 287);
            this.buttonUbiquitiCBRemove.Name = "buttonUbiquitiCBRemove";
            this.buttonUbiquitiCBRemove.Size = new System.Drawing.Size(84, 26);
            this.buttonUbiquitiCBRemove.TabIndex = 7;
            this.buttonUbiquitiCBRemove.Text = "Remove";
            this.buttonUbiquitiCBRemove.UseVisualStyleBackColor = false;
            this.buttonUbiquitiCBRemove.Click += new System.EventHandler(this.buttonUbiquitiCBRemove_Click);
            // 
            // buttonUbiquitiCBAdd
            // 
            this.buttonUbiquitiCBAdd.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonUbiquitiCBAdd.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonUbiquitiCBAdd.FlatAppearance.BorderSize = 3;
            this.buttonUbiquitiCBAdd.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonUbiquitiCBAdd.Location = new System.Drawing.Point(10, 287);
            this.buttonUbiquitiCBAdd.Name = "buttonUbiquitiCBAdd";
            this.buttonUbiquitiCBAdd.Size = new System.Drawing.Size(58, 26);
            this.buttonUbiquitiCBAdd.TabIndex = 6;
            this.buttonUbiquitiCBAdd.Text = "Add";
            this.buttonUbiquitiCBAdd.UseVisualStyleBackColor = false;
            this.buttonUbiquitiCBAdd.Click += new System.EventHandler(this.buttonUbiquitiCBAdd_Click);
            // 
            // listViewUbiquitiCommandsBatches
            // 
            this.listViewUbiquitiCommandsBatches.BackColor = System.Drawing.SystemColors.Info;
            this.listViewUbiquitiCommandsBatches.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4});
            this.listViewUbiquitiCommandsBatches.GridLines = true;
            this.listViewUbiquitiCommandsBatches.HideSelection = false;
            this.listViewUbiquitiCommandsBatches.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.listViewUbiquitiCommandsBatches.LabelWrap = false;
            this.listViewUbiquitiCommandsBatches.Location = new System.Drawing.Point(11, 22);
            this.listViewUbiquitiCommandsBatches.MultiSelect = false;
            this.listViewUbiquitiCommandsBatches.Name = "listViewUbiquitiCommandsBatches";
            this.listViewUbiquitiCommandsBatches.Size = new System.Drawing.Size(210, 251);
            this.listViewUbiquitiCommandsBatches.TabIndex = 5;
            this.listViewUbiquitiCommandsBatches.UseCompatibleStateImageBehavior = false;
            this.listViewUbiquitiCommandsBatches.View = System.Windows.Forms.View.List;
            this.listViewUbiquitiCommandsBatches.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listViewUbiquitiCommandsBatches_DoubleClick);
            // 
            // columnHeader3
            // 
            this.columnHeader3.Tag = "";
            this.columnHeader3.Width = 200;
            // 
            // textBoxUbiquiti
            // 
            this.textBoxUbiquiti.BackColor = System.Drawing.SystemColors.InactiveCaptionText;
            this.textBoxUbiquiti.Cursor = System.Windows.Forms.Cursors.Default;
            this.textBoxUbiquiti.ForeColor = System.Drawing.Color.White;
            this.textBoxUbiquiti.Location = new System.Drawing.Point(20, 15);
            this.textBoxUbiquiti.Multiline = true;
            this.textBoxUbiquiti.Name = "textBoxUbiquiti";
            this.textBoxUbiquiti.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxUbiquiti.Size = new System.Drawing.Size(291, 324);
            this.textBoxUbiquiti.TabIndex = 10;
            this.textBoxUbiquiti.WordWrap = false;
            // 
            // GroupBoxUbiquitiComPortSettings
            // 
            this.GroupBoxUbiquitiComPortSettings.BackColor = System.Drawing.Color.LightGray;
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.buttonApplyCom3Settings);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.comboBoxPort3FlowControl);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.comboBoxPort3StopBits);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.comboBoxPort3BaudRate);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.comboBoxPort3Parity);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.comboBoxPort3DataBits);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.comboBoxPort3Number);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.labelComPort3Status);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.label8);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.label9);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.label10);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.label11);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.label12);
            this.GroupBoxUbiquitiComPortSettings.Controls.Add(this.label13);
            this.GroupBoxUbiquitiComPortSettings.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.GroupBoxUbiquitiComPortSettings.Location = new System.Drawing.Point(830, 15);
            this.GroupBoxUbiquitiComPortSettings.Name = "GroupBoxUbiquitiComPortSettings";
            this.GroupBoxUbiquitiComPortSettings.Size = new System.Drawing.Size(260, 324);
            this.GroupBoxUbiquitiComPortSettings.TabIndex = 11;
            this.GroupBoxUbiquitiComPortSettings.TabStop = false;
            this.GroupBoxUbiquitiComPortSettings.Text = "Com Port Settings";
            // 
            // buttonApplyCom3Settings
            // 
            this.buttonApplyCom3Settings.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonApplyCom3Settings.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonApplyCom3Settings.FlatAppearance.BorderSize = 3;
            this.buttonApplyCom3Settings.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonApplyCom3Settings.Location = new System.Drawing.Point(179, 282);
            this.buttonApplyCom3Settings.Name = "buttonApplyCom3Settings";
            this.buttonApplyCom3Settings.Size = new System.Drawing.Size(70, 26);
            this.buttonApplyCom3Settings.TabIndex = 5;
            this.buttonApplyCom3Settings.Text = "Apply";
            this.buttonApplyCom3Settings.UseVisualStyleBackColor = false;
            this.buttonApplyCom3Settings.Click += new System.EventHandler(this.buttonApplyCom3Settings_Click);
            // 
            // comboBoxPort3FlowControl
            // 
            this.comboBoxPort3FlowControl.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort3FlowControl.FormattingEnabled = true;
            this.comboBoxPort3FlowControl.Location = new System.Drawing.Point(128, 232);
            this.comboBoxPort3FlowControl.Name = "comboBoxPort3FlowControl";
            this.comboBoxPort3FlowControl.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort3FlowControl.TabIndex = 4;
            // 
            // comboBoxPort3StopBits
            // 
            this.comboBoxPort3StopBits.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort3StopBits.FormattingEnabled = true;
            this.comboBoxPort3StopBits.Location = new System.Drawing.Point(128, 154);
            this.comboBoxPort3StopBits.Name = "comboBoxPort3StopBits";
            this.comboBoxPort3StopBits.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort3StopBits.TabIndex = 4;
            // 
            // comboBoxPort3BaudRate
            // 
            this.comboBoxPort3BaudRate.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort3BaudRate.FormattingEnabled = true;
            this.comboBoxPort3BaudRate.Location = new System.Drawing.Point(128, 77);
            this.comboBoxPort3BaudRate.Name = "comboBoxPort3BaudRate";
            this.comboBoxPort3BaudRate.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort3BaudRate.TabIndex = 4;
            // 
            // comboBoxPort3Parity
            // 
            this.comboBoxPort3Parity.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort3Parity.FormattingEnabled = true;
            this.comboBoxPort3Parity.Location = new System.Drawing.Point(128, 193);
            this.comboBoxPort3Parity.Name = "comboBoxPort3Parity";
            this.comboBoxPort3Parity.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort3Parity.TabIndex = 4;
            // 
            // comboBoxPort3DataBits
            // 
            this.comboBoxPort3DataBits.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort3DataBits.FormattingEnabled = true;
            this.comboBoxPort3DataBits.Location = new System.Drawing.Point(128, 115);
            this.comboBoxPort3DataBits.Name = "comboBoxPort3DataBits";
            this.comboBoxPort3DataBits.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort3DataBits.TabIndex = 4;
            // 
            // comboBoxPort3Number
            // 
            this.comboBoxPort3Number.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort3Number.FormattingEnabled = true;
            this.comboBoxPort3Number.Location = new System.Drawing.Point(128, 40);
            this.comboBoxPort3Number.Name = "comboBoxPort3Number";
            this.comboBoxPort3Number.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort3Number.TabIndex = 4;
            // 
            // labelComPort3Status
            // 
            this.labelComPort3Status.AutoSize = true;
            this.labelComPort3Status.BackColor = System.Drawing.SystemColors.InfoText;
            this.labelComPort3Status.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelComPort3Status.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.labelComPort3Status.Location = new System.Drawing.Point(13, 287);
            this.labelComPort3Status.Name = "labelComPort3Status";
            this.labelComPort3Status.Size = new System.Drawing.Size(127, 19);
            this.labelComPort3Status.TabIndex = 3;
            this.labelComPort3Status.Text = "Port not opened";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.BackColor = System.Drawing.Color.Transparent;
            this.label8.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label8.ForeColor = System.Drawing.Color.Black;
            this.label8.Location = new System.Drawing.Point(13, 232);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(99, 19);
            this.label8.TabIndex = 3;
            this.label8.Text = "Flow Control";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.BackColor = System.Drawing.Color.Transparent;
            this.label9.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label9.Location = new System.Drawing.Point(37, 154);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(75, 19);
            this.label9.TabIndex = 3;
            this.label9.Text = "Stop Bits";
            this.label9.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.BackColor = System.Drawing.Color.Transparent;
            this.label10.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label10.Location = new System.Drawing.Point(26, 77);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(86, 19);
            this.label10.TabIndex = 3;
            this.label10.Text = "Baud Rate";
            this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.BackColor = System.Drawing.Color.Transparent;
            this.label11.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label11.Location = new System.Drawing.Point(60, 193);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(52, 19);
            this.label11.TabIndex = 3;
            this.label11.Text = "Parity";
            this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.BackColor = System.Drawing.Color.Transparent;
            this.label12.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label12.Location = new System.Drawing.Point(37, 115);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(75, 19);
            this.label12.TabIndex = 3;
            this.label12.Text = "Data bits";
            this.label12.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.BackColor = System.Drawing.Color.Transparent;
            this.label13.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label13.Location = new System.Drawing.Point(58, 40);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(54, 19);
            this.label13.TabIndex = 3;
            this.label13.Text = "Port #";
            this.label13.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // listViewUbiquitiRadioCommands
            // 
            this.listViewUbiquitiRadioCommands.BackColor = System.Drawing.SystemColors.MenuBar;
            this.listViewUbiquitiRadioCommands.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            Command,
            this.CmdCode,
            this.CmdData});
            this.listViewUbiquitiRadioCommands.GridLines = true;
            this.listViewUbiquitiRadioCommands.HideSelection = false;
            this.listViewUbiquitiRadioCommands.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.listViewUbiquitiRadioCommands.LabelWrap = false;
            this.listViewUbiquitiRadioCommands.Location = new System.Drawing.Point(325, 15);
            this.listViewUbiquitiRadioCommands.MultiSelect = false;
            this.listViewUbiquitiRadioCommands.Name = "listViewUbiquitiRadioCommands";
            this.listViewUbiquitiRadioCommands.Size = new System.Drawing.Size(245, 324);
            this.listViewUbiquitiRadioCommands.TabIndex = 5;
            this.listViewUbiquitiRadioCommands.UseCompatibleStateImageBehavior = false;
            this.listViewUbiquitiRadioCommands.View = System.Windows.Forms.View.Details;
            this.listViewUbiquitiRadioCommands.MouseClick += new System.Windows.Forms.MouseEventHandler(this.listViewUbiquitiRadioCommands_Click);
            this.listViewUbiquitiRadioCommands.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listViewUbiquitiRadioCommands_DoubleClick);
            // 
            // CmdCode
            // 
            this.CmdCode.Text = "Cmd";
            // 
            // CmdData
            // 
            this.CmdData.Text = "CmdData";
            this.CmdData.Width = 120;
            // 
            // tabPage2
            // 
            this.tabPage2.BackgroundImage = global::WindowsFormsApplication1.Properties.Resources.networkArchictecture;
            this.tabPage2.Controls.Add(this.groupBox1);
            this.tabPage2.Controls.Add(this.textBoxSTHacked);
            this.tabPage2.Controls.Add(this.GroupBoxSTHackedComPortSettings);
            this.tabPage2.Controls.Add(this.listViewSTHackedRadioCommands);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(1100, 357);
            this.tabPage2.TabIndex = 3;
            this.tabPage2.Text = "semtech hacked";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.Color.LightGray;
            this.groupBox1.Controls.Add(this.buttonSTHackedCBEdit);
            this.groupBox1.Controls.Add(this.buttonSTHackedCBRemove);
            this.groupBox1.Controls.Add(this.buttonSTHackedCBAdd);
            this.groupBox1.Controls.Add(this.listViewSTHackedCommandsBatches);
            this.groupBox1.Location = new System.Drawing.Point(585, 15);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(230, 324);
            this.groupBox1.TabIndex = 16;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Commands Batches";
            // 
            // buttonSTHackedCBEdit
            // 
            this.buttonSTHackedCBEdit.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonSTHackedCBEdit.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonSTHackedCBEdit.FlatAppearance.BorderSize = 3;
            this.buttonSTHackedCBEdit.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonSTHackedCBEdit.Location = new System.Drawing.Point(164, 287);
            this.buttonSTHackedCBEdit.Name = "buttonSTHackedCBEdit";
            this.buttonSTHackedCBEdit.Size = new System.Drawing.Size(56, 26);
            this.buttonSTHackedCBEdit.TabIndex = 7;
            this.buttonSTHackedCBEdit.Text = "Edit";
            this.buttonSTHackedCBEdit.UseVisualStyleBackColor = false;
            this.buttonSTHackedCBEdit.Click += new System.EventHandler(this.buttonSTHackedCBEdit_Click);
            // 
            // buttonSTHackedCBRemove
            // 
            this.buttonSTHackedCBRemove.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonSTHackedCBRemove.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonSTHackedCBRemove.FlatAppearance.BorderSize = 3;
            this.buttonSTHackedCBRemove.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonSTHackedCBRemove.Location = new System.Drawing.Point(73, 287);
            this.buttonSTHackedCBRemove.Name = "buttonSTHackedCBRemove";
            this.buttonSTHackedCBRemove.Size = new System.Drawing.Size(84, 26);
            this.buttonSTHackedCBRemove.TabIndex = 7;
            this.buttonSTHackedCBRemove.Text = "Remove";
            this.buttonSTHackedCBRemove.UseVisualStyleBackColor = false;
            this.buttonSTHackedCBRemove.Click += new System.EventHandler(this.buttonSTHackedCBRemove_Click);
            // 
            // buttonSTHackedCBAdd
            // 
            this.buttonSTHackedCBAdd.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonSTHackedCBAdd.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonSTHackedCBAdd.FlatAppearance.BorderSize = 3;
            this.buttonSTHackedCBAdd.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonSTHackedCBAdd.Location = new System.Drawing.Point(10, 287);
            this.buttonSTHackedCBAdd.Name = "buttonSTHackedCBAdd";
            this.buttonSTHackedCBAdd.Size = new System.Drawing.Size(58, 26);
            this.buttonSTHackedCBAdd.TabIndex = 6;
            this.buttonSTHackedCBAdd.Text = "Add";
            this.buttonSTHackedCBAdd.UseVisualStyleBackColor = false;
            this.buttonSTHackedCBAdd.Click += new System.EventHandler(this.buttonSTHackedCBAdd_Click);
            // 
            // listViewSTHackedCommandsBatches
            // 
            this.listViewSTHackedCommandsBatches.BackColor = System.Drawing.SystemColors.Info;
            this.listViewSTHackedCommandsBatches.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader5,
            this.columnHeader6});
            this.listViewSTHackedCommandsBatches.GridLines = true;
            this.listViewSTHackedCommandsBatches.HideSelection = false;
            this.listViewSTHackedCommandsBatches.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.listViewSTHackedCommandsBatches.LabelWrap = false;
            this.listViewSTHackedCommandsBatches.Location = new System.Drawing.Point(11, 22);
            this.listViewSTHackedCommandsBatches.MultiSelect = false;
            this.listViewSTHackedCommandsBatches.Name = "listViewSTHackedCommandsBatches";
            this.listViewSTHackedCommandsBatches.Size = new System.Drawing.Size(210, 251);
            this.listViewSTHackedCommandsBatches.TabIndex = 5;
            this.listViewSTHackedCommandsBatches.UseCompatibleStateImageBehavior = false;
            this.listViewSTHackedCommandsBatches.View = System.Windows.Forms.View.List;
            this.listViewSTHackedCommandsBatches.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listViewSTHackedCommandsBatches_DoubleClick);
            // 
            // columnHeader5
            // 
            this.columnHeader5.Tag = "";
            this.columnHeader5.Width = 200;
            // 
            // textBoxSTHacked
            // 
            this.textBoxSTHacked.BackColor = System.Drawing.SystemColors.InactiveCaptionText;
            this.textBoxSTHacked.Cursor = System.Windows.Forms.Cursors.Default;
            this.textBoxSTHacked.ForeColor = System.Drawing.Color.White;
            this.textBoxSTHacked.Location = new System.Drawing.Point(20, 15);
            this.textBoxSTHacked.Multiline = true;
            this.textBoxSTHacked.Name = "textBoxSTHacked";
            this.textBoxSTHacked.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxSTHacked.Size = new System.Drawing.Size(291, 324);
            this.textBoxSTHacked.TabIndex = 14;
            this.textBoxSTHacked.WordWrap = false;
            // 
            // GroupBoxSTHackedComPortSettings
            // 
            this.GroupBoxSTHackedComPortSettings.BackColor = System.Drawing.Color.LightGray;
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.buttonApplyCom4Settings);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.comboBoxPort4FlowControl);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.comboBoxPort4StopBits);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.comboBoxPort4BaudRate);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.comboBoxPort4Parity);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.comboBoxPort4DataBits);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.comboBoxPort4Number);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.labelComPort4Status);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.label14);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.label15);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.label16);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.label17);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.label18);
            this.GroupBoxSTHackedComPortSettings.Controls.Add(this.label19);
            this.GroupBoxSTHackedComPortSettings.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.GroupBoxSTHackedComPortSettings.Location = new System.Drawing.Point(830, 15);
            this.GroupBoxSTHackedComPortSettings.Name = "GroupBoxSTHackedComPortSettings";
            this.GroupBoxSTHackedComPortSettings.Size = new System.Drawing.Size(260, 324);
            this.GroupBoxSTHackedComPortSettings.TabIndex = 15;
            this.GroupBoxSTHackedComPortSettings.TabStop = false;
            this.GroupBoxSTHackedComPortSettings.Text = "Com Port Settings";
            // 
            // buttonApplyCom4Settings
            // 
            this.buttonApplyCom4Settings.BackColor = System.Drawing.SystemColors.ControlLight;
            this.buttonApplyCom4Settings.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.buttonApplyCom4Settings.FlatAppearance.BorderSize = 3;
            this.buttonApplyCom4Settings.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.buttonApplyCom4Settings.Location = new System.Drawing.Point(179, 282);
            this.buttonApplyCom4Settings.Name = "buttonApplyCom4Settings";
            this.buttonApplyCom4Settings.Size = new System.Drawing.Size(70, 26);
            this.buttonApplyCom4Settings.TabIndex = 5;
            this.buttonApplyCom4Settings.Text = "Apply";
            this.buttonApplyCom4Settings.UseVisualStyleBackColor = false;
            this.buttonApplyCom4Settings.Click += new System.EventHandler(this.buttonApplyCom4Settings_Click);
            // 
            // comboBoxPort4FlowControl
            // 
            this.comboBoxPort4FlowControl.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort4FlowControl.FormattingEnabled = true;
            this.comboBoxPort4FlowControl.Location = new System.Drawing.Point(128, 232);
            this.comboBoxPort4FlowControl.Name = "comboBoxPort4FlowControl";
            this.comboBoxPort4FlowControl.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort4FlowControl.TabIndex = 4;
            // 
            // comboBoxPort4StopBits
            // 
            this.comboBoxPort4StopBits.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort4StopBits.FormattingEnabled = true;
            this.comboBoxPort4StopBits.Location = new System.Drawing.Point(128, 154);
            this.comboBoxPort4StopBits.Name = "comboBoxPort4StopBits";
            this.comboBoxPort4StopBits.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort4StopBits.TabIndex = 4;
            // 
            // comboBoxPort4BaudRate
            // 
            this.comboBoxPort4BaudRate.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort4BaudRate.FormattingEnabled = true;
            this.comboBoxPort4BaudRate.Location = new System.Drawing.Point(128, 77);
            this.comboBoxPort4BaudRate.Name = "comboBoxPort4BaudRate";
            this.comboBoxPort4BaudRate.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort4BaudRate.TabIndex = 4;
            // 
            // comboBoxPort4Parity
            // 
            this.comboBoxPort4Parity.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort4Parity.FormattingEnabled = true;
            this.comboBoxPort4Parity.Location = new System.Drawing.Point(128, 193);
            this.comboBoxPort4Parity.Name = "comboBoxPort4Parity";
            this.comboBoxPort4Parity.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort4Parity.TabIndex = 4;
            // 
            // comboBoxPort4DataBits
            // 
            this.comboBoxPort4DataBits.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort4DataBits.FormattingEnabled = true;
            this.comboBoxPort4DataBits.Location = new System.Drawing.Point(128, 115);
            this.comboBoxPort4DataBits.Name = "comboBoxPort4DataBits";
            this.comboBoxPort4DataBits.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort4DataBits.TabIndex = 4;
            // 
            // comboBoxPort4Number
            // 
            this.comboBoxPort4Number.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.comboBoxPort4Number.FormattingEnabled = true;
            this.comboBoxPort4Number.Location = new System.Drawing.Point(128, 40);
            this.comboBoxPort4Number.Name = "comboBoxPort4Number";
            this.comboBoxPort4Number.Size = new System.Drawing.Size(121, 24);
            this.comboBoxPort4Number.TabIndex = 4;
            // 
            // labelComPort4Status
            // 
            this.labelComPort4Status.AutoSize = true;
            this.labelComPort4Status.BackColor = System.Drawing.SystemColors.InfoText;
            this.labelComPort4Status.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.labelComPort4Status.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.labelComPort4Status.Location = new System.Drawing.Point(13, 287);
            this.labelComPort4Status.Name = "labelComPort4Status";
            this.labelComPort4Status.Size = new System.Drawing.Size(127, 19);
            this.labelComPort4Status.TabIndex = 3;
            this.labelComPort4Status.Text = "Port not opened";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.BackColor = System.Drawing.Color.Transparent;
            this.label14.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label14.ForeColor = System.Drawing.Color.Black;
            this.label14.Location = new System.Drawing.Point(13, 232);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(99, 19);
            this.label14.TabIndex = 3;
            this.label14.Text = "Flow Control";
            this.label14.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.BackColor = System.Drawing.Color.Transparent;
            this.label15.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label15.Location = new System.Drawing.Point(37, 154);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(75, 19);
            this.label15.TabIndex = 3;
            this.label15.Text = "Stop Bits";
            this.label15.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.BackColor = System.Drawing.Color.Transparent;
            this.label16.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label16.Location = new System.Drawing.Point(26, 77);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(86, 19);
            this.label16.TabIndex = 3;
            this.label16.Text = "Baud Rate";
            this.label16.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.BackColor = System.Drawing.Color.Transparent;
            this.label17.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label17.Location = new System.Drawing.Point(60, 193);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(52, 19);
            this.label17.TabIndex = 3;
            this.label17.Text = "Parity";
            this.label17.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.BackColor = System.Drawing.Color.Transparent;
            this.label18.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label18.Location = new System.Drawing.Point(37, 115);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(75, 19);
            this.label18.TabIndex = 3;
            this.label18.Text = "Data bits";
            this.label18.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.BackColor = System.Drawing.Color.Transparent;
            this.label19.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label19.Location = new System.Drawing.Point(58, 40);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(54, 19);
            this.label19.TabIndex = 3;
            this.label19.Text = "Port #";
            this.label19.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // listViewSTHackedRadioCommands
            // 
            this.listViewSTHackedRadioCommands.BackColor = System.Drawing.SystemColors.MenuBar;
            this.listViewSTHackedRadioCommands.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            columnHeader7,
            this.columnHeader8,
            this.columnHeader9});
            this.listViewSTHackedRadioCommands.GridLines = true;
            this.listViewSTHackedRadioCommands.HideSelection = false;
            this.listViewSTHackedRadioCommands.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.listViewSTHackedRadioCommands.LabelWrap = false;
            this.listViewSTHackedRadioCommands.Location = new System.Drawing.Point(325, 15);
            this.listViewSTHackedRadioCommands.MultiSelect = false;
            this.listViewSTHackedRadioCommands.Name = "listViewSTHackedRadioCommands";
            this.listViewSTHackedRadioCommands.Size = new System.Drawing.Size(245, 324);
            this.listViewSTHackedRadioCommands.TabIndex = 13;
            this.listViewSTHackedRadioCommands.UseCompatibleStateImageBehavior = false;
            this.listViewSTHackedRadioCommands.View = System.Windows.Forms.View.Details;
            this.listViewSTHackedRadioCommands.MouseClick += new System.Windows.Forms.MouseEventHandler(this.listViewSTHackedRadioCommands_Click);
            this.listViewSTHackedRadioCommands.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listViewSTHackedRadioCommands_DoubleClick);
            // 
            // columnHeader8
            // 
            this.columnHeader8.Text = "Cmd";
            // 
            // columnHeader9
            // 
            this.columnHeader9.Text = "CmdData";
            this.columnHeader9.Width = 120;
            // 
            // textBoxGeneralData
            // 
            this.textBoxGeneralData.BackColor = System.Drawing.SystemColors.InactiveCaptionText;
            this.textBoxGeneralData.Cursor = System.Windows.Forms.Cursors.Default;
            this.textBoxGeneralData.ForeColor = System.Drawing.Color.White;
            this.textBoxGeneralData.Location = new System.Drawing.Point(29, 397);
            this.textBoxGeneralData.Multiline = true;
            this.textBoxGeneralData.Name = "textBoxGeneralData";
            this.textBoxGeneralData.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxGeneralData.Size = new System.Drawing.Size(762, 166);
            this.textBoxGeneralData.TabIndex = 7;
            this.textBoxGeneralData.WordWrap = false;
            this.textBoxGeneralData.TextChanged += new System.EventHandler(this.textBoxGeneralData_TextChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlLight;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(1110, 571);
            this.ControlBox = false;
            this.Controls.Add(this.textBoxGeneralData);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.buttonExitApplication);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "LoRas control application";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.GroupBoxPicComPortSettings.ResumeLayout(false);
            this.GroupBoxPicComPortSettings.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.PICtail.ResumeLayout(false);
            this.PICtail.PerformLayout();
            this.groupBoxPicCommandsBatches.ResumeLayout(false);
            this.Semtech.ResumeLayout(false);
            this.Semtech.PerformLayout();
            this.groupBoxSemtechCommandsBatches.ResumeLayout(false);
            this.GroupBoxSemtechComPortSettings.ResumeLayout(false);
            this.GroupBoxSemtechComPortSettings.PerformLayout();
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.groupBoxUbiquitiCommandsBatches.ResumeLayout(false);
            this.GroupBoxUbiquitiComPortSettings.ResumeLayout(false);
            this.GroupBoxUbiquitiComPortSettings.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.GroupBoxSTHackedComPortSettings.ResumeLayout(false);
            this.GroupBoxSTHackedComPortSettings.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonExitApplication;
        private System.Windows.Forms.ListBox listBoxPicRadioCommands;
        private System.Windows.Forms.TextBox textBoxPic;
        private System.Windows.Forms.Label labelPort1Number;
        private System.IO.Ports.SerialPort SerialPortPic;
        private System.Windows.Forms.GroupBox GroupBoxPicComPortSettings;
        private System.Windows.Forms.ComboBox comboBoxPort1Number;
        private System.Windows.Forms.ComboBox comboBoxPort1StopBits;
        private System.Windows.Forms.ComboBox comboBoxPort1BaudRate;
        private System.Windows.Forms.ComboBox comboBoxPort1DataBits;
        private System.Windows.Forms.Label labelPort1StopBits;
        private System.Windows.Forms.Label labelPort1BaudRate;
        private System.Windows.Forms.Label labelPort1DataBits;
        private System.Windows.Forms.ComboBox comboBoxPort1FlowControl;
        private System.Windows.Forms.ComboBox comboBoxPort1Parity;
        private System.Windows.Forms.Label labelPort1FlowControl;
        private System.Windows.Forms.Label labelPort1Parity;
        private System.Windows.Forms.Button buttonApplyCom1Settings;
        private System.Windows.Forms.Label labelComPort1Status;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage PICtail;
        private System.Windows.Forms.TabPage Semtech;
        private System.Windows.Forms.TextBox textBoxSemtech;
        private System.Windows.Forms.GroupBox GroupBoxSemtechComPortSettings;
        private System.Windows.Forms.Button buttonApplyCom2Settings;
        private System.Windows.Forms.ComboBox comboBoxPort2FlowControl;
        private System.Windows.Forms.ComboBox comboBoxPort2StopBits;
        private System.Windows.Forms.ComboBox comboBoxPort2BaudRate;
        private System.Windows.Forms.ComboBox comboBoxPort2Parity;
        private System.Windows.Forms.ComboBox comboBoxPort2DataBits;
        private System.Windows.Forms.ComboBox comboBoxPort2Number;
        private System.Windows.Forms.Label labelComPort2Status;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ListBox listBoxSemtechRadioCommands;
        private System.IO.Ports.SerialPort SerialPortSemtech;
        private System.Windows.Forms.ListView listViewPicCommandsBatches;
        private System.Windows.Forms.ColumnHeader BatchName;
        private System.Windows.Forms.GroupBox groupBoxPicCommandsBatches;
        private System.Windows.Forms.Button buttonPicCBEdit;
        private System.Windows.Forms.Button buttonPicCBRemove;
        private System.Windows.Forms.Button buttonPicCBAdd;
        private System.Windows.Forms.TextBox textBoxGeneralData;
        private System.Windows.Forms.ColumnHeader BatchContent;
        private System.Windows.Forms.GroupBox groupBoxSemtechCommandsBatches;
        private System.Windows.Forms.Button buttonSemtechCBEdit;
        private System.Windows.Forms.Button buttonSemtechCBRemove;
        private System.Windows.Forms.Button buttonSemtechCBAdd;
        private System.Windows.Forms.ListView listViewSemtechCommandsBatches;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.GroupBox groupBoxUbiquitiCommandsBatches;
        private System.Windows.Forms.Button buttonUbiquitiCBEdit;
        private System.Windows.Forms.Button buttonUbiquitiCBRemove;
        private System.Windows.Forms.Button buttonUbiquitiCBAdd;
        private System.Windows.Forms.ListView listViewUbiquitiCommandsBatches;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.TextBox textBoxUbiquiti;
        private System.Windows.Forms.GroupBox GroupBoxUbiquitiComPortSettings;
        private System.Windows.Forms.Button buttonApplyCom3Settings;
        private System.Windows.Forms.ComboBox comboBoxPort3FlowControl;
        private System.Windows.Forms.ComboBox comboBoxPort3StopBits;
        private System.Windows.Forms.ComboBox comboBoxPort3BaudRate;
        private System.Windows.Forms.ComboBox comboBoxPort3Parity;
        private System.Windows.Forms.ComboBox comboBoxPort3DataBits;
        private System.Windows.Forms.ComboBox comboBoxPort3Number;
        private System.Windows.Forms.Label labelComPort3Status;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        public System.IO.Ports.SerialPort SerialPortUbiquiti;
        private System.Windows.Forms.ListView listViewUbiquitiRadioCommands;
        private System.Windows.Forms.ColumnHeader CmdCode;
        public System.Windows.Forms.ColumnHeader CmdData;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button buttonSTHackedCBEdit;
        private System.Windows.Forms.Button buttonSTHackedCBRemove;
        private System.Windows.Forms.Button buttonSTHackedCBAdd;
        private System.Windows.Forms.ListView listViewSTHackedCommandsBatches;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.TextBox textBoxSTHacked;
        private System.Windows.Forms.GroupBox GroupBoxSTHackedComPortSettings;
        private System.Windows.Forms.Button buttonApplyCom4Settings;
        private System.Windows.Forms.ComboBox comboBoxPort4FlowControl;
        private System.Windows.Forms.ComboBox comboBoxPort4StopBits;
        private System.Windows.Forms.ComboBox comboBoxPort4BaudRate;
        private System.Windows.Forms.ComboBox comboBoxPort4Parity;
        private System.Windows.Forms.ComboBox comboBoxPort4DataBits;
        private System.Windows.Forms.ComboBox comboBoxPort4Number;
        private System.Windows.Forms.Label labelComPort4Status;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.ListView listViewSTHackedRadioCommands;
        private System.Windows.Forms.ColumnHeader columnHeader8;
        public System.Windows.Forms.ColumnHeader columnHeader9;
        private System.IO.Ports.SerialPort SerialPortSTHacked;
    }
}

