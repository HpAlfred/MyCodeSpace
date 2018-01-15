using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using XNetSocketClient;//

namespace 按类型自动查找通信
{
    class Program
    {
        static void Main(string[] args)
        {
            string err;
            #region 查找设备
            XNet xnet = XNet.FindLocalDevice("127.0.0.1", 2323, DeviceType.XDPLC, out err);//对电脑的所有（USB/串口）端口进行查找XDPLC
            if (xnet == null)
            {
                return;//查找失败
            }
            #endregion

            #region 通信
            XNetComm hComm = new XNetComm(xnet);//4.构造通信句柄

            //读10个寄存器
            short[] readRegs = new short[10];//10个
            try
            {
                hComm.Read(PLCRegType.D, 0, readRegs);//从D1000位置开始读10个寄存器，存入readRegs
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
