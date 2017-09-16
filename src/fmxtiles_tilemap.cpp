/* ===============================================================================
 * FmxTiles - A C++Builder 2.5D tile engine with FireMonkey(FMX)
 * ===============================================================================
 * MIT License
 *
 * Copyright (c) 2017 Florian Koch <flko@mail.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ===============================================================================
 */

#include <fmx.h>
#pragma hdrstop

#include "fmxtiles_tilemap.h"

#include <vector>
#include <memory>
#include <map>

#ifndef __clang__
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#endif

#include "fmxtiles_tile.h"
#include "fmxtiles_mastertilestack.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace Fmxtiles
{

#ifdef __clang__
using std::make_shared;
#else
using boost::make_shared;
#endif

TTileMap::TTileMap(
    TMasterTileStack& MasterTileStack,
    std::size_t SizeX,
    std::size_t SizeY,
    std::size_t SizeZ,
    int TileWidth,
    int TileHeight
    )
    :   FMasterTileStack(MasterTileStack),
        FSizeX(SizeX),
        FSizeY(SizeY),
        FSizeZ(SizeZ),
        FTileWidth(TileWidth),
        FTileHeight(TileHeight)
{
    FTiles.resize(FSizeX);

    for (std::size_t x = 0; x < FSizeX; ++x)
    {
        FTiles[x].resize(FSizeY);

        for (std::size_t y = 0; y < FSizeY; ++y)
        {
            FTiles[x][y].resize(FSizeZ);

            for (std::size_t z = 0; z < FSizeZ; ++z)
            {
                FTiles[x][y][z].MasterTile  = FMasterTileStack[L"00000"];
                FTiles[x][y][z].IsSelected  = false;
            }
        }
    }
}
//---------------------------------------------------------------------------

TTileMap::~TTileMap()
{

}
//---------------------------------------------------------------------------

} // namespace Fmxtiles

