using System;
using System.Diagnostics;
using System.Windows.Forms;

namespace NEWorldLauncher
{
    public partial class DebugForm : Form
    {
        Process process;

        public DebugForm(GameScene game)
        {
            InitializeComponent();
            process = game.Process;
            process.Exited += Process_Exited;
            process.BeginOutputReadLine();
            process.OutputDataReceived += Process_OutputDataReceived;
            outTextBox.AppendText($"启动游戏，参数是[{process.StartInfo?.Arguments}]\n");
        }

        private void Process_Exited(object sender, EventArgs e)
        {
            Invoke((Action)delegate
            {
                outTextBox.AppendText($"游戏退出，返回值是[{process.ExitCode}]\n");
                killButton.Text = "已退出";
                killButton.Enabled = false;
            });
        }

        private void Process_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            if (!string.IsNullOrWhiteSpace(e.Data))
            {
                Invoke((Action)delegate
                {
                    outTextBox.AppendText(e.Data + '\n');
                });
            }
        }

        private void DebugForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!process.HasExited)
            {
                if (MessageBox.Show("确定要停止调试？", "调试面板", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    process.Kill();
                    process.Dispose();
                }
                else
                    e.Cancel = true;
            }
            else
                process.Dispose();
        }

        private void killButton_Click(object sender, EventArgs e)
        {
            if (!process.HasExited)
                process.Kill();
        }
    }
}
