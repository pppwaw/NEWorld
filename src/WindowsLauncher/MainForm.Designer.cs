namespace NEWorldLauncher
{
    partial class MainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.gameListBox = new System.Windows.Forms.ListBox();
            this.gameListMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openFolderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.launchTab = new System.Windows.Forms.TabPage();
            this.debugCheckBox = new System.Windows.Forms.CheckBox();
            this.refreshButton = new System.Windows.Forms.Button();
            this.Label5 = new System.Windows.Forms.Label();
            this.PictureBox1 = new System.Windows.Forms.PictureBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.commandTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.launchButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.downloadTab = new System.Windows.Forms.TabPage();
            this.downloadLabel = new System.Windows.Forms.Label();
            this.downloadRefreshButton = new System.Windows.Forms.Button();
            this.downloadProgressBar = new System.Windows.Forms.ProgressBar();
            this.downloadButton = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.downloadListBox = new System.Windows.Forms.ListBox();
            this.webTab = new System.Windows.Forms.TabPage();
            this.webBrowser = new System.Windows.Forms.WebBrowser();
            this.webMenu = new System.Windows.Forms.MenuStrip();
            this.backWebToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.forwardWebToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.refreshWebToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tiebaWebToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.officialWebToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openWebToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gameListMenu.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.launchTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PictureBox1)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.downloadTab.SuspendLayout();
            this.webTab.SuspendLayout();
            this.webMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // gameListBox
            // 
            this.gameListBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gameListBox.ContextMenuStrip = this.gameListMenu;
            this.gameListBox.FormattingEnabled = true;
            this.gameListBox.ItemHeight = 12;
            this.gameListBox.Location = new System.Drawing.Point(6, 23);
            this.gameListBox.Name = "gameListBox";
            this.gameListBox.Size = new System.Drawing.Size(137, 256);
            this.gameListBox.TabIndex = 1;
            this.gameListBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.gameListBox_MouseDown);
            // 
            // gameListMenu
            // 
            this.gameListMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openFolderToolStripMenuItem,
            this.deleteToolStripMenuItem});
            this.gameListMenu.Name = "gameListMenu";
            this.gameListMenu.Size = new System.Drawing.Size(173, 70);
            this.gameListMenu.Opening += new System.ComponentModel.CancelEventHandler(this.gameListMenu_Opening);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.deleteToolStripMenuItem.Text = "删除此版本";
            this.deleteToolStripMenuItem.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
            // 
            // openFolderToolStripMenuItem
            // 
            this.openFolderToolStripMenuItem.Name = "openFolderToolStripMenuItem";
            this.openFolderToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.openFolderToolStripMenuItem.Text = "打开此版本文件夹";
            this.openFolderToolStripMenuItem.Click += new System.EventHandler(this.openFolderToolStripMenuItem_Click);
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.launchTab);
            this.tabControl.Controls.Add(this.downloadTab);
            this.tabControl.Controls.Add(this.webTab);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.Multiline = true;
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(434, 311);
            this.tabControl.TabIndex = 2;
            this.tabControl.Selected += new System.Windows.Forms.TabControlEventHandler(this.tabControl_Selected);
            // 
            // launchTab
            // 
            this.launchTab.Controls.Add(this.debugCheckBox);
            this.launchTab.Controls.Add(this.refreshButton);
            this.launchTab.Controls.Add(this.Label5);
            this.launchTab.Controls.Add(this.PictureBox1);
            this.launchTab.Controls.Add(this.groupBox1);
            this.launchTab.Controls.Add(this.launchButton);
            this.launchTab.Controls.Add(this.label1);
            this.launchTab.Controls.Add(this.gameListBox);
            this.launchTab.Location = new System.Drawing.Point(4, 22);
            this.launchTab.Name = "launchTab";
            this.launchTab.Padding = new System.Windows.Forms.Padding(3);
            this.launchTab.Size = new System.Drawing.Size(426, 285);
            this.launchTab.TabIndex = 0;
            this.launchTab.Text = "启动游戏";
            this.launchTab.UseVisualStyleBackColor = true;
            // 
            // debugCheckBox
            // 
            this.debugCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.debugCheckBox.AutoSize = true;
            this.debugCheckBox.Location = new System.Drawing.Point(348, 217);
            this.debugCheckBox.Name = "debugCheckBox";
            this.debugCheckBox.Size = new System.Drawing.Size(72, 16);
            this.debugCheckBox.TabIndex = 12;
            this.debugCheckBox.Text = "调试模式";
            this.debugCheckBox.UseVisualStyleBackColor = true;
            // 
            // refreshButton
            // 
            this.refreshButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.refreshButton.Location = new System.Drawing.Point(149, 256);
            this.refreshButton.Name = "refreshButton";
            this.refreshButton.Size = new System.Drawing.Size(75, 23);
            this.refreshButton.TabIndex = 3;
            this.refreshButton.Text = "刷新";
            this.refreshButton.UseVisualStyleBackColor = true;
            this.refreshButton.Click += new System.EventHandler(this.refreshButton_Click);
            // 
            // Label5
            // 
            this.Label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.Label5.AutoSize = true;
            this.Label5.ForeColor = System.Drawing.Color.Gray;
            this.Label5.Location = new System.Drawing.Point(221, 164);
            this.Label5.Name = "Label5";
            this.Label5.Size = new System.Drawing.Size(65, 12);
            this.Label5.TabIndex = 11;
            this.Label5.Text = "—新创无际";
            // 
            // PictureBox1
            // 
            this.PictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.PictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("PictureBox1.Image")));
            this.PictureBox1.Location = new System.Drawing.Point(215, 73);
            this.PictureBox1.Name = "PictureBox1";
            this.PictureBox1.Size = new System.Drawing.Size(205, 122);
            this.PictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.PictureBox1.TabIndex = 10;
            this.PictureBox1.TabStop = false;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.commandTextBox);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(215, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(205, 61);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "高级设置";
            // 
            // commandTextBox
            // 
            this.commandTextBox.Location = new System.Drawing.Point(6, 32);
            this.commandTextBox.Name = "commandTextBox";
            this.commandTextBox.Size = new System.Drawing.Size(188, 21);
            this.commandTextBox.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 17);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 12);
            this.label2.TabIndex = 0;
            this.label2.Text = "命令行指令：";
            // 
            // launchButton
            // 
            this.launchButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.launchButton.Location = new System.Drawing.Point(340, 239);
            this.launchButton.Name = "launchButton";
            this.launchButton.Size = new System.Drawing.Size(80, 40);
            this.launchButton.TabIndex = 3;
            this.launchButton.Text = "启动";
            this.launchButton.UseVisualStyleBackColor = true;
            this.launchButton.Click += new System.EventHandler(this.launchButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 5);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(125, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "请选择要打开的版本：";
            // 
            // downloadTab
            // 
            this.downloadTab.Controls.Add(this.downloadLabel);
            this.downloadTab.Controls.Add(this.downloadRefreshButton);
            this.downloadTab.Controls.Add(this.downloadProgressBar);
            this.downloadTab.Controls.Add(this.downloadButton);
            this.downloadTab.Controls.Add(this.label3);
            this.downloadTab.Controls.Add(this.downloadListBox);
            this.downloadTab.Location = new System.Drawing.Point(4, 22);
            this.downloadTab.Name = "downloadTab";
            this.downloadTab.Padding = new System.Windows.Forms.Padding(3);
            this.downloadTab.Size = new System.Drawing.Size(426, 285);
            this.downloadTab.TabIndex = 1;
            this.downloadTab.Text = "下载游戏";
            this.downloadTab.UseVisualStyleBackColor = true;
            // 
            // downloadLabel
            // 
            this.downloadLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.downloadLabel.AutoSize = true;
            this.downloadLabel.Location = new System.Drawing.Point(230, 11);
            this.downloadLabel.Name = "downloadLabel";
            this.downloadLabel.Size = new System.Drawing.Size(0, 12);
            this.downloadLabel.TabIndex = 5;
            // 
            // downloadRefreshButton
            // 
            this.downloadRefreshButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.downloadRefreshButton.Location = new System.Drawing.Point(149, 256);
            this.downloadRefreshButton.Name = "downloadRefreshButton";
            this.downloadRefreshButton.Size = new System.Drawing.Size(75, 23);
            this.downloadRefreshButton.TabIndex = 4;
            this.downloadRefreshButton.Text = "刷新";
            this.downloadRefreshButton.UseVisualStyleBackColor = true;
            this.downloadRefreshButton.Click += new System.EventHandler(this.downloadRefreshButton_Click);
            // 
            // downloadProgressBar
            // 
            this.downloadProgressBar.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.downloadProgressBar.Location = new System.Drawing.Point(149, 35);
            this.downloadProgressBar.Name = "downloadProgressBar";
            this.downloadProgressBar.Size = new System.Drawing.Size(271, 23);
            this.downloadProgressBar.TabIndex = 3;
            // 
            // downloadButton
            // 
            this.downloadButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.downloadButton.Location = new System.Drawing.Point(149, 6);
            this.downloadButton.Name = "downloadButton";
            this.downloadButton.Size = new System.Drawing.Size(75, 23);
            this.downloadButton.TabIndex = 2;
            this.downloadButton.Text = "下载";
            this.downloadButton.UseVisualStyleBackColor = true;
            this.downloadButton.Click += new System.EventHandler(this.downloadButton_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 5);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(125, 12);
            this.label3.TabIndex = 1;
            this.label3.Text = "请选择要下载的版本：";
            // 
            // downloadListBox
            // 
            this.downloadListBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.downloadListBox.FormattingEnabled = true;
            this.downloadListBox.ItemHeight = 12;
            this.downloadListBox.Location = new System.Drawing.Point(6, 23);
            this.downloadListBox.Name = "downloadListBox";
            this.downloadListBox.Size = new System.Drawing.Size(137, 256);
            this.downloadListBox.TabIndex = 0;
            // 
            // webTab
            // 
            this.webTab.Controls.Add(this.webBrowser);
            this.webTab.Controls.Add(this.webMenu);
            this.webTab.Location = new System.Drawing.Point(4, 22);
            this.webTab.Name = "webTab";
            this.webTab.Padding = new System.Windows.Forms.Padding(3);
            this.webTab.Size = new System.Drawing.Size(426, 285);
            this.webTab.TabIndex = 2;
            this.webTab.Text = "帖子/官网";
            this.webTab.UseVisualStyleBackColor = true;
            // 
            // webBrowser
            // 
            this.webBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.webBrowser.Location = new System.Drawing.Point(3, 27);
            this.webBrowser.MinimumSize = new System.Drawing.Size(20, 20);
            this.webBrowser.Name = "webBrowser";
            this.webBrowser.Size = new System.Drawing.Size(420, 255);
            this.webBrowser.TabIndex = 0;
            // 
            // webMenu
            // 
            this.webMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.backWebToolStripMenuItem,
            this.forwardWebToolStripMenuItem,
            this.refreshWebToolStripMenuItem,
            this.tiebaWebToolStripMenuItem,
            this.officialWebToolStripMenuItem,
            this.openWebToolStripMenuItem});
            this.webMenu.Location = new System.Drawing.Point(3, 3);
            this.webMenu.Name = "webMenu";
            this.webMenu.Padding = new System.Windows.Forms.Padding(0);
            this.webMenu.Size = new System.Drawing.Size(420, 24);
            this.webMenu.TabIndex = 1;
            // 
            // backWebToolStripMenuItem
            // 
            this.backWebToolStripMenuItem.Name = "backWebToolStripMenuItem";
            this.backWebToolStripMenuItem.Size = new System.Drawing.Size(44, 24);
            this.backWebToolStripMenuItem.Text = "后退";
            this.backWebToolStripMenuItem.Click += new System.EventHandler(this.backWebToolStripMenuItem_Click);
            // 
            // forwardWebToolStripMenuItem
            // 
            this.forwardWebToolStripMenuItem.Name = "forwardWebToolStripMenuItem";
            this.forwardWebToolStripMenuItem.Size = new System.Drawing.Size(44, 24);
            this.forwardWebToolStripMenuItem.Text = "前进";
            this.forwardWebToolStripMenuItem.Click += new System.EventHandler(this.forwardWebToolStripMenuItem_Click);
            // 
            // refreshWebToolStripMenuItem
            // 
            this.refreshWebToolStripMenuItem.Name = "refreshWebToolStripMenuItem";
            this.refreshWebToolStripMenuItem.Size = new System.Drawing.Size(44, 24);
            this.refreshWebToolStripMenuItem.Text = "刷新";
            this.refreshWebToolStripMenuItem.Click += new System.EventHandler(this.refreshWebToolStripMenuItem_Click);
            // 
            // tiebaWebToolStripMenuItem
            // 
            this.tiebaWebToolStripMenuItem.Name = "tiebaWebToolStripMenuItem";
            this.tiebaWebToolStripMenuItem.Size = new System.Drawing.Size(44, 24);
            this.tiebaWebToolStripMenuItem.Text = "帖子";
            this.tiebaWebToolStripMenuItem.Click += new System.EventHandler(this.tiebaWebToolStripMenuItem_Click);
            // 
            // officialWebToolStripMenuItem
            // 
            this.officialWebToolStripMenuItem.Name = "officialWebToolStripMenuItem";
            this.officialWebToolStripMenuItem.Size = new System.Drawing.Size(44, 24);
            this.officialWebToolStripMenuItem.Text = "官网";
            this.officialWebToolStripMenuItem.Click += new System.EventHandler(this.officialWebToolStripMenuItem_Click);
            // 
            // openWebToolStripMenuItem
            // 
            this.openWebToolStripMenuItem.Name = "openWebToolStripMenuItem";
            this.openWebToolStripMenuItem.Size = new System.Drawing.Size(164, 24);
            this.openWebToolStripMenuItem.Text = "用默认浏览器打开当前页面";
            this.openWebToolStripMenuItem.Click += new System.EventHandler(this.openWebToolStripMenuItem_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(434, 311);
            this.Controls.Add(this.tabControl);
            this.MainMenuStrip = this.webMenu;
            this.MinimumSize = new System.Drawing.Size(450, 350);
            this.Name = "MainForm";
            this.Text = "NEWorldLauncher 0.0.6";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.gameListMenu.ResumeLayout(false);
            this.tabControl.ResumeLayout(false);
            this.launchTab.ResumeLayout(false);
            this.launchTab.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PictureBox1)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.downloadTab.ResumeLayout(false);
            this.downloadTab.PerformLayout();
            this.webTab.ResumeLayout(false);
            this.webTab.PerformLayout();
            this.webMenu.ResumeLayout(false);
            this.webMenu.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ListBox gameListBox;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage launchTab;
        private System.Windows.Forms.TabPage downloadTab;
        private System.Windows.Forms.Button launchButton;
        internal System.Windows.Forms.Label Label5;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox commandTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button refreshButton;
        internal System.Windows.Forms.PictureBox PictureBox1;
        private System.Windows.Forms.CheckBox debugCheckBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ListBox downloadListBox;
        private System.Windows.Forms.Button downloadRefreshButton;
        private System.Windows.Forms.ProgressBar downloadProgressBar;
        private System.Windows.Forms.Button downloadButton;
        private System.Windows.Forms.Label downloadLabel;
        private System.Windows.Forms.TabPage webTab;
        private System.Windows.Forms.WebBrowser webBrowser;
        private System.Windows.Forms.MenuStrip webMenu;
        private System.Windows.Forms.ToolStripMenuItem backWebToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem forwardWebToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem refreshWebToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem tiebaWebToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem officialWebToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openWebToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip gameListMenu;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openFolderToolStripMenuItem;
    }
}

