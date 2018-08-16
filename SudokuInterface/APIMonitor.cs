using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace SudokuInterface
{
    public enum RequestType { Solve, IsSolved, GetSolved, GetCurrent, Reset, None, Shutdown, GetBoardData, Initialize };

    public class Request
    {
        public RequestType myRequestType;
        public object myRequester;
        byte[] myByteArrayData;
        int[] myIntArrayData;
        bool myBoolData;
        int myIntData;
        long myLongData;
        int myQueueSize;

        public Request (object theRequester, RequestType theRequestType)
        {
            myRequester = theRequester;
            myRequestType = theRequestType;
            myByteArrayData = new byte[SudokuLibApi.RESPONSE_SIZE];
            myIntArrayData = new int[SudokuLibApi.RESPONSE_SIZE];
            myBoolData = false;
            myLongData = 0;
            myIntData = 0;
            myQueueSize = 0;
        }

        public byte[] GetByteData()
        {
            return myByteArrayData;
        }

        public int[] GetIntArrayData()
        {
            return myIntArrayData;
        }

        public bool GetBoolData()
        {
            return myBoolData;
        }

        public void SetData(byte[] theData)
        {
            myByteArrayData = theData;
        }

        public void SetData(int[] theData)
        {
            myIntArrayData = theData;
        }

        public void SetData(bool theData)
        {
            myBoolData = theData;
        }

        public void SetLongData(long theData)
        {
            myLongData = theData;
        }

        public long GetLongData()
        {
            return myLongData;
        }

        public int GetQueueSize()
        {
            return myQueueSize;
        }

        public void SetQueueSize(int theQueueSize)
        {
            myQueueSize = theQueueSize;
        }

        public void SetIntData(int theData)
        {
            myIntData = theData;
        }

        public int GetIntData()
        {
            return myIntData;
        }
    }

    // The APIMonitor is an example of the Singleton pattern in C#
    public class APIMonitor
    {
        private static volatile APIMonitor instance;
        private static object syncRoot = new Object();
        private static ConcurrentQueue<Request> myRequests = new ConcurrentQueue<Request>();
        Request myExecutingRequest;

        private APIMonitor() { }

        public static APIMonitor Instance
        {
            get
            {
                if (instance == null)
                {
                    lock (syncRoot)
                    {
                        if (instance == null)
                            instance = new APIMonitor();
                    }
                }

                return instance;
            }
        }

        private void Initialize()
        {
        }

        public void Run()
        {
            if (myRequests.Count > 0)
            {
                ProcessRequests();
            }
        }

        private void ProcessRequests()
        {
            if (myExecutingRequest == null)
            {
                if (myRequests.Count > 0)
                {
                    Request req = myRequests.First();
                    myExecutingRequest = req;

                    if (myRequests.TryDequeue(out req))
                    {
                        if (req.myRequestType == RequestType.Initialize)
                        {
                            try
                            {
                                SudokuLibApi.Initialize(req.GetByteData());
                            }
                            catch (Exception e)
                            {
                                ErrorMessage em = new ErrorMessage(e.Message);
                                em.ShowDialog();
                            }
                        }
                        if (req.myRequestType == RequestType.Solve)
                        {
                            SudokuLibApi.Solve();
                        }
                        else if (req.myRequestType == RequestType.Reset)
                        {
                            SudokuLibApi.Reset();
                        }
                        else if (req.myRequestType == RequestType.IsSolved)
                        {
                            bool theResult = SudokuLibApi.GetIsSolved();
                            req.SetData(theResult);
                            req.SetQueueSize(myRequests.Count);


                            SolverFSM m = (SolverFSM)req.myRequester;
                            if (m != null)
                            {
                                m.RequestResponse(req);
                            }
                        }
                        else if (req.myRequestType == RequestType.GetSolved)
                        {
                            IntPtr theCurrentBoard = SudokuLibApi.GetCurrentBoard();
                            byte[] bytes = new byte[SudokuLibApi.RESPONSE_SIZE];
                            Marshal.Copy(theCurrentBoard, bytes, 0, SudokuLibApi.RESPONSE_SIZE);
                            req.SetData(bytes);
                            req.SetQueueSize(myRequests.Count);

                            SolverFSM m = (SolverFSM)req.myRequester;
                            if (m != null)
                            {
                                m.RequestResponse(req);
                            }
                        }   
                        else if (req.myRequestType == RequestType.Shutdown)
                        {
                            bool theResult = SudokuLibApi.Shutdown();
                            req.SetData(theResult);
                            req.SetQueueSize(myRequests.Count);


                            SolverFSM m = (SolverFSM)req.myRequester;
                            if (m != null)
                            {
                                m.RequestResponse(req);
                            }
                        }
                        else if (req.myRequestType == RequestType.GetBoardData)
                        {
                            IntPtr theBoardData = SudokuLibApi.GetBoardData();
                            int[] bytes = new int[SudokuLibApi.RESPONSE_SIZE];
                            Marshal.Copy(theBoardData, bytes, 0, SudokuLibApi.RESPONSE_SIZE);
                            req.SetData(bytes);
                            req.SetQueueSize(myRequests.Count);

                            SolverFSM m = (SolverFSM)req.myRequester;
                            if (m != null)
                            {
                                m.RequestResponse(req);
                            }
                        }
                    }

                    myExecutingRequest = null;
                }
            }
        }

        public void NewRequest(Request theReq)
        {
            try
            {
                if (theReq != null)
                {
                    try
                    {
                        myRequests.Enqueue(theReq);
                    }
                    catch (Exception f)
                    {
                        ErrorMessage em = new ErrorMessage(f.Message);
                        em.ShowDialog();
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}
