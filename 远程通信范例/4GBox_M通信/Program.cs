using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using XNetSocketClient;
using Repackage;
using System.Threading;


//注意与XC PLC通信需要多一个dll：Repackage.dll
//同时XC不支持协议栈，查找设备时是对4GBox_M查找，通信也是对4GBox_M通信
namespace _4GBox_M通信
{
    class Program
    {
        static void Main(string[] args)
        {

            #region 与通信服务建立tcp连接
            CommTcp commTcp = new CommTcp("127.0.0.1", 2323);//1.连接到服务 X_Net_Windows.exe
            try
            {
                commTcp.Connect();
            }
            catch (XNetException ex)
            {
                Console.WriteLine("连接服务失败" + ex.Message);
                return;
            }
            #endregion

            #region 配置通信参数
            //设备ID，安全模式，PLC密码，用户名，密码（目前，安全模式为1，用户名密码都为“”空）
            XNetConnectRemoteParams xnetRemoteParams = new XNetConnectRemoteParams("020001003b0f00004", 1, "12345678", "", "");

            //远程通信的通信端口为240
            XNetConnectParams XNetParams = new XNetConnectParams(240, DeviceType.GBOX_4G, null, xnetRemoteParams);//查找4GBox_M而不是PLC

            XNet xnet = new XNet(XNetParams, commTcp);
            #endregion

            #region 建立远程连接
            try
            {
                xnet.Connect_RemoteDevice();
            }
            catch (XNetException ex)
            {
                Console.WriteLine("建立远程连接失败：" + ex.Message);
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
                Console.WriteLine(ex.Message);
                return;
            }
            #endregion

            #region 通信
            XNetComm hComm = new XNetComm(xnet);
            XNetComm_XC h_XCComm = new XNetComm_XC(hComm);//构造XC的通信句柄

            //读10个寄存器
            short[] readRegs = new short[10];//10个
            try
            {
                h_XCComm.Read(XCPLCRegType.D, 1000, readRegs);//从D1000位置开始读10个寄存器，存入readRegs
                
            }
            catch (XNetException ex)
            {
                Console.WriteLine(ex.Message);//读取失败的错误信息
                return;
            }


            //写10个寄存器
            short[] writeRegs = new short[10];//10个
            try
            {
                h_XCComm.Write(XCPLCRegType.D, 2000, writeRegs);//从D2000位置开始写10个寄存器，把writeRegs写入
            }
            catch (XNetException ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }
            #endregion

            Console.WriteLine("成功");
            Console.ReadLine();
        }
    }
}
