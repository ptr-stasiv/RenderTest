using System.IO;
using Sharpmake;

namespace sharpmake
{
    [Generate]
    public class EngineGuiProject : Project
    {
        public EngineGuiProject()
        {
            Name = "EngineGUI";

            SourceRootPath = @"[project.SharpmakeCsPath]/gui/src";

            AddTargets(new Target(Platform.win64, DevEnv.vs2019, Optimization.Debug | Optimization.Release));
        }

        [Configure]
        public void ConfigureAll(Project.Configuration config, Target target)
        {
            config.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.Latest);
            config.Options.Add(Options.Vc.General.WarningLevel.EnableAllWarnings);

            config.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);

            config.ProjectPath = @"[project.SharpmakeCsPath]/gui";

            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/gui/extern/ul/include");
            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]/gui/extern/ul/lib");

            config.LibraryFiles.AddRange(new Strings("AppCore", "Ultralight", "UltralightCore", "WebCore"));

            config.Output = Configuration.OutputType.Lib;

            if (target.Optimization == Optimization.Release)
            {
                config.TargetPath = @"[project.SharpmakeCsPath]/gui/binaries/release";
                config.IntermediatePath = @"[project.SharpmakeCsPath]/gui/binaries/int/release";
            }
            else if (target.Optimization == Optimization.Debug)
            {
                config.TargetPath = @"[project.SharpmakeCsPath]/gui/binaries/debug";
                config.IntermediatePath = @"[project.SharpmakeCsPath]/gui/binaries/int/debug";
                config.Options.Add(Options.Vc.Compiler.RuntimeLibrary.MultiThreaded);
            }
        }
    }

    [Generate]
    public class RenderTestProject : Project
    {
        public RenderTestProject()
        {
            Name = "RenderTest";

            SourceRootPath = @"[project.SharpmakeCsPath]/engine/src";

            AddTargets(new Target(Platform.win64, DevEnv.vs2019, Optimization.Debug | Optimization.Release));
        }

        [Configure]
        public void ConfigureAll(Project.Configuration config, Target target)
        {
            config.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.Latest);
            config.Options.Add(Options.Vc.General.WarningLevel.EnableAllWarnings);

            config.Defines.Add("GLEW_STATIC");

            config.ProjectPath = @"[project.SharpmakeCsPath]/engine";

            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/engine/extern/glfw/include");
            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/engine/extern/glew/include");
            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/engine/src");
            config.IncludePaths.Add(@"[project.SharpmakeCsPath]/gui/src");
            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]/engine/extern/glfw/lib");
            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]/engine/extern/glew/lib");

            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]/engine/extern/glfw/lib");
            config.LibraryPaths.Add(@"[project.SharpmakeCsPath]/engine/extern/glew/lib");

            config.LibraryFiles.AddRange(new Strings("opengl32", "glfw3", "glew32s", "msvcrt"));

            config.AddPrivateDependency<EngineGuiProject>(target);

            config.Output = Configuration.OutputType.Exe;

            if (target.Optimization == Optimization.Release)
            {
                config.TargetPath = @"[project.SharpmakeCsPath]/engine/binaries/release";
                config.IntermediatePath = @"[project.SharpmakeCsPath]/engine/binaries/int/release";
            }
            else if (target.Optimization == Optimization.Debug)
            {
                config.TargetPath = @"[project.SharpmakeCsPath]/engine/binaries/debug";
                config.IntermediatePath = @"[project.SharpmakeCsPath]/engine/binaries/int/debug";
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

            config.AddProject<EngineGuiProject>(target);
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
