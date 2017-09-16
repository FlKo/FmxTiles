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

#include "fmxtiles_loopsync.h"

#include <System.Diagnostics.hpp>

#include <memory>
#include <map>

#ifndef __clang__
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#endif

#include "fmxtiles_stage.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace Fmxtiles
{

#ifdef __clang__
using std::make_shared;
#else
using boost::make_shared;
#endif
//---------------------------------------------------------------------------

__fastcall TLoopSync::TLoopSync(TComponent* AOwner, TStage& Stage)
    :   TAnimation(AOwner),
        FStage(Stage),
        FLastElapsedTicks(0)
{
}
//---------------------------------------------------------------------------

__fastcall TLoopSync::~TLoopSync()
{

}
//---------------------------------------------------------------------------

void TLoopSync::GetTimeValues(float& Delta, float& Absolute)
{
    __int64 ElapsedTicks    = FWatch.ElapsedTicks;
    __int64 TickDelta       = ElapsedTicks - FLastElapsedTicks;

    FLastElapsedTicks       = ElapsedTicks;

    Delta = static_cast<float>(static_cast<double>(TickDelta) / FWatch.Frequency);
    Absolute = static_cast<float>(static_cast<double>(ElapsedTicks) / FWatch.Frequency);
}
//---------------------------------------------------------------------------


void __fastcall TLoopSync::FirstFrame()
{
    // We need to call 'Create' here since TStopWatch is a struct that depends
    // on one to call the function manually (it is assumed that one calls
    // Create as the constructor from Object Pascal)
    FWatch.Create();

    // Run the watch
    FWatch.Start();
}
//---------------------------------------------------------------------------

void __fastcall TLoopSync::ProcessAnimation()
{
    float Delta;
    float Absolute;

    // Get the current time values
    GetTimeValues(Delta, Absolute);

    // Process the user input
    FStage.ProcessUserInput(Delta, Absolute);

    // Process events
    FStage.ProcessEvents(Delta, Absolute);

    // Begin the scene drawing
    FStage.BeginScene();

    // Clear the current scene
    FStage.Clear();

    // Draw the tile layer
    FStage.DrawTiles(Delta, Absolute);

    // Draw the character layer
    FStage.DrawCharacters(Delta, Absolute);

    // Draw the GUI layer
    FStage.DrawGui(Delta, Absolute);

    // Manage Sfx
    FStage.ManageSfx(Delta, Absolute);

    // End the scene drawing
    FStage.EndScene();
}
//---------------------------------------------------------------------------

} // namespace Fmxtiles

