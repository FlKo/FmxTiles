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

#include "fmxtiles_stage.h"

#include <FMX.Media.hpp>

#include <memory>
#include <map>

#ifndef __clang__
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#endif

#include "fmxtiles_tilemap.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace Fmxtiles
{

#ifdef __clang__
using std::make_shared;
#else
using boost::make_shared;
#endif

TStage::TStage(
    TCanvas* Canvas,
    float CanvasWidth,
    float CanvasHeight,
    float ScrollBorderWidth,
    float ScrollSpeed,
    const String& TileStorageDir,
    const String& GuiElementsDir,
    const String& SfxDir
    )
    :   FCanvas(Canvas),
        FCanvasWidth(CanvasWidth),
        FCanvasHeight(CanvasHeight),
        FScrollBorderWidth(ScrollBorderWidth),
        FScrollSpeed(ScrollSpeed),
        FBlinkerOpacity(1.0),
        FBlinkerCountDown(false),
        FFPS(0.0),
        FFPSCycle(0),
        FFPSTimer(0.0),
        FMasterTileStack(TileStorageDir, true),
        FGuiElementStack(GuiElementsDir, true),
        FSfxDir(IncludeTrailingPathDelimiter(SfxDir)),
        FTileMap(FMasterTileStack, 100, 100, 1, 64, 32),
        FSfxMusicPlayer(new TMediaPlayer(NULL))
{
    // Set the camera pos to center
    FCameraPos = TPointF(FCanvasWidth / 2.0, FCanvasHeight / 4.0);

    // Set the GUI main interface
    FGuiMainInterface = FGuiElementStack[L"00000"];
}
//---------------------------------------------------------------------------

TStage::~TStage()
{

}
//---------------------------------------------------------------------------

void TStage::BeginScene()
{
    FCanvas->BeginScene();
}
//---------------------------------------------------------------------------

void TStage::EndScene()
{
    FCanvas->EndScene();
}
//---------------------------------------------------------------------------

void TStage::ProcessUserInput(float Delta, float Absolute)
{
    // Check if mouse is within the stage range
    if (!FMouseOptions.InStage)
        return;

    // Calculate the stage's rect
    TRect RC = TRect(0, 0, FCanvasWidth, FCanvasHeight);

    // Abbreviate some variable names needed for the next calculations
    int WD  = FCanvasWidth;
    int HG  = FCanvasHeight;
    int BW  = FScrollBorderWidth;

    // Calculate the scroll area rects
    TRect RLeft     = TRect(0, 0, BW, HG);
    TRect RTop      = TRect(0, 0, WD, BW);
    TRect RRight    = TRect(WD - BW, 0, WD, HG);
    TRect RBottom   = TRect(0, HG - BW, WD, HG);

    // Set the correct flags when the mouse cursor is within a scroll area
    bool MouseInLeftScrollArea      = PtInRect(RLeft, FMouseOptions.Pos);
    bool MouseInTopScrollArea       = PtInRect(RTop, FMouseOptions.Pos);
    bool MouseInRightScrollArea     = PtInRect(RRight, FMouseOptions.Pos);
    bool MouseInBottomScrollArea    = PtInRect(RBottom, FMouseOptions.Pos);

    // Calculate the new camera position if we have a mouse scroll
    if (MouseInLeftScrollArea)
        FCameraPos.x += FScrollSpeed * Delta;
    if (MouseInTopScrollArea)
        FCameraPos.y += FScrollSpeed * Delta;
    if (MouseInRightScrollArea)
        FCameraPos.x -= FScrollSpeed * Delta;
    if (MouseInBottomScrollArea)
        FCameraPos.y -= FScrollSpeed * Delta;

    // Get the current tile coordinate below the mouse cursor
    FMouseOptions.TileNo = GetMapTileCoord(FMouseOptions.Pos, FCameraPos);

    if (
        (FMouseOptions.TileNo.x < 0)
            || (FMouseOptions.TileNo.y < 0)
            || (FMouseOptions.TileNo.x >= static_cast<long>(FTileMap.SizeX))
            || (FMouseOptions.TileNo.y >= static_cast<long>(FTileMap.SizeY))
            )
    {
        return;
    }

    TTile& CurrentTile = FTileMap.Tiles[FMouseOptions.TileNo.x][FMouseOptions.TileNo.y][0];

    // If we have a left mouse click...
    if (FMouseOptions.ClickLeft)
        CurrentTile.IsSelected = true;

    // If we have a right mouse click...
    if (FMouseOptions.ClickRight)
        CurrentTile.IsSelected = false;
}
//---------------------------------------------------------------------------

void TStage::ProcessEvents(float Delta, float Absolute)
{

}
//---------------------------------------------------------------------------

void TStage::Clear()
{
    FillRect(TRect(0, 0, FCanvasWidth, FCanvasHeight), claBlack);
}
//---------------------------------------------------------------------------

void TStage::DrawTiles(float Delta, float Absolute)
{
    // If FPS timer has expired...
    if (FFPSTimer >= 1.0)
    {
        // ...reset FPS counting
        FFPSTimer   = 0;
        FFPS        = FFPSCycle;
        FFPSCycle   = 0.0;
    }

    FFPSTimer += Delta;
    ++FFPSCycle;

    if (!FBlinkerCountDown)
    {
        FBlinkerOpacity += Delta;
        FBlinkerCountDown = FBlinkerOpacity >= 0.9 ? true : false;
    }
    else
    {
        FBlinkerOpacity -= Delta;
        FBlinkerCountDown = FBlinkerOpacity <= 0.7 ? false : true;
    }

    for (std::size_t z = 0; z < FTileMap.SizeZ; ++z)
    {
        for (std::size_t y = 0; y < FTileMap.SizeY; ++y)
        {
            for (std::size_t x = 0; x < FTileMap.SizeX; ++x)
            {
                TTile& CurrentTile = FTileMap.Tiles[x][y][z];

                TPoint DrawPos =
                    GetDrawPos(
                        x,
                        y,
                        z,
                        FCameraPos.x,
                        FCameraPos.y,
                        CurrentTile.MasterTile->Bitmap->Width,
                        CurrentTile.MasterTile->Bitmap->Height
                        );

                if ((DrawPos.x >= -CurrentTile.MasterTile->Bitmap->Width)
                        && (DrawPos.x < FCanvasWidth)
                        && (DrawPos.Y >= -CurrentTile.MasterTile->Bitmap->Width)
                        && (DrawPos.y < FCanvasHeight)
                        )
                {
                    float HighlightOpacity = 1.0;

                    if (CurrentTile.IsSelected)
                        HighlightOpacity = FBlinkerOpacity;

                    DrawBitmap(
                        DrawPos.x,
                        DrawPos.y,
                        HighlightOpacity,
                        CurrentTile.MasterTile->Bitmap.get()
                        );

                    if (FMouseOptions.TileNo == TPoint(x, y))
                    {
                        DrawBitmap(
                            DrawPos.x,
                            DrawPos.y,
                            1.0,
                            FMasterTileStack[L"00080"]->Bitmap.get()
                            );
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------

void TStage::DrawCharacters(float Delta, float Absolute)
{

}
//---------------------------------------------------------------------------

void TStage::DrawGui(float Delta, float Absolute)
{
    DrawBitmap(0.0, 0.0, 1.0, FGuiMainInterface->Bitmap.get());

    TextOut(
        128,
        43,
        String().sprintf(L"%.2f fps", FFPS),
        "Goudy Old Style",
        15,
        0xFF815122,
        true,
        0xFF5C361B
        );
}
//---------------------------------------------------------------------------

void TStage::ManageSfx(float Delta, float Absolute)
{
    if (!FSfxOptions.MusicIsPlaying)
    {
        FSfxOptions.MusicIsPlaying          = true;
        FSfxOptions.MusicFadeDuration       = 4.0;
        FSfxOptions.MusicFadeStartTime      = Absolute;
        FSfxOptions.MusicFadeInIsActive     = true;

        // Load the music file
        FSfxMusicPlayer->FileName = FSfxDir + L"00000.staf";

        // Set the start position to a random one
        FSfxMusicPlayer->CurrentTime =
            GetRandomPosition(FSfxMusicPlayer->Duration / MediaTimeScale) * MediaTimeScale;

        // Set the volume to 0
        FSfxMusicPlayer->Volume = 0.0;

        // Play the file
        FSfxMusicPlayer->Play();
    }

    if (FSfxOptions.MusicFadeInIsActive)
    {
        float CurrentVolume =
            (Absolute - FSfxOptions.MusicFadeStartTime) / FSfxOptions.MusicFadeDuration;

        if (CurrentVolume >= 1.0)
        {
            FSfxOptions.MusicFadeInIsActive = false;
            CurrentVolume = 1.0;
        }

        FSfxMusicPlayer->Volume = CurrentVolume;
    }

    if (FSfxOptions.MusicFadeOutIsActive)
    {
        float CurrentVolume =
            1.0 - ((Absolute - FSfxOptions.MusicFadeStartTime) / FSfxOptions.MusicFadeDuration);

        if (CurrentVolume <= 1.0)
        {
            FSfxOptions.MusicFadeOutIsActive = false;
            CurrentVolume = 0.0;
        }

        FSfxMusicPlayer->Volume = CurrentVolume;
    }
}
//---------------------------------------------------------------------------

void TStage::HandleMouseMove(float MousePosX, float MousePosY)
{
    FMouseOptions.Pos = TPoint(MousePosX, MousePosY);
}
//---------------------------------------------------------------------------

void TStage::HandleMouseDown(TMouseButton Button)
{
    switch (Button)
    {
        case TMouseButton::mbLeft:      FMouseOptions.ClickLeft     = true;     break;
        case TMouseButton::mbRight:     FMouseOptions.ClickRight    = true;     break;
    }
}
//---------------------------------------------------------------------------

void TStage::HandleMouseUp(TMouseButton Button)
{
    switch (Button)
    {
        case TMouseButton::mbLeft:      FMouseOptions.ClickLeft     = false;    break;
        case TMouseButton::mbRight:     FMouseOptions.ClickRight    = false;    break;
    }
}
//---------------------------------------------------------------------------

void TStage::HandleMouseEnter()
{
    FMouseOptions.InStage = true;
}
//---------------------------------------------------------------------------

void TStage::HandleMouseLeave()
{
    FMouseOptions.InStage = false;
}
//---------------------------------------------------------------------------

void TStage::DrawBitmap(int X, int Y, float Opacity, TBitmap* Bitmap)
{
    TRectF SourceRect   = RectF(0, 0, Bitmap->Width, Bitmap->Height);
    TRectF DestRect     = RectF(X, Y, Bitmap->Width + X, Bitmap->Height + Y);

    FCanvas->DrawBitmap(Bitmap, SourceRect, DestRect, Opacity, true);
}
//---------------------------------------------------------------------------

TPoint TStage::GetMapTileCoord(
    TPointF ScreenPos,
    TPointF CameraPos
    )
{
    TPoint RetVal;

    RetVal.x =
        ((ScreenPos.x - CameraPos.x) / (FTileMap.TileWidth/2)
            + ((ScreenPos.y - CameraPos.y) / (FTileMap.TileHeight/2))) / 2;

    RetVal.y =
        ((ScreenPos.y - CameraPos.y) / (FTileMap.TileHeight/2)
            - ((ScreenPos.x - CameraPos.x) / (FTileMap.TileWidth/2))) / 2;

    return RetVal;
}
//---------------------------------------------------------------------------

TPoint TStage::GetDrawPos(
    int TileNoX,
    int TileNoY,
    int TileNoZ,
    int CameraPosX,
    int CameraPosY,
    int TileImgWidth,
    int TileImgHeight
    )
{
    TPoint RetVal;

    RetVal.x =
        CameraPosX
            - (TileImgWidth - FTileMap.TileWidth)
            - (FTileMap.TileWidth/2)
            + ((TileNoX - TileNoY) * FTileMap.TileWidth/2);

    RetVal.y =
        CameraPosY
            - (TileImgHeight - FTileMap.TileHeight)
            + ((TileNoX + TileNoY) * FTileMap.TileHeight/2)
            - (TileNoZ*FTileMap.TileHeight);

    return RetVal;
}
//---------------------------------------------------------------------------

void TStage::TextOut(
    int X,
    int Y,
    const String& Text,
    const String& Font,
    int Size72DPI,
    TAlphaColor Color,
    bool Shadow,
    TAlphaColor ShadowColor
    )
{
    TBrushKind FillKindBackup   = FCanvas->Fill->Kind;
    TAlphaColor FillColorBackup = FCanvas->Fill->Color;
    float FontSizeBackup        = FCanvas->Font->Size;
    String FontFamilyBackup     = FCanvas->Font->Family;

    FCanvas->Fill->Kind     = TBrushKind::Solid;

    FCanvas->Font->Size     = Size72DPI * 96 / 72;
    FCanvas->Font->Family   = Font;

    float TextWidth     = FCanvas->TextWidth(Text);
    float TextHeight    = FCanvas->TextHeight(Text);

    if (Shadow)
    {

        FCanvas->Fill->Color = ShadowColor;

        FCanvas->FillText(
            RectF(X+1, Y+1, X+1+TextWidth, Y+ 1+TextHeight),
            Text,
            false,
            1.0,
            static_cast<TFillTextFlags>(0),
            TTextAlign::Leading,
            TTextAlign::Leading
            );
    }

    FCanvas->Fill->Color = Color;

    FCanvas->FillText(
        RectF(X, Y, X+TextWidth, Y+TextHeight),
        Text,
        false,
        1.0,
        static_cast<TFillTextFlags>(0),
        TTextAlign::Leading,
        TTextAlign::Leading
        );

    FCanvas->Fill->Kind     = FillKindBackup;
    FCanvas->Fill->Color    = FillColorBackup;
    FCanvas->Font->Size     = FontSizeBackup;
    FCanvas->Font->Family   = FontFamilyBackup;
}
//---------------------------------------------------------------------------

void TStage::FillRect(const TRect& Rect, TAlphaColor Color)
{
    TRectF LRectF = RectF(Rect.left, Rect.top, Rect.right, Rect.bottom);

    FCanvas->Fill->Color = Color;

    InflateRect(LRectF, -(FCanvas->Stroke->Thickness / 2), -(FCanvas->Stroke->Thickness / 2));
    FCanvas->FillRect(LRectF, 0, 0, AllCorners, 1.0);
}
//---------------------------------------------------------------------------

__int64 TStage::GetRandomPosition(__int64 Duration)
{
    return static_cast<__int64>(RandomRange(0, static_cast<int>(Duration)));
}
//---------------------------------------------------------------------------

} // namespace Fmxtiles

