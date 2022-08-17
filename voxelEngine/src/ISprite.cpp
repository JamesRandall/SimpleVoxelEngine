#include "ISprite.h"
#include <fstream>
#include <vector>
#include <cstring>
#include "VoxelSprite.h"
#include <algorithm>
#include "SpriteVoxel.h"

class MV_Vec3i {
public:
	int x, y, z;

	MV_Vec3i() {
		x = y = z = 0;
	}

	MV_Vec3i &Set(int x, int y, int z) {
		this->x = x;
		this->y = y;
		this->z = z;
		return *this;
	}

	MV_Vec3i &Zero(void) {
		x = y = z = 0;
		return *this;
	}
};

class MV_Voxel {
public:
	unsigned char x, y, z, colorIndex;

	MV_Voxel() {
		x = y = z = colorIndex = 0;
	}
};

class MV_RGBA {
public:
	unsigned char r, g, b, a;

	MV_RGBA() {
		r = g = b = a = 0;
	}
};


const unsigned int mv_default_palette[256] = {
	0x00000000, 0xffffffff, 0xffccffff, 0xff99ffff, 0xff66ffff, 0xff33ffff, 0xff00ffff, 0xffffccff, 0xffccccff, 0xff99ccff, 0xff66ccff, 0xff33ccff, 0xff00ccff, 0xffff99ff, 0xffcc99ff, 0xff9999ff,
	0xff6699ff, 0xff3399ff, 0xff0099ff, 0xffff66ff, 0xffcc66ff, 0xff9966ff, 0xff6666ff, 0xff3366ff, 0xff0066ff, 0xffff33ff, 0xffcc33ff, 0xff9933ff, 0xff6633ff, 0xff3333ff, 0xff0033ff, 0xffff00ff,
	0xffcc00ff, 0xff9900ff, 0xff6600ff, 0xff3300ff, 0xff0000ff, 0xffffffcc, 0xffccffcc, 0xff99ffcc, 0xff66ffcc, 0xff33ffcc, 0xff00ffcc, 0xffffcccc, 0xffcccccc, 0xff99cccc, 0xff66cccc, 0xff33cccc,
	0xff00cccc, 0xffff99cc, 0xffcc99cc, 0xff9999cc, 0xff6699cc, 0xff3399cc, 0xff0099cc, 0xffff66cc, 0xffcc66cc, 0xff9966cc, 0xff6666cc, 0xff3366cc, 0xff0066cc, 0xffff33cc, 0xffcc33cc, 0xff9933cc,
	0xff6633cc, 0xff3333cc, 0xff0033cc, 0xffff00cc, 0xffcc00cc, 0xff9900cc, 0xff6600cc, 0xff3300cc, 0xff0000cc, 0xffffff99, 0xffccff99, 0xff99ff99, 0xff66ff99, 0xff33ff99, 0xff00ff99, 0xffffcc99,
	0xffcccc99, 0xff99cc99, 0xff66cc99, 0xff33cc99, 0xff00cc99, 0xffff9999, 0xffcc9999, 0xff999999, 0xff669999, 0xff339999, 0xff009999, 0xffff6699, 0xffcc6699, 0xff996699, 0xff666699, 0xff336699,
	0xff006699, 0xffff3399, 0xffcc3399, 0xff993399, 0xff663399, 0xff333399, 0xff003399, 0xffff0099, 0xffcc0099, 0xff990099, 0xff660099, 0xff330099, 0xff000099, 0xffffff66, 0xffccff66, 0xff99ff66,
	0xff66ff66, 0xff33ff66, 0xff00ff66, 0xffffcc66, 0xffcccc66, 0xff99cc66, 0xff66cc66, 0xff33cc66, 0xff00cc66, 0xffff9966, 0xffcc9966, 0xff999966, 0xff669966, 0xff339966, 0xff009966, 0xffff6666,
	0xffcc6666, 0xff996666, 0xff666666, 0xff336666, 0xff006666, 0xffff3366, 0xffcc3366, 0xff993366, 0xff663366, 0xff333366, 0xff003366, 0xffff0066, 0xffcc0066, 0xff990066, 0xff660066, 0xff330066,
	0xff000066, 0xffffff33, 0xffccff33, 0xff99ff33, 0xff66ff33, 0xff33ff33, 0xff00ff33, 0xffffcc33, 0xffcccc33, 0xff99cc33, 0xff66cc33, 0xff33cc33, 0xff00cc33, 0xffff9933, 0xffcc9933, 0xff999933,
	0xff669933, 0xff339933, 0xff009933, 0xffff6633, 0xffcc6633, 0xff996633, 0xff666633, 0xff336633, 0xff006633, 0xffff3333, 0xffcc3333, 0xff993333, 0xff663333, 0xff333333, 0xff003333, 0xffff0033,
	0xffcc0033, 0xff990033, 0xff660033, 0xff330033, 0xff000033, 0xffffff00, 0xffccff00, 0xff99ff00, 0xff66ff00, 0xff33ff00, 0xff00ff00, 0xffffcc00, 0xffcccc00, 0xff99cc00, 0xff66cc00, 0xff33cc00,
	0xff00cc00, 0xffff9900, 0xffcc9900, 0xff999900, 0xff669900, 0xff339900, 0xff009900, 0xffff6600, 0xffcc6600, 0xff996600, 0xff666600, 0xff336600, 0xff006600, 0xffff3300, 0xffcc3300, 0xff993300,
	0xff663300, 0xff333300, 0xff003300, 0xffff0000, 0xffcc0000, 0xff990000, 0xff660000, 0xff330000, 0xff0000ee, 0xff0000dd, 0xff0000bb, 0xff0000aa, 0xff000088, 0xff000077, 0xff000055, 0xff000044,
	0xff000022, 0xff000011, 0xff00ee00, 0xff00dd00, 0xff00bb00, 0xff00aa00, 0xff008800, 0xff007700, 0xff005500, 0xff004400, 0xff002200, 0xff001100, 0xffee0000, 0xffdd0000, 0xffbb0000, 0xffaa0000,
	0xff880000, 0xff770000, 0xff550000, 0xff440000, 0xff220000, 0xff110000, 0xffeeeeee, 0xffdddddd, 0xffbbbbbb, 0xffaaaaaa, 0xff888888, 0xff777777, 0xff555555, 0xff444444, 0xff222222, 0xff111111
};

