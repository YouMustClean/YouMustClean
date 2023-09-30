# WindowsDaemon

A daemon to run WallpaperGenerator everyday and set up wallpapers.

## Compilation & Deployment

To compile this daemon, you must have a Visual Studio on your machine, with .NET development Kit.

When you have these, change your working directory in PowerShell to this directory (where this readme file is) and execute the following command. (Don't forget to add `dotnet` command into your PATH environmental variable.)

```powershell
dotnet publish -c Release -r win-x64 --self-contained true -p:PublishReadyToRun=true
```

Then, the executable binary and other binary dependencies will be placed under `./bin/Release/<dotnet-version>/win-x64/publish/`.
