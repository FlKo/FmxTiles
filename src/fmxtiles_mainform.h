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

#ifndef fmxtiles_mainformH
#define fmxtiles_mainformH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Ani.hpp>
#include <FMX.Gestures.hpp>

#include "fmxtiles_stage.h"
#include "fmxtiles_loopsync.h"
//---------------------------------------------------------------------------

class TMainForm : public TForm
{
__published:
    TPaintBox *pbStage;
    TGestureManager *gmStage;
    void __fastcall pbStageMouseMove(TObject *Sender, TShiftState Shift, float X, float Y);
    void __fastcall pbStageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, float X,
          float Y);
    void __fastcall pbStageMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, float X,
          float Y);
    void __fastcall pbStageMouseEnter(TObject *Sender);
    void __fastcall pbStageMouseLeave(TObject *Sender);
    void __fastcall pbStageGesture(TObject *Sender, const TGestureEventInfo &EventInfo, bool &Handled);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar, TShiftState Shift);
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key, System::WideChar &KeyChar, TShiftState Shift);

public:
    __fastcall TMainForm(TComponent* Owner);

private:
    Fmxtiles::TStage        FStage;
    Fmxtiles::TLoopSync     *FLoopSync;

#ifdef _DEBUG
    bool                    FIsPanning;
#endif
};
//---------------------------------------------------------------------------

extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
