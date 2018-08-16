using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Forms;


namespace SudokuInterface
{
    public partial class Main : Form
    {
        public TextBox[,] myInputBoxes;
        TextBox[,] myResultsDisplay;

        SolverFSM mySolverFSM;

        Thread myAPIServiceThread;
        Thread myFSMThread;

        public Main()
        {
            InitializeComponent();

            try
            {
                GuidanceManager.Initialize();

                numGuesses.Text = "0";
                lowestScoreLabel.Text = "0";
                surrenderCountLabel.Text = "0";
                invalidCountLabel.Text = "0";

                //enableGuessingButton.Enabled = false;
                //disableGuessing.Enabled = false;

                mySolverFSM = new SolverFSM(this);

                // Start the threads
                // This thread communicates with the solver engine in the DLL
                myAPIServiceThread = new Thread(() => StartAPIServices());
                myAPIServiceThread.IsBackground = true;
                myAPIServiceThread.Start();

                // This thread runs the display FSM by getting results from the server engine.
                myFSMThread = new Thread(() => mySolverFSM.Run());
                myFSMThread.IsBackground = true;
                myFSMThread.Start();

                o_block_1_1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                o_block_1_2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                o_block_1_3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                o_block_2_1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                o_block_2_2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                o_block_2_3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                o_block_3_1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                o_block_3_2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                o_block_3_3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;

                o_block_1_1.BackColor = System.Drawing.Color.Wheat;
                o_block_1_2.BackColor = System.Drawing.Color.Wheat;
                o_block_1_3.BackColor = System.Drawing.Color.Wheat;
                o_block_2_1.BackColor = System.Drawing.Color.Wheat;
                o_block_2_2.BackColor = System.Drawing.Color.Wheat;
                o_block_2_3.BackColor = System.Drawing.Color.Wheat;
                o_block_3_1.BackColor = System.Drawing.Color.Wheat;
                o_block_3_2.BackColor = System.Drawing.Color.Wheat;
                o_block_3_3.BackColor = System.Drawing.Color.Wheat;

                i_block_1_1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                i_block_1_2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                i_block_1_3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                i_block_2_1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                i_block_2_2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                i_block_2_3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                i_block_3_1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                i_block_3_2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
                i_block_3_3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;

                i_block_1_1.BackColor = System.Drawing.Color.Wheat;
                i_block_1_2.BackColor = System.Drawing.Color.Wheat;
                i_block_1_3.BackColor = System.Drawing.Color.Wheat;
                i_block_2_1.BackColor = System.Drawing.Color.Wheat;
                i_block_2_2.BackColor = System.Drawing.Color.Wheat;
                i_block_2_3.BackColor = System.Drawing.Color.Wheat;
                i_block_3_1.BackColor = System.Drawing.Color.Wheat;
                i_block_3_2.BackColor = System.Drawing.Color.Wheat;
                i_block_3_3.BackColor = System.Drawing.Color.Wheat;

                
            }
            catch (Exception e1)
            {
                ErrorMessage em = new ErrorMessage(e1.Message);
                em.ShowDialog();
            }

            try
            {
                myResultsDisplay = new TextBox[10, 10];
                myInputBoxes = new TextBox[10, 10];

                myInputBoxes[1, 1] = i_sqr_1_1;
                myInputBoxes[1, 2] = i_sqr_1_2;
                myInputBoxes[1, 3] = i_sqr_1_3;
                myInputBoxes[1, 4] = i_sqr_1_4;
                myInputBoxes[1, 5] = i_sqr_1_5;
                myInputBoxes[1, 6] = i_sqr_1_6;
                myInputBoxes[1, 7] = i_sqr_1_7;
                myInputBoxes[1, 8] = i_sqr_1_8;
                myInputBoxes[1, 9] = i_sqr_1_9;

                myInputBoxes[2, 1] = i_sqr_2_1;
                myInputBoxes[2, 2] = i_sqr_2_2;
                myInputBoxes[2, 3] = i_sqr_2_3;
                myInputBoxes[2, 4] = i_sqr_2_4;
                myInputBoxes[2, 5] = i_sqr_2_5;
                myInputBoxes[2, 6] = i_sqr_2_6;
                myInputBoxes[2, 7] = i_sqr_2_7;
                myInputBoxes[2, 8] = i_sqr_2_8;
                myInputBoxes[2, 9] = i_sqr_2_9;

                myInputBoxes[3, 1] = i_sqr_3_1;
                myInputBoxes[3, 2] = i_sqr_3_2;
                myInputBoxes[3, 3] = i_sqr_3_3;
                myInputBoxes[3, 4] = i_sqr_3_4;
                myInputBoxes[3, 5] = i_sqr_3_5;
                myInputBoxes[3, 6] = i_sqr_3_6;
                myInputBoxes[3, 7] = i_sqr_3_7;
                myInputBoxes[3, 8] = i_sqr_3_8;
                myInputBoxes[3, 9] = i_sqr_3_9;

                myInputBoxes[4, 1] = i_sqr_4_1;
                myInputBoxes[4, 2] = i_sqr_4_2;
                myInputBoxes[4, 3] = i_sqr_4_3;
                myInputBoxes[4, 4] = i_sqr_4_4;
                myInputBoxes[4, 5] = i_sqr_4_5;
                myInputBoxes[4, 6] = i_sqr_4_6;
                myInputBoxes[4, 7] = i_sqr_4_7;
                myInputBoxes[4, 8] = i_sqr_4_8;
                myInputBoxes[4, 9] = i_sqr_4_9;

                myInputBoxes[5, 1] = i_sqr_5_1;
                myInputBoxes[5, 2] = i_sqr_5_2;
                myInputBoxes[5, 3] = i_sqr_5_3;
                myInputBoxes[5, 4] = i_sqr_5_4;
                myInputBoxes[5, 5] = i_sqr_5_5;
                myInputBoxes[5, 6] = i_sqr_5_6;
                myInputBoxes[5, 7] = i_sqr_5_7;
                myInputBoxes[5, 8] = i_sqr_5_8;
                myInputBoxes[5, 9] = i_sqr_5_9;

                myInputBoxes[6, 1] = i_sqr_6_1;
                myInputBoxes[6, 2] = i_sqr_6_2;
                myInputBoxes[6, 3] = i_sqr_6_3;
                myInputBoxes[6, 4] = i_sqr_6_4;
                myInputBoxes[6, 5] = i_sqr_6_5;
                myInputBoxes[6, 6] = i_sqr_6_6;
                myInputBoxes[6, 7] = i_sqr_6_7;
                myInputBoxes[6, 8] = i_sqr_6_8;
                myInputBoxes[6, 9] = i_sqr_6_9;

                myInputBoxes[7, 1] = i_sqr_7_1;
                myInputBoxes[7, 2] = i_sqr_7_2;
                myInputBoxes[7, 3] = i_sqr_7_3;
                myInputBoxes[7, 4] = i_sqr_7_4;
                myInputBoxes[7, 5] = i_sqr_7_5;
                myInputBoxes[7, 6] = i_sqr_7_6;
                myInputBoxes[7, 7] = i_sqr_7_7;
                myInputBoxes[7, 8] = i_sqr_7_8;
                myInputBoxes[7, 9] = i_sqr_7_9;

                myInputBoxes[8, 1] = i_sqr_8_1;
                myInputBoxes[8, 2] = i_sqr_8_2;
                myInputBoxes[8, 3] = i_sqr_8_3;
                myInputBoxes[8, 4] = i_sqr_8_4;
                myInputBoxes[8, 5] = i_sqr_8_5;
                myInputBoxes[8, 6] = i_sqr_8_6;
                myInputBoxes[8, 7] = i_sqr_8_7;
                myInputBoxes[8, 8] = i_sqr_8_8;
                myInputBoxes[8, 9] = i_sqr_8_9;

                myInputBoxes[9, 1] = i_sqr_9_1;
                myInputBoxes[9, 2] = i_sqr_9_2;
                myInputBoxes[9, 3] = i_sqr_9_3;
                myInputBoxes[9, 4] = i_sqr_9_4;
                myInputBoxes[9, 5] = i_sqr_9_5;
                myInputBoxes[9, 6] = i_sqr_9_6;
                myInputBoxes[9, 7] = i_sqr_9_7;
                myInputBoxes[9, 8] = i_sqr_9_8;
                myInputBoxes[9, 9] = i_sqr_9_9;

                myResultsDisplay[1, 1] = o_sqr_1_1;
                myResultsDisplay[1, 2] = o_sqr_1_2;
                myResultsDisplay[1, 3] = o_sqr_1_3;
                myResultsDisplay[1, 4] = o_sqr_1_4;
                myResultsDisplay[1, 5] = o_sqr_1_5;
                myResultsDisplay[1, 6] = o_sqr_1_6;
                myResultsDisplay[1, 7] = o_sqr_1_7;
                myResultsDisplay[1, 8] = o_sqr_1_8;
                myResultsDisplay[1, 9] = o_sqr_1_9;

                myResultsDisplay[2, 1] = o_sqr_2_1;
                myResultsDisplay[2, 2] = o_sqr_2_2;
                myResultsDisplay[2, 3] = o_sqr_2_3;
                myResultsDisplay[2, 4] = o_sqr_2_4;
                myResultsDisplay[2, 5] = o_sqr_2_5;
                myResultsDisplay[2, 6] = o_sqr_2_6;
                myResultsDisplay[2, 7] = o_sqr_2_7;
                myResultsDisplay[2, 8] = o_sqr_2_8;
                myResultsDisplay[2, 9] = o_sqr_2_9;

                myResultsDisplay[3, 1] = o_sqr_3_1;
                myResultsDisplay[3, 2] = o_sqr_3_2;
                myResultsDisplay[3, 3] = o_sqr_3_3;
                myResultsDisplay[3, 4] = o_sqr_3_4;
                myResultsDisplay[3, 5] = o_sqr_3_5;
                myResultsDisplay[3, 6] = o_sqr_3_6;
                myResultsDisplay[3, 7] = o_sqr_3_7;
                myResultsDisplay[3, 8] = o_sqr_3_8;
                myResultsDisplay[3, 9] = o_sqr_3_9;

                myResultsDisplay[4, 1] = o_sqr_4_1;
                myResultsDisplay[4, 2] = o_sqr_4_2;
                myResultsDisplay[4, 3] = o_sqr_4_3;
                myResultsDisplay[4, 4] = o_sqr_4_4;
                myResultsDisplay[4, 5] = o_sqr_4_5;
                myResultsDisplay[4, 6] = o_sqr_4_6;
                myResultsDisplay[4, 7] = o_sqr_4_7;
                myResultsDisplay[4, 8] = o_sqr_4_8;
                myResultsDisplay[4, 9] = o_sqr_4_9;

                myResultsDisplay[5, 1] = o_sqr_5_1;
                myResultsDisplay[5, 2] = o_sqr_5_2;
                myResultsDisplay[5, 3] = o_sqr_5_3;
                myResultsDisplay[5, 4] = o_sqr_5_4;
                myResultsDisplay[5, 5] = o_sqr_5_5;
                myResultsDisplay[5, 6] = o_sqr_5_6;
                myResultsDisplay[5, 7] = o_sqr_5_7;
                myResultsDisplay[5, 8] = o_sqr_5_8;
                myResultsDisplay[5, 9] = o_sqr_5_9;

                myResultsDisplay[6, 1] = o_sqr_6_1;
                myResultsDisplay[6, 2] = o_sqr_6_2;
                myResultsDisplay[6, 3] = o_sqr_6_3;
                myResultsDisplay[6, 4] = o_sqr_6_4;
                myResultsDisplay[6, 5] = o_sqr_6_5;
                myResultsDisplay[6, 6] = o_sqr_6_6;
                myResultsDisplay[6, 7] = o_sqr_6_7;
                myResultsDisplay[6, 8] = o_sqr_6_8;
                myResultsDisplay[6, 9] = o_sqr_6_9;

                myResultsDisplay[7, 1] = o_sqr_7_1;
                myResultsDisplay[7, 2] = o_sqr_7_2;
                myResultsDisplay[7, 3] = o_sqr_7_3;
                myResultsDisplay[7, 4] = o_sqr_7_4;
                myResultsDisplay[7, 5] = o_sqr_7_5;
                myResultsDisplay[7, 6] = o_sqr_7_6;
                myResultsDisplay[7, 7] = o_sqr_7_7;
                myResultsDisplay[7, 8] = o_sqr_7_8;
                myResultsDisplay[7, 9] = o_sqr_7_9;

                myResultsDisplay[8, 1] = o_sqr_8_1;
                myResultsDisplay[8, 2] = o_sqr_8_2;
                myResultsDisplay[8, 3] = o_sqr_8_3;
                myResultsDisplay[8, 4] = o_sqr_8_4;
                myResultsDisplay[8, 5] = o_sqr_8_5;
                myResultsDisplay[8, 6] = o_sqr_8_6;
                myResultsDisplay[8, 7] = o_sqr_8_7;
                myResultsDisplay[8, 8] = o_sqr_8_8;
                myResultsDisplay[8, 9] = o_sqr_8_9;

                myResultsDisplay[9, 1] = o_sqr_9_1;
                myResultsDisplay[9, 2] = o_sqr_9_2;
                myResultsDisplay[9, 3] = o_sqr_9_3;
                myResultsDisplay[9, 4] = o_sqr_9_4;
                myResultsDisplay[9, 5] = o_sqr_9_5;
                myResultsDisplay[9, 6] = o_sqr_9_6;
                myResultsDisplay[9, 7] = o_sqr_9_7;
                myResultsDisplay[9, 8] = o_sqr_9_8;
                myResultsDisplay[9, 9] = o_sqr_9_9;

                for (int row = 1; row <= 9; ++row)
                {
                    for (int col = 1; col <= 9; ++col)
                    {
                        SetControlBgColor(myResultsDisplay[row, col], System.Drawing.Color.White);
                        SetControlBgColor(myInputBoxes[row, col], System.Drawing.Color.White);
                    }
                }

                clearButton.Select();
            }
            catch (Exception e)
            {

            }
        }

