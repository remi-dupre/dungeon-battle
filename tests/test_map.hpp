#include <cxxtest/TestSuite.h>

#include <map>
#include <string>
#include <random>

#include "../src/generation/pattern.hpp"
#include "../src/generation/room.hpp"
#include "../src/generation/level.hpp"
#include "../src/generation/gen_pattern.hpp"
#include "../src/generation/space.hpp"

#include "../src/utility.hpp"
#include "../src/map.hpp"



class MapTester : public CxxTest::TestSuite
{
public:
    /* Test that we can access a chunk only after its creation.
     */
    void testChunkExist()
    {
        Map map;

        // Check that the chunk (1337, 42) doesn't exist.
        TS_ASSERT(!map.hasChunk(1337, 42));
        // TS_ASSERT_THROWS_ANYTHING(map.chunkAt(1337, 42)); // seems assert doesn't throw an exception.

        // Check that it can be added and then accessed.
        map.setChunk(1337, 42, Chunk());

        TS_ASSERT(map.hasChunk(1337, 42));
        map.chunkAt(1337, 42);
    }

    /* Test that function to access a cell are ok.
     */
    void testCellExist()
    {
        Map map;

        // Check that the cell doesn't exist at first.
        TS_ASSERT(!map.hasCell(1337, 42));
        // TS_ASSERT_EQUALS(map.cellAt(1337, 42), CellType::Empty);

        // Check that we can correctly select it from the chunk.
        auto chunk_pos = Chunk::sector(1337, 42);
        auto relat_pos = Chunk::relative(1337, 42);

        TS_ASSERT(!map.hasChunk(chunk_pos.first, chunk_pos.second));

        // Create the chunk
        map.setChunk(chunk_pos.first, chunk_pos.second, Chunk());

        TS_ASSERT_EQUALS(map.cellAt(1337, 42), CellType::Empty);
        TS_ASSERT(map.hasChunk(chunk_pos.first, chunk_pos.second));
        TS_ASSERT(map.hasCell(1337, 42));

        map.cellAt(1337, 42) = CellType::Floor;
        TS_ASSERT_EQUALS(map.cellAt(1337, 42), CellType::Floor);


        auto chunk = map.chunkAt(chunk_pos.first, chunk_pos.second);
        TS_ASSERT_EQUALS(chunk.cellAt(relat_pos.first, relat_pos.second), CellType::Floor);
    }
};
