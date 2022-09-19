using Sharpmake;

namespace Puma
{
    [Sharpmake.Generate]
    class PumaEngine : Puma.Common.IExternLib
    {
        public PumaEngine()
            : base ("Puma", @"puma\source\puma")
        { }

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);

            conf.AddPrivateDependency<Puma.Nina>(target);
            conf.AddPrivateDependency<Puma.Leo>(target);
            conf.AddPublicDependency<Puma.Pina>(target);
            conf.AddPublicDependency<Puma.Time>(target);
            conf.AddPublicDependency<Puma.Utils>(target);
            conf.AddPublicDependency<Puma.Logger>(target);

            conf.IncludePrivatePaths.Add(@"\private");
            conf.IncludePaths.Add(@"\public");

            conf.PrecompHeader = @"precompiledengine.h";
            conf.PrecompSource = @"precompiledengine.cpp";

            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);

            conf.SolutionFolder = "puma";
        }
    }
}
