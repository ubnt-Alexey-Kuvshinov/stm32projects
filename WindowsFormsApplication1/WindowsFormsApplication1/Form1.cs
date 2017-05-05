using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Controls;
using System.Xml;
using System.IO.Ports;
using System.Threading;
using System.Timers;
using System.Text.RegularExpressions;

using System.IO;




namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        private const int WM_DEVICECHANGE = 0x0219;
        private const int DBT_DEVICEARRIVAL = 0x8000;
        private const int DBT_DEVICEREMOVECOMPLETE = 0x8004;

        //working variables
        bool listViewPicCommandsBatchesEditMode = false;
        bool listViewSemtechCommandsBatchesEditMode = false;
        bool listViewUbiquitiCommandsBatchesEditMode = false;
        bool listViewSTHackedCommandsBatchesEditMode = false;
        int listViewPicCommandsBatchesSelectedItem = 0;
        int listViewSemtechCommandsBatchesSelectedItem = 0;
        int listViewUbiquitiCommandsBatchesSelectedItem = 0;
        int listViewSTHackedCommandsBatchesSelectedItem = 0;
        string textBoxPicContent;
        string textBoxSemtechContent;
        string textBoxUbiquitiContent;
        string textBoxSTHackedContent;
        enum XmlDataTarget : byte { PicRadioCommand, SemtechRadioCommand, UbiquitiRadioCommand, STHackedRadioCommand,
                                    PicRadioCommandBatch, SemtechRadioCommandBatch, UbiquitiRadioCommandBatch, STHackedRadioCommandBatch,
                                    Unknown };
        enum XmlAttributeType : byte { Name, Value, Data, Unknown };
        string PicComPortData = "";
        string SemtechComPortData = "";

        System.Timers.Timer timerPicComPort = new System.Timers.Timer(15);
        System.Timers.Timer timerSemtechComPort = new System.Timers.Timer(15);
        System.Timers.Timer timerUbiquitiComPort = new System.Timers.Timer(3000);
        System.Timers.Timer timerSTHackedComPort = new System.Timers.Timer(15);

        bool xmlFileError = false;
        string izvrat;

        byte[] ubiquitiInput = new byte[0];
        public static FileStream FsPicture;

        delegate void itguid(string text);                                                   //inter-thread GUI updte delegates
