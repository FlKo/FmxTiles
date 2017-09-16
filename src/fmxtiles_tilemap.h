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

#ifndef fmxtiles_tilemapH
#define fmxtiles_tilemapH
//---------------------------------------------------------------------------

#include <vector>
#include <memory>
#include <map>

#ifndef __clang__
#include <boost/shared_ptr.hpp>
#endif

#include "fmxtiles_tile.h"
#include "fmxtiles_mastertilestack.h"
//---------------------------------------------------------------------------

namespace Fmxtiles
{

#ifdef __clang__
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif
//---------------------------------------------------------------------------

class TTileMap
{
public:
    TTileMap(
        TMasterTileStack& MasterTileStack,
        std::size_t SizeX=10,
        std::size_t SizeY=10,
        std::size_t SizeZ=1,
        int TileWidth=64,
        int TileHeight=64
        );

    virtual ~TTileMap();

    __property TMasterTileStack& MasterTileStack = {read=FMasterTileStack};

    __property std::size_t SizeX    = {read=FSizeX};
    __property std::size_t SizeY    = {read=FSizeY};
    __property std::size_t SizeZ    = {read=FSizeZ};

    __property int TileWidth    = {read=FTileWidth};
    __property int TileHeight   = {read=FTileHeight};

    __property std::vector<std::vector<std::vector<TTile> > > Tiles = {read=FTiles};

private:
    TMasterTileStack &FMasterTileStack;

    std::size_t     FSizeX;
    std::size_t     FSizeY;
    std::size_t     FSizeZ;
    int             FTileWidth;
    int             FTileHeight;

    std::vector<std::vector<std::vector<TTile> > >  FTiles;
};
//---------------------------------------------------------------------------

} // namespace Fmxtiles

#endif

