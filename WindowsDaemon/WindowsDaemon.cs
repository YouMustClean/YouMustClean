namespace YMC;

using CliWrap;
using CliWrap.Buffered;
using System.Data;
using System.Diagnostics.Contracts;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

public class WindowsDaemon : BackgroundService
{
    private readonly ILogger<WindowsDaemon> _ilogger;
    private readonly string _log_file_save_path;
    private DateTime _last_change_time;
    private StreamWriter? _fp_log;

    private const string _data_root = @"%APPDATA%\YouMustClean\";
    private const string _WallpaperGenerator_bin_path = "ymc-wallpaper-generator.exe";
    private readonly string _LUA_PATH = _data_root + @"lua\csv\?.lua;;";
    private readonly string _path_to_toml_config = _data_root + "wallpaper_configuration.toml";
    private readonly string _path_to_image_output = _data_root + "wallpaper.png";
    private readonly string _path_to_additinal_lua_script = _data_root + "script.lua";

    private const uint _SPI_SET_DESK_WALLPAPER = 0x14;
    private const uint _SPIF_UPDATE_INI_FILE = 0x01;
    [DllImport("user32.dll", EntryPoint = "SystemParametersInfo")]
    public static extern int SystemParametersInfo(uint uiAction, uint uiParam, string pvParam, uint fWinIni);

    public WindowsDaemon(ILogger<WindowsDaemon> logger)
    {
        _ilogger = logger;

        _log_file_save_path = _data_root + "DaemonLog";
        _log_file_save_path = Environment.ExpandEnvironmentVariables(_log_file_save_path);

        _last_change_time = DateTime.Today.AddHours(-24).AddHours(4);   ///< 4 A.M. yesterday

        _LUA_PATH = Environment.ExpandEnvironmentVariables(_LUA_PATH);
        _path_to_toml_config = Environment.ExpandEnvironmentVariables(_path_to_toml_config);
        _path_to_image_output = Environment.ExpandEnvironmentVariables(_path_to_image_output);
        _path_to_additinal_lua_script = Environment.ExpandEnvironmentVariables(_path_to_additinal_lua_script);
    }

    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        while (!stoppingToken.IsCancellationRequested)
        {
            _ilogger.LogInformation("YMC::WindowsDaemon runnint at: {time}", DateTimeOffset.Now);
            int pause_time = 60 * 1000;

            try
            {
                // Check if is a different day (4 A.M.)
                if (DateTime.Now <= _last_change_time.AddHours(24))
                {
                    pause_time = (int)((_last_change_time.AddHours(24) - DateTime.Now).TotalSeconds * 1000);
                    pause_time = (int)(0.5 * pause_time);
                    goto SKIP;
                }
                appendToLog(String.Format("A fresh new day! Current day: {0}, last changed day: {1}", DateTime.Now.Day, _last_change_time.Day));
                // Generate wallpaper
                string? stdout;
                string? stderr;
                runWallpaperGenerator(out stdout, out stderr);
                appendToLog(String.Format("Wallpaper Generator stdout:\n{0}", stdout));
                appendToLog(String.Format("Wallpaper Generator stderr:\n{0}", stderr));
                // Set wallpaper
                SystemParametersInfo(_SPI_SET_DESK_WALLPAPER, 0, _path_to_image_output, _SPIF_UPDATE_INI_FILE);
                // Update time
                _last_change_time = DateTime.Today.AddHours(4);
            }
            catch (Exception e)
            {
                _ilogger.LogWarning("{0}", e);
                appendToLog("<WindowsDaemon>"
                    + String.Format(" {0} ", DateTime.Now)
                    + "AN ERROR OCCURRED!\n"
                    + String.Format("{0}\n", e));
                // Retry after an hour
                pause_time = 60 * pause_time;
            }

        SKIP:
            // Check every minute
            await Task.Delay(pause_time, stoppingToken);
        }
    }

    private int runWallpaperGenerator(out string stdout, out string stderr)
    {
        var proc = Cli.Wrap(Environment.ExpandEnvironmentVariables(_WallpaperGenerator_bin_path))
                      .WithEnvironmentVariables(env => env.Set("LUA_PATH", _LUA_PATH))
                      .WithArguments(new[] { _path_to_toml_config, _path_to_image_output, _path_to_additinal_lua_script })
                      .WithWorkingDirectory(Environment.ExpandEnvironmentVariables(_data_root))
                      .WithValidation(CommandResultValidation.None);

        appendToLog(String.Format("Wallpaper Generator process starts at: {0}", DateTime.Now.ToString("yyyy/MMM/dd HH:mm:ss.ff")));
        var result = proc.ExecuteBufferedAsync().Task.Result;
        appendToLog(String.Format("Wallpaper Generator process ends at: {0}", DateTime.Now.ToString("yyyy/MMM/dd HH:mm:ss.ff")));
        appendToLog(String.Format("Wallpaper Generator used {0} seconds!", result.RunTime));

        stdout = result.StandardOutput;
        stderr = result.StandardError;

        return result.ExitCode;
    }

    private StreamWriter refreshLogFilePointer()
    {
        // Date as file name
        string log_fname = String.Format("{0}.log.txt", DateTime.Now.ToString("d"));
        log_fname = Path.Combine(_log_file_save_path, log_fname);
        // FileStream would throw an error if the directory doesn't exist first
        Directory.CreateDirectory(_log_file_save_path);
        _ilogger.LogInformation("Saving log at: {0}", log_fname);
        return new StreamWriter(log_fname, true);   ///< Append to file
    }

    private void appendToLog(string content)
    {
        _fp_log = refreshLogFilePointer();
        _fp_log.WriteLine(content);
        _fp_log.Close();
    }
}
