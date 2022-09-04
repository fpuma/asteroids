namespace Puma
{
    [Sharpmake.Generate]
    class AsteroidsSln : Puma.Common.IMySolution
    {
        public AsteroidsSln()
            : base("Asteroids")
        { }

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);
            conf.AddProject<Puma.PumaEngine>(target);
            conf.AddProject<Puma.Asteroids>(target);
            conf.AddProject<Puma.AsteroidsApp>(target);
        }
    }

    [Sharpmake.Generate]
    class ExternLibs : Puma.Common.IMySolution
    {
        public ExternLibs()
            : base("ExternLibs")
        { }

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);

            conf.AddProject<Extern.Box2D>(target);
        }
    }
}

