namespace Puma
{
    [Sharpmake.Generate]
    class Pina : Puma.Common.IExternLib
    {
        public Pina()
            : base("Pina", @"puma\extern\pina\source\pina")
        { }

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);

            conf.AddPublicDependency<Puma.Utils>(target);

            conf.IncludePrivatePaths.Add(@"\private");
            conf.IncludePaths.Add(@"\public");

            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);

            conf.SolutionFolder = "puma/modules/public";
        }
    }
}