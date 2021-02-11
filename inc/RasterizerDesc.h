#include <cstdint>

struct RasterizerDesc {
	uint32_t Height;
	uint32_t Width;
	Format BackBufferFormat;
	PrimitiveTopology Topology;
};

enum class PrimitiveTopology {
	Triangle,
	Line
};

enum class Format {
	R8G8B8A8,
	R8G8B8,
};