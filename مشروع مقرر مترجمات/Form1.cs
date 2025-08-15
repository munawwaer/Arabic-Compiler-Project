using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Text;
using System.Drawing;
using System.Data;

namespace مشروع_مقرر_مترجمات
{
    public enum ActionType { paset, cut };
    // يمثل حالة تراجع واحدة
    public class UndoAction
    {
        public string Text { get; set; }
        public int StartIndex { get; set; }

        public ActionType Action { get; set; } = ActionType.cut;
        public UndoAction(string text, int startIndex, ActionType action)
        {
            Text = text;
            StartIndex = startIndex;
            Action = action;
        }
    }
    public partial class Form1 : Form
    {

        private string clipboard = "";
        private readonly Stack<UndoAction> undoStack = new Stack<UndoAction>(); // نستخدم Stack للتراجع العكسي
        // المكونات الرئيسية للواجهة
        private MenuStrip menuStrip;
        ToolStripMenuItem undoMenuItem;
        private TextBox mainTextBox,ResultTokens;
        //        // اسم الـ DLL يجب أن يكون مطابقاً للملف الذي قمت بترجمته
        //private const string DllName = "Tokenizer.dll";



        ////        // استيراد دالة التحليل
        ////        // CharSet.Ansi يضمن توافق النصوص بين C# و C++
        //[DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        //private static extern string tokenize_line_w(string input);
        ////        // CharSet.Ansi يضمن توافق النصوص بين C# و C++
        //[DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        //private static extern void reset_toknizer();
        // متغير لتخزين مسار الملف المفتوح حالياً
        private string currentFilePath = null;

        public Form1()
        {
      
            InitializeComponent();
            UpdateUndoMenuItem();
            menuStrip.RightToLeft= RightToLeft.Yes;
            this.Resize += Form1_Resize;
        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            ResultTokens.Size = new System.Drawing.Size(this.Width / 2, this.Height);
            mainTextBox.Size = new System.Drawing.Size(this.Width / 2 - 30, this.Height);
            ResultTokens.Location = new Point(this.Width / 2 - 20, 70);

        }

        private void New_Click(object sender, EventArgs e)
        {
            mainTextBox.Clear();
            currentFilePath = null;
            this.Text = "مفكرتي - ملف جديد";
        }

