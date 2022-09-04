namespace Puma
{
    [Sharpmake.Generate]
    class Asteroids : Puma.Common.IMyLib
    {
        public Asteroids()
            : base("Asteroids", @"asteroids")
        { }

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);

            conf.AddPublicDependency<Puma.PumaEngine>(target);

            conf.IncludePaths.Add(SourceRootPath);
            conf.IncludePaths.Add(Puma.Leo.SharedFilesPath);
            conf.IncludePaths.Add(Puma.Nina.SharedFilesPath);

            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);
        }
    }
}