int MV_ID(int a, int b, int c, int d) {
	return (a) | (b << 8) | (c << 16) | (d << 24);
}

struct voxChunk {
	int id;
	int contentSize;
	int childrenSize;
	int end;
};

void readChunk(std::ifstream& spriteStream, voxChunk& voxChunk)
{
	spriteStream.read(reinterpret_cast<char*>(&voxChunk.id), sizeof(int));
	spriteStream.read(reinterpret_cast<char*>(&voxChunk.contentSize), sizeof(int));
	spriteStream.read(reinterpret_cast<char*>(&voxChunk.childrenSize), sizeof(int));
	voxChunk.end = static_cast<int>(spriteStream.tellg()) + voxChunk.contentSize + voxChunk.childrenSize;
}

unsigned int getVoxelIndex(spriteSize size, unsigned int x, unsigned int y, unsigned int z) { return z * size.height * size.width + y * size.width + x; }

std::shared_ptr<std::vector<std::shared_ptr<ISpriteVoxel>>> loadSpriteVoxels(std::string spriteFilename, spriteSize& voxelSpriteSize)
{
	const int ID_VOX = MV_ID('V', 'O', 'X', ' ');
	const int ID_MAIN = MV_ID('M', 'A', 'I', 'N');
	const int ID_SIZE = MV_ID('S', 'I', 'Z', 'E');
	const int ID_XYZI = MV_ID('X', 'Y', 'Z', 'I');
	const int ID_RGBA = MV_ID('R', 'G', 'B', 'A');
	const int VERSION = 150;

	std::vector<MV_Voxel> voxels;
	MV_RGBA palette[256];

	std::memcpy(palette, mv_default_palette, sizeof(mv_default_palette));

	std::ifstream spriteStream(spriteFilename, std::ios::in | std::ios::binary);
	if (!spriteStream.is_open())
	{
		return nullptr;
	}

	int header;
	int versionNumber;

	spriteStream.read(reinterpret_cast<char*>(&header), sizeof(int));
	if (header != ID_VOX)
	{
		return nullptr;
	}

	spriteStream.read(reinterpret_cast<char*>(&versionNumber), sizeof(int));
	if (versionNumber != VERSION)
	{
		return nullptr;
	}

	voxChunk mainChunk;
	readChunk(spriteStream, mainChunk);
	MV_Vec3i size;

	if (mainChunk.id != ID_MAIN)
	{
		return nullptr;
	}
	spriteStream.seekg(mainChunk.contentSize, std::ios::cur);

	while (spriteStream.tellg() < mainChunk.end)
	{
		voxChunk subChunk;
		readChunk(spriteStream, subChunk);
		if (subChunk.id == ID_SIZE) {
			// size
			spriteStream.read(reinterpret_cast<char*>(&size), sizeof(MV_Vec3i));
			if (size.x <= 0 || size.y <= 0 || size.z <= 0) {
				printf("error : invalid size : %d %d %d\n", size.x, size.y, size.z);
				return nullptr;
			}
		}
		else if (subChunk.id == ID_XYZI) {
			// numVoxels
			int numVoxels;
			spriteStream.read(reinterpret_cast<char*>(&numVoxels), sizeof(int));
			if (numVoxels < 0) {
				printf("error : negative number of voxels : %d\n", numVoxels);
				return nullptr;
			}

			// voxels
			if (numVoxels > 0) {
				voxels.resize(numVoxels);
				spriteStream.read(reinterpret_cast<char*>(voxels.data()), sizeof(MV_Voxel) * numVoxels);
			}
		}
		else if (subChunk.id == ID_RGBA) {
			// NOTICE : the color 0~254 are mapped to the palette 1~255
			spriteStream.read(reinterpret_cast<char*>(palette + 1), sizeof(MV_RGBA) * 255);

			// NOTICE : skip the last unused color
			MV_RGBA unused;
			spriteStream.read(reinterpret_cast<char*>(&unused), sizeof(MV_RGBA));
		}
		spriteStream.seekg(subChunk.end, std::ios::beg);
	}

	// we flip vox z to our y
	voxelSpriteSize = spriteSize(size.x, size.z, size.y);
	unsigned vectorSize = size.x * size.z * size.y;
	std::shared_ptr<std::vector<std::shared_ptr<ISpriteVoxel>>> spriteVoxels = std::make_shared<std::vector<std::shared_ptr<ISpriteVoxel>>>();
	spriteVoxels->reserve(vectorSize);
	for (unsigned index = 0; index < vectorSize; index++)
	{
		spriteVoxels->push_back(nullptr);
	}

	std::for_each(voxels.begin(), voxels.end(), [&palette, &spriteVoxels, voxelSpriteSize](const MV_Voxel& mvVoxel)
	{
		MV_RGBA* paletteColor = static_cast<MV_RGBA*>(&palette[mvVoxel.colorIndex]);
		color voxelColor(paletteColor->r / 255.0f, paletteColor->g / 255.0f, paletteColor->b / 255.0f);
		std::shared_ptr<ISpriteVoxel> voxel = std::make_shared<SpriteVoxel>(voxelColor, true);
		(*spriteVoxels)[getVoxelIndex(voxelSpriteSize, mvVoxel.x, mvVoxel.z, mvVoxel.y)] = voxel;
	});
	
	return spriteVoxels;
}

