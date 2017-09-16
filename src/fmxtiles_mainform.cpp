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

#include "fmxtiles_mainform.h"

#include "fmxtiles_stage.h"
#include "fmxtiles_loopsync.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.fmx"

TMainForm *MainForm;
//---------------------------------------------------------------------------

using namespace Fmxtiles;
using Ani::TAnimator;

__fastcall TMainForm::TMainForm(TComponent* Owner)
    :   TForm(Owner),
        // Create the stage
        FStage(
            pbStage->Canvas,
            pbStage->Width,
            pbStage->Height,
            40,
            240,
            ExtractFilePath(ParamStr(0)) + L"tiles",
            ExtractFilePath(ParamStr(0)) + L"gui",
            ExtractFilePath(ParamStr(0)) + L"sfx"
            ),
        // Create the loop sync
        FLoopSync(new TLoopSync(this, FStage))
{
    // Set the loop sync's parameters
    FLoopSync->Loop     = true;
    FLoopSync->Name     = L"LoopSync";
    FLoopSync->Parent   = this;

    // Start the loop sync
    TAnimator::StartAnimation(this, L"LoopSync");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::pbStageMouseMove(TObject *Sender, TShiftState Shift, float X, float Y)
{
    FStage.HandleMouseMove(X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::pbStageMouseDown(
    TObject *Sender,
    TMouseButton Button,
    TShiftState Shift,
    float X,
    float Y
    )
{
    FStage.HandleMouseDown(Button);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::pbStageMouseUp(
    TObject *Sender,
    TMouseButton Button,
    TShiftState Shift,
    float X,
    float Y
    )
{
    FStage.HandleMouseUp(Button);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::pbStageMouseEnter(TObject *Sender)
{
    FStage.HandleMouseEnter();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::pbStageMouseLeave(TObject *Sender)
{
    FStage.HandleMouseLeave();
}
//---------------------------------------------------------------------------

