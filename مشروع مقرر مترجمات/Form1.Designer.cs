using System;
using System.Drawing;
using System.Windows.Forms;

namespace مشروع_مقرر_مترجمات
{
    partial class Form1
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
            // --- إعداد النافذة الرئيسية ---
            this.Text = "مفكرتي";
            this.Size = new System.Drawing.Size(800, 600);
            this.StartPosition = FormStartPosition.CenterScreen;
       
     

            // --- إعداد القائمة العلوية ---
            menuStrip = new MenuStrip();
            this.Controls.Add(menuStrip);
            this.MainMenuStrip = menuStrip;

            // --- إعداد مربع النص الرئيسي ---
            mainTextBox = new TextBox();
            mainTextBox.Multiline = true;
            //mainTextBox.Dock = DockStyle.Left; // يملأ مربع النص كامل النافذة
            mainTextBox.Size=new System.Drawing.Size(this.Width/2-30,this.Height);
            mainTextBox.ScrollBars = ScrollBars.Both; // إظهار أشرطة التمرير
            mainTextBox.Font = new System.Drawing.Font("Arial", 12F);
            mainTextBox.WordWrap = true;
            mainTextBox.Top = 70;

            ResultTokens = new TextBox();
            //ResultTokens.Dock = DockStyle.Right;
            ResultTokens.ScrollBars = ScrollBars.Both; // إظهار أشرطة التمرير
            ResultTokens.Size=new System.Drawing.Size(this.Width/2,this.Height);
            ResultTokens.Location = new Point(this.Width/2-20, 70);
            ResultTokens.Font = new System.Drawing.Font("Arial", 12F);
            ResultTokens.Multiline = true;
            ResultTokens.WordWrap = true;
          
            this.Controls.Add(mainTextBox);
            this.Controls.Add(ResultTokens);

            // --- إنشاء قائمة "ملف" (File) ---
            ToolStripMenuItem fileMenu = new ToolStripMenuItem("ملف");
            menuStrip.Items.Add(fileMenu);

            // إضافة عناصر قائمة "ملف"
            ToolStripMenuItem newMenuItem = new ToolStripMenuItem("جديد", null, New_Click);
            newMenuItem.ShortcutKeys = Keys.Control | Keys.N; // اختصار: Ctrl+N
            fileMenu.DropDownItems.Add(newMenuItem);

            ToolStripMenuItem openMenuItem = new ToolStripMenuItem("فتح...", null, Open_Click);
            openMenuItem.ShortcutKeys = Keys.Control | Keys.O; // اختصار: Ctrl+O
            fileMenu.DropDownItems.Add(openMenuItem);

            ToolStripMenuItem saveMenuItem = new ToolStripMenuItem("حفظ", null, Save_Click);
            saveMenuItem.ShortcutKeys = Keys.Control | Keys.S; // اختصار: Ctrl+S
            fileMenu.DropDownItems.Add(saveMenuItem);

            ToolStripMenuItem saveAsMenuItem = new ToolStripMenuItem("حفظ باسم...", null, SaveAs_Click);
            fileMenu.DropDownItems.Add(saveAsMenuItem);

            fileMenu.DropDownItems.Add(new ToolStripSeparator()); // فاصل في القائمة

            ToolStripMenuItem exitMenuItem = new ToolStripMenuItem("خروج", null, Exit_Click);
            fileMenu.DropDownItems.Add(exitMenuItem);

            // --- إنشاء قائمة "تحرير" (Edit) ---
            ToolStripMenuItem editMenu = new ToolStripMenuItem("تحرير");
            menuStrip.Items.Add(editMenu);

            // إضافة عناصر قائمة "تحرير"
            undoMenuItem = new ToolStripMenuItem("تراجع", null, Undo_Click);
            undoMenuItem.ShortcutKeys = Keys.Control | Keys.Z;
            editMenu.DropDownItems.Add(undoMenuItem);

            editMenu.DropDownItems.Add(new ToolStripSeparator());

            ToolStripMenuItem cutMenuItem = new ToolStripMenuItem("قص", null, Cut_Click);
            cutMenuItem.ShortcutKeys = Keys.Control | Keys.X;
            editMenu.DropDownItems.Add(cutMenuItem);

            ToolStripMenuItem copyMenuItem = new ToolStripMenuItem("نسخ", null, Copy_Click);
            copyMenuItem.ShortcutKeys = Keys.Control | Keys.C;
            editMenu.DropDownItems.Add(copyMenuItem);

            ToolStripMenuItem pasteMenuItem = new ToolStripMenuItem("لصق", null, Paste_Click);
            pasteMenuItem.ShortcutKeys = Keys.Control | Keys.V;
            editMenu.DropDownItems.Add(pasteMenuItem);

            editMenu.DropDownItems.Add(new ToolStripSeparator());

            ToolStripMenuItem selectAllMenuItem = new ToolStripMenuItem("تحديد الكل", null, SelectAll_Click);
            selectAllMenuItem.ShortcutKeys = Keys.Control | Keys.A;
            editMenu.DropDownItems.Add(selectAllMenuItem);
            // Button to trigger the analysis
            Button analyzeButton = new Button();
            analyzeButton.Text = "تحليل وتقسيم الكود";
            analyzeButton.Dock = DockStyle.Top;
            analyzeButton.Height = 40;
            analyzeButton.Font = new Font("Tajawal", 12F, FontStyle.Bold);
            analyzeButton.BackColor = Color.FromArgb(45, 52, 71);
            analyzeButton.ForeColor = Color.White;
            analyzeButton.FlatStyle = FlatStyle.Flat;
            analyzeButton.FlatAppearance.BorderSize = 0;
            analyzeButton.Click += new EventHandler(AnalyzeButton_Click); // Link button to its function
         
            this.Controls.Add(analyzeButton);
        }
    }
}
#endregion