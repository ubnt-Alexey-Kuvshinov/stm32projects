namespace WindowsFormsApplication1
{
    partial class Alarm
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
            this.dateTimePickerTime = new System.Windows.Forms.DateTimePicker();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonOK = new System.Windows.Forms.Button();
            this.dateTimePickerDate = new System.Windows.Forms.DateTimePicker();
            this.checkBoxEveryDay = new System.Windows.Forms.CheckBox();
            this.checkBoxEveryHour = new System.Windows.Forms.CheckBox();
            this.checkBoxEveryMinute = new System.Windows.Forms.CheckBox();
            this.checkBoxEverySecond = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // dateTimePickerTime
            // 
            this.dateTimePickerTime.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.dateTimePickerTime.Location = new System.Drawing.Point(31, 86);
            this.dateTimePickerTime.Name = "dateTimePickerTime";
            this.dateTimePickerTime.ShowUpDown = true;
            this.dateTimePickerTime.Size = new System.Drawing.Size(200, 20);
            this.dateTimePickerTime.TabIndex = 1;
            // 
            // buttonCancel
            // 
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(31, 135);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 5;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(31, 184);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 4;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // dateTimePickerDate
            // 
            this.dateTimePickerDate.Location = new System.Drawing.Point(31, 49);
            this.dateTimePickerDate.Name = "dateTimePickerDate";
            this.dateTimePickerDate.Size = new System.Drawing.Size(200, 20);
            this.dateTimePickerDate.TabIndex = 0;
            // 
            // checkBoxEveryDay
            // 
            this.checkBoxEveryDay.AutoSize = true;
            this.checkBoxEveryDay.Location = new System.Drawing.Point(136, 122);
            this.checkBoxEveryDay.Name = "checkBoxEveryDay";
            this.checkBoxEveryDay.Size = new System.Drawing.Size(83, 17);
            this.checkBoxEveryDay.TabIndex = 6;
            this.checkBoxEveryDay.Text = "Ignore Days";
            this.checkBoxEveryDay.UseVisualStyleBackColor = true;
            // 
            // checkBoxEveryHour
            // 
            this.checkBoxEveryHour.AutoSize = true;
            this.checkBoxEveryHour.Location = new System.Drawing.Point(136, 145);
            this.checkBoxEveryHour.Name = "checkBoxEveryHour";
            this.checkBoxEveryHour.Size = new System.Drawing.Size(87, 17);
            this.checkBoxEveryHour.TabIndex = 6;
            this.checkBoxEveryHour.Text = "Ignore Hours";
            this.checkBoxEveryHour.UseVisualStyleBackColor = true;
            // 
            // checkBoxEveryMinute
            // 
            this.checkBoxEveryMinute.AutoSize = true;
            this.checkBoxEveryMinute.Location = new System.Drawing.Point(136, 168);
            this.checkBoxEveryMinute.Name = "checkBoxEveryMinute";
            this.checkBoxEveryMinute.Size = new System.Drawing.Size(96, 17);
            this.checkBoxEveryMinute.TabIndex = 6;
            this.checkBoxEveryMinute.Text = "Ignore Minutes";
            this.checkBoxEveryMinute.UseVisualStyleBackColor = true;
            // 
            // checkBoxEverySecond
            // 
            this.checkBoxEverySecond.AutoSize = true;
            this.checkBoxEverySecond.Location = new System.Drawing.Point(136, 191);
            this.checkBoxEverySecond.Name = "checkBoxEverySecond";
            this.checkBoxEverySecond.Size = new System.Drawing.Size(101, 17);
            this.checkBoxEverySecond.TabIndex = 6;
            this.checkBoxEverySecond.Text = "Ignore Seconds";
            this.checkBoxEverySecond.UseVisualStyleBackColor = true;
            // 
            // Alarm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 261);
            this.Controls.Add(this.checkBoxEverySecond);
            this.Controls.Add(this.checkBoxEveryMinute);
            this.Controls.Add(this.checkBoxEveryHour);
            this.Controls.Add(this.checkBoxEveryDay);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonOK);
            this.Controls.Add(this.dateTimePickerTime);
            this.Controls.Add(this.dateTimePickerDate);
            this.Name = "Alarm";
            this.Text = "Alarm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        public System.Windows.Forms.DateTimePicker dateTimePickerTime;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Button buttonOK;
        public System.Windows.Forms.DateTimePicker dateTimePickerDate;
        public System.Windows.Forms.CheckBox checkBoxEveryDay;
        public System.Windows.Forms.CheckBox checkBoxEveryHour;
        public System.Windows.Forms.CheckBox checkBoxEveryMinute;
        public System.Windows.Forms.CheckBox checkBoxEverySecond;
    }
}