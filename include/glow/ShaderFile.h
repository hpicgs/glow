#pragma once

#include <set>
#include <string>

#include <glow/declspec.h>


namespace glow
{

class Shader;

class GLOW_API ShaderFile
{
public:
	ShaderFile(const std::string& filePath);

	virtual ~ShaderFile();

	void registerShader(Shader* shader);
	void deregisterShader(Shader* shader);

	void reload();
	static void reloadAll();
protected:
	bool loadFileContent();
protected:
	std::string _filePath;
	std::set<Shader*> _shaders;
	std::string _fileContent;
protected:
	typedef std::set<ShaderFile*> FileRegistry;
	static FileRegistry _fileRegistry;

	static void registerFile(ShaderFile* file);
	static void deregisterFile(ShaderFile* file);
};

} // namespace glow