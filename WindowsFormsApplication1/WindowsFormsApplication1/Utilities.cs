using System;
using System.IO;
using System.Text.RegularExpressions;
using System.Collections.Generic;
using System.Globalization;
using System.Windows.Forms;
using System.Linq;


using WindowsFormsApplication1;

enum RadioCommands : byte { GET_DATE = 0x02, SET_DATE, GET_TIME, SET_TIME, GET_CURRENT = 0x73, GET_TEMP = 0xF7, GET_PICTURE = 0xF9, PICTURE_DATA, END_PICTURE, RX_MESSAGE, SET_ALARM = 0xFE, GET_TEXT = 0xFF };
enum RtcDayOfWeek : byte { mon = 0x01, tue, wed, thu, fri, sat, sun };
enum RtcMonth : byte { jan = 0x01, feb, mar, apr, may, jun, jul, aug, sep, oct = 0x10, nov, dec };


public static class utilities
{
    static Dictionary<RtcDayOfWeek, string> weekdays = new Dictionary<RtcDayOfWeek, string>()
    { {RtcDayOfWeek.mon, "Mon"}, {RtcDayOfWeek.tue, "Tue"}, {RtcDayOfWeek.wed, "Wed"}, {RtcDayOfWeek.thu, "Thu"}, {RtcDayOfWeek.fri, "Fri"}, {RtcDayOfWeek.sat, "Sat"}, {RtcDayOfWeek.sun, "Sun"} };

    static Dictionary<RtcMonth, string> months = new Dictionary<RtcMonth, string>()
    { {RtcMonth.jan, "Jan"}, {RtcMonth.feb, "Feb"}, {RtcMonth.mar, "Mar"}, {RtcMonth.apr, "Apr"}, {RtcMonth.may, "May"}, {RtcMonth.jun, "Jun"}, {RtcMonth.jul, "Jul"}, {RtcMonth.aug, "Aug"}, {RtcMonth.sep, "Sep"}, {RtcMonth.oct, "Oct"}, {RtcMonth.nov, "Nov"}, {RtcMonth.dec, "Dec"} };


    public static string ByteTohexString(byte data)
    {
        string result = "";

        if ((data >> 4) < 10)
            result += (char)('0' + (data >> 4));
        else
            result += (char)('A' + (data >> 4) - 10);


        if ((data & 0x0F) < 10)
            result += (char)('0' + (data & 0x0F));
        else
            result += (char)('A' + (data & 0x0F) - 10);

        return result;
    }

    public static byte hexStringToByte(string instr)
    {
        //byte[] data = System.Text.Encoding.UTF8.GetBytes(instr);
        byte[] data = System.Text.Encoding.ASCII.GetBytes(instr);
        byte output;

        if ('0' <= data[0] && data[0] <= '9') output = (byte)(data[0] - '0');
        else if ('a' <= data[0] && data[0] <= 'f') output = (byte)(data[0] - 'a' + 10);
        else if ('A' <= data[0] && data[0] <= 'F') output = (byte)(data[0] - 'A' + 10);
        else throw new System.ArgumentException("not a valid Hex value", instr);
        output <<= 4;
        if ('0' <= data[1] && data[1] <= '9') output += (byte)(data[1] - '0');
        else if ('a' <= data[1] && data[1] <= 'f') output += (byte)(data[1] - 'a' + 10);
        else if ('A' <= data[1] && data[1] <= 'F') output += (byte)(data[1] - 'A' + 10);
        else throw new System.ArgumentException("not a valid Hex value", instr);

        return output;
    }


    public static byte[] utilHexToBin(string data)
    {
        string[] values = Regex.Split(data, " ");
        byte[] result = new byte[values.Length];

        for(int i = 0; i < values.Length; i++)
        {
            if (values[i].Length != 2)
                throw new System.ArgumentException("not a valid Hex size", data);

            result[i] = hexStringToByte(values[i]);
        }

        return result;
    }


