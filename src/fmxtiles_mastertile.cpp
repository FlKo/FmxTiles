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

#include "fmxtiles_mastertile.h"

#include <memory>
#include <map>

#ifndef __clang__
#include <boost/shared_ptr.hpp>
#endif
//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace Fmxtiles
{

using Fmx::Graphics::TBitmap;
//---------------------------------------------------------------------------

TMasterTile::TMasterTile(const String& FilePath)
    :   FBitmap(new TBitmap)
{
    // Check whether the image file exists
    if (!FileExists(FilePath))
        throw Exception(L"Tile image '" + FilePath + L"' does not exist");

    // Load the image
    FBitmap->LoadFromFile(FilePath);

    // Get the file name without extension
    FName = ChangeFileExt(ExtractFileName(FilePath), L"");
}
//---------------------------------------------------------------------------

TMasterTile::~TMasterTile()
{

}
//---------------------------------------------------------------------------

} // namespace Fmxtiles
