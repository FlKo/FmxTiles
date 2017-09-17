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

#include "fmxtiles_mastertilestack.h"

#include <System.IOUtils.hpp>
#include <System.Zip.hpp>

#include <memory>
#include <map>

#ifndef __clang__
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#endif

#include "fmxtiles_tile.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace Fmxtiles
{

#ifdef __clang__
using std::make_shared;
#else
using boost::make_shared;
#endif

TMasterTileStack::TMasterTileStack(const String& StorageFolder, bool ProcessPacks)
{
    if (DirectoryExists(StorageFolder, false))
        LoadFromFolder(StorageFolder, ProcessPacks);
}
//---------------------------------------------------------------------------

TMasterTileStack::~TMasterTileStack()
{

}
//---------------------------------------------------------------------------

void TMasterTileStack::LoadFromFolder(const String& StorageFolder, bool ProcessPacks)
{
    const String &Path = IncludeTrailingPathDelimiter(StorageFolder);

    TSearchRec SR;

    // Iterate over all files in the storage directory
    if (FindFirst(Path + L"*.*", faAnyFile, SR) == 0)
    {
        do
    {
            const String &FullPath  = Path + SR.Name;

            if ((SR.Name == L".") || (SR.Name == L".."))
            {
                continue;
            }
            else if (ExtractFileExt(SR.Name).LowerCase() == L".png")
            {
                const String &Name = ChangeFileExt(SR.Name, L"");

                FMasterTiles[Name] = make_shared<TMasterTile>(FullPath);
            }
            else if ((ExtractFileExt(SR.Name).LowerCase() == L".stpck") && ProcessPacks)
            {
                // Create a GUID string as a temporary folder name
                TGUID Guid;
                CreateGUID(Guid);
                const String& TempFolderName = GUIDToString(Guid);

                const String& TempFolderPath =
                    System::Ioutils::TPath::GetTempPath() + TempFolderName + L"/";

                // Create the directory
                ForceDirectories(TempFolderPath);

                __try
                {
                    // Create the pack TZipFile object
                    std::unique_ptr<TZipFile> Pack(new TZipFile);

                    // Assign the decompress stream event (for decryption)
                    Pack->OnCreateDecompressStream = OnCreateDecompressStream;

                    // Open the pack zip file
                    Pack->Open(FullPath, zmRead);

                    // Extract the pack content into the temporary folder
                    Pack->ExtractAll(TempFolderPath);

                    // Close the pack
                    Pack->Close();

                    // Recursively load the extracted files
                    LoadFromFolder(TempFolderPath, false);
                }
                __finally
                {
                    // Delete the temp directory if it exists
                    if (DirectoryExists(TempFolderPath), false)
                        TDirectory::Delete(TempFolderPath);
                }
            }
        } while (FindNext(SR) == 0);

        FindClose(SR);
    }
}
//---------------------------------------------------------------------------

TStream* __fastcall TMasterTileStack::OnCreateDecompressStream(
    TStream* const InStream,
    TZipFile* const ZipFile,
    const TZipHeader &Item,
    bool IsEncrypted
    )
{
    return InStream;
}
//---------------------------------------------------------------------------

} // namespace Fmxtiles