    public static string FilterMessageFromUbiquitiRadio(byte[] msgData)
    {
        string result = "";
        DateTime dt;

        /**/
        switch (msgData[1])
        {
            case (byte)RadioCommands.GET_TEXT:                                      //display text

                result = System.Text.Encoding.ASCII.GetString(msgData.Skip(2).ToArray()).Replace("-", " ");
                return result + "\r\n";

            case (byte)RadioCommands.GET_DATE:                                      //format date

                if (weekdays.ContainsKey((RtcDayOfWeek)msgData[2]))
                    result += weekdays[(RtcDayOfWeek)msgData[2]] + ", ";
                else
                    result += "?, ";

                result += msgData[3].ToString() + " ";

                if (months.ContainsKey((RtcMonth)msgData[4]))
                    result += months[(RtcMonth)msgData[4]] + " ";
                else
                    result += "? ";

                result += "20" + (msgData[5] >> 4).ToString() + (msgData[5] & 0x0F).ToString();

                if (!DateTime.TryParse(result, out dt))
                    result += " : the date is invalid";

                return result + "\r\n";

            case (byte)RadioCommands.GET_TIME:                                      //format time

                result = ByteTohexString(msgData[2]) + ":" + ByteTohexString(msgData[3]) + ":" + ByteTohexString(msgData[4]);
                if (!DateTime.TryParse(result, out dt))
                    result += " : the time is invalid";
                else
                { 
                    DateTime time = new DateTime(2000, 1, 1, (msgData[2] >> 4) * 10 + (msgData[2] & 0x0F), (msgData[3] >> 4) * 10 + (msgData[3] & 0x0F), (msgData[4] >> 4) * 10 + (msgData[4] & 0x0F));
                    result = time.ToLongTimeString();
                }

                return result + "\r\n";


            case (byte)RadioCommands.GET_CURRENT:                                      //format time

                if (4 == msgData.Length)
                {
                    Int16 tmp;

                    tmp = (Int16)((msgData[2] << 8) | msgData[3]);

                    result = "device current: ";
                    result += (tmp * 1.25).ToString();
                    return result + " ma\r\n";
                }
                break;

            case (byte)RadioCommands.GET_TEMP:
            case (byte)RadioCommands.RX_MESSAGE:

                msgData = msgData.Skip(1).ToArray();
                break;

            case (byte)RadioCommands.GET_PICTURE:                                      //get picture command confirmation from the camera

                    if (null != WindowsFormsApplication1.Form1.FsPicture)
                        WindowsFormsApplication1.Form1.FsPicture.Close();

                    WindowsFormsApplication1.Form1.FsPicture = new FileStream("pic.jpg", FileMode.OpenOrCreate, FileAccess.Write);
                    return "opened file pic.jpg \r\n";

            case (byte)RadioCommands.PICTURE_DATA:                                      //next picture data packet received from the camera

                WindowsFormsApplication1.Form1 form1 = (WindowsFormsApplication1.Form1)Application.OpenForms["Form1"];

                result = ByteTohexString(msgData[3]) + " " + ByteTohexString(msgData[2]) + " CKSUM exp: " + ByteTohexString(msgData[msgData.Length - 2]);

                byte i, sum = 0;
                for (i = 2; i < msgData.Length- 2; i++)
                    sum += msgData[i];

                result += " CKSUM calc: " + ByteTohexString(sum);
                if (sum != msgData[msgData.Length - 2]) {                                               //verify the checksum
                    result += " mismatch! " + BitConverter.ToString(msgData).Replace("-", " ");
                    form1.SendToComPort(ref form1.SerialPortUbiquiti, new byte[] { 0xFA, 0x01 });       //request packet resend
                }
                else
                {
                    WindowsFormsApplication1.Form1.FsPicture.Write(msgData, 6, msgData.Length - 8);     //store data to file
                    form1.SendToComPort(ref form1.SerialPortUbiquiti, new byte[] { 0xFA, 0x00 });       //request next packet
                }

                return result + "\r\n";


            case (byte)RadioCommands.END_PICTURE:

                WindowsFormsApplication1.Form1.FsPicture.Close();
                return "picture transfer complete \r\n";

            default:
                break;
        }
        
        return BitConverter.ToString(msgData.Skip(1).ToArray()).Replace("-", " ") + "\r\n";
    }


