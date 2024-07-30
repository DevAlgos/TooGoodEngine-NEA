#pragma once

#include "Core/Base.h"
#include "ECS/MemoryAllocator.h"

#include <filesystem>
#include <fstream>

namespace TooGoodEngine {

	class FileReader
	{
	public:
		FileReader(const std::filesystem::path& path);
		~FileReader();

		inline const size_t GetPosition() { _VerifyStream(); return m_File.tellg(); };

		StrongRef<MemoryAllocator> Read(size_t bytes);
		void  SeekPosition(size_t position);
		const size_t GetSize();

	private:
		void _VerifyStream();

	private:
		std::ifstream m_File;
	};

}