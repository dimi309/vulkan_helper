vulkan_helper
=============

A less onerous way to use the Vulkan API.

[[Source Code]](https://github.com/dimi309/vkzos) [[API Documentation]](https://dimi309.github.io/vkzos)

Building & Testing
==================

Windows:
	
	cd resources
	cd shaders
	glslangValidator -V fragmentShader.frag -o fragmentShader.spv
	glslangValidator -V vertexShader.vert -o vertexShader.spv
	cd ..
	cd ..
	mkdir build
	cd build
	cmake .. -G"Visual Studio 17 2022"
	cmake --build .
	cd bin
	test.exe
	
Linux & MacOS:
	
	cd resources
	cd shaders
	glslangValidator -V fragmentShader.frag -o fragmentShader.spv
	glslangValidator -V vertexShader.vert -o vertexShader.spv
	cd ..
	cd ..
	mkdir build
	cd build
	cmake .. 
	cmake --build .
	cd bin
	./test