    public static bool FilterCommandsToUbiquitiRadio(ref string command)
    {
        //       byte[] msgData = utilHexToBin(text.TrimEnd('\r', '\n', ' '));
        byte[] msgData = utilHexToBin(command);

        switch (msgData[0])
        {
            case (byte)RadioCommands.SET_DATE:                                      //pick date and convert it to the RTC format

                WindowsFormsApplication1.Form2 customForm = new WindowsFormsApplication1.Form2();
                customForm.dateTimePicker1.Format = DateTimePickerFormat.Long;
                customForm.dateTimePicker1.ShowUpDown = false;
                var fr = customForm.ShowDialog();

                if (DialogResult.Cancel == fr)                                      //data pick was canceled
                {
                    customForm.Dispose();
                    return false;
                }
                else
                {
                    if(DayOfWeek.Sunday == customForm.dateTimePicker1.Value.DayOfWeek)
                        command = " " + ByteTohexString((byte)RtcDayOfWeek.sun);
                    else
                        command = " " + ByteTohexString((byte)customForm.dateTimePicker1.Value.DayOfWeek);
                    command += " " + ByteTohexString((byte)customForm.dateTimePicker1.Value.Day);
                    command += " " + ByteTohexString((byte)(((customForm.dateTimePicker1.Value.Month / 10) << 4) + customForm.dateTimePicker1.Value.Month % 10));
                    command += " " + ByteTohexString((byte)((((customForm.dateTimePicker1.Value.Year - 2000) / 10) << 4) + (customForm.dateTimePicker1.Value.Year - 2000) % 10));
                }

                customForm.Dispose();
                return true;

            case (byte)RadioCommands.SET_TIME:                                      //pick time and convert it to the RTC format

                WindowsFormsApplication1.Form2 customForm2 = new WindowsFormsApplication1.Form2();
                customForm2.dateTimePicker1.Format = DateTimePickerFormat.Time;
                customForm2.dateTimePicker1.ShowUpDown = true;
                var fr2 = customForm2.ShowDialog();

                if (DialogResult.Cancel == fr2)                                      //time pick was canceled
                {
                    customForm2.Dispose();
                    return false;
                }
                else
                {
                    command = " " + ByteTohexString((byte)(((customForm2.dateTimePicker1.Value.Hour / 10) << 4) + customForm2.dateTimePicker1.Value.Hour % 10));
                    command += " " + ByteTohexString((byte)(((customForm2.dateTimePicker1.Value.Minute / 10) << 4) + customForm2.dateTimePicker1.Value.Minute % 10));
                    command += " " + ByteTohexString((byte)(((customForm2.dateTimePicker1.Value.Second / 10) << 4) + customForm2.dateTimePicker1.Value.Second % 10));
                }

                customForm2.Dispose();
                return true;

            case (byte)RadioCommands.SET_ALARM:                                      //pick time and convert it to the RTC format

                WindowsFormsApplication1.Alarm customForm3 = new WindowsFormsApplication1.Alarm();
                var fr3 = customForm3.ShowDialog();

                if (DialogResult.Cancel == fr3)                                      //time pick was canceled
                {
                    customForm3.Dispose();
                    return false;
                }
                else
                {
                    command = " " + ByteTohexString((byte)(((customForm3.dateTimePickerTime.Value.Hour / 10) << 4) + customForm3.dateTimePickerTime.Value.Hour % 10));
                    command += " " + ByteTohexString((byte)(((customForm3.dateTimePickerTime.Value.Minute / 10) << 4) + customForm3.dateTimePickerTime.Value.Minute % 10));
                    command += " " + ByteTohexString((byte)(((customForm3.dateTimePickerTime.Value.Second / 10) << 4) + customForm3.dateTimePickerTime.Value.Second % 10));

                    command += " " + ByteTohexString((byte)(customForm3.dateTimePickerDate.Value.Day));
                    if (customForm3.checkBoxEveryDay.Checked) command += " 80"; else command += " 00";
                    if (customForm3.checkBoxEveryDay.Checked) command += " 80"; else command += " 00";
                    if (customForm3.checkBoxEveryDay.Checked) command += " 80"; else command += " 00";
                    if (customForm3.checkBoxEveryDay.Checked) command += " 80"; else command += " 00";
                }

                customForm3.Dispose();
                return true;
            default:
                break;
        }

        return false;
    }
}