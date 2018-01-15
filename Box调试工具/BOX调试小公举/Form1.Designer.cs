namespace BOX调试小公举
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.FindDevicePanel = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.SetupRemoteLinkPanel = new System.Windows.Forms.Panel();
            this.IDBox = new System.Windows.Forms.MaskedTextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.PasswordBox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.SetupRemoteLinkBtn = new System.Windows.Forms.Button();
            this.DeviceTypeCB = new System.Windows.Forms.ComboBox();
            this.OffSetNum = new System.Windows.Forms.NumericUpDown();
            this.RegNum = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.ReadBtn = new System.Windows.Forms.Button();
            this.ShowTextBox = new System.Windows.Forms.TextBox();
            this.FindDevicePanel.SuspendLayout();
            this.SetupRemoteLinkPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.OffSetNum)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.RegNum)).BeginInit();
            this.SuspendLayout();
            // 
            // FindDevicePanel
            // 
            this.FindDevicePanel.Controls.Add(this.DeviceTypeCB);
            this.FindDevicePanel.Controls.Add(this.label1);
            this.FindDevicePanel.Location = new System.Drawing.Point(335, 24);
            this.FindDevicePanel.Name = "FindDevicePanel";
            this.FindDevicePanel.Size = new System.Drawing.Size(271, 121);
            this.FindDevicePanel.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(30, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "机型：";
            // 
            // SetupRemoteLinkPanel
            // 
            this.SetupRemoteLinkPanel.Controls.Add(this.label3);
            this.SetupRemoteLinkPanel.Controls.Add(this.PasswordBox);
            this.SetupRemoteLinkPanel.Controls.Add(this.label2);
            this.SetupRemoteLinkPanel.Controls.Add(this.IDBox);
            this.SetupRemoteLinkPanel.Location = new System.Drawing.Point(30, 24);
            this.SetupRemoteLinkPanel.Name = "SetupRemoteLinkPanel";
            this.SetupRemoteLinkPanel.Size = new System.Drawing.Size(275, 121);
            this.SetupRemoteLinkPanel.TabIndex = 4;
            // 
            // IDBox
            // 
            this.IDBox.InsertKeyMode = System.Windows.Forms.InsertKeyMode.Overwrite;
            this.IDBox.Location = new System.Drawing.Point(97, 18);
            this.IDBox.Mask = "000-000-000-aaaa-0000";
            this.IDBox.Name = "IDBox";
            this.IDBox.PromptChar = ' ';
            this.IDBox.Size = new System.Drawing.Size(142, 21);
            this.IDBox.TabIndex = 0;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(35, 22);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(47, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "BoxID：";
            // 
            // PasswordBox
            // 
            this.PasswordBox.Location = new System.Drawing.Point(97, 46);
            this.PasswordBox.MaxLength = 8;
            this.PasswordBox.Name = "PasswordBox";
            this.PasswordBox.Size = new System.Drawing.Size(142, 21);
            this.PasswordBox.TabIndex = 2;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(41, 49);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "密码：";
            // 
            // SetupRemoteLinkBtn
            // 
            this.SetupRemoteLinkBtn.Location = new System.Drawing.Point(51, 187);
            this.SetupRemoteLinkBtn.Name = "SetupRemoteLinkBtn";
            this.SetupRemoteLinkBtn.Size = new System.Drawing.Size(75, 23);
            this.SetupRemoteLinkBtn.TabIndex = 4;
            this.SetupRemoteLinkBtn.Text = "建立远程连接";
            this.SetupRemoteLinkBtn.UseVisualStyleBackColor = true;
            this.SetupRemoteLinkBtn.Click += new System.EventHandler(this.SetupRemoteLinkBtn_Click);
            // 
            // DeviceTypeCB
            // 
            this.DeviceTypeCB.FormattingEnabled = true;
            this.DeviceTypeCB.Items.AddRange(new object[] {
            "PLC_XD ",
            "GBOX_4G",
            "WBOX",
            "PLC_XE"});
            this.DeviceTypeCB.Location = new System.Drawing.Point(129, 18);
            this.DeviceTypeCB.Name = "DeviceTypeCB";
            this.DeviceTypeCB.Size = new System.Drawing.Size(121, 20);
            this.DeviceTypeCB.TabIndex = 5;
            this.DeviceTypeCB.Text = "WBOX";
            // 
            // OffSetNum
            // 
            this.OffSetNum.Location = new System.Drawing.Point(95, 272);
            this.OffSetNum.Name = "OffSetNum";
            this.OffSetNum.Size = new System.Drawing.Size(53, 21);
            this.OffSetNum.TabIndex = 6;
            // 
            // RegNum
            // 
            this.RegNum.Location = new System.Drawing.Point(95, 306);
            this.RegNum.Name = "RegNum";
            this.RegNum.Size = new System.Drawing.Size(53, 21);
            this.RegNum.TabIndex = 7;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 274);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(53, 12);
            this.label4.TabIndex = 8;
            this.label4.Text = "起始地址";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 308);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(65, 12);
            this.label5.TabIndex = 9;
            this.label5.Text = "寄存器数量";
            // 
            // ReadBtn
            // 
            this.ReadBtn.Location = new System.Drawing.Point(67, 350);
            this.ReadBtn.Name = "ReadBtn";
            this.ReadBtn.Size = new System.Drawing.Size(75, 23);
            this.ReadBtn.TabIndex = 10;
            this.ReadBtn.Text = "读取";
            this.ReadBtn.UseVisualStyleBackColor = true;
            this.ReadBtn.Click += new System.EventHandler(this.ReadBtn_Click);
            // 
            // ShowTextBox
            // 
            this.ShowTextBox.Location = new System.Drawing.Point(228, 210);
            this.ShowTextBox.Multiline = true;
            this.ShowTextBox.Name = "ShowTextBox";
            this.ShowTextBox.Size = new System.Drawing.Size(378, 148);
            this.ShowTextBox.TabIndex = 11;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(731, 401);
            this.Controls.Add(this.ShowTextBox);
            this.Controls.Add(this.ReadBtn);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.RegNum);
            this.Controls.Add(this.OffSetNum);
            this.Controls.Add(this.SetupRemoteLinkBtn);
            this.Controls.Add(this.SetupRemoteLinkPanel);
            this.Controls.Add(this.FindDevicePanel);
            this.Name = "Form1";
            this.Text = "Box小工具";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.FindDevicePanel.ResumeLayout(false);
            this.FindDevicePanel.PerformLayout();
            this.SetupRemoteLinkPanel.ResumeLayout(false);
            this.SetupRemoteLinkPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.OffSetNum)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.RegNum)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel FindDevicePanel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel SetupRemoteLinkPanel;
        private System.Windows.Forms.Button SetupRemoteLinkBtn;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox PasswordBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.MaskedTextBox IDBox;
        private System.Windows.Forms.ComboBox DeviceTypeCB;
        private System.Windows.Forms.NumericUpDown OffSetNum;
        private System.Windows.Forms.NumericUpDown RegNum;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button ReadBtn;
        private System.Windows.Forms.TextBox ShowTextBox;
    }
}

