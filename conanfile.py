from conans import ConanFile, CMake
# from parktoma.vscconan import update_cpp_tools

class AnyNetConan(ConanFile):
	settings = "os", "compiler", "build_type", "arch"
	requires = "boost/1.71.0@conan/stable"
	generators = "cmake"

#	def imports(self):
#		update_cpp_tools(self, conanfile_path=__file__)
