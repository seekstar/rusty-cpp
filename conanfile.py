from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps

class rusty_cppRecipe(ConanFile):
    name = "rusty-cpp"
    version = "0.1.10"

    # Optional metadata
    license = "dual licensed under the Apache License v2.0 and the MIT License"
    author = "Jiansheng Qiu jianshengqiu.cs@gmail.com"
    url = "https://github.com/seekstar/rusty-cpp"
    description = "Write C++ like rust"
    topics = ("C++", "Rust")

    # Binary configuration
    settings = "build_type"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "include/*"
    # We can avoid copying the sources to the build folder in the cache
    no_copy_source = True

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def package(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_info(self):
        # For header-only packages, libdirs and bindirs are not used
        # so it's necessary to set those as empty.
        self.cpp_info.set_property("cmake_target_name", "rusty-cpp")
        self.cpp_info.libdirs = []
        self.cpp_info.bindirs = []