        // فتح ملف نصي
        private void Open_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.Filter = "Text Files (*.txt)|*.txt|All files (*.*)|*.*";
                ofd.Title = "فتح ملف نصي";
                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        currentFilePath = ofd.FileName;
                        mainTextBox.Text = File.ReadAllText(currentFilePath);
                        this.Text = "مفكرتي - " + Path.GetFileName(currentFilePath);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("حدث خطأ أثناء قراءة الملف: " + ex.Message, "خطأ", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        // حفظ التغييرات في الملف الحالي
        private void Save_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(currentFilePath))
            {
                // إذا لم يتم حفظ الملف من قبل، استدع دالة "حفظ باسم"
                SaveAs_Click(sender, e);
            }
            else
            {
                try
                {
                    File.WriteAllText(currentFilePath, mainTextBox.Text);
                    this.Text = "مفكرتي - " + Path.GetFileName(currentFilePath);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("حدث خطأ أثناء حفظ الملف: " + ex.Message, "خطأ", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        // حفظ المحتوى في ملف جديد
        private void SaveAs_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog sfd = new SaveFileDialog())
            {
                sfd.Filter = "Text Files (*.txt)|*.txt|All files (*.*)|*.*";
                sfd.Title = "حفظ الملف باسم";
                if (sfd.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        currentFilePath = sfd.FileName;
                        File.WriteAllText(currentFilePath, mainTextBox.Text);
                        this.Text = "مفكرتي - " + Path.GetFileName(currentFilePath);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("حدث خطأ أثناء حفظ الملف: " + ex.Message, "خطأ", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        // الخروج من التطبيق
        private void Exit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        // --- وظائف قائمة "تحرير" ---

        private void Undo_Click(object sender, EventArgs e)
        {
            if (undoStack.Count > 0)
            {
                UndoAction lastAction = undoStack.Pop();

                switch (lastAction.Action)
                {
                    case ActionType.cut:
                        mainTextBox.Text = mainTextBox.Text.Insert(lastAction.StartIndex, lastAction.Text);
                        mainTextBox.SelectionStart = lastAction.StartIndex + lastAction.Text.Length;
                        break;

                    case ActionType.paset:
                        mainTextBox.Text = mainTextBox.Text.Remove(lastAction.StartIndex, lastAction.Text.Length);
                        mainTextBox.SelectionStart = lastAction.StartIndex;
                        break;
                }
                UpdateUndoMenuItem();
            }
            else
            {
                UpdateUndoMenuItem();
            }
        }
        private void UpdateUndoMenuItem()
        {
            undoMenuItem.Enabled = undoStack.Count > 0;
        }
        private void Cut_Click(object sender, EventArgs e)
        {

            if (mainTextBox.SelectionLength > 0)
            {
                int start = mainTextBox.SelectionStart;
                string cutText = mainTextBox.SelectedText;

                // حفظ الحالة في التراجع
                undoStack.Push(new UndoAction(cutText, start, ActionType.cut));

                clipboard = cutText;
                UpdateUndoMenuItem();
                // إزالة النص
                mainTextBox.Text = mainTextBox.Text.Remove(start, cutText.Length);
                mainTextBox.SelectionStart = start;
            }

        }

        private void Copy_Click(object sender, EventArgs e)
        {

            if (mainTextBox.SelectionLength > 0)
            {
                clipboard = mainTextBox.SelectedText;
            }
        }

        private void Paste_Click(object sender, EventArgs e)
        {

            int index = mainTextBox.SelectionStart;
            mainTextBox.Text = mainTextBox.Text.Insert(index, clipboard);
            mainTextBox.SelectionStart = index + clipboard.Length;

            string cutText = clipboard;

            // حفظ الحالة في التراجع
            undoStack.Push(new UndoAction(cutText, index, ActionType.paset));

            UpdateUndoMenuItem();
        }


        private void SelectAll_Click(object sender, EventArgs e)
        {
            mainTextBox.SelectAll();
        }
        // 1. Import the new function from the DLL
        [DllImport("Tokenizer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.LPWStr)]
        private static extern string analyze_syntax_w(string input);

        // 2. Update the AnalyzeButton_Click event handler
        private void AnalyzeButton_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrWhiteSpace(mainTextBox.Text))
            {
                MessageBox.Show("لا يوجد نص لتحليله!", "تنبيه");
                return;
            }

            try
            {
                // Call the new syntax analysis function
                string result = analyze_syntax_w(mainTextBox.Text);

                // Display the single result message
                MessageBox.Show(result, "نتيجة التحليل النحوي", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"An error occurred while calling the DLL: {ex.Message}", "DLL Error");
            }
        }
        //private void AnalyzeButton_Click(object sender, EventArgs e)
        //{
        //    if (mainTextBox == null || string.IsNullOrWhiteSpace(mainTextBox.Text))
        //    {
        //        MessageBox.Show("لا يوجد نص لتحليل", "تنبيه", MessageBoxButtons.OK, MessageBoxIcon.Information);
        //        return;
        //    }

        //    // 1. تحليل الكود وتخزين النتائج في قائمة (هذا الجزء يبقى كما هو)
        //    //var analysisResults = new List<Tuple<string, string>>();
        //    //reset_toknizer();
        //    string resultString;


        //        try
        //        {
        //             resultString = tokenize_line_w(mainTextBox.Text);
        //        foreach (var item in resultString.Split('\n'))
        //        {
        //            ResultTokens.Text += item+"\r\n";
        //        }    
        //        }
        //        catch (DllNotFoundException)
        //        {
        //            MessageBox.Show($"لم يتم العثور على ملف الـ DLL.\nتأكد من وجود ملف '{DllName}' في نفس مجلد البرنامج.", "خطأ فادح", MessageBoxButtons.OK, MessageBoxIcon.Error);
        //            return;
        //        }


        //    // --- الإضافة الجديدة هنا: حفظ النتائج في ملف نصي ---
        //    try
        //    {
        //        StringBuilder fileContent = new StringBuilder();

        //        // أولاً: حفظ الكود الأصلي كاملاً
        //        fileContent.AppendLine("--- الكود الأصلي المدخل ---");
        //        fileContent.AppendLine("============================");
        //        fileContent.AppendLine(mainTextBox.Text);
        //        fileContent.AppendLine();
        //        fileContent.AppendLine();

        //        // ثانياً: حفظ سلسلة الرموز الكاملة
        //        fileContent.AppendLine("--- سلسلة الرموز الناتجة (Tokens) ---");
        //        fileContent.AppendLine("===================================");

        //          fileContent.Append(resultString + " "); // إضافة مسافة بين رموز كل سطر


        //        // تحديد مسار الملف ليكون بجانب الملف التنفيذي للبرنامج
        //        string filePath = System.IO.Path.Combine(Application.StartupPath, "analysis_log.txt");

        //        // كتابة كل المحتوى إلى الملف مع دعم اللغة العربية (UTF-8)
        //        System.IO.File.AppendAllText(filePath, fileContent.ToString(), Encoding.UTF8);

        //    }
        //    catch (Exception ex)
        //    {
        //        MessageBox.Show($"حدث خطأ أثناء حفظ الملف: {ex.Message}", "خطأ في الحفظ", MessageBoxButtons.OK, MessageBoxIcon.Error);
        //    }


        //}

    }
}
// In MainForm.cs