//        delegate void itguid2(string[] commands, ref string[] codes);
        delegate void itguid3(ref string command);

        public Form1()
        {
            InitializeComponent();
        }

        private void EnumerateComPorts()
        {
            string[] ports = SerialPort.GetPortNames();
            int i = 0;

            comboBoxPort1Number.Items.Clear();
            comboBoxPort2Number.Items.Clear();
            comboBoxPort3Number.Items.Clear();
            comboBoxPort4Number.Items.Clear();

            foreach (string port in ports)
            {
                comboBoxPort1Number.Items.Add(port);
                comboBoxPort2Number.Items.Add(port);
                comboBoxPort3Number.Items.Add(port);
                comboBoxPort4Number.Items.Add(port);
/*                switch (i)
                {
                    case 0:
                        comboBoxPort4Number.Text = port;
                        break;
                    case 1:
                        comboBoxPort3Number.Text = port;
                        break;
                    case 2:
                        comboBoxPort2Number.Text = port;
                        break;
                    case 3:
                        comboBoxPort1Number.Text = port;
                        break;
                    default:
                        break;
                }
*/                i++;
            }
        }


        private void Form1_Load(object sender, EventArgs e)
        {
            // Initialize ComPort1 controls
            GroupBoxPicComPortSettings.BackColor = Color.FromArgb(128, 180, 180, 180);
            GroupBoxSemtechComPortSettings.BackColor = Color.FromArgb(128, 180, 180, 180);
            GroupBoxUbiquitiComPortSettings.BackColor = Color.FromArgb(128, 180, 180, 180);
            GroupBoxSTHackedComPortSettings.BackColor = Color.FromArgb(128, 180, 180, 180);

            EnumerateComPorts();

            comboBoxPort1BaudRate.Items.AddRange(new string[] { "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200" });
            comboBoxPort1BaudRate.Text = "57600";
            comboBoxPort2BaudRate.Items.AddRange(new string[] { "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200" });
            comboBoxPort2BaudRate.Text = SerialPortSemtech.BaudRate.ToString();
            comboBoxPort3BaudRate.Items.AddRange(new string[] { "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200" });
            comboBoxPort3BaudRate.Text = "115200";
            comboBoxPort4BaudRate.Items.AddRange(new string[] { "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200" });
            comboBoxPort4BaudRate.Text = "115200";

            comboBoxPort1DataBits.Items.AddRange(new string[] { "5", "6", "7", "8" });
            comboBoxPort1DataBits.Text = SerialPortPic.DataBits.ToString();
            comboBoxPort2DataBits.Items.AddRange(new string[] { "5", "6", "7", "8" });
            comboBoxPort2DataBits.Text = SerialPortSemtech.DataBits.ToString();
            comboBoxPort3DataBits.Items.AddRange(new string[] { "5", "6", "7", "8" });
            comboBoxPort3DataBits.Text = "8";
            comboBoxPort4DataBits.Items.AddRange(new string[] { "5", "6", "7", "8" });
            comboBoxPort4DataBits.Text = "8";

            foreach (string s in Enum.GetNames(typeof(StopBits)))
            {
                comboBoxPort1StopBits.Items.Add(s);
                comboBoxPort2StopBits.Items.Add(s);
                comboBoxPort3StopBits.Items.Add(s);
                comboBoxPort4StopBits.Items.Add(s);
            }
            comboBoxPort1StopBits.Text = SerialPortPic.StopBits.ToString();
            comboBoxPort2StopBits.Text = SerialPortSemtech.StopBits.ToString();
            comboBoxPort3StopBits.Text = SerialPortUbiquiti.StopBits.ToString();
            comboBoxPort4StopBits.Text = SerialPortSTHacked.StopBits.ToString();

            foreach (string s in Enum.GetNames(typeof(Parity)))
            {
                comboBoxPort1Parity.Items.Add(s);
                comboBoxPort2Parity.Items.Add(s);
                comboBoxPort3Parity.Items.Add(s);
                comboBoxPort4Parity.Items.Add(s);
            }
            comboBoxPort1Parity.Text = SerialPortPic.Parity.ToString();
            comboBoxPort2Parity.Text = SerialPortSemtech.Parity.ToString();
            comboBoxPort3Parity.Text = SerialPortUbiquiti.Parity.ToString();
            comboBoxPort4Parity.Text = SerialPortSTHacked.Parity.ToString();

            foreach (string s in Enum.GetNames(typeof(Handshake)))
            {
                comboBoxPort1FlowControl.Items.Add(s);
                comboBoxPort2FlowControl.Items.Add(s);
                comboBoxPort3FlowControl.Items.Add(s);
                comboBoxPort4FlowControl.Items.Add(s);
            }
            comboBoxPort1FlowControl.Text = SerialPortPic.Handshake.ToString();
            comboBoxPort2FlowControl.Text = SerialPortSemtech.Handshake.ToString();
            comboBoxPort3FlowControl.Text = SerialPortUbiquiti.Handshake.ToString();
            comboBoxPort4FlowControl.Text = SerialPortSTHacked.Handshake.ToString();

            //          buttonApplyCom1Settings_Click(new object(), new EventArgs());       //initialize com ports
            //           buttonApplyCom2Settings_Click(new object(), new EventArgs());
            //           buttonApplyCom3Settings_Click(new object(), new EventArgs());


            //assign components callbacks
            textBoxPic.KeyPress += new KeyPressEventHandler(textBoxkeypressed);
            textBoxSemtech.KeyPress += new KeyPressEventHandler(textBoxkeypressed);
            textBoxUbiquiti.KeyPress += new KeyPressEventHandler(textBoxkeypressed);
            textBoxSTHacked.KeyPress += new KeyPressEventHandler(textBoxkeypressed);
            //SerialPortPic.DataReceived += new SerialDataReceivedEventHandler(PicComPortHandler);
            SerialPortPic.DataReceived += PicComPortHandler;
            SerialPortSemtech.DataReceived += new SerialDataReceivedEventHandler(SemtechComPortHandler);
            SerialPortUbiquiti.DataReceived += UbiquitiComPortHandler;
            SerialPortUbiquiti.ReadTimeout = 100;    // 20;
            SerialPortSTHacked.DataReceived += STHackedComPortHandler;
//SerialPortSTHacked.ReadTimeout = 1500;
            timerPicComPort.Elapsed += PicComPortTimer;
            timerSemtechComPort.Elapsed += SemtechComPortTimer;
            timerUbiquitiComPort.Elapsed += UbiquitiComPortTimer;
            timerSTHackedComPort.Elapsed += STHackedComPortTimer;
            //timerSTHackedComPort.Interval = 20;

            //populate commands and command batches lists
            XmlDataTarget xmlDataTarget = XmlDataTarget.Unknown;
            XmlAttributeType xmlAttributeType = XmlAttributeType.Unknown;
            string attributeName = "";
            string attributeValue = "";

            try
            {
                XmlTextReader reader = new XmlTextReader("configurable data.xml");

                while (reader.Read())
                {
                    switch (reader.NodeType)
                    {
                        case XmlNodeType.Element: // The node is an element.
                            xmlAttributeType = XmlAttributeType.Unknown;

                            switch (reader.Name)
                            {
                                case "RadioCommand":
                                    switch (reader.GetAttribute("type"))
                                    {
                                        case "Pic":
                                            xmlDataTarget = XmlDataTarget.PicRadioCommand;
                                            break;
                                        case "Semtech":
                                            xmlDataTarget = XmlDataTarget.SemtechRadioCommand;
                                            break;
                                        case "Ubiquiti":
                                            xmlDataTarget = XmlDataTarget.UbiquitiRadioCommand;
                                            break;
                                        case "STHacked":
                                            xmlDataTarget = XmlDataTarget.STHackedRadioCommand;
                                            break;
                                        default:
                                            xmlDataTarget = XmlDataTarget.Unknown;
                                            break;
                                    }
                                    break;
                                case "RadioCommandsBatch":
                                    switch (reader.GetAttribute("type"))
                                    {
                                        case "Pic":
                                            xmlDataTarget = XmlDataTarget.PicRadioCommandBatch;
                                            break;
                                        case "Semtech":
                                            xmlDataTarget = XmlDataTarget.SemtechRadioCommandBatch;
                                            break;
                                        case "Ubiquiti":
                                            xmlDataTarget = XmlDataTarget.UbiquitiRadioCommandBatch;
                                            break;
                                        case "STHacked":
                                            xmlDataTarget = XmlDataTarget.STHackedRadioCommandBatch;
                                            break;
                                        default:
                                            xmlDataTarget = XmlDataTarget.Unknown;
                                            break;
                                    }
                                    break;
                                case "name":
                                    xmlAttributeType = XmlAttributeType.Name;
                                    break;
                                case "value":
                                    xmlAttributeType = XmlAttributeType.Value;
                                    break;
                                case "data":
                                    xmlAttributeType = XmlAttributeType.Data;
                                    break;
                                default:
                                    xmlDataTarget = XmlDataTarget.Unknown;
                                    break;
                            }
                            break;
                        case XmlNodeType.Text:              //data of each element
                            switch (xmlAttributeType)
                            {
                                case XmlAttributeType.Name:
                                    attributeName = reader.Value;
                                    break;
                                case XmlAttributeType.Value:
                                    switch (xmlDataTarget)
                                    {
                                        case XmlDataTarget.PicRadioCommand:
                                            listBoxPicRadioCommands.Items.Add(reader.Value);
                                            break;
                                        case XmlDataTarget.SemtechRadioCommand:
                                            listBoxSemtechRadioCommands.Items.Add(reader.Value);
                                            break;
                                        case XmlDataTarget.UbiquitiRadioCommand:
                                            attributeValue = reader.Value;
                                            break;
                                        case XmlDataTarget.STHackedRadioCommand:
                                            attributeValue = reader.Value;
                                            break;
                                        case XmlDataTarget.PicRadioCommandBatch:
                                            System.Windows.Forms.ListViewItem lbi = new System.Windows.Forms.ListViewItem(attributeName);
                                            lbi.SubItems.Add(reader.Value);
                                            listViewPicCommandsBatches.Items.Add(lbi);
                                            break;
                                        case XmlDataTarget.SemtechRadioCommandBatch:
                                            lbi = new System.Windows.Forms.ListViewItem(attributeName);
                                            lbi.SubItems.Add(reader.Value);
                                            listViewSemtechCommandsBatches.Items.Add(lbi);
                                            break;
                                        case XmlDataTarget.UbiquitiRadioCommandBatch:
                                            lbi = new System.Windows.Forms.ListViewItem(attributeName);
                                            lbi.SubItems.Add(reader.Value);
                                            listViewUbiquitiCommandsBatches.Items.Add(lbi);
                                            break;
                                        case XmlDataTarget.STHackedRadioCommandBatch:
                                            lbi = new System.Windows.Forms.ListViewItem(attributeName);
                                            lbi.SubItems.Add(reader.Value);
                                            listViewSTHackedCommandsBatches.Items.Add(lbi);
                                            break;
                                    }
                                    xmlAttributeType = XmlAttributeType.Unknown;
                                    break;
                                case XmlAttributeType.Data:
                                    switch (xmlDataTarget)
                                    {
                                        case XmlDataTarget.UbiquitiRadioCommand:
                                            System.Windows.Forms.ListViewItem lbi = new System.Windows.Forms.ListViewItem(attributeName);
                                            lbi.SubItems.Add(attributeValue);
                                            lbi.SubItems.Add(reader.Value);
                                            listViewUbiquitiRadioCommands.Items.Add(lbi);
                                            break;
                                        case XmlDataTarget.STHackedRadioCommand:
                                            lbi = new System.Windows.Forms.ListViewItem(attributeName);
                                            lbi.SubItems.Add(attributeValue);
                                            lbi.SubItems.Add(reader.Value);
                                            listViewSTHackedRadioCommands.Items.Add(lbi);
                                            break;
                                    }
                                    xmlAttributeType = XmlAttributeType.Unknown;
                                    break;
                            }
                            break;
                        case XmlNodeType.EndElement: //Display the end of the element.
                            xmlAttributeType = XmlAttributeType.Unknown;
                            break;
                    }
                }
                reader.Close();
            }
            catch (Exception ex)
            {
                xmlFileError = true;

                if (ex is System.IO.FileNotFoundException)
                    MessageBoxShow("configurable data file not found, loading default commands");
                else if (ex is System.Xml.XmlException)
                    MessageBoxShow("error parsing xml file, loading default commands");
                else
                    throw;

                // Initialize commands lists
                listBoxPicRadioCommands.Items.AddRange(new string[] { "radio set mod lora", "radio get mod", "radio set freq 915000000",  "radio get freq", "radio set pwr 2", "radio get pwr", "radio set rxbw 125", "radio get rxbw",
                "radio set sf12", "radio get sf", "radio set afcbw 41.7", "radio get afcbw", "radio set fdev 25000", "radio get fdev", "radio set prlen 8", "radio get prlen", "radio set crc on", "radio get crc",
                "radio set iqi off", "radio get iqi", "radio set cr 4/5", "radio get cr", "radio set wdt 0", "radio get wdt", "radio set sync 12", "radio get sync", "radio set bw 125", "radio get bw",
                "mac pause", "radio rx 0", "sys reset"});

                listBoxSemtechRadioCommands.Items.AddRange(new string[] { "ra:mod lora", "ra:mod?", "ra:freq 915", "ra:freq?", "RA:PAS PA_BOOST", "RA:PAS?", "RA:OCP 170", "RA:OCP?",
                "RA:BGR 7", "RA:BGR?", "ra:POW 14", "ra:POW?", "RA:LNAB ON", "RA:LNAB?", "RA:LOR:BW 125", "RA:LOR:BW?", "RA:LOR:SF 12", "RA:LOR:SF?",
                "RA:LOR:PREL 8", "RA:LOR:PREL?", "RA:LOR:CRC ON", "RA:LOR:CRC?", "RA:LOR:INVR OFF", "RA:LOR:INVR?", "RA:LOR:INVT OFF", "RA:LOR:INVT?", "RA:LOR:CR 1", "RA:LOR:CR?",
                });

                //ListBoxItem li = new ListBoxItem();
                //    string ss = "blia";
                //              li.ContextMenu.ToolTip = new ListBoxItem.co.ToolTip(); //.Attributes.Add("title", "tool tip text");
                //          System.Windows.Forms.ToolTip ToolTip1 = new System.Windows.Forms.ToolTip();
                //          ToolTip1.SetToolTip(listBoxPicRadioCommands, "Hello");
                //          listBoxPicRadioCommands.Items.Add(li);
            }
        }    //private void Form1_Load(object sender, EventArgs e)


        private void buttonExitApplication_Click(object sender, EventArgs e)
        {
            if(false == xmlFileError)                               //update settings file only if it was successfully loaded
            { 
                //save editable data
                XmlWriterSettings settings = new XmlWriterSettings();
                settings.Indent = true;

                XmlWriter writer = XmlWriter.Create("configurable data.xml", settings);
                {
                    writer.WriteStartDocument();
                    writer.WriteStartElement("root");

                    writer.WriteStartElement("RadioCommand");
                    writer.WriteAttributeString("type", "Pic");
                    foreach (string s in listBoxPicRadioCommands.Items)
                    {
                        writer.WriteElementString("value", s);
                    }
                    writer.WriteEndElement();

                    writer.WriteStartElement("RadioCommand");
                    writer.WriteAttributeString("type", "Semtech");
                    foreach (string s in listBoxSemtechRadioCommands.Items)
                    {
                        writer.WriteElementString("value", s);
                    }
                    writer.WriteEndElement();

                    foreach (System.Windows.Forms.ListViewItem batch in listViewUbiquitiRadioCommands.Items)
                    {
                        writer.WriteStartElement("RadioCommand");
                        writer.WriteAttributeString("type", "Ubiquiti");
                        writer.WriteElementString("name", batch.SubItems[0].Text);
                        writer.WriteElementString("value", batch.SubItems[1].Text);
                        writer.WriteElementString("data", batch.SubItems[2].Text);
                        writer.WriteEndElement();
                    }

                    foreach (System.Windows.Forms.ListViewItem batch in listViewSTHackedRadioCommands.Items)
                    {
                        writer.WriteStartElement("RadioCommand");
                        writer.WriteAttributeString("type", "STHacked");
                        writer.WriteElementString("name", batch.SubItems[0].Text);
                        writer.WriteElementString("value", batch.SubItems[1].Text);
                        writer.WriteElementString("data", batch.SubItems[2].Text);
                        writer.WriteEndElement();
                    }

                    foreach (System.Windows.Forms.ListViewItem batch in listViewPicCommandsBatches.Items)
                    {
                        writer.WriteStartElement("RadioCommandsBatch");
                        writer.WriteAttributeString("type", "Pic");
                            writer.WriteElementString("name", batch.SubItems[0].Text);
                            writer.WriteElementString("value", batch.SubItems[1].Text);
                        writer.WriteEndElement();
                    }

                    foreach (System.Windows.Forms.ListViewItem batch in listViewSemtechCommandsBatches.Items)
                    {
                        writer.WriteStartElement("RadioCommandsBatch");
                        writer.WriteAttributeString("type", "Semtech");
                        writer.WriteElementString("name", batch.SubItems[0].Text);
                        writer.WriteElementString("value", batch.SubItems[1].Text);
                        writer.WriteEndElement();
                    }

                   foreach (System.Windows.Forms.ListViewItem batch in listViewUbiquitiCommandsBatches.Items)
                   {
                       writer.WriteStartElement("RadioCommandsBatch");
                        writer.WriteAttributeString("type", "Ubiquiti");
                        writer.WriteElementString("name", batch.SubItems[0].Text);
                        writer.WriteElementString("value", batch.SubItems[1].Text);
                        writer.WriteEndElement();
                   }

                    foreach (System.Windows.Forms.ListViewItem batch in listViewSTHackedCommandsBatches.Items)
                    {
                        writer.WriteStartElement("RadioCommandsBatch");
                        writer.WriteAttributeString("type", "STHacked");
                        writer.WriteElementString("name", batch.SubItems[0].Text);
                        writer.WriteElementString("value", batch.SubItems[1].Text);
                        writer.WriteEndElement();
                    }

                    writer.WriteEndElement();
                    writer.WriteEndDocument();
                    writer.Flush();
                    writer.Close();
                }
            }
            Application.Exit();
        }


        private void buttonApplyCom1Settings_Click(object sender, EventArgs e)
        {
            try
            {
                SerialPortPic.Close();
                SerialPortPic.PortName = comboBoxPort1Number.Text;
                SerialPortPic.BaudRate = Int32.Parse(comboBoxPort1BaudRate.Text);
                SerialPortPic.DataBits = int.Parse(comboBoxPort1DataBits.Text);
                SerialPortPic.StopBits = (StopBits)Enum.Parse(typeof(StopBits), comboBoxPort1StopBits.Text);
                SerialPortPic.Parity = (Parity)Enum.Parse(typeof(Parity), comboBoxPort1Parity.Text);
                SerialPortPic.Handshake = (Handshake)Enum.Parse(typeof(Handshake), comboBoxPort1FlowControl.Text);
                SerialPortPic.Open();
                labelComPort1Status.Text = "Opened successfuly";
            }
            catch (UnauthorizedAccessException) { labelComPort1Status.Text = "Port unavailable"; }
            catch (ArgumentOutOfRangeException) { labelComPort1Status.Text = "Invalid parameter/s"; }
            catch (ArgumentException) { labelComPort1Status.Text = "Invalid port name"; }
            catch (System.IO.IOException) { labelComPort1Status.Text = "Invalid state"; }
            catch (InvalidOperationException) { labelComPort1Status.Text = "Already opened"; }
        }

        private void buttonApplyCom2Settings_Click(object sender, EventArgs e)
        {
            try
            {
                SerialPortSemtech.Close();
                SerialPortSemtech.PortName = comboBoxPort2Number.Text;
                SerialPortSemtech.BaudRate = Int32.Parse(comboBoxPort2BaudRate.Text);
                SerialPortSemtech.DataBits = int.Parse(comboBoxPort2DataBits.Text);
                SerialPortSemtech.StopBits = (StopBits)Enum.Parse(typeof(StopBits), comboBoxPort2StopBits.Text);
                SerialPortSemtech.Parity = (Parity)Enum.Parse(typeof(Parity), comboBoxPort2Parity.Text);
                SerialPortSemtech.Handshake = (Handshake)Enum.Parse(typeof(Handshake), comboBoxPort2FlowControl.Text);
                SerialPortSemtech.Open();
                labelComPort2Status.Text = "Opened successfuly";
            }
            catch (UnauthorizedAccessException) { labelComPort2Status.Text = "Port unavailable"; }
            catch (ArgumentOutOfRangeException) { labelComPort2Status.Text = "Invalid parameter/s"; }
            catch (ArgumentException) { labelComPort2Status.Text = "Invalid port name"; }
            catch (System.IO.IOException) { labelComPort2Status.Text = "Invalid state"; }
            catch (InvalidOperationException) { labelComPort2Status.Text = "Already opened"; }
        }

        private void buttonApplyCom3Settings_Click(object sender, EventArgs e)
        {
            try
            {
                SerialPortUbiquiti.Close();
                SerialPortUbiquiti.PortName = comboBoxPort3Number.Text;
                SerialPortUbiquiti.BaudRate = Int32.Parse(comboBoxPort3BaudRate.Text);
                SerialPortUbiquiti.DataBits = int.Parse(comboBoxPort3DataBits.Text);
                SerialPortUbiquiti.StopBits = (StopBits)Enum.Parse(typeof(StopBits), comboBoxPort3StopBits.Text);
                SerialPortUbiquiti.Parity = (Parity)Enum.Parse(typeof(Parity), comboBoxPort3Parity.Text);
                SerialPortUbiquiti.Handshake = (Handshake)Enum.Parse(typeof(Handshake), comboBoxPort3FlowControl.Text);
                SerialPortUbiquiti.Open();
                labelComPort3Status.Text = "Opened successfuly";
            }
            catch (UnauthorizedAccessException) { labelComPort3Status.Text = "Port unavailable"; }
            catch (ArgumentOutOfRangeException) { labelComPort3Status.Text = "Invalid parameter/s"; }
            catch (ArgumentException) { labelComPort3Status.Text = "Invalid port name"; }
            catch (System.IO.IOException) { labelComPort3Status.Text = "Invalid state"; }
            catch (InvalidOperationException) { labelComPort3Status.Text = "Already opened"; }
        }

        private void buttonApplyCom4Settings_Click(object sender, EventArgs e)
        {
            try
            {
                SerialPortSTHacked.Close();
                SerialPortSTHacked.PortName = comboBoxPort4Number.Text;
                SerialPortSTHacked.BaudRate = Int32.Parse(comboBoxPort4BaudRate.Text);
                SerialPortSTHacked.DataBits = int.Parse(comboBoxPort4DataBits.Text);
                SerialPortSTHacked.StopBits = (StopBits)Enum.Parse(typeof(StopBits), comboBoxPort4StopBits.Text);
                SerialPortSTHacked.Parity = (Parity)Enum.Parse(typeof(Parity), comboBoxPort4Parity.Text);
                SerialPortSTHacked.Handshake = (Handshake)Enum.Parse(typeof(Handshake), comboBoxPort4FlowControl.Text);
                SerialPortSTHacked.Open();
                labelComPort4Status.Text = "Opened successfuly";
            }
            catch (UnauthorizedAccessException) { labelComPort4Status.Text = "Port unavailable"; }
            catch (ArgumentOutOfRangeException) { labelComPort4Status.Text = "Invalid parameter/s"; }
            catch (ArgumentException) { labelComPort4Status.Text = "Invalid port name"; }
            catch (System.IO.IOException) { labelComPort4Status.Text = "Invalid state"; }
            catch (InvalidOperationException) { labelComPort4Status.Text = "Already opened"; }
        }

        //------------------------------- Radio Commands processing ---------------------------------------------------

        public void listBoxPicRadioCommands_DoubleClick(object sender, EventArgs e)
        {
            if (listBoxPicRadioCommands.SelectedItem != null)
            {
                if(listViewPicCommandsBatchesEditMode)
                {
                    textBoxPic.AppendText((string)listBoxPicRadioCommands.SelectedItem + "\r\n");
                }
                else if (SendToComPort(ref SerialPortPic, (string)listBoxPicRadioCommands.SelectedItem))
                    textBoxPic.AppendText((string)listBoxPicRadioCommands.SelectedItem + "\r\n");
            }
        }

        private void listBoxSemtechRadioCommands_DoubleClick(object sender, EventArgs e)
        {
            if (listBoxSemtechRadioCommands.SelectedItem != null)
            {
                if(listViewSemtechCommandsBatchesEditMode)
                {
                    textBoxSemtech.AppendText((string)listBoxSemtechRadioCommands.SelectedItem + "\r\n");
                }
                else if (SendToComPort(ref SerialPortSemtech, (string)listBoxSemtechRadioCommands.SelectedItem))
                    textBoxSemtech.AppendText((string)listBoxSemtechRadioCommands.SelectedItem + "\r\n");
            }
        }


        private void listViewUbiquitiRadioCommands_Click(object sender, MouseEventArgs e)
        {
            textBoxUbiquiti.AppendText((string)listViewUbiquitiRadioCommands.SelectedItems[0].SubItems[0].Text);
            if("SET_" == listViewUbiquitiRadioCommands.SelectedItems[0].SubItems[0].Text.Substring(0, 4))
            {
                string command = listViewUbiquitiRadioCommands.SelectedItems[0].SubItems[1].Text;   // + " " + listViewUbiquitiRadioCommands.SelectedItems[0].SubItems[2].Text;
                //textBoxUbiquiti.AppendText(" " + utilities.FilterCommandsToUbiquitiRadio(listViewUbiquitiRadioCommands.SelectedItems[0].SubItems[1].Text
                //                            + " " + listViewUbiquitiRadioCommands.SelectedItems[0].SubItems[2].Text));

                if(utilities.FilterCommandsToUbiquitiRadio(ref command))
                {
                    textBoxUbiquiti.AppendText(" " + command);
                    try
                    {
                        SendToComPort(ref SerialPortUbiquiti, utilities.utilHexToBin(listViewUbiquitiRadioCommands.SelectedItems[0].SubItems[1].Text + command));
                    }
                    catch (ArgumentException ex) { MessageBoxShow(ex.ToString()); }
                }
                else
                    textBoxUbiquiti.AppendText(" " + listViewUbiquitiRadioCommands.SelectedItems[0].SubItems[2].Text);
            }

            textBoxUbiquiti.AppendText("\r\n");
        }

        private void listViewUbiquitiRadioCommands_DoubleClick(object sender, MouseEventArgs e)
        {
            if (listViewUbiquitiRadioCommands.SelectedItems != null)
            {
                if (listViewUbiquitiCommandsBatchesEditMode)
                {
 //                   textBoxUbiquiti.AppendText((string)listViewUbiquitiRadioCommands.SelectedItems[0].SubItems[1].Text + "\r\n");
                }
                else
                {
                    try
                    {
                        SendToComPort(ref SerialPortUbiquiti, utilities.utilHexToBin(listViewUbiquitiRadioCommands.SelectedItems[0].SubItems[1].Text +
                            " " + listViewUbiquitiRadioCommands.SelectedItems[0].SubItems[2].Text));
                    }
                    catch (ArgumentException ex) { MessageBoxShow(ex.ToString()); }
                }
            }
        }


        private void listViewSTHackedRadioCommands_Click(object sender, MouseEventArgs e)
        {
            textBoxSTHacked.AppendText((string)listViewSTHackedRadioCommands.SelectedItems[0].SubItems[0].Text);
            if ("SET_" == listViewSTHackedRadioCommands.SelectedItems[0].SubItems[0].Text.Substring(0, 4))
            {
                string command = listViewSTHackedRadioCommands.SelectedItems[0].SubItems[1].Text;

                if (utilities.FilterCommandsToUbiquitiRadio(ref command))
                {
                    textBoxSTHacked.AppendText(" " + command);
                    try
                    {
                        SendToComPort(ref SerialPortSTHacked, utilities.utilHexToBin(listViewSTHackedRadioCommands.SelectedItems[0].SubItems[1].Text + command));
                    }
                    catch (ArgumentException ex) { MessageBoxShow(ex.ToString()); }
                }
                else
                    textBoxSTHacked.AppendText(" " + listViewSTHackedRadioCommands.SelectedItems[0].SubItems[2].Text);
            }

            textBoxSTHacked.AppendText("\r\n");
        }

        private void listViewSTHackedRadioCommands_DoubleClick(object sender, MouseEventArgs e)
        {
            if (listViewSTHackedRadioCommands.SelectedItems != null)
            {
                if (listViewSTHackedCommandsBatchesEditMode)
                {
                }
                else
                {
                    try
                    {
                        SendToComPort(ref SerialPortSTHacked, utilities.utilHexToBin(listViewSTHackedRadioCommands.SelectedItems[0].SubItems[1].Text +
                            " " + listViewSTHackedRadioCommands.SelectedItems[0].SubItems[2].Text));
                    }
                    catch (ArgumentException ex) { MessageBoxShow(ex.ToString()); }
                }
            }
        }

        //------------------------------- Pic Commands Batches processing -------------------------------------------------------
        private void SendCommandsBatchToPicRadio(string batch)
        {
            string[] commands = Regex.Split(batch, "\r\n");
            string[] command;

            foreach (string s in commands)
            {
                command = Regex.Split(s, @"\s+");
                switch(command[0])
                {
                    case "Delay":
                        Thread.Sleep(int.Parse(command[1]));
                        break;
                    default:
                        SendToComPort(ref SerialPortPic, s);
                        UpdatePicTextBox(s + "\r\n");
                        Thread.Sleep(50);
                        break;
                }
            }
        }

        private void listViewPicCommandsBatches_DoubleClick(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (listViewPicCommandsBatches.SelectedItems != null)
            {

                ThreadWithState tws = new ThreadWithState(listViewPicCommandsBatches.SelectedItems[0].SubItems[1].Text, SendCommandsBatchToPicRadio);
                Thread t = new Thread(new ThreadStart(tws.ThreadProc));
                t.Start();
            }
        }

        private void buttonPicCBAdd_Click(object sender, EventArgs e)
        {
            if ("Add" == buttonPicCBAdd.Text)
            {
                listViewPicCommandsBatches.LabelEdit = true;
                System.Windows.Forms.ListViewItem lbi = new System.Windows.Forms.ListViewItem("new batch");
                lbi.SubItems.Add("");
                listViewPicCommandsBatches.Items.Add(lbi);
                lbi.BeginEdit();

                listViewPicCommandsBatchesEditMode = true;
                textBoxPicContent = textBoxPic.Text;
                listViewPicCommandsBatches.Items[listViewPicCommandsBatches.Items.Count - 1].Selected = true;
                textBoxPic.Text = listViewPicCommandsBatches.SelectedItems[0].SubItems[1].Text;
                listViewPicCommandsBatchesSelectedItem = listViewPicCommandsBatches.SelectedItems[0].Index;

                buttonPicCBAdd.Text = "Done";
                buttonPicCBRemove.Enabled = false;
                buttonPicCBEdit.Enabled = false;
            }
            else
            {
                listViewPicCommandsBatchesEditMode = false;
                listViewPicCommandsBatches.Items[listViewPicCommandsBatchesSelectedItem].Selected = true;
                listViewPicCommandsBatches.SelectedItems[0].SubItems[1].Text = textBoxPic.Text;
                textBoxPic.Text = textBoxPicContent;
                buttonPicCBAdd.Text = "Add";
                listViewPicCommandsBatches.LabelEdit = false;
                buttonPicCBRemove.Enabled = true;
                buttonPicCBEdit.Enabled = true;
            }
        }

        private void buttonPicCBRemove_Click(object sender, EventArgs e)
        {
            foreach (System.Windows.Forms.ListViewItem eachItem in listViewPicCommandsBatches.SelectedItems)
                listViewPicCommandsBatches.Items.Remove(eachItem);
        }

        private void buttonPicCBEdit_Click(object sender, EventArgs e)
        {
            if ("Edit" == buttonPicCBEdit.Text)
            {
                if (listViewPicCommandsBatches.SelectedItems.Count > 0)
                {
                    listViewPicCommandsBatchesSelectedItem = listViewPicCommandsBatches.SelectedItems[0].Index;
                    listViewPicCommandsBatchesEditMode = true;
                    textBoxPicContent = textBoxPic.Text;
                    textBoxPic.Text = listViewPicCommandsBatches.SelectedItems[0].SubItems[1].Text;
                    listViewPicCommandsBatches.LabelEdit = true;
                    //listViewPicCommandsBatches.SelectedItems[0].BeginEdit();
                    buttonPicCBAdd.Enabled = false;
                    buttonPicCBRemove.Enabled = false;
                    buttonPicCBEdit.Text = "Done";
                }
            }
            else
            {
                listViewPicCommandsBatchesEditMode = false;
                listViewPicCommandsBatches.LabelEdit = false;
                listViewPicCommandsBatches.Items[listViewPicCommandsBatchesSelectedItem].Selected = true;
                listViewPicCommandsBatches.SelectedItems[0].SubItems[1].Text = textBoxPic.Text;
                textBoxPic.Text = textBoxPicContent;
                buttonPicCBAdd.Enabled = true;
                buttonPicCBRemove.Enabled = true;
                buttonPicCBEdit.Text = "Edit";
            }
        }

        //------------------------------- Semtech Commands Batches processing -------------------------------------------------------
        private void SendCommandsBatchToSemtechRadio(string batch)
        {
            string[] commands = Regex.Split(batch, "\r\n");
            string[] command;

            foreach (string s in commands)
            {
                command = Regex.Split(s, @"\s+");
                switch (command[0])
                {
                    case "Delay":
                        UpdateSemtechTextBox("Delay" + command[1] + "\r\n");
                        Thread.Sleep(int.Parse(command[1]));
                        break;
                    default:
                        SendToComPort(ref SerialPortSemtech, s);
                        UpdateSemtechTextBox(s + "\r\n");
                        Thread.Sleep(50);
                        break;
                }
            }
        }

        private void listViewSemtechCommandsBatches_DoubleClick(object sender, MouseEventArgs e)
        {
            if (listViewSemtechCommandsBatches.SelectedItems != null)
            {

                ThreadWithState tws = new ThreadWithState(listViewSemtechCommandsBatches.SelectedItems[0].SubItems[1].Text, SendCommandsBatchToSemtechRadio);
                Thread t = new Thread(new ThreadStart(tws.ThreadProc));
                t.Start();
            }
        }

        private void buttonSemtechCBAdd_Click(object sender, EventArgs e)
        {
            if ("Add" == buttonSemtechCBAdd.Text)
            {
                listViewSemtechCommandsBatches.LabelEdit = true;
                System.Windows.Forms.ListViewItem lbi = new System.Windows.Forms.ListViewItem("new batch");
                lbi.SubItems.Add("");
                listViewSemtechCommandsBatches.Items.Add(lbi);
                lbi.BeginEdit();

                listViewSemtechCommandsBatchesEditMode = true;
                textBoxSemtechContent = textBoxSemtech.Text;
                listViewSemtechCommandsBatches.Items[listViewSemtechCommandsBatches.Items.Count - 1].Selected = true;
                textBoxSemtech.Text = listViewSemtechCommandsBatches.SelectedItems[0].SubItems[1].Text;
                listViewSemtechCommandsBatchesSelectedItem = listViewSemtechCommandsBatches.SelectedItems[0].Index;

                buttonSemtechCBAdd.Text = "Done";
                buttonSemtechCBRemove.Enabled = false;
                buttonSemtechCBEdit.Enabled = false;
            }
            else
            {
                listViewSemtechCommandsBatchesEditMode = false;
                listViewSemtechCommandsBatches.Items[listViewSemtechCommandsBatchesSelectedItem].Selected = true;
                listViewSemtechCommandsBatches.SelectedItems[0].SubItems[1].Text = textBoxSemtech.Text;
                textBoxSemtech.Text = textBoxSemtechContent;
                buttonSemtechCBAdd.Text = "Add";
                listViewSemtechCommandsBatches.LabelEdit = false;
                buttonSemtechCBRemove.Enabled = true;
                buttonSemtechCBEdit.Enabled = true;
            }
        }

        private void buttonSemtechCBRemove_Click(object sender, EventArgs e)
        {
            foreach (System.Windows.Forms.ListViewItem eachItem in listViewSemtechCommandsBatches.SelectedItems)
                listViewSemtechCommandsBatches.Items.Remove(eachItem);
        }

        private void buttonSemtechCBEdit_Click(object sender, EventArgs e)
        {
            if ("Edit" == buttonSemtechCBEdit.Text)
            {
                if (listViewSemtechCommandsBatches.SelectedItems.Count > 0)
                {
                    listViewSemtechCommandsBatchesSelectedItem = listViewSemtechCommandsBatches.SelectedItems[0].Index;
                    listViewSemtechCommandsBatchesEditMode = true;
                    textBoxSemtechContent = textBoxSemtech.Text;
                    textBoxSemtech.Text = listViewSemtechCommandsBatches.SelectedItems[0].SubItems[1].Text;
                    listViewSemtechCommandsBatches.LabelEdit = true;
                    buttonSemtechCBAdd.Enabled = false;
                    buttonSemtechCBRemove.Enabled = false;
                    buttonSemtechCBEdit.Text = "Done";
                }
            }
            else
            {
                listViewSemtechCommandsBatchesEditMode = false;
                listViewSemtechCommandsBatches.LabelEdit = false;
                listViewSemtechCommandsBatches.Items[listViewSemtechCommandsBatchesSelectedItem].Selected = true;
                listViewSemtechCommandsBatches.SelectedItems[0].SubItems[1].Text = textBoxSemtech.Text;
                textBoxSemtech.Text = textBoxSemtechContent;
                buttonSemtechCBAdd.Enabled = true;
                buttonSemtechCBRemove.Enabled = true;
                buttonSemtechCBEdit.Text = "Edit";
            }
        }

        //------------------------------- Ubiquiti Commands Batches processing -------------------------------------------------------
        private void GetUbiquitiCommandCode(ref string command)
        {
            if (listViewUbiquitiRadioCommands.InvokeRequired)
            {
                itguid3 d = new itguid3(GetUbiquitiCommandCode);
                this.Invoke(d, command );
            }
            else
            { 
                foreach (System.Windows.Forms.ListViewItem cmd in listViewUbiquitiRadioCommands.Items)
                    if (command == cmd.Text)
                    {
                        izvrat = cmd.SubItems[1].Text;
                        return;
                    }

                throw new System.ArgumentException("could not find command: ", command);
            }
        }

        private void SendCommandsBatchToUbiquitiRadio(string batch)
        {
            string[] commands = Regex.Split(batch, "\r\n");
            string[] command;

            foreach (string s in commands)
            {
                command = Regex.Split(s, " ");
                switch (command[0])
                {
                    case "Delay":
                        UpdateUbiquitiTextBox("Delay" + command[1] + "\r\n");
                        Thread.Sleep(int.Parse(command[1]));
                        break;
                    default:
                        try
                        {
                            GetUbiquitiCommandCode(ref command[0]);
                            command[0] = izvrat;
                        }
                        catch (ArgumentException ex) { MessageBoxShow(ex.ToString()); return; }

                        try
                        {
                            SendToComPort(ref SerialPortUbiquiti, utilities.utilHexToBin(String.Join(" ", command)));
                        }
                        catch (ArgumentException ex) { MessageBoxShow(ex.ToString()); return; }

                        UpdateUbiquitiTextBox(s + "\r\n");
                        Thread.Sleep(50);

                        break;
                }
            }
        }

        private void listViewUbiquitiCommandsBatches_DoubleClick(object sender, MouseEventArgs e)
        {
            if (listViewUbiquitiCommandsBatches.SelectedItems != null)
            {

                ThreadWithState tws = new ThreadWithState(listViewUbiquitiCommandsBatches.SelectedItems[0].SubItems[1].Text, SendCommandsBatchToUbiquitiRadio);
                Thread t = new Thread(new ThreadStart(tws.ThreadProc));
                t.Start();
            }
        }

        private void buttonUbiquitiCBAdd_Click(object sender, EventArgs e)
        {
            if ("Add" == buttonUbiquitiCBAdd.Text)
            {
                listViewUbiquitiCommandsBatches.LabelEdit = true;
                System.Windows.Forms.ListViewItem lbi = new System.Windows.Forms.ListViewItem("new batch");
                lbi.SubItems.Add("");
                listViewUbiquitiCommandsBatches.Items.Add(lbi);
                lbi.BeginEdit();

                listViewUbiquitiCommandsBatchesEditMode = true;
                textBoxUbiquitiContent = textBoxUbiquiti.Text;
                listViewUbiquitiCommandsBatches.Items[listViewUbiquitiCommandsBatches.Items.Count - 1].Selected = true;
                textBoxUbiquiti.Text = listViewUbiquitiCommandsBatches.SelectedItems[0].SubItems[1].Text;
                listViewUbiquitiCommandsBatchesSelectedItem = listViewUbiquitiCommandsBatches.SelectedItems[0].Index;

                buttonUbiquitiCBAdd.Text = "Done";
                buttonUbiquitiCBRemove.Enabled = false;
                buttonUbiquitiCBEdit.Enabled = false;
            }
            else
            {
                listViewUbiquitiCommandsBatchesEditMode = false;
                listViewUbiquitiCommandsBatches.Items[listViewUbiquitiCommandsBatchesSelectedItem].Selected = true;
                listViewUbiquitiCommandsBatches.SelectedItems[0].SubItems[1].Text = textBoxUbiquiti.Text;
                textBoxUbiquiti.Text = textBoxUbiquitiContent;
                buttonUbiquitiCBAdd.Text = "Add";
                listViewUbiquitiCommandsBatches.LabelEdit = false;
                buttonUbiquitiCBRemove.Enabled = true;
                buttonUbiquitiCBEdit.Enabled = true;
            }
        }

        private void buttonUbiquitiCBRemove_Click(object sender, EventArgs e)
        {
            foreach (System.Windows.Forms.ListViewItem eachItem in listViewUbiquitiCommandsBatches.SelectedItems)
                listViewUbiquitiCommandsBatches.Items.Remove(eachItem);
        }

        private void buttonUbiquitiCBEdit_Click(object sender, EventArgs e)
        {
            if ("Edit" == buttonUbiquitiCBEdit.Text)
            {
                if (listViewUbiquitiCommandsBatches.SelectedItems.Count > 0)
                {
                    listViewUbiquitiCommandsBatchesSelectedItem = listViewUbiquitiCommandsBatches.SelectedItems[0].Index;
                    listViewUbiquitiCommandsBatchesEditMode = true;
                    textBoxUbiquitiContent = textBoxUbiquiti.Text;
                    textBoxUbiquiti.Text = listViewUbiquitiCommandsBatches.SelectedItems[0].SubItems[1].Text;
                    listViewUbiquitiCommandsBatches.LabelEdit = true;
                    buttonUbiquitiCBAdd.Enabled = false;
                    buttonUbiquitiCBRemove.Enabled = false;
                    buttonUbiquitiCBEdit.Text = "Done";
                }
            }
            else
            {
                listViewUbiquitiCommandsBatchesEditMode = false;
                listViewUbiquitiCommandsBatches.LabelEdit = false;
                listViewUbiquitiCommandsBatches.Items[listViewUbiquitiCommandsBatchesSelectedItem].Selected = true;
                listViewUbiquitiCommandsBatches.SelectedItems[0].SubItems[1].Text = textBoxUbiquiti.Text;
                textBoxUbiquiti.Text = textBoxUbiquitiContent;
                buttonUbiquitiCBAdd.Enabled = true;
                buttonUbiquitiCBRemove.Enabled = true;
                buttonUbiquitiCBEdit.Text = "Edit";
            }
        }


        //------------------------------- STHacked Commands Batches processing -------------------------------------------------------
        private void GetSTHackedCommandCode(ref string command)
        {
            if (listViewSTHackedRadioCommands.InvokeRequired)
            {
                itguid3 d = new itguid3(GetSTHackedCommandCode);
                this.Invoke(d, command);
            }
            else
            {
                foreach (System.Windows.Forms.ListViewItem cmd in listViewSTHackedRadioCommands.Items)
                    if (command == cmd.Text)
                    {
                        izvrat = cmd.SubItems[1].Text;
                        return;
                    }

                throw new System.ArgumentException("could not find command: ", command);
            }
        }

        private void SendCommandsBatchToSTHackedRadio(string batch)
        {
            string[] commands = Regex.Split(batch, "\r\n");
            string[] command;

            foreach (string s in commands)
            {
                command = Regex.Split(s, " ");
                switch (command[0])
                {
                    case "Delay":
                        UpdateSTHackedTextBox("Delay" + command[1] + "\r\n");
                        Thread.Sleep(int.Parse(command[1]));
                        break;
                    default:
                        try
                        {
                            GetSTHackedCommandCode(ref command[0]);
                            command[0] = izvrat;
                        }
                        catch (ArgumentException ex) { MessageBoxShow(ex.ToString()); return; }

                        try
                        {
                            SendToComPort(ref SerialPortSTHacked, utilities.utilHexToBin(String.Join(" ", command)));
                        }
                        catch (ArgumentException ex) { MessageBoxShow(ex.ToString()); return; }

                        UpdateSTHackedTextBox(s + "\r\n");
                        Thread.Sleep(50);

                        break;
                }
            }
        }

        private void listViewSTHackedCommandsBatches_DoubleClick(object sender, MouseEventArgs e)
        {
            if (listViewSTHackedCommandsBatches.SelectedItems != null)
            {

                ThreadWithState tws = new ThreadWithState(listViewSTHackedCommandsBatches.SelectedItems[0].SubItems[1].Text, SendCommandsBatchToSTHackedRadio);
                Thread t = new Thread(new ThreadStart(tws.ThreadProc));
                t.Start();
            }
        }

        private void buttonSTHackedCBAdd_Click(object sender, EventArgs e)
        {
            if ("Add" == buttonSTHackedCBAdd.Text)
            {
                listViewSTHackedCommandsBatches.LabelEdit = true;
                System.Windows.Forms.ListViewItem lbi = new System.Windows.Forms.ListViewItem("new batch");
                lbi.SubItems.Add("");
                listViewSTHackedCommandsBatches.Items.Add(lbi);
                lbi.BeginEdit();

                listViewSTHackedCommandsBatchesEditMode = true;
                textBoxSTHackedContent = textBoxSTHacked.Text;
                listViewSTHackedCommandsBatches.Items[listViewSTHackedCommandsBatches.Items.Count - 1].Selected = true;
                textBoxSTHacked.Text = listViewSTHackedCommandsBatches.SelectedItems[0].SubItems[1].Text;
                listViewSTHackedCommandsBatchesSelectedItem = listViewSTHackedCommandsBatches.SelectedItems[0].Index;

                buttonSTHackedCBAdd.Text = "Done";
                buttonSTHackedCBRemove.Enabled = false;
                buttonSTHackedCBEdit.Enabled = false;
            }
            else
            {
                listViewSTHackedCommandsBatchesEditMode = false;
                listViewSTHackedCommandsBatches.Items[listViewSTHackedCommandsBatchesSelectedItem].Selected = true;
                listViewSTHackedCommandsBatches.SelectedItems[0].SubItems[1].Text = textBoxSTHacked.Text;
                textBoxSTHacked.Text = textBoxSTHackedContent;
                buttonSTHackedCBAdd.Text = "Add";
                listViewSTHackedCommandsBatches.LabelEdit = false;
                buttonSTHackedCBRemove.Enabled = true;
                buttonSTHackedCBEdit.Enabled = true;
            }
        }

        private void buttonSTHackedCBRemove_Click(object sender, EventArgs e)
        {
            foreach (System.Windows.Forms.ListViewItem eachItem in listViewSTHackedCommandsBatches.SelectedItems)
                listViewSTHackedCommandsBatches.Items.Remove(eachItem);
        }

        private void buttonSTHackedCBEdit_Click(object sender, EventArgs e)
        {
            if ("Edit" == buttonSTHackedCBEdit.Text)
            {
                if (listViewSTHackedCommandsBatches.SelectedItems.Count > 0)
                {
                    listViewSTHackedCommandsBatchesSelectedItem = listViewSTHackedCommandsBatches.SelectedItems[0].Index;
                    listViewSTHackedCommandsBatchesEditMode = true;
                    textBoxSTHackedContent = textBoxSTHacked.Text;
                    textBoxSTHacked.Text = listViewSTHackedCommandsBatches.SelectedItems[0].SubItems[1].Text;
                    listViewSTHackedCommandsBatches.LabelEdit = true;
                    buttonSTHackedCBAdd.Enabled = false;
                    buttonSTHackedCBRemove.Enabled = false;
                    buttonSTHackedCBEdit.Text = "Done";
                }
            }
            else
            {
                listViewSTHackedCommandsBatchesEditMode = false;
                listViewSTHackedCommandsBatches.LabelEdit = false;
                listViewSTHackedCommandsBatches.Items[listViewSTHackedCommandsBatchesSelectedItem].Selected = true;
                listViewSTHackedCommandsBatches.SelectedItems[0].SubItems[1].Text = textBoxSTHacked.Text;
                textBoxSTHacked.Text = textBoxSTHackedContent;
                buttonSTHackedCBAdd.Enabled = true;
                buttonSTHackedCBRemove.Enabled = true;
                buttonSTHackedCBEdit.Text = "Edit";
            }
        }

        //------------------------------- Sending Radio Commands from Text Boxes ---------------------------------------

        //sends command entered in the textbox to COM port unless the textbox is used for commands batch editing
        private void textBoxkeypressed(Object sender, KeyPressEventArgs e)
        {
            if (((char)Keys.Return == e.KeyChar) && (((System.Windows.Forms.TextBox)sender).Lines.Length > 0) 
                && (((System.Windows.Forms.TextBox)sender).Lines[((System.Windows.Forms.TextBox)sender).Lines.Length - 1].Length > 0))
            {
                if ("textBoxPic" == ((System.Windows.Forms.TextBox)sender).Name)
                {
                    if (false == listViewPicCommandsBatchesEditMode)
                        SendToComPort(ref SerialPortPic, textBoxPic.Lines[textBoxPic.Lines.Length - 1]);
                }
                else if ("textBoxSemtech" == ((System.Windows.Forms.TextBox)sender).Name)
                {
                    if (false == listViewSemtechCommandsBatchesEditMode)
                        SendToComPort(ref SerialPortSemtech, textBoxSemtech.Lines[textBoxSemtech.Lines.Length - 1]);
                }
                else if ("textBoxUbiquiti" == ((System.Windows.Forms.TextBox)sender).Name)
                {
                    if (false == listViewUbiquitiCommandsBatchesEditMode)
                    {
                        string[] text = Regex.Split(textBoxUbiquiti.Lines[textBoxUbiquiti.Lines.Length - 1], " ");

                        foreach (System.Windows.Forms.ListViewItem cmd in listViewUbiquitiRadioCommands.Items)
                        {
                            if (text[0] == cmd.SubItems[0].Text)
                            {
                                text[0] = cmd.SubItems[1].Text;

                                if ("GET_" == cmd.SubItems[0].Text.Substring(0, 4))
                                {
                                    Array.Resize(ref text, text.Length + 1);
                                    text[text.Length - 1] = cmd.SubItems[2].Text;
                                }

                                break;
                            }
                        }
                        try
                        {
                            SendToComPort(ref SerialPortUbiquiti, utilities.utilHexToBin(String.Join(" ", text)));
                        }
                        catch (ArgumentException ex) { MessageBoxShow(ex.ToString()); }
                    }
                }
                else if (false == listViewSTHackedCommandsBatchesEditMode)
                {
                    string[] text = Regex.Split(textBoxSTHacked.Lines[textBoxSTHacked.Lines.Length - 1], " ");

                    foreach (System.Windows.Forms.ListViewItem cmd in listViewSTHackedRadioCommands.Items)
                    {
                        if (text[0] == cmd.SubItems[0].Text)
                        {
                            text[0] = cmd.SubItems[1].Text;

                            if ("GET_" == cmd.SubItems[0].Text.Substring(0, 4))
                            {
                                Array.Resize(ref text, text.Length + 1);
                                text[text.Length - 1] = cmd.SubItems[2].Text;
                            }

                            break;
                        }
                    }
                    try
                    {
                        SendToComPort(ref SerialPortSTHacked, utilities.utilHexToBin(String.Join(" ", text)));
                    }
                    catch (ArgumentException ex) { MessageBoxShow(ex.ToString()); }
                }
            }
        }

        //------------------------------- Pic Text Box processing -------------------------------------------------------

        //limits the size of text displayed in the text box
        private void textBoxPic_TextChanged(object sender, EventArgs e)
        {
            const byte MaxLines = 50;

            if (this.textBoxPic.Lines.Count() > MaxLines)
            {
                string[] lines = this.textBoxPic.Lines;
                textBoxPic.Lines = lines.Skip(textBoxPic.Lines.Count() - MaxLines).ToArray();
                // textBoxPic.Top = textBoxPic.Lines.Count() - 1;
                textBoxPic.SelectionStart = textBoxPic.Text.Length;
                textBoxPic.ScrollToCaret();
            }
        }

        //uptages text box from any thread
        private void UpdatePicTextBox(string text)
        {
            if (listViewPicCommandsBatchesEditMode)
                textBoxPicContent += text;
            else if (textBoxPic.InvokeRequired)
            {
                itguid d = new itguid(UpdatePicTextBox);
                this.Invoke(d, text);
            }
            else
                textBoxPic.AppendText(text);
        }

        //------------------------------- Semtech Text Box processing -------------------------------------------------------

        private void textBoxSemtech_TextChanged(object sender, EventArgs e)
        {
            const byte MaxLines = 50;

            if (this.textBoxSemtech.Lines.Count() > MaxLines)
            {
                string[] lines = this.textBoxSemtech.Lines;
                textBoxSemtech.Lines = lines.Skip(textBoxSemtech.Lines.Count() - MaxLines).ToArray();
            }
        }

        private void UpdateSemtechTextBox(string text)
        {
            if (listViewSemtechCommandsBatchesEditMode)
                textBoxSemtechContent += text;
            else if (textBoxSemtech.InvokeRequired)
            {
                itguid d = new itguid(UpdateSemtechTextBox);
                this.Invoke(d, text);
            }
            else
                textBoxSemtech.AppendText(text);
        }

        //------------------------------- Ubiquiti Text Box processing -------------------------------------------------------

        private void textBoxUbiquiti_TextChanged(object sender, EventArgs e)
        {
            const byte MaxLines = 50;

            if (this.textBoxUbiquiti.Lines.Count() > MaxLines)
            {
                string[] lines = this.textBoxUbiquiti.Lines;
                textBoxUbiquiti.Lines = lines.Skip(textBoxUbiquiti.Lines.Count() - MaxLines).ToArray();
            }
        }

        private void UpdateUbiquitiTextBox(string text)
        {
            if (listViewUbiquitiCommandsBatchesEditMode)
                textBoxUbiquitiContent += text;
            else if (textBoxUbiquiti.InvokeRequired)
            {
                itguid d = new itguid(UpdateUbiquitiTextBox);
                this.Invoke(d, text);
            }
            else
                textBoxUbiquiti.AppendText(text);
        }

        //------------------------------- STHacked Text Box processing -------------------------------------------------------

        private void textBoxSTHacked_TextChanged(object sender, EventArgs e)
        {
            const byte MaxLines = 50;

            if (this.textBoxSTHacked.Lines.Count() > MaxLines)
            {
                string[] lines = this.textBoxSTHacked.Lines;
                textBoxSTHacked.Lines = lines.Skip(textBoxSTHacked.Lines.Count() - MaxLines).ToArray();
            }
        }

        private void UpdateSTHackedTextBox(string text)
        {
            if (listViewSTHackedCommandsBatchesEditMode)
                textBoxSTHackedContent += text;
            else if (textBoxSTHacked.InvokeRequired)
            {
                itguid d = new itguid(UpdateSTHackedTextBox);
                this.Invoke(d, text);
            }
            else
                textBoxSTHacked.AppendText(text);
        }

        //------------------------------- COM port processing ------------------------------------------------------------

        public bool SendToComPort(ref SerialPort port, string data)
        {
            try
            {
                port.Write(data + "\r\n");
                return true;
            }
            catch (InvalidOperationException) { MessageBoxShow("Com port is not open"); }
            catch (ArgumentNullException) { MessageBoxShow("Attempting to write an empty string"); }
            catch (TimeoutException) { MessageBoxShow("Write time-out"); }
            return false;
        }

        public bool SendToComPort(ref SerialPort port, byte[] data)
        {
            try
            {
                port.Write(data, 0, data.Length);
                port.Write("\r\n");
                return true;
            }
            catch (InvalidOperationException) { MessageBoxShow("Com port is not open"); }
            catch (ArgumentNullException) { MessageBoxShow("Attempting to write an empty string"); }
            catch (TimeoutException) { MessageBoxShow("Write time-out"); }
            return false;
        }

        //------------------------------- Pic COM port processing --------------------------------------------------------

        private void PicDisplayComPortData(string text)
        {
            UpdatePicTextBox(text);
            DisplayGeneralData("Pic Radio: " + text);
        }

        private void PicComPortTimer(Object source, ElapsedEventArgs e)
        {
            timerPicComPort.Enabled = false;
            PicDisplayComPortData(PicComPortData);
        }

        private void PicComPortHandler(object sender, SerialDataReceivedEventArgs e)
        {
            timerPicComPort.Interval = 20;

            if (false == timerPicComPort.Enabled) //new radio message started
            {
                PicComPortData = "";
                timerPicComPort.Enabled = true;
                timerPicComPort.Start();
            }

            PicComPortData += ((SerialPort)sender).ReadExisting();
        }


        //------------------------------- Semtech COM port processing -----------------------------------------------------

        private void SemtechDisplayComPortData(string text)
        {
            UpdateSemtechTextBox(text);
            DisplayGeneralData("Semtech Radio: " + text);
        }

        private void SemtechComPortTimer(Object source, ElapsedEventArgs e)
        {
            timerSemtechComPort.Enabled = false;
            SemtechDisplayComPortData(SemtechComPortData);
        }

        private void SemtechComPortHandler(object sender, SerialDataReceivedEventArgs e)
        {
            timerSemtechComPort.Interval = 20;

            if(false == timerSemtechComPort.Enabled) //new radio message started
            {
                SemtechComPortData = "";
                timerSemtechComPort.Enabled = true;
            }

            SemtechComPortData += ((SerialPort)sender).ReadExisting();

            timerSemtechComPort.Start();
        }

        //------------------------------- Ubiquiti COM port processing -----------------------------------------------------