        //==================================================
        // Start communicating with the solver engine.
        private void StartAPIServices()
        {
            while (true)
            {
                APIMonitor.Instance.Run();
                Thread.Sleep(100);
            }
        }

        //==================================================
        // Convert textbox data into int
        private int getValue(ref TextBox tb)
        {
            if (tb.Text == string.Empty)
            {
                return 0;
            }

            return Int32.Parse(tb.Text);
        }

        //==================================================
        // Process solve button click
        private void solveButton_Click(object sender, EventArgs e)
        {
            // Validate the board input, if it's good tickle the solver FSM.

            ResetResultsBoard();

            int numClues = 0;

            for (int i = 1; i <= 9; ++i)
            {
                for (int j = 1; j <= 9; ++j)
                {
                    try
                    {
                        if (myInputBoxes[i, j].Text != string.Empty && myInputBoxes[i, j].Text != "?")
                        {
                            if (!(0 <= Int32.Parse(myInputBoxes[i, j].Text) && Int32.Parse(myInputBoxes[i, j].Text) <= 9))
                            {
                                string s = "Square ( " + i.ToString() + ", " + j.ToString() + " ) has an illegal value.";
                                ErrorMessage em = new ErrorMessage(s);
                                em.ShowDialog();
                                return;
                            }
                            ++numClues;
                        }
                    }
                    catch (Exception e1)
                    {
                        string s = "Square ( " + i.ToString() + ", " + j.ToString() + " ) has illegal characters.";
                        ErrorMessage em = new ErrorMessage(s);
                        em.ShowDialog();
                        return;
                    }
                }
            }

            if (numClues < 16)
            {
                string s = "A valid sudoku must have at least 16 clues.";
                ErrorMessage em = new ErrorMessage(s);
                em.ShowDialog();
                return;
            }

            mySolverFSM.EventOccured(SolverFSM.transitionEvent.SOLVE_CLICKED, null);
        }

