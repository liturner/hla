from conan import ConanFile
from conan.tools.cmake import CMakeDeps
from conan.tools.env import VirtualRunEnv, VirtualBuildEnv


class MyProject(ConanFile):
    settings = "os", "arch", "compiler", "build_type"

    def requirements(self):
        self.requires("eigen/3.4.0")
        self.requires("gtest/1.15.0")

    def generate(self):
        # Create, but do not call generate. This prevents the virtual env files being generated
        VirtualBuildEnv(self)
        VirtualRunEnv(self)

        # Generate the CMake find files which tell CMake where to look for the libs etc.
        cmake = CMakeDeps(self)
        cmake.generate()
