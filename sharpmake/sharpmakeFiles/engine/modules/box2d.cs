namespace Extern
{
    [Sharpmake.Generate]
    class Box2D : Puma.Common.IExternLib
    {
        public Box2D()
            : base("Box2D", @"puma\extern\leo\extern\box2d")
        { }

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);

            conf.SourceFilesBuildExcludeRegex.Add("glad");
            conf.SourceFilesBuildExcludeRegex.Add("glfw");
            conf.SourceFilesBuildExcludeRegex.Add("imgui");
            conf.SourceFilesBuildExcludeRegex.Add("sajson");
            conf.SourceFilesBuildExcludeRegex.Add("testbed");
            conf.SourceFilesBuildExcludeRegex.Add("unit-test");

            conf.IncludePrivatePaths.Add(SourceRootPath + @"\src");
            conf.IncludePaths.Add(SourceRootPath + @"\include");
        }
    }
}

namespace Export
{
    [Sharpmake.Export]
    class Box2D : Extern.Box2D
    {
        public Box2D()
            : base()
        { }
    }
}