        //==================================================
        // Process clear button click
        private void clearButton_Click(object sender, EventArgs e)
        {
            mySolverFSM.EventOccured(SolverFSM.transitionEvent.CLEAR_CLICKED, null);
        }

        //==================================================
        // Empty the contents of the results board
        private void ResetResultsBoard()
        {
            for (int i = 1; i <= 9; ++i)
            {
                for (int j = 1; j <= 9; ++j)
                {
                    SetControlText(myResultsDisplay[i, j], string.Empty);
                }
            }
        }

        // This code is necessary because C# blocks operations across threads.
        // The FSM is on another thread and it is controlling what we display here.
        Action<Control, string> setterCallback_str = (toSet, text) => toSet.Text = text;

        private void SetControlText(Control toSet, string text)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(setterCallback_str, toSet, text);
            }
            else
            {
                setterCallback_str(toSet, text);
            }
        }

        Action<Control, bool> setterCallbackEnabled = (toSet, val) => toSet.Enabled = val;
        private void SetControlEnabled(Control toSet, bool theVal)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(setterCallbackEnabled, toSet, theVal);
            }
            else
            {
                setterCallbackEnabled(toSet, theVal);
            }
        }

        Action<Control, System.Drawing.Color> setterControlBgColor = (toSet, val) => toSet.BackColor = val;
        private void SetControlBgColor(Control toSet, System.Drawing.Color theVal)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(setterControlBgColor, toSet, theVal);
            }
            else
            {
                setterControlBgColor(toSet, theVal);
            }
        }
        //==================================================
        // Display the results board
        public void DisplayResultsBoard(byte[] theData)
        {
            int rowIdx = 1;
            int colIdx = 1;

            for (int i = 0; i < SudokuLibApi.RESPONSE_SIZE; ++i)
            {
                if (theData[i] == 0)
                {
                    ++colIdx;
                    if (colIdx > 9)
                    {
                        colIdx = 1;
                        ++rowIdx;
                        if (rowIdx > 9)
                        {
                            break;
                        }
                    }
                    ++i;
                }

                var theSquareData = theData[i].ToString();
                SetControlText(myResultsDisplay[rowIdx, colIdx], theSquareData);

                if (myInputBoxes[rowIdx,colIdx].Text != theSquareData)
                {
                    SetControlBgColor(myResultsDisplay[rowIdx, colIdx], System.Drawing.Color.LimeGreen);
                }
                else if (myInputBoxes[rowIdx, colIdx].Text == theSquareData)
                {
                    SetControlBgColor(myResultsDisplay[rowIdx, colIdx], System.Drawing.Color.LightGray);
                }
            }
        }

        private void enableGuessingButton_Click(object sender, EventArgs e)
        {
            SudokuLibApi.EnableGuessing();
        }

        private void disableGuessing_Click(object sender, EventArgs e)
        {
            SudokuLibApi.DisableGuessing();
        }

        //==================================================
        // Process the events in the manual input box
        private void manualInputBox_TextChanged(object sender, EventArgs e)
        {
            label1.Text = "Manual entry... 0 or ? for unknown squares.";

            string[] s = manualInputBox.Text.Split(' ');

            int col = 1, row = 1;

            for (int i = 0; i < s.Count(); ++i)
            {
                if (row > 9 || col > 9)
                {
                    label1.Text = "81 entries only please!";
                }
                else
                {
                    label1.Text = s.Length + " entries specified";

                    if (!(s[i] == "0" || s[i] == "?" || s[i] == "["))
                    {
                        myInputBoxes[row, col].Text = s[i];
                        SetControlBgColor(myInputBoxes[row, col], System.Drawing.Color.LightGray);
                    }
                    else if (s[i] == "[")
                    {
                        myInputBoxes[row, col].Text = string.Empty;
                        ++i;
                        while (i < s.Count() && s[i] != "]")
                        {
                            myInputBoxes[row, col].Text += s[i] + " ";
                            ++i;
                        }

                        if (s[i] != "]")
                        {
                            myInputBoxes[row, col].Text += "]";
                        }
                    }
                    else
                    {
                        myInputBoxes[row, col].Text = "?";
                        SetControlBgColor(myInputBoxes[row, col], System.Drawing.Color.LimeGreen);
                    }

                    ++col;
                    if (col > 9)
                    {
                        ++row;
                        col = 1;
                    }
                }
            }
        }

        private void exitButtonClick(object sender, EventArgs e)
        {
            mySolverFSM.EventOccured(SolverFSM.transitionEvent.EXIT_BUTTON_CLICKED, null);
        }

        public void ResetDisplay()
        {
            SetControlText(manualInputBox, string.Empty);
            SetControlText(label1, "Fast board entry");
            SetControlText(numGuesses, "0");
            SetControlText(lowestScoreLabel, "0");
            SetControlText(surrenderCountLabel, "0");
            SetControlText(invalidCountLabel, "0");
            SetControlEnabled(clearButton, true);

            for (int i = 1; i <= 9; ++i)
            {
                for (int j = 1; j <= 9; ++j)
                {
                    SetControlBgColor(myInputBoxes[i, j], System.Drawing.Color.White);
                    SetControlText(myInputBoxes[i, j], string.Empty);
                    SetControlText(myResultsDisplay[i, j], string.Empty);
                    SetControlBgColor(myResultsDisplay[i, j], System.Drawing.Color.White);
                }
            }
        }

        public void SetBestScore(string theData)
        {
            SetControlText(lowestScoreLabel, theData);
        }

        public void SetNumGuesses(string theData)
        {
            SetControlText(numGuesses, theData);
        }

        public void SetInvalidCount(string theData)
        {
            SetControlText(invalidCountLabel, theData);
        }

        public void SetSurrenderCount(string theData)
        {
            SetControlText(surrenderCountLabel, theData);
        }

        public void DisableClearButton()
        {
            SetControlEnabled(clearButton, false);
        }

        public void EnableClearButton()
        {
            SetControlEnabled(clearButton, true);
        }

        public void DisableExitButton()
        {
            SetControlEnabled(exitButton, false);
        }

        public void EnableExitButton()
        {
            SetControlEnabled(exitButton, true);
        }

        public void Shutdown()
        {
            CloseForm(this);
        }

        Action<Form> setterCallback_Close = (toSet) => Application.Exit();

        private void CloseForm(Form toSet)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(setterCallback_Close, toSet);
            }
            else
            {
                setterCallback_Close(toSet);
            }
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            mySolverFSM.EventOccured(SolverFSM.transitionEvent.EXIT_BUTTON_CLICKED, null);
        }

        private void Main_Load(object sender, EventArgs e)
        {

        }

        private void textBox218_TextChanged(object sender, EventArgs e)
        {

        }

        private void i_sqr_9_8_TextChanged(object sender, EventArgs e)
        {

        }

        private void manualInputBox_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.FAST_ENTRY);
        }

        private void board_mouse_hover()
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.BOARD_INPUT);
        }

        private void i_sqr_4_4_MouseHover(object sender, EventArgs e)
        {
            board_mouse_hover();
        }

        private void i_sqr_4_5_MouseHover(object sender, EventArgs e)
        {
            board_mouse_hover();
        }

        private void i_sqr_4_6_MouseHover(object sender, EventArgs e)
        {
            board_mouse_hover();
        }

        private void i_sqr_5_4_MouseHover(object sender, EventArgs e)
        {
            board_mouse_hover();
        }

        private void i_sqr_5_5_MouseHover_1(object sender, EventArgs e)
        {
            board_mouse_hover();
        }
        private void i_sqr_5_6_MouseHover(object sender, EventArgs e)
        {
            board_mouse_hover();
        }

        private void i_sqr_6_4_MouseHover(object sender, EventArgs e)
        {
            board_mouse_hover();
        }

        private void i_sqr_6_5_MouseHover(object sender, EventArgs e)
        {
            board_mouse_hover();
        }

        private void i_sqr_6_6_MouseHover(object sender, EventArgs e)
        {
            board_mouse_hover();
        }

        private void resetHelpButton_Click(object sender, EventArgs e)
        {
            GuidanceManager.ResetAllGuidances();
        }

        private void copyBoard_Click(object sender, EventArgs e)
        {
            string textData = "";

            for (int i = 1; i <= 9; ++i)
            {
                for (int j = 1; j <= 9; ++j)
                {
                    if (myInputBoxes[i, j].Text == string.Empty)
                    {
                        textData += "0";
                    }
                    else
                    {
                        textData += myInputBoxes[i, j].Text;
                    }

                    if (!(i == 9 && j == 9))
                    {
                        textData += " ";
                    }
                }
            }

            // After this call, the data (string) is placed on the clipboard and tagged
            // with a data format of "Text".
            Clipboard.SetData(DataFormats.Text, (Object)textData);
        }

        private void label4_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.GUESSES);
        }

        private void label5_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.SURRENDERS);
        }

        private void invalidLabel_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.DEAD_ENDS);
        }

        private void label6_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.BEST_SCORE);
        }

        private void groupBox1_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.killAll();
        }

        private void o_sqr_4_4_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.OUTPUT_BOARD);
        }

        private void o_sqr_4_5_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.OUTPUT_BOARD);
        }

        private void o_sqr_4_6_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.OUTPUT_BOARD);
        }

        private void o_sqr_5_4_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.OUTPUT_BOARD);
        }

        private void o_sqr_5_6_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.OUTPUT_BOARD);
        }

        private void o_sqr_5_5_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.OUTPUT_BOARD);
        }

        private void o_sqr_6_4_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.OUTPUT_BOARD);
        }

        private void o_sqr_6_5_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.OUTPUT_BOARD);
        }

        private void o_sqr_6_6_MouseHover(object sender, EventArgs e)
        {
            GuidanceManager.ProvideGuidance(GuidanceManager.guidanceType.OUTPUT_BOARD);
        }

        public void SetRowReduce(string theVal)
        {
            SetControlText(rowRedBox, theVal);
        }

        public void SetColReduce(string theVal)
        {
            SetControlText(colRedBox, theVal);
        }

        public void SetBlkReduce(string theVal)
        {
            SetControlText(blockRedBox, theVal);
        }

        public void SetStrandedSingles(string theVal)
        {
            SetControlText(strandedSinglesBox, theVal);
        }

        public void SetPointingPairs(string theVal)
        {
            SetControlText(pointingPairsBox, theVal);
        }

        public void SetNakedPairs(string theVal)
        {
            SetControlText(nakedPairsBox, theVal);
        }

        public void SetXWing(string theVal)
        {
            SetControlText(xWingBox, theVal);
        }

        public void SetSimpleChains(string theVal)
        {
            SetControlText(simpleChainsBox, theVal);
        }

        private void disableGuidanceButton_Click(object sender, EventArgs e)
        {
            GuidanceManager.DisableAllGuidances();
        }
    }
}