std::shared_ptr<ISprite> ISprite::load(std::string spriteFilename, unsigned type)
{
	spriteSize size(0,0,0);
	std::shared_ptr<std::vector<std::shared_ptr<ISpriteVoxel>>> voxels = loadSpriteVoxels(spriteFilename, size);
	std::shared_ptr<VoxelSprite> voxelSprite = std::make_shared<VoxelSprite>(size, spriteVec3(0, 0, 0), *voxels);
	voxelSprite->setType(type);
	return voxelSprite;
} 

std::shared_ptr<ISprite> ISprite::load(const std::vector<std::string>& spriteFrameFilenames, float timeBetweenFrames, float frameChangeDeferral, unsigned type)
{
	spriteSize size(0, 0, 0);
	std::vector<std::vector<std::shared_ptr<ISpriteVoxel>>> frameVoxels;

	std::for_each(std::begin(spriteFrameFilenames), std::end(spriteFrameFilenames), [&frameVoxels, &size](std::string filename)
	{
		std::shared_ptr<std::vector<std::shared_ptr<ISpriteVoxel>>> voxels = loadSpriteVoxels(filename, size);
		frameVoxels.push_back(*voxels);
	});

	std::shared_ptr<VoxelSprite> voxelSprite = std::make_shared<VoxelSprite>(size, spriteVec3(0, 0, 0), frameVoxels, timeBetweenFrames, frameChangeDeferral);
	voxelSprite->setType(type);
	return voxelSprite;
}