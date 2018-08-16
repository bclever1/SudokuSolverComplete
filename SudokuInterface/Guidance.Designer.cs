namespace SudokuInterface
{
    partial class Guidance
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.dontShowCheckBox = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("Papyrus", 12F, System.Drawing.FontStyle.Bold);
            this.label1.Location = new System.Drawing.Point(65, 38);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(535, 440);
            this.label1.TabIndex = 1;
            this.label1.Text = "label1";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // dontShowCheckBox
            // 
            this.dontShowCheckBox.AutoSize = true;
            this.dontShowCheckBox.Font = new System.Drawing.Font("Papyrus", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dontShowCheckBox.Location = new System.Drawing.Point(211, 507);
            this.dontShowCheckBox.Name = "dontShowCheckBox";
            this.dontShowCheckBox.Size = new System.Drawing.Size(230, 29);
            this.dontShowCheckBox.TabIndex = 2;
            this.dontShowCheckBox.Text = "Don\'t show me this again";
            this.dontShowCheckBox.UseVisualStyleBackColor = true;
            this.dontShowCheckBox.CheckedChanged += new System.EventHandler(this.dontShowCheckBox_CheckedChanged);
            // 
            // Guidance
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(650, 640);
            this.Controls.Add(this.dontShowCheckBox);
            this.Controls.Add(this.label1);
            this.Name = "Guidance";
            this.Text = "Guidance";
            this.Leave += new System.EventHandler(this.Guidance_Leave);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox dontShowCheckBox;
    }
}