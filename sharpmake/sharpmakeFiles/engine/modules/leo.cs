namespace Puma
{
    [Sharpmake.Generate]
    class Leo : Puma.Common.IExternLib
    {
        public static readonly string SharedFilesPath = Puma.SharpmakeUtils.GetExternPath() + @"\puma\extern\leo\source\leo\shared";
        public Leo()
            : base("Leo", @"puma\extern\leo\source\leo")
        { 
        }

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);

            conf.AddPrivateDependency<Export.Box2D>(target);
            conf.AddPublicDependency<Puma.Utils>(target);

            conf.IncludePrivatePaths.Add(@"\private");
            conf.IncludePaths.Add(@"\public");
            conf.IncludePaths.Add(@"\shared");

            conf.PrecompHeader = @"precompiledphysics.h";
            conf.PrecompSource = @"precompiledphysics.cpp";

            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);

            conf.SolutionFolder = "Modules";
        }
    }
}