using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace مشروع_مقرر_مترجمات
{
    public partial class FormTable : Form
    {
     public   DataGridView resultsGridView=new DataGridView();
        public FormTable(DataTable h)
        {
            InitializeComponent();

            resultsGridView.DataSource = h;

            resultsGridView.Columns[0].DefaultCellStyle.Font = new Font("Consolas", 10); // أو أي خط برمجي

            // ضبط اتجاه عمود "الرموز" من اليسار إلى اليمين
            resultsGridView.Columns[0].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
       
         resultsGridView.Columns[1].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;

            // منع تأثير الضغط على الرؤوس
            foreach (DataGridViewColumn col in resultsGridView.Columns)
            {
                col.SortMode = DataGridViewColumnSortMode.NotSortable;
            }
        }
    }
}


      

