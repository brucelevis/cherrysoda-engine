#include "Chunk.h"

#include "Voxel/World.h"

#include "ChunkGraphicsComponent.h"

using cherrysoda::STL;

Chunk::Chunk()
{
	STL::Resize(m_blocks, BlocksAmount());
	STL::Fill(m_blocks, Block());

	Add(new ChunkGraphicsComponent);
}

int Chunk::GetBlockSurrounding(int x, int y, int z)
{
	int result = 0;
	if (GetBlockType(x + 1, y, z) == Block::Type::None) {
		result |= 1;
	}
	if (GetBlockType(x - 1, y, z) == Block::Type::None) {
		result |= 1 << 1;
	}
	if (GetBlockType(x, y + 1, z) == Block::Type::None) {
		result |= 1 << 2;
	}
	if (GetBlockType(x, y - 1, z) == Block::Type::None) {
		result |= 1 << 3;
	}
	if (GetBlockType(x, y, z + 1) == Block::Type::None) {
		result |= 1 << 4;
	}
	if (GetBlockType(x, y, z - 1) == Block::Type::None) {
		result |= 1 << 5;
	}
	return result;
}

Block* Chunk::GetBlock(int x, int y, int z)
{
	int index = GetBlockIndex(x, y, z); 
	if (index < 0) {
		if (m_world) {
			int wx = x + static_cast<int>(Position()[0] + 0.5f);
			int wy = y + static_cast<int>(Position()[1] + 0.5f);
			int wz = z + static_cast<int>(Position()[2] + 0.5f);
			return m_world->GetBlock(wx, wy, wz);
		}
		return nullptr;
	}
	return GetBlocks() + index;
}

void Chunk::FillAllBlocks(Block::Type type)
{
	for (int i = 0; i < BlocksAmount(); ++i)
	{
		GetBlocks()[i].m_type = type;
	}
}

void Chunk::Update()
{
	base::Update();

	if (m_changed) {
		Get<ChunkGraphicsComponent>()->RebuildMesh();
		m_changed = false;
	}
}