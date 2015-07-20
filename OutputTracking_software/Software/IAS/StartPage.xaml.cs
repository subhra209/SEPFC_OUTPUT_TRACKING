using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Configuration;
using System.Data;
using System.Data.Sql;
using System.Data.SqlClient;
using ias.andonmanager;
using System.Threading;
using System.Windows.Threading;

namespace IAS
{
    /// <summary>
    /// Interaction logic for startPage.xaml
    /// </summary>
    /// 
    enum CMD { SYNCHRONIZE=0x75,GET_CYCLE_TIME = 0x76, SET_REFERENCE = 0x77,SET_OPERATORS = 0x72,
                SET_REFERENCE_CODE = 0x71};
    public partial class StartPage : Window
    {
        AndonManager andonManager = null;
        String _dbConnectionString = String.Empty;
        DataAccess dataAccess = null;
        Queue<int> lineQ = null;
        Queue<int> departmentQ = null;
        DataTable departmentTable = null;
        lineCollection lines = null;
        ShiftCollection shifts = null;
        ContactCollection contacts = null;

        Dictionary<int, Issue> Issues = null;
        List<double> timeout = null;

        System.Timers.Timer commandTimer = null;
        String dataSeperator = ";";
        String timingsDataSeperator = ":";

        Queue<TimeSpan> hourlyUpdateTimings;
        Queue<TimeSpan> shiftUpdateTimings;

        DateTime? nextHourlyUpdateTiming;
        DateTime? nextShiftUpdateTiming;
        Reference reference;

        String[] LINES = { "HDY", "SDY" };
        public enum DeviceCommand
        {
            SET_PLANNED_QUANTITY = 1, SET_SCHEDULE = 2, SET_SHIFT_TIMINGS = 3,
            SET_BREAK_TIMINGS = 4, SET_RTC = 5, SET_QA_PERIOD = 6, SET_LINE_NAME = 7,
            SET_LINE_MODEL = 8, UPDATE_LINE_DATA = 9
        };

        public StartPage()
        {
            try
            {
                InitializeComponent();
                _dbConnectionString  = ConfigurationSettings.AppSettings["DBConStr"];

                DataAccess.conStr = _dbConnectionString;

                MainMenu _mainMenu = new MainMenu(_dbConnectionString);
                _mainFrame.Navigate(_mainMenu);
                dataAccess = new DataAccess();
                lines = dataAccess.getLines();
                lineQ = new Queue<int>();

                foreach (line l in lines)
                    lineQ.Enqueue(l.ID);

                reference = new Reference();

                andonManager = new AndonManager(AndonManager.MODE.SLAVE);                        
                
                andonManager.andonAlertEvent += new EventHandler<AndonAlertEventArgs>(andonManager_andonAlertEvent);

                andonManager.start();

            }

            catch( Exception e)
            {
                tbMsg.Text+= e.Message;
            }
        }

        

      

        void andonManager_andonAlertEvent(object sender, AndonAlertEventArgs e)
        {
            int recordId = -1;
            int cmd;
            int line;
            try
            {
                cmd = e.StationLog[0];
                
                e.StationLog.RemoveAt(0);
                switch (cmd)
                {

                    case (int) CMD.SYNCHRONIZE:
                        List<Byte> rtcData = new List<byte>();
                        DateTime now = DateTime.Now;
                        rtcData.Add(intToBCD(now.Second));
                        rtcData.Add(intToBCD(now.Minute));
                        rtcData.Add(intToBCD(now.Hour));

                        rtcData.Add((byte)(now.DayOfWeek+1));
                        rtcData.Add(intToBCD(now.Day));
                        rtcData.Add(intToBCD(now.Month));
                        rtcData.Add(intToBCD(now.Year -2000));
                        andonManager.addTransaction(01, AndonCommand.CMD_SET_RTC, rtcData);
                        break;


                    case (int) CMD.SET_REFERENCE:
                        line = e.StationLog[0];
                        e.StationLog.RemoveAt(0);
                        char[] refe = new char[e.StationLog.Count-1];
                        for(int i = 0; i< e.StationLog.Count-1; i++)
                            refe[i] = (char)e.StationLog[i];

                        reference.Code = new String(refe);

                        updateMsg("SET REFERENCE for line:" + LINES[line - 2]+"-"+reference.Code);

                        break;


                    case (int)CMD.SET_REFERENCE_CODE:
                        line = e.StationLog[0];
                        e.StationLog.RemoveAt(0);
                        char[] code = new char[e.StationLog.Count - 1];
                        for (int i = 0; i < e.StationLog.Count - 1; i++)
                            code[i] = (char)e.StationLog[i];

                        reference.Code = new String(code);

                        reference = dataAccess.getReference(line, reference);
                        if (reference.Name == String.Empty)
                        {
                            updateMsg("Invalid Code for:" + LINES[line - 2] + "-" + reference.Code);
                        }
                        else
                        {
                            updateMsg("Setting Reference for line:" + LINES[line - 2] + "-" + reference.Name);
                            var data = new List<Byte>();
                            data.Add((Byte)line);
                            data.AddRange(new List<Byte>(Encoding.ASCII.GetBytes(reference.Name)));
                            andonManager.addTransaction(01, AndonCommand.CMD_SET_REFERENCE, data);
                        }
                        

                        break;






                    case (int) CMD.SET_OPERATORS:
                        //line = e.StationId;
                        //int operators = e.StationLog[0];
                        //updateMsg("SET CYCLE TIME for line:" + LINES[line - 2]+"-"+operators.ToString());
                        break;

                    case (int)CMD.GET_CYCLE_TIME:
                        line = e.StationId;
                        reference = dataAccess.getCycleTime(line,  reference);
                        updateMsg("Setting Cycle Time for line:" + LINES[line - 2]);
                        if (reference.CycleTime != 0 && reference.BottleNeckTime != 0)
                        {
                            byte[] ct = BitConverter.GetBytes((short)reference.CycleTime);
                            byte[] bt = BitConverter.GetBytes((short)reference.BottleNeckTime);

                            var data = new List<Byte>();
                            data.Add((byte)line);
                            data.AddRange(ct.ToList());
                            data.AddRange(bt.ToList());

                            andonManager.addTransaction(01, AndonCommand.CMD_SET_CYCLE_TIME, data);
                        }

                        break;
                }
                        
               
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message + Environment.NewLine + ex.StackTrace, "Error", MessageBoxButton.OK);
            }

        }



