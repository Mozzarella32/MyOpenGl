#include "Utilities.hpp"

#ifdef _DEBUG

void _GLGetError(const char* file, int line, const char* call) {
	std::ofstream o("Error.log", std::ios::app);
	//while (GLenum error = glGetError()) {
	GLenum error = glGetError();
	if (error) {
		o << "[OpenGL GLCALL Error] " << gluErrorString(error) << " in " << file << ":" << line << " Call: " << call << std::endl;
	}
	else {
		o << "[OpenGL GLCALL Sucess] Call: " << call << " in " << file << ":" << line << std::endl;
	}
	//}
}

void _ErrorLog(const char* file, int line, const char* message) {
	std::ofstream o("Error.log", std::ios::app);
	o << "[ErrorLog] Message: " << message << " in " << file << ":" << line << std::endl;
}

void _GLContextCheck() {
	std::ofstream o("Error.log", std::ios::app);
	int majorVersion, minorVersion;
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
	o << "[Context] Major: " << majorVersion
		<< " Minor: " << minorVersion
		<< "\nVendor:   " << glGetString(GL_VENDOR)
		<< "\nRenderer: " << glGetString(GL_RENDERER)
		<< "\nGlsl:     " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}

#endif