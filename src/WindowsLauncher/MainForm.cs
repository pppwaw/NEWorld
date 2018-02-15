using System;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Windows.Forms;

namespace NEWorldLauncher
{
    public partial class MainForm : Form
    {
        string[] downloadAddresses;
        bool downloading;

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            Stat.ask();
            Stat.collectOpenGL();
            refreshGameList();
            webBrowser.ScriptErrorsSuppressed = true;
            gotoTieba();
        }

        private void refreshGameList()
        {
            gameListBox.Items.Clear();
            string[] games = GameScene.GetGameNames();
            if (games.Length == 0)
                MessageBox.Show("找不到任何游戏文件夹！请把游戏文件夹放到NEWorld文件夹中或下载游戏！", "NEWorldLauncher");
            else
                gameListBox.Items.AddRange(games);
        }

        private void tabControl_Selected(object sender, TabControlEventArgs e)
        {
            if (e.TabPageIndex == 1 && downloadListBox.Items.Count == 0)
                refreshDownloadList();
            // Not user-friendly
            //if (e.TabPageIndex == 2)
            //{
            //    WindowState = FormWindowState.Maximized;
            //    gotoTieba();
            //}
            //else
            //    WindowState = FormWindowState.Normal;
        }


        private void gameListBox_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                int clickedIndex = gameListBox.IndexFromPoint(e.Location);
                if (clickedIndex == -1)
                    gameListBox.ClearSelected();
                else
                    gameListBox.SetSelected(clickedIndex, true);
            }
        }

        private void gameListMenu_Opening(object sender, CancelEventArgs e)
        {
            e.Cancel = gameListBox.SelectedIndex == -1;
        }

        private void openFolderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var gameName = gameListBox.Items[gameListBox.SelectedIndex].ToString();
            Process.Start(Path.Combine(Def.gamesPath, gameName));
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var gameName = gameListBox.Items[gameListBox.SelectedIndex].ToString();
            Directory.Delete(Path.Combine(Def.gamesPath, gameName), true);
            refreshGameList();
        }

        private void launchButton_Click(object sender, EventArgs e)
        {
            if (downloading && MessageBox.Show(
                "正在下载，你确定要开始吗？（如果选择是则启动器不会退出）", "NEWorldLauncher",
                MessageBoxButtons.YesNo) != DialogResult.Yes) return;
            int index = gameListBox.SelectedIndex;
            if (index != -1)
            {
                var game = new GameScene(gameListBox.Items[index].ToString());
                game.Launch(debugCheckBox.Checked, commandTextBox.Text);
                if (debugCheckBox.Checked)
                    new DebugForm(game).Show();
                else if (!downloading)
                    Application.Exit();
            }
        }

        private void refreshButton_Click(object sender, EventArgs e)
        {
            refreshGameList();
        }

        private void refreshDownloadList()
        {
            var client = new WebClient();
            client.DownloadStringCompleted +=
                delegate (object sender, DownloadStringCompletedEventArgs e)
                {
                    if (e.Error == null)
                    {
                        downloadListBox.Items.Clear();
                        string[] lines = e.Result.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
                        Array.Resize(ref downloadAddresses, lines.Length / 2);
                        for (int i = 0; i < lines.Length / 2; i++)
                        {
                            downloadListBox.Items.Add(lines[2 * i]);
                            downloadAddresses[i] = lines[2 * i + 1];
                        }
                    }
                    else
                        MessageBox.Show(e.Error.Message, "刷新出错");
                    downloadRefreshButton.Text = "刷新";
                    downloadRefreshButton.Enabled = true;
                    client.Dispose();
                };
            client.DownloadStringAsync(new Uri(Def.downloadListUri));
            downloadRefreshButton.Text = "正在刷新";
            downloadRefreshButton.Enabled = false;
        }

        private void downloadRefreshButton_Click(object sender, EventArgs e)
        {
            refreshDownloadList();
        }

        private void downloadButton_Click(object sender, EventArgs e)
        {
            if (downloadListBox.SelectedIndex != -1)
                download(downloadAddresses[downloadListBox.SelectedIndex]);
        }

        private void download(string address)
        {
            downloading = true;
            downloadButton.Text = "下载中";
            downloadButton.Enabled = false;
            downloadProgressBar.Value = 0;
            var client = new WebClient();
            client.DownloadProgressChanged +=
                delegate (object sender, DownloadProgressChangedEventArgs e)
                {
                    downloadProgressBar.Maximum = (int)e.TotalBytesToReceive;
                    downloadProgressBar.Value = (int)e.BytesReceived;
                    downloadLabel.Text = string.Format("{0}% {1}KB/{2}KB",
                        e.ProgressPercentage, e.BytesReceived / 1024, e.TotalBytesToReceive / 1024);
                };
            client.DownloadFileCompleted +=
                delegate (object sender, AsyncCompletedEventArgs e)
                {
                    if (e.Error == null)
                        unzip();
                    else
                    {
                        MessageBox.Show(e.Error.Message, "下载出错");
                        downloadButton.Text = "下载";
                        downloadButton.Enabled = true;
                    }
                    client.Dispose();
                };
            client.DownloadFileAsync(new Uri(address), Def.tempFile);
        }


        private void unzip()
        {
            downloadButton.Text = "解压中";
            downloadLabel.Text = "下载完成";
            var unzipProcess = new Process();
            unzipProcess.StartInfo.FileName = Path.GetFullPath(Def.tempFile);
            unzipProcess.StartInfo.WorkingDirectory = Def.gamesPath;
            unzipProcess.EnableRaisingEvents = true;
            unzipProcess.Exited += delegate (object sender, EventArgs e)
            {
                if (unzipProcess.ExitCode == 0)
                {
                    downloading = false;
                    Invoke((Action)delegate
                    {
                        MessageBox.Show("下载并解压完成！", "NEWorldLauncher");
                        refreshGameList();
                    });
                }
                else
                {
                    Invoke((Action)delegate
                    {
                        MessageBox.Show(string.Format("自解压返回值为[{0}]",
                            unzipProcess.ExitCode), "解压出错");
                    });
                }
                unzipProcess.Dispose();
                File.Delete(Def.tempFile);
                Invoke((Action)delegate
                {
                    downloadButton.Text = "下载";
                    downloadButton.Enabled = true;
                    downloadLabel.Text = "";
                    downloadProgressBar.Value = 0;
                });
            };
            unzipProcess.Start();
        }

        private void backWebToolStripMenuItem_Click(object sender, EventArgs e)
        {
            webBrowser.GoBack();
        }

        private void forwardWebToolStripMenuItem_Click(object sender, EventArgs e)
        {
            webBrowser.GoForward();
        }

        private void refreshWebToolStripMenuItem_Click(object sender, EventArgs e)
        {
            webBrowser.Refresh();
        }

        private void tiebaWebToolStripMenuItem_Click(object sender, EventArgs e)
        {
            gotoTieba();
        }

        private void gotoTieba()
        {
            //把页数设成一个很大的数，就会跳转到最后一页
            webBrowser.Navigate("http://tieba.baidu.com/p/2822071396?pn=233333");
        }

        private void officialWebToolStripMenuItem_Click(object sender, EventArgs e)
        {
            webBrowser.Navigate("http://infinideas.org");
        }

        private void openWebToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Process.Start(webBrowser.Url.ToString());
        }
    }
}
