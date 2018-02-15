using System.Collections.Generic;
using System.Diagnostics;
using System.IO;

namespace NEWorldLauncher
{
    public class GameScene
    {
        public string Name { get; set; }
        public Process Process { get; set; }

        public GameScene(string name)
        {
            Name = name;
            var gamePath = Path.GetFullPath(Path.Combine(Def.gamesPath, Name));
            Process = new Process();
            Process.StartInfo.FileName = Path.Combine(gamePath, Def.exeName);
            Process.StartInfo.WorkingDirectory = gamePath;
        }

        public void Launch(bool debug, string command)
        {
            Process.StartInfo.Arguments = command;
            if (debug)
            {
                Process.StartInfo.UseShellExecute = false;
                Process.StartInfo.RedirectStandardOutput = true;
                Process.EnableRaisingEvents = true;
            }
            Process.Start();
        }

        public static string[] GetGameNames()
        {
            if (!Directory.Exists(Def.gamesPath))
                Directory.CreateDirectory(Def.gamesPath);
            var names = new List<string>();
            foreach (var i in Directory.GetDirectories(Def.gamesPath))
                if (File.Exists(Path.Combine(i, Def.exeName)))
                    names.Add(Path.GetFileName(i));
            return names.ToArray();
        }
    }
}