/*
        private void UbiquitiDisplayComPortData(byte[] input)
        {
            DisplayGeneralData("Ubiquiti Radio: " + BitConverter.ToString(input.ToArray()).Replace("-", " ") + "\r\n");

            try
            {
                while (input.Length > 0) 
                    if(input[0] <= input.Length && input[0] > 0)
                    {
                        UpdateUbiquitiTextBox(utilities.FilterMessageFromUbiquitiRadio(input.Take(input[0]).ToArray()));
                        input = input.Skip(input[0]).ToArray();
                    }
                    else
                        throw new System.Exception("wrong message size");
            }
            catch (Exception ex)
            {
                MessageBoxShow(ex.ToString());
                MessageBoxShow(BitConverter.ToString(input.ToArray()).Replace("-", " "));
            }

        }
*/

        private void UbiquitiDisplayComPortData(byte[] input)
        {
            DisplayGeneralData("Ubiquiti Radio: " + BitConverter.ToString(input.ToArray()).Replace("-", " ") + "\r\n");

 //           if (null == ubiquitiInput)
 //               ubiquitiInput = input;
 //           else
                ubiquitiInput = ubiquitiInput.Concat(input).ToArray();

            try
            {
                while ((ubiquitiInput.Length > 0) && (ubiquitiInput[0] > 0) && (ubiquitiInput[0] <= ubiquitiInput.Length))
                {
                    //if (ubiquitiInput.Length > 2)
                    //    ;
                    UpdateUbiquitiTextBox(utilities.FilterMessageFromUbiquitiRadio(ubiquitiInput.Take(ubiquitiInput[0]).ToArray()));
                    ubiquitiInput = ubiquitiInput.Skip(ubiquitiInput[0]).ToArray();

                }
            }
            catch (Exception ex)
            {
                MessageBoxShow(ex.ToString());
                MessageBoxShow(BitConverter.ToString(input.ToArray()).Replace("-", " "));
            }

        }

        /*
                private void UbiquitiComPortTimer(Object source, ElapsedEventArgs e)
                {
                    timerUbiquitiComPort.Enabled = false;
                    byte[] data = new byte[SerialPortUbiquiti.BytesToRead];
                    SerialPortUbiquiti.Read(data, 0, data.Length);
                    UbiquitiDisplayComPortData(data);
                }

                private void UbiquitiComPortHandler(object sender, SerialDataReceivedEventArgs e)
                {
                    timerUbiquitiComPort.Start();
                }
        */

        private void UbiquitiComPortTimer(Object source, ElapsedEventArgs e)
        {
            //          timerUbiquitiComPort.Enabled = false;
            timerUbiquitiComPort.Stop();
            //ubiquitiInput = null;
            ubiquitiInput = new byte[0];
        }

        private void UbiquitiComPortHandler(object sender, SerialDataReceivedEventArgs e)
        {
            byte[] data = new byte[SerialPortUbiquiti.BytesToRead];
            if(data.Length > 0) {
                SerialPortUbiquiti.Read(data, 0, data.Length);
                UbiquitiDisplayComPortData(data);
                timerUbiquitiComPort.Stop();
                timerUbiquitiComPort.Start();
            }
        }

        //------------------------------- STHacked COM port processing -----------------------------------------------------

        private void STHackedDisplayComPortData(byte[] input)
        {
            DisplayGeneralData("STHacked Radio: " + BitConverter.ToString(input).Replace("-", " ") + "\r\n");

            try
            {
                while (input.Length > 0) 
                    if(input[0] <= input.Length && input[0] > 0)
                    {
                        UpdateSTHackedTextBox(utilities.FilterMessageFromUbiquitiRadio(input.Take(input[0]).ToArray()));
                        input = input.Skip(input[0]).ToArray();
                    }
                    else
                        throw new System.Exception("wrong message size");
            }
            catch (Exception ex)
            {
                MessageBoxShow(ex.ToString());
                MessageBoxShow(BitConverter.ToString(input.ToArray()).Replace("-", " "));
            }
        }

        private void STHackedComPortTimer(Object source, ElapsedEventArgs e)
        {
            timerSTHackedComPort.Enabled = false;
            byte[] data = new byte [SerialPortSTHacked.BytesToRead];
            SerialPortSTHacked.Read(data, 0, data.Length);
            STHackedDisplayComPortData(data);
        }

        private void STHackedComPortHandler(object sender, SerialDataReceivedEventArgs e)
        {
            timerSTHackedComPort.Start();
        }

        //------------------------------- general texbox processing -----------------------------------------------------
        private void textBoxGeneralData_TextChanged(object sender, EventArgs e)
        {
            //TODO: limit the text size
        }


        private void DisplayGeneralData(string text)
        {
            if (textBoxGeneralData.InvokeRequired)
            {
                itguid d = new itguid(DisplayGeneralData);
                this.Invoke(d, text);
            }
            else
                textBoxGeneralData.AppendText(DateTime.Now.ToString("h:mm:ss.fff tt") + " " + text);
        }

        private void MessageBoxShow(string text)
        {
            DisplayGeneralData(text + "\r\n");
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == WM_DEVICECHANGE)   //                DisplayGeneralData(m.WParam.ToString() + "\r\n");
                switch ((int)m.WParam)
                {
                    case DBT_DEVICEREMOVECOMPLETE:
                    case DBT_DEVICEARRIVAL:
                        EnumerateComPorts();
                        break;
                }
        }

    }   //public partial class Form1 : Form
}



namespace WindowsFormsApplication1
{
    public class ThreadWithState
    {
        // State information used in the task.
        private string commands;
        //private int value;

        private ExampleCallback callback;

        // The constructor obtains the state information.
        public ThreadWithState(string text, ExampleCallback callbackDelegate)
        {
            commands = text;
            callback = callbackDelegate;
        }

        // The thread procedure performs the task, such as formatting
        // and printing a document.
        public void ThreadProc()
        {
            //   MessageBox.Show(commands);
            if (callback != null)
                callback(commands);
        }
    }

    public delegate void ExampleCallback(string text);

}