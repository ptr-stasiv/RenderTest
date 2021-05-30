using Sharpmake;

namespace sharpmake
{
    [Generate]
    public class RenderTestProject : Project
    {
        public RenderTestProject()
        {
            Name = "RenderTest";

            SourceRootPath = @"[project.SharpmakeCsPath]/engine/src";

            AddTargets(new Target(Platform.win64, DevEnv.vs2019, Optimization.Debug | Optimization.Release | Optimization.Retail));
        }

        [Configure]
        public void ConfigureAll(Project.Configuration config, Target target)
        {
            config.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.Latest);
            config.Options.Add(Options.Vc.General.WarningLevel.EnableAllWarnings);

            config.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);



            //Defines

            config.Defines.Add("GLEW_STATIC");

            if (target.Optimization == Optimization.Debug)
                config.Defines.Add("DEBUG");
            else
                config.Defines.Add("RELEASE");


            if (target.Platform == Platform.win64)
                config.Defines.Add("WINDOWS");



            config.ProjectPath = @"[project.SharpmakeCsPath]/engine";

            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/engine/extern/glfw/include");
            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/engine/extern/glew/include");
            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/engine/src/vendors");
            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/engine/src");
            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/gui/src");
            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]/engine/extern/glfw/lib");
            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]/engine/extern/glew/lib");

            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]/engine/extern/glfw/lib");
            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]/engine/extern/glew/lib");


            config.LibraryFiles.AddRange(new Strings("opengl32", "glfw3", "glew32s", "msvcrt"));;


            config.Output = Configuration.OutputType.Exe;

            config.TargetPath = @"[project.SharpmakeCsPath]/engine/binaries/[target.Optimization]";
            config.IntermediatePath = @"[project.SharpmakeCsPath]/engine/binaries/int/[target.Optimization]";


            //Related to my issues with vs
            if (target.Optimization == Optimization.Debug)
                config.Options.Add(Options.Vc.Compiler.RuntimeLibrary.MultiThreaded);
        }
    }

    [Generate]
    public class TestsProject : Project
    {
        public TestsProject()
        {
            Name = "Tests";

            SourceRootPath = @"[project.SharpmakeCsPath]/tests/";

            AddTargets(new Target(Platform.win64, DevEnv.vs2019, Optimization.Debug | Optimization.Release | Optimization.Retail));
        }

        [Configure]
        public void ConfigureAll(Project.Configuration config, Target target)
        {
            config.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.Latest);
            config.Options.Add(Options.Vc.General.WarningLevel.EnableAllWarnings);

            config.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);


            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/tests/extern/googletest/include");
            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/engine/src");

            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]/tests/extern/googletest/lib");

            config.LibraryFiles.Add("googletestlib");


            config.AddPrivateDependency<RenderTestProject>(target);


            config.Output = Configuration.OutputType.Exe;

            config.TargetPath = @"[project.SharpmakeCsPath]/engine/binaries/[target.Optimization]";
            config.IntermediatePath = @"[project.SharpmakeCsPath]/engine/binaries/int/[target.Optimization]";
        }
    }

    [Generate]
    public class MainSolution : Solution
    {
        public MainSolution()
        {
            Name = "Engine-Workspace";

            AddTargets(new Target(Platform.win64, DevEnv.vs2019, Optimization.Debug | Optimization.Release | Optimization.Retail));
        }

        [Configure]
        public void ConfigureAll(Solution.Configuration config, Target target)
        {
            config.SolutionPath = @"[solution.SharpmakeCsPath]";

            config.AddProject<RenderTestProject>(target);
            config.AddProject<TestsProject>(target);

            config.SetStartupProject<RenderTestProject>();
        }

        [Main]
        public static void SharpmakeMain(Arguments args)
        {
            args.Generate<MainSolution>();
        }
    }
}
