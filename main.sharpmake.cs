using System.IO;
using Sharpmake;

namespace sharpmake
{
    [Generate]
    public class RenderTestProject : Project
    {
        public RenderTestProject()
        {
            Name = "RenderTest";

            SourceRootPath = @"[project.SharpmakeCsPath]/src";

            AddTargets(new Target(Platform.win64, DevEnv.vs2019, Optimization.Debug | Optimization.Release));
        }

        [Configure]
        public void ConfigureAll(Project.Configuration config, Target target)
        {
            config.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.Latest);
            config.Options.Add(Options.Vc.General.WarningLevel.EnableAllWarnings);

            config.Defines.Add("GLEW_STATIC");

            config.ProjectPath = @"[project.SharpmakeCsPath]";

            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/extern/glfw/include");
            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/extern/glew/include");
            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]/extern/glfw/lib");
            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]/extern/glew/lib");

            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]extern/glfw/lib");
            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]extern/glew/lib");

            config.LibraryFiles.AddRange(new Strings("opengl32", "glfw3", "glew32s", "msvcrt"));

            config.Output = Configuration.OutputType.Exe;

            if (target.Optimization == Optimization.Release)
            {
                config.TargetPath = @"[project.SharpmakeCsPath]/binaries/[project.Name]/release";
                config.IntermediatePath = @"[project.SharpmakeCsPath]/binaries/int/[project.Name]/release";
            }
            else if (target.Optimization == Optimization.Debug)
            {
                config.TargetPath = @"[project.SharpmakeCsPath]/binaries/[project.Name]/debug";
                config.IntermediatePath = @"[project.SharpmakeCsPath]/binaries/int/[project.Name]/debug";
                config.Options.Add(Options.Vc.Compiler.RuntimeLibrary.MultiThreaded);
            }
        }
    }

    [Generate]
    public class MainSolution : Solution
    {
        public MainSolution()
        {
            Name = "Engine-Workspace";

            AddTargets(new Target(Platform.win64, DevEnv.vs2019, Optimization.Debug | Optimization.Release));
        }

        [Configure]
        public void ConfigureAll(Solution.Configuration config, Target target)
        {
            config.SolutionPath = @"[solution.SharpmakeCsPath]";

            config.AddProject<RenderTestProject>(target);

            config.SetStartupProject<RenderTestProject>();
        }

        [Main]
        public static void SharpmakeMain(Arguments args)
        {
            args.Generate<MainSolution>();
        }
    }
}
