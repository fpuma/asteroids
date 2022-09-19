using Sharpmake;

namespace Puma
{
    [Sharpmake.Generate]
    class Time : Puma.Common.IExternLib
    {
        public Time()
            : base ("Time", @"puma\extern\timelibrary\source\time")
        { }

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);

            conf.IncludePrivatePaths.Add(@"\private");
            conf.IncludePaths.Add(@"\public");

            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);

            conf.SolutionFolder = "puma/modules/public";
        }
    }
}
