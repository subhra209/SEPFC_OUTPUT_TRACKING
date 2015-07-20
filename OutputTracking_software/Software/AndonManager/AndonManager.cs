using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Timers;
using System.Configuration;
using System.ComponentModel;
using System.IO.Ports;
using System.Collections;
using System.Diagnostics;

using ias.devicedriver;

namespace ias.andonmanager
{
    
    public partial class AndonManager
    {
        public event EventHandler<AndonAlertEventArgs> andonAlertEvent;   //the handler for alerts

        public enum MODE { NONE = 0, MASTER = 1, SLAVE = 2 };
        
        SerialPortDriver spDriver = null;               //the serial port driver
        RS485Driver rs485Driver = null;                 //the rs485 driver

        XbeeDriver xbeeDriver = null;

        String communicationPort = String.Empty;
        public String CommunicationPort
        {
            get { return communicationPort; }
        }

        struct LineResponse
        {
            public int id;
            public DateTime timeStamp;  //response time stamp
            

            public List<Byte> data ;
 
            public LineResponse( DateTime tstp, int id , List<Byte> data)
            {
                timeStamp = tstp;

                this.id = id;
                this.data = data;
            }
        }

        List<Byte> txPacket = null;
        List<Byte> rxPacket = null;
        List<Byte> partialPacket = null;
        Queue<Byte> rxDataQ = null;


        System.Timers.Timer transactionTimer = null;
        System.Timers.Timer simulationTimer = null;
        int responseTimeout = 50; //response timeout in milliseconds

        Queue<int> stations = null;
        Queue<int> departments = null;

        String simulation = String.Empty;//simulation control

        private Queue<TransactionInfo> transactionQ = null;

        Byte RESP_SOF = 0xAA;    //start of FRAME
        Byte RESP_EOF = 0xBB;    //end of FRAME

        Byte CMD_SOF = 0xAA;    //start of FRAME
        Byte CMD_EOF = 0xBB;    //end of FRAME

        MODE mode = MODE.NONE;

        int retries = 0;
        int NO_OF_RETRIES = 3;
        String xbeeIdentifier = String.Empty;
        TraceSource _andonTrace = null;
        TextWriterTraceListener _andonTraceListener = null;

        public AndonManager(Queue<int> stationList, Queue<int> departmentList , MODE mode)
        {
            try
            {
                responseTimeout = int.Parse(ConfigurationSettings.AppSettings["ResponseTimeout"]);
                this.mode = mode;

                transactionTimer = new System.Timers.Timer(responseTimeout);
                transactionTimer.Elapsed += new ElapsedEventHandler(transactionTimer_Elapsed);
                transactionTimer.AutoReset = false;

                simulation = ConfigurationSettings.AppSettings["SIMULATION"];

                if (simulation != "Yes")
                {
                    spDriver = new SerialPortDriver(19200,8,StopBits.One,Parity.None,Handshake.None);

                    communicationPort = ConfigurationSettings.AppSettings["PORT"];
            
                    
	                rs485Driver = new RS485Driver();
                    xbeeDriver = new XbeeDriver(XbeeDriver.COMMUNICATION_MODE.API_ESC);
                    xbeeIdentifier = ConfigurationSettings.AppSettings["XBeeIdentifier"];


                    //communicationPort = findXbeePort(xbeeIdentifier);   //find the port of xbee

                    //if (communicationPort == String.Empty)
                    //{
                    //    throw new AndonManagerException(" Error : Xbee Device Not Found ");
                    //}



	
	                stations = stationList;
	                departments = departmentList;
	
	                transactionQ = new Queue<TransactionInfo>();
                    
                    
                }
                else{
                	
                	simulationTimer = new System.Timers.Timer(2 * 1000);
                	simulationTimer.Elapsed += new ElapsedEventHandler(simulationTimer_Elapsed);
                    simulationTimer.AutoReset = false;
                	
                }

                _andonTrace = new TraceSource("andonTrace");
                _andonTrace.Switch = new SourceSwitch("andonTraceSwitch");


                String xbeeTraceFile = ConfigurationSettings.AppSettings["AndonTraceFile"];

                if (xbeeTraceFile != String.Empty)
                {
                    _andonTraceListener = new TextWriterTraceListener(xbeeTraceFile);
                    _andonTrace.Listeners.Add(_andonTraceListener);
                    _andonTrace.Switch.Level = SourceLevels.Information;
                }
                else
                {
                    _andonTrace.Switch.Level = SourceLevels.Off;
                }



            }
            catch (Exception e)
            {
                throw new AndonManagerException("Andon Manager Initialization Error:"+e.Message);
            }
        }

