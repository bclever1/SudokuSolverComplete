using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SudokuInterface
{
    public partial class Guidance : Form
    {
        private GuidanceManager.guidanceType myType;

        public Guidance(string theMessage, GuidanceManager.guidanceType theType)
        {
            InitializeComponent();
            label1.Text = theMessage;
            myType = theType;
        }

        public void kill()
        {
            Close();
        }

        public GuidanceManager.guidanceType GetGuidanceType()
        {
            return myType;
        }

        private void Guidance_Leave(object sender, EventArgs e)
        {
            GuidanceManager.RemoveGuidance(myType);
        }

        private void dontShowCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            GuidanceManager.SetBlocked(myType, dontShowCheckBox.Checked);
            GuidanceManager.RemoveGuidance(myType);
        }
    }
}
