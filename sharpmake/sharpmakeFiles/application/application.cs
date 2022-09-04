namespace Puma
{
    [Sharpmake.Generate]
    class AsteroidsApp : Puma.Common.IMyApplication
    {
        public AsteroidsApp()
            : base("AsteroidsApp", @"application")
        { }

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);

            //conf.AddPublicDependency<Puma.PumaEngine>(target);
            conf.AddPublicDependency<Puma.Asteroids>(target);

            conf.IncludePaths.Add(SourceRootPath);
            //conf.IncludePaths.Add(Puma.Leo.SharedFilesPath);
            //conf.IncludePaths.Add(Puma.Nina.SharedFilesPath);

            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);
        }
    }
}