        public AndonManager( MODE mode)
        {
            try
            {
                responseTimeout = int.Parse(ConfigurationSettings.AppSettings["ResponseTimeout"]);
                this.mode = mode;

                transactionTimer = new System.Timers.Timer(responseTimeout);
                transactionTimer.Elapsed += new ElapsedEventHandler(transactionTimer_Elapsed);
                transactionTimer.AutoReset = false;

                simulation = ConfigurationSettings.AppSettings["SIMULATION"];

                if (simulation != "Yes")
                {
                    spDriver = new SerialPortDriver(19200, 8, StopBits.One, Parity.None, Handshake.None);

                    communicationPort = ConfigurationSettings.AppSettings["PORT"];


                    rs485Driver = new RS485Driver();
                    xbeeDriver = new XbeeDriver(XbeeDriver.COMMUNICATION_MODE.API_ESC);
                    xbeeIdentifier = ConfigurationSettings.AppSettings["XBeeIdentifier"];


                    //communicationPort = findXbeePort(xbeeIdentifier);   //find the port of xbee

                    //if (communicationPort == String.Empty)
                    //{
                    //    throw new AndonManagerException(" Error : Xbee Device Not Found ");
                    //}




                    //stations = stationList;
                    //departments = departmentList;

                    transactionQ = new Queue<TransactionInfo>();


                }
                else
                {

                    simulationTimer = new System.Timers.Timer(2 * 1000);
                    simulationTimer.Elapsed += new ElapsedEventHandler(simulationTimer_Elapsed);
                    simulationTimer.AutoReset = false;

                }

                _andonTrace = new TraceSource("andonTrace");
                _andonTrace.Switch = new SourceSwitch("andonTraceSwitch");


                String xbeeTraceFile = ConfigurationSettings.AppSettings["AndonTraceFile"];

                if (xbeeTraceFile != String.Empty)
                {
                    _andonTraceListener = new TextWriterTraceListener(xbeeTraceFile);
                    _andonTrace.Listeners.Add(_andonTraceListener);
                    _andonTrace.Switch.Level = SourceLevels.Information;
                }
                else
                {
                    _andonTrace.Switch.Level = SourceLevels.Off;
                }

            }
            catch (Exception e)
            {
                throw new AndonManagerException("Andon Manager Initialization Error:" + e.Message);
            }
        }


        void simulationTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
        	simulationTimer.Stop();
        	List<int> departments = new List<int>();
        	departments.Add(1);
        	departments.Add(2);
            //departments.Add(0);
        	
        	
        	
        	//AndonAlertEventArgs alertEvent
               // = new AndonAlertEventArgs(DateTime.Now,1,
        		//                          createLog(departments));

            if (andonAlertEvent != null)
            {
                //andonAlertEvent(this, alertEvent);
            }
        	
        }

        public void start()
        {
            try
            {
            	if( simulation == "Yes")
            	{
            		simulationTimer.Start();
            		return;
            		
            	}
                spDriver.open(communicationPort);
                int i = 3;
                do
                {
                    if (spDriver.IsOpen == false)
                    {
                        spDriver.Close();
                        Thread.Sleep(500);
                    }
                } while (--i > 0);

                if (spDriver.IsOpen == false)
                throw new Exception("unable to open serial port");

                

                rxPacket = new List<byte>();
                partialPacket = new List<byte>();
                rxDataQ = new Queue<byte>();


                transactionTimer.Start();

            }
            catch (Exception e)
            {
                spDriver = null;
                throw new AndonManagerException("Unable to start Andon Manager:" + e.Message);
            }
        }


        public void stop()
        {
			    if( simulation == "Yes")
            	{
			    	simulationTimer.Stop();
            		return;
            		
            	}
            rxPacket = null;

            if (spDriver != null)
            {
                spDriver.abort = true;
                Thread.Sleep(10);
            }

            if( transactionTimer.Enabled )
            transactionTimer.Stop();
            

            Thread.Sleep(100);
            
        }

        void transactionTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            List<Byte> packet = null;
            int startIndex= -1;
            int endIndex= -1;

            transactionTimer.Stop();

