from conans import ConanFile, CMake, tools
import os

class Small3dConan(ConanFile):
    name = "vkzos"
    license = "MIT"
    author = "dimi309"
    url = "https://github.com/dimi309/vkzos"
    description = "A less onerous way to use the Vulkan API"
    topics = ("vulkan", "gamedev")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch", "cppstd"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    requires = "vulkan-loader/1.2.190", "vulkan-headers/1.2.190"
    generators = "cmake"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src.CMakeLists.txt", "../src*", "../include*", "LICENSE.md"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
  
    def source(self):
        shutil.copy("src.CMakeLists.txt", "src/CMakeLists.txt")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["VKZOS_TESTS"] = False
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*zos.h", dst="include", src="include")
        self.copy(pattern="LICENSE*", dst="licenses", src="",  ignore_case=True, keep_path=False)
        self.copy(pattern="*.lib", dst="lib", keep_path=False)
        self.copy(pattern="*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["vkzos"]
      

