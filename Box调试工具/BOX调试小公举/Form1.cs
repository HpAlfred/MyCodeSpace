using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using XNetSocketClient;

namespace BOX调试小公举
{
    public partial class Form1 : Form
    {
        CommTcp commTcp;
        XNetConnectRemoteParams xnetRemoteParams;
        XNetConnectParams XNetParams;
        XNet xnet;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            XNetSocketClient.XNetServerManager.Start();
        }

        private void SetupRemoteLinkBtn_Click(object sender, EventArgs e)
        {
            #region 与通信服务建立tcp连接
            commTcp = new CommTcp("127.0.0.1", 2323);//1.连接到服务 X_Net_Windows.exe
            try
            {
                commTcp.Connect();
            }
            catch (XNetException ex)
            {
                MessageBox.Show("连接服务失败" + ex.Message);
                return;
            }
            #endregion

            #region 配置通信参数
            //设备ID，安全模式，PLC密码，用户名，密码（目前，安全模式为1，用户名密码都为“”空）
            xnetRemoteParams = new XNetConnectRemoteParams(IDBox.Text, 1, PasswordBox.Text, "", "");

            DeviceType deviceType;
            switch (DeviceTypeCB.Text)
            {
                case "PLC_XD":
                    deviceType = DeviceType.XDPLC;
            	    break;
                case "GBOX_4G":
                    deviceType = DeviceType.GBOX_4G;
            	    break;
                case "WBOX":
                    deviceType = DeviceType.WBOX;
            	    break;
                default:
                    deviceType = DeviceType.WBOX;
                    break;
            }
            //远程通信的通信端口为240，支持各种查找，这里没填ID是类型查找。
            XNetParams = new XNetConnectParams(240, deviceType, null, xnetRemoteParams);

            xnet = new XNet(XNetParams, commTcp);
            #endregion


            #region 建立远程连接
            try
            {
                xnet.Connect_RemoteDevice();
            }
            catch (XNetException ex)
            {
                MessageBox.Show("建立远程连接失败：" + ex.Message);
                return;
            }
            #endregion

            #region 查找设备
            try
            {
                xnet.Connect_Find_Device();//查找设备
            }
            catch (XNetException ex)
            {
                MessageBox.Show(ex.Message);
                return;
            }
            #endregion
            MessageBox.Show("成功" );
        }

        private void ReadBtn_Click(object sender, EventArgs e)
        {
            #region 通信
            XNetComm hComm = new XNetComm(xnet);//构造通信句柄

            //读10个寄存器
            short[] readRegs = new short[(int)(RegNum.Value)];//100个
            try
            {
                hComm.Read(PLCRegType.F0, (int)(OffSetNum.Value), readRegs);//从D1000位置开始读10个寄存器，存入readRegs
            }
            catch (XNetException ex)
            {
                MessageBox.Show(ex.Message);//读取失败的错误信息
                return;
            }
            MessageBox.Show("成功");
            #endregion
            
            ShowTextBox.Text = "";

            foreach (short a in readRegs)
            {
                ShowTextBox.Text += a.ToString("x4");
                ShowTextBox.Text += " ";
            }
            
        }

        

        
    }
}