            if (!spDriver.IsOpen)
            {
                throw new AndonManagerException("Serial Port Closed");

            }

            int bytesReceived = spDriver.BytesToRead;

            if (bytesReceived > 0)  //check whether bytes have been received
            {
                Byte[] tempBuff = new Byte[spDriver.BytesToRead];


                spDriver.Read(tempBuff, 0, bytesReceived); //copy the received bytes into temp buffer

                for (int i = 0; i < bytesReceived; i++)   //copy from temp buffer to packet
                {
                    rxPacket.Add(tempBuff[i]);
                }
                List<Byte> parsedRxPacket = rxPacket;
                //List<Byte>parsedRxPacket = xbeeDriver.parseRxPacket(rxPacket);
                if( parsedRxPacket == null )
                {
                    retries++;
                    if (retries >= NO_OF_RETRIES)
                    {
                        retries = 0;
                        rxPacket.Clear();
                        startTransaction();
                        return;
                    }
                    transactionTimer.Start();
                    return;
                }
                retries = 0;
                parsedRxPacket.InsertRange(0,partialPacket.GetRange(0,partialPacket.Count));
                partialPacket.Clear();

                //while (parsedRxPacket.Contains(RESP_SOF) && parsedRxPacket.Contains(RESP_EOF))     // if the packet contains start of frame
                while (parsedRxPacket.Contains(CMD_SOF) && parsedRxPacket.Contains(CMD_EOF))     // if the packet contains start of frame
                {

                    startIndex = parsedRxPacket.FindIndex(findRespSof);

                    endIndex = parsedRxPacket.FindIndex(findRespEof);

                    if (startIndex > endIndex)
                    {
                        parsedRxPacket.RemoveRange(0, startIndex);
                        continue;
                    }

                    packet = new List<byte>();
                    packet.AddRange(parsedRxPacket.GetRange(startIndex, (endIndex - startIndex) + 1));

                    processResponse(packet);      //process it

                    parsedRxPacket.RemoveRange(0, endIndex + 1);

                }
                partialPacket.AddRange( parsedRxPacket.GetRange(0 , parsedRxPacket.Count));
            }

            startTransaction();

