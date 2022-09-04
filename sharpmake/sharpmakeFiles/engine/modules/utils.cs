namespace Puma
{
    [Sharpmake.Generate]
    class Utils : Puma.Common.IExternLib
    {
        public Utils()
            : base("Utils", @"puma\extern\utilslibrary\source\utils")
        { }

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);

            conf.IncludePrivatePaths.Add(@"\private");
            conf.IncludePaths.Add(@"\public");

            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);

            conf.SolutionFolder = "Modules";
        }
    }
}