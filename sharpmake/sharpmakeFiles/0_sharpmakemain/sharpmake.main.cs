using System.IO;

[module: Sharpmake.Include(@"..\base\*")]

//Examples
[module: Sharpmake.Include(@"..\engine\*")]
[module: Sharpmake.Include(@"..\engine\modules\*")]
[module: Sharpmake.Include(@"..\asteroids\*")]
[module: Sharpmake.Include(@"..\application\*")]

public static class SharpmakeMainClass
{
    [Sharpmake.Main]
    public static void SharpmakeMain(Sharpmake.Arguments sharpmakeArgs)
    {
        sharpmakeArgs.Generate<Puma.AsteroidsSln>();
        sharpmakeArgs.Generate<Puma.ExternLibs>();
    }
}