            return;
        }

        private bool findRespSof(Byte b)
        {
            if (b == (Byte)RESP_SOF)
                return true;
            else return false;
        }
        private bool findRespEof(Byte b)
        {
            if (b == (Byte)RESP_EOF)
                return true;
            else return false;
        }

        void startTransaction()
        {
            List<Byte> rs485packet = null;
            List<Byte> txPacket = null;

            if( transactionQ.Count > 0)
            {
                TransactionInfo tInfo = transactionQ.Dequeue();


                rs485packet = rs485Driver.Packetize((byte)tInfo.deviceId,(byte) tInfo.command, tInfo.data);
                 
                if (rs485packet != null)
                {
                    //txPacket = xbeeDriver.getTxPacket(tInfo.deviceId, rs485packet);
                    //byte[] txBuffer = txPacket.ToArray();
                    byte[] txBuffer = rs485packet.ToArray();
                    spDriver.Write(txBuffer, 0, rs485packet.Count);
                    //spDriver.WriteToPort(txBuffer);

                    #region TRACE_CODE
                    String traceString = DateTime.Now.ToString();
                    foreach (Byte b in txBuffer)
                    {
                        traceString += "0x" + b.ToString("x2") + " ";
                    }
                    traceString += Environment.NewLine;

                    _andonTrace.TraceInformation(traceString);
                    foreach (TraceListener l in _andonTrace.Listeners)
                    {
                        l.Flush();
                    }

                    #endregion


                }

                

            }
            transactionTimer.Start();
          
        }

        void processResponse(List<Byte> packet)
        {

            if( (packet == null) || (packet.Count <= 0 ))
            return;
            Byte status = 0xFF;
            byte deviceId = 0xFF;
            List<Byte> responseData;

            rs485Driver.Parse(packet, out status, out deviceId, out responseData);


                
            LineResponse lineReponse = new LineResponse();

            lineReponse.data = responseData;
            lineReponse.id = deviceId;
            lineReponse.timeStamp = DateTime.Now;
            updateStationStatus(lineReponse);

        }

        void updateStationStatus(LineResponse lineResponse)
        {
            try
            {
                if (lineResponse.data == null || (lineResponse.data.Count == 0))
                {
                    return;
                }

               // List<LogEntry> log = parseResponse(lineResponse.data);


                AndonAlertEventArgs alertEvent
                    = new AndonAlertEventArgs(lineResponse.timeStamp, lineResponse.id, lineResponse.data);

                if (andonAlertEvent != null)
                {
                    andonAlertEvent(this, alertEvent);
                }
                        
                

                        
            }
            catch( Exception te)
            {
                    throw te;
            }

        }

        List<LogEntry> parseResponse(List<Byte> responseData)
        {
            List<LogEntry> log = new List<LogEntry>();

            LogEntry lgEntry = new LogEntry();

            Byte[] tempBuff = {(Byte)( responseData[1] - '0') , (Byte)(responseData[0] - '0')};
            lgEntry.Station = tempBuff[1]*10 + tempBuff[0];

            lgEntry.Department = responseData[2] - '0';

            responseData.RemoveRange(0,3);

           lgEntry.Data = System.Text.Encoding.UTF8.GetString(responseData.ToArray());

           log.Add(lgEntry);       

            return log;


        }

        public void addTransaction(int deviceId, AndonCommand command, List<Byte> data)
        {
            transactionQ.Enqueue(new TransactionInfo(deviceId, command, data));
        }
        
        
        List<LogEntry> createLog(List<int> departments )
        {
        	List<LogEntry> log = new List<LogEntry>();
        	
        	foreach( int i in departments )
        	{
        		log.Add( new LogEntry(1 , i , "5" ));
        	}
        	return log;
        	
        }



        //private String findXbeePort(String identifier)
        //{

        //    txPacket = xbeeDriver.getNodeIdentifierPacket(); //create the packet to find the identifier

        //    String[] portNames = SerialPort.GetPortNames(); //get all the available serial ports
        //    String communicationPort = String.Empty;

        //    foreach (String port in portNames)      //search each of the ports
        //    {
        //        if (communicationPort == String.Empty)  //if no port has been found
        //        {

        //            try
        //            {
        //                spDriver = new SerialPortDriver(); //create the serial port driver

        //                if (spDriver.open(port) == false)
        //                {
        //                    continue;
        //                }

        //                spDriver.WriteToPort(txPacket.ToArray());
        //                transactionTimer.Start();
        //                transactionEvent.WaitOne();

        //                if (rxPacket == null)
        //                {
        //                    spDriver.Close();

        //                    continue;
        //                }
        //                List<Byte> nodeIdBytes =
        //                            xbeeDriver.parseATresponse(rxPacket, XbeeDriver.AT_COMMANDS[(int)XbeeDriver.AT_COMMAND_INDEX.NI]);

        //                char[] nodeIdArr = new char[nodeIdBytes.Count];
        //                for (int i = 0; i < nodeIdBytes.Count; i++)
        //                {
        //                    nodeIdArr[i] = (char)nodeIdBytes[i];
        //                }

        //                String id = new String(nodeIdArr);

        //                if (id == identifier)
        //                {
        //                    communicationPort = port;
        //                }

        //                spDriver.Close();
        //                spDriver.Dispose();

        //            }
        //            catch (System.TimeoutException)
        //            {
        //                spDriver.Close();
        //                spDriver.Dispose();

        //                continue;
        //            }
        //            catch (Xbee_Exception x)
        //            {
        //                spDriver.Close();
        //                spDriver.Dispose();
        //                continue;
        //            }
        //        }
        //    }
        //    return communicationPort;
        //}


        
      
    }
    

    public class AndonManagerException : Exception
    {
        public String message = String.Empty;
        public AndonManagerException(String msg )
        {
            message = msg;
        }
    }

    public class AndonAlertEventArgs : EventArgs
    {
        DateTime eventTstp;
        public DateTime EventTimeStamp
        {
            get { return eventTstp; }
        }

        int stationId = 0;
        public int StationId
        {
            get { return stationId; }
            set { stationId = value; }
        }
        List<byte> stationLog = null;
        public List<byte> StationLog
        {
            get { return stationLog; }
        }


        public AndonAlertEventArgs(DateTime eventTstp, int stationId,List<byte> stnLog)
        {
            this.eventTstp = eventTstp;
            
            this.stationId = stationId;
            this.stationLog = stnLog;
                       

        }

        public void addLogEntry(List<byte> logEntry)
        {

            stationLog = logEntry;
            

        }

    }





    
}
