using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.IO;
using System.Timers;
using System.Reflection;
using MySql.Data.MySqlClient;
using System.Runtime.InteropServices;

namespace WindowsService_test
{
    public partial class Service1 : ServiceBase
    {
        readonly Timer _timer;

        readonly Timer _SyncTimer;


        private static readonly string FileName = Path.GetDirectoryName ( Assembly.GetExecutingAssembly ( ).Location ) + @"\" + "test.txt";

        [DllImport("MFCLibrary1.dll", EntryPoint = "test1", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool test1();

        [DllImport("MFCLibrary1.dll", EntryPoint = "GetValue", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetValue(int ramid, int pos);

        public Service1 ( )
        {
            InitializeComponent ( );

            _timer = new Timer ( 200 )
            {
                AutoReset = true ,
                Enabled = true
            };

            _SyncTimer = new Timer(300)
            {
                AutoReset = true,
                Enabled = true
            };

            //_timer.Elapsed += saveData;
            //_SyncTimer.Elapsed += SyncData;

        }

        protected override void OnStart ( string [ ] args )
        {
            _timer.Elapsed += saveData;
            _SyncTimer.Elapsed += SyncData;

            this.witre ( string.Format ( "Start DateTime {0}" , DateTime.Now ) );
        }

        protected override void OnStop ( )
        {
            _timer.Elapsed -= saveData;
            _SyncTimer.Elapsed -= SyncData;

            this.witre ( string.Format ( "Stop DateTime {0}" , DateTime.Now ) + Environment.NewLine );
        }

        void witre ( string context )
        {
            StreamWriter sw = File.AppendText ( FileName );
            sw.WriteLine ( context );
            sw.Flush ( );
            sw.Close ( );
        }

        public void saveData(object sender, ElapsedEventArgs e)
        {
            String connetStr = "server=cd-cdb-qqazzp58.sql.tencentcdb.com;port=62674;user=root;password=xfgdgfA2019@; database =ctdb;";
            // server=127.0.0.1/localhost 代表本机，端口号port默认是3306可以不写
            MySqlConnection conn = new MySqlConnection(connetStr);
            try
            {
                conn.Open();//打开通道，建立连接，可能出现异常,使用try catch语句
                Console.WriteLine("已经建立连接");
                DateTime dateTime = DateTime.Now;
                while (true)
                {
                    DateTime dateTimenow = DateTime.Now;
                    TimeSpan ts1 = new TimeSpan(dateTime.Ticks);
                    TimeSpan ts2 = new TimeSpan(dateTimenow.Ticks);
                    TimeSpan ts3 = ts1.Subtract(ts2).Duration();


                    if (1000 <= ts3.TotalMilliseconds)
                    {
                        for (int i = 0; i < 24; i++)
                        {
                            int c = GetValue(10, i);
                            string sql = string.Format("update  service_telesignal_data set data = {0} where id = {1}", c, i);
                            MySqlCommand cmd1 = new MySqlCommand(sql, conn);
                            int result1 = cmd1.ExecuteNonQuery();
                        }

                        for (int i = 0; i < 41; i++)
                        {
                            int offset = 500 + i;
                            int c = GetValue(10, offset);
                            string sql = string.Format("update  service_telemetry_data set data = {0} where id = {1}", c, i);
                            MySqlCommand cmd2 = new MySqlCommand(sql, conn);
                            int result2 = cmd2.ExecuteNonQuery();
                        }
                        dateTime = DateTime.Now;
                    }

                }
            }
            catch (MySqlException ex)
            {
                switch (ex.Number)
                {
                    case 0:
                        Console.WriteLine("Cannot connect to server.  Contact administrator");
                        break;
                    case 1045:
                        Console.WriteLine("Invalid username/password, please try again");
                        break;
                }
            }
            finally
            {
                conn.Close();
            }


            this.witre(string.Format("Run DateTime {0}", DateTime.Now));
        }



        public void SyncData(object sender, ElapsedEventArgs e) 
        {
            try
            {
                Process proc = new Process();
                string targetDir = string.Format(@"D:\bat\");

                proc.StartInfo.WorkingDirectory = targetDir;
                proc.StartInfo.FileName = "servicerestart.bat";
                proc.StartInfo.Arguments = string.Format("10");
                proc.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                proc.Start();

                proc.WaitForExit();

                
            }
            catch (Exception )
            {

            }

        }

    }
}
