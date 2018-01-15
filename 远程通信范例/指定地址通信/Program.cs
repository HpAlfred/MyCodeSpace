using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using XNetSocketClient;//

namespace 指定地址通信
{
    class Program
    {
        static void Main(string[] args)
        {
            #region 与通信服务建立tcp连接
            CommTcp commTcp = new CommTcp("127.0.0.1", 2323);//1.连接服务exe
            try
            {
                commTcp.Connect();
            }
            catch (XNetException ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }
            #endregion

            #region 配置通信参数
            XNetConnectParams XNetParams = new XNetConnectParams("192.168.6.6", DeviceType.XDPLC);//2.指定设备地址

            XNet xnet = new XNet(XNetParams, commTcp);//3.构造信捷协议栈

            #endregion

            #region 通信
            XNetComm hComm = new XNetComm(xnet);//4.构造通信句柄

            //读10个寄存器
            try
            {
                short[] readRegs = new short[10];//10个
                hComm.Read(PLCRegType.D, 1000, readRegs);//从D1000位置开始读10个寄存器，存入readRegs
            }
            catch (XNetException ex)
            {
                Console.WriteLine(ex.Message);//读取失败的错误信息
                return;
            }


            //写10个寄存器
            try
            {
                short[] writeRegs = new short[10];//10个
                hComm.Write(PLCRegType.D, 1000, writeRegs);//从D1000位置开始写10个寄存器，把writeRegs写入
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
