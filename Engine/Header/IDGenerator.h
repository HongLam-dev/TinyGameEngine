#pragma once
#include <random>
#include<unordered_set>

namespace TinyEngine {
	class IDGenerator
	{
	public:
		static uint64_t GenerateID()
		{
			static std::random_device rd;
			static std::mt19937_64 generator(rd());

			static std::uniform_int_distribution<uint64_t> distribution;

			uint64_t id = distribution(generator);

			do
			{
				id = distribution(generator);
			} while (!Instance().usedIDs.insert(id).second);
			return id;
		}
		static void RemoveID(uint64_t id)
		{
			Instance().usedIDs.erase(id);
		}

		static bool AddID(uint64_t id) {
			return Instance().usedIDs.insert(id).second;
		}
	private:
		static IDGenerator& Instance() {
			static IDGenerator generator;
			return generator;
		}
		std::unordered_set<uint64_t> usedIDs;
	};

}