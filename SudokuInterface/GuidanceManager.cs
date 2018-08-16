using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SudokuInterface
{
    public static class GuidanceManager
    {
        public enum guidanceType { BEST_SCORE, BOARD_INPUT, DEAD_ENDS, FAST_ENTRY, GUESSES, SURRENDERS, OUTPUT_BOARD, LAST_TYPE };
        static List<Guidance> myGuidances;

        private static bool[] dontShow;

        static GuidanceManager()
        {
            myGuidances = new List<Guidance>();

            dontShow = new bool[(int)guidanceType.LAST_TYPE];

            for (int i = 0; i < (int)guidanceType.LAST_TYPE; ++i)
            {
                dontShow[i] = false;
            }
        }

        public static void Initialize()
        {
            myGuidances = new List<Guidance>();

            dontShow = new bool[(int)guidanceType.LAST_TYPE];

            for (int i = 0; i < (int)guidanceType.LAST_TYPE; ++i)
            {
                dontShow[i] = false;
            }
        }

        public static void SetBlocked(guidanceType theType, bool theVal)
        {
            dontShow[(int)theType] = theVal;
        }

        public static void ProvideGuidance(guidanceType theType)
        {
            if (dontShow[(int)theType] == false)
            {
                bool found = false;
                foreach (var guidance in myGuidances)
                {
                    if (guidance.GetGuidanceType() == theType)
                    {
                        // The guidance is already being displayed
                        found = true;
                        break;
                    }
                }

                if (found == false)
                {
                    string text = "Generic guidance message";

                    switch (theType)
                    {
                        case guidanceType.BOARD_INPUT:
                            text = "You may enter the values into the input board via mouse, keyboard and TAB key.";
                            text += " Unknown squares may be left blank or filled with a 0.";
                            break;
                        case guidanceType.FAST_ENTRY:
                            text = "This is the fast entry box. You may enter the values via keyboard but unknown squares require a 0 be provided.";
                            text += " You may also copy-and-paste a position from a text file into this box.";
                            break;
                        case guidanceType.GUESSES:
                            text = "Guesses are the alternative positions generated from a valid position when all other algorithmic methods have failed.";
                            text += " One day I hope to have this number down to 0.";
                            break;
                        case guidanceType.SURRENDERS:
                            text = "A solver surrenders and makes guesses when it can't make any more progress using algorithmic techniques.";
                            break;
                        case guidanceType.DEAD_ENDS:
                            text = "This is the number of dead ends produced in the solving process.";
                            break;
                        case guidanceType.BEST_SCORE:
                            text = "Best score shows the progress of the solving process. 405 means a solution has been found.";
                            break;
                        case guidanceType.OUTPUT_BOARD:
                            text = "Results are shown here. Some puzzles can take a long time. The indicators below show the solving progress.";
                            break;
                    }
                    Guidance em = new Guidance(text, theType);
                    myGuidances.Add(em);
                    em.Show();
                }
            }
        }

        public static void RemoveGuidance(guidanceType theType)
        {
            foreach (var guidance in myGuidances)
            {
                if (guidance.GetGuidanceType() == theType)
                {
                    myGuidances.Remove(guidance);
                    guidance.kill();
                    break;
                }
            }
        }

        public static void killAll()
        {
            while (myGuidances.Count > 0)
            {
                foreach (var guidance in myGuidances)
                {
                    myGuidances.Remove(guidance);
                    guidance.kill();
                    break;
                }
            }
        }

        public static void ResetAllGuidances()
        {
            for (int i = 0; i < (int)guidanceType.LAST_TYPE; ++i)
            {
                dontShow[i] = false;
            }
        }

        public static void DisableAllGuidances()
        {
            killAll();

            for (int i = 0; i < (int)guidanceType.LAST_TYPE; ++i)
            {
                dontShow[i] = true;
            }
        }
    }
}