        private Byte intToBCD(int data)
        {
            byte msb = (byte)(data / 10);
            byte lsb = (byte)(data % 10);

            byte bcd = (byte)((msb << 4) | lsb);
            return bcd;
        }
       

        private String[] separateCommandData(String cmdData)
        {

            return
                cmdData.Split(dataSeperator.ToCharArray());
        }



        void updateMsg(String msg)
        {
            tbMsg.Dispatcher.BeginInvoke(DispatcherPriority.Background,
         new Action(() =>
         {
             tbMsg.Text += msg + Environment.NewLine;
         }));
        }


        TimeSpan stringtoTS(String time)
        {
            String sep = ":";
            String[] details = time.Split(sep.ToCharArray());
            int hours = Convert.ToInt32(details[0]);
            int minutes = Convert.ToInt32(details[1]);
            int seconds = Convert.ToInt32(details[2]);

            return new TimeSpan(hours, minutes, seconds);
        }


        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            commandTimer.Stop();
            commandTimer.Close();
            commandTimer.Dispose();

            if (andonManager != null)
                andonManager.stop();

        }
    }



    public class Availability
    {
        public int Line{get;set;}
        public List<IssueDetails> issues;

        public Availability()
        {
            issues = new List<IssueDetails>();
        }

        public void Add(IssueDetails issue)
        {
            if (issues.Count == 0)
            {
                issues.Add(issue);
                return;
            }
           foreach( IssueDetails id in issues)
           {

               if ((issue.Raised < id.Raised) && (issue.Resolved <= id.Resolved)) // overlap case 1
               {
                   issue.Resolved = id.Raised;
                   issues.Add(issue);
                   break;
               }
               else if ((issue.Raised >= id.Raised) && (issue.Resolved > id.Resolved)) // overlap case 2
               {
                   issue.Raised = id.Resolved;
                   issues.Add(issue);
                   break;
               }
               else if ((issue.Raised >= id.Raised) && (issue.Raised <= id.Resolved)) //overlap case 3
               {
                   continue;
               }
               else
               {
                   issues.Add(issue);
                   break ;
               }
           }
           
        }

        public int getAvailability(TimeSpan from , TimeSpan to)
        {
            int availability = 0;
            foreach (IssueDetails i in issues)
            {
                TimeSpan downtime = i.Resolved - i.Raised;
                availability += downtime.Hours * 60 * 60 + downtime.Minutes * 60 + downtime.Seconds;
            }

            int totalavailability = (to - from).Hours *60*60 + (to - from).Minutes*60 + (to - from).Seconds;

             
            int availabilityPercentage = ((totalavailability - availability) * 100/ totalavailability);

            if (availabilityPercentage < 0) availabilityPercentage = 0;

            return availabilityPercentage;
        }

    }



    public class IssueDetails
    {
        public int Line { get; set; }
        public int Station { get; set; }
        public int Tolerance { get; set; }

        public TimeSpan Raised { get; set; }
        public TimeSpan Resolved { get; set; }

        public IssueDetails()
        {
        }
    }
}
