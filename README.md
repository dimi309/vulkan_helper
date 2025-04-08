vulkan_helper
=============

A less onerous way to use the Vulkan API.

[[Source Code]](https://github.com/dimi309/vulkan_helper) [[API Documentation]](https://dimi309.github.io/vulkan_helper)

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
	
Linux:
	
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

