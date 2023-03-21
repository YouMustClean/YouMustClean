
IHost host = Host.CreateDefaultBuilder(args)
    .ConfigureServices(services =>
    {
        services.AddHostedService<YMC.WindowsDaemon>();
    })
    .Build();

host.Run();
