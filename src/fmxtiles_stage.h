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

#ifndef fmxtiles_stageH
#define fmxtiles_stageH
//---------------------------------------------------------------------------

#include <FMX.Media.hpp>

#include <memory>
#include <map>

#ifndef __clang__
#include <boost/shared_ptr.hpp>
#endif

#include "fmxtiles_tilemap.h"
#include "fmxtiles_guielementstack.h"
#include "fmxtiles_sfx.h"
//---------------------------------------------------------------------------

namespace Fmxtiles
{

#ifdef __clang__
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif
//---------------------------------------------------------------------------

struct TMouseOptions
{
    TMouseOptions()
        :   TileNo(TPoint(0, 0)),
            ClickLeft(false),
            ClickRight(false),
            InStage(false),
            Pos(TPoint(0, 0))
    {}

    TPoint  TileNo;

    bool    ClickLeft;
    bool    ClickRight;

    bool    InStage;
    TPoint  Pos;
};
//---------------------------------------------------------------------------

struct TSfxOptions
{
    TSfxOptions()
        :   MusicFadeDuration(0.0),
            MusicFadeStartTime(0.0),
            MusicFadeInIsActive(false),
            MusicFadeOutIsActive(false),
            MusicIsPlaying(false)
    {}

    float MusicFadeDuration;
    float MusicFadeStartTime;
    bool  MusicFadeInIsActive;
    bool  MusicFadeOutIsActive;
    bool  MusicIsPlaying;
};
//---------------------------------------------------------------------------

class TStage
{
public:

    TStage(
        TCanvas* Canvas,
        float CanvasWidth,
        float CanvasHeight,
        float ScrollBorderWidth,
        float ScrollSpeed,
        const String& TileStorageDir,
        const String& GuiElementsDir,
        const String& SfxDir
        );

    virtual ~TStage();

    void    BeginScene();
    void    EndScene();

    void    ProcessUserInput(float Delta, float Absolute);
    void    ProcessEvents(float Delta, float Absolute);
    void    Clear();
    void    DrawTiles(float Delta, float Absolute);
    void    DrawCharacters(float Delta, float Absolute);
    void    DrawGui(float Delta, float Absolute);

    void    ManageSfx(float Delta, float Absolute);

    void    HandleMouseMove(float MousePosX, float MousePosY);
    void    HandleMouseDown(TMouseButton Button);
    void    HandleMouseUp(TMouseButton Button);
    void    HandleMouseEnter();
    void    HandleMouseLeave();

private:
    void    DrawBitmap(int X, int Y, float Opacity, TBitmap* Bitmap);
    TPoint  GetMapTileCoord(TPointF ScreenPos, TPointF CameraPos);

    TPoint  GetDrawPos(
        int TileNoX,
        int TileNoY,
        int TileNoZ,
        int CameraPosX,
        int CameraPosY,
        int TileImgWidth,
        int TileImgHeight
        );

    void TextOut(
        int X,
        int Y,
        const String& Text,
        const String& Font,
        int Size72DPI,
        TAlphaColor Color,
        bool Shadow=false,
        TAlphaColor ShadowColor=claBlack
        );

    void FillRect(const TRect& Rect, TAlphaColor Color);

    __int64 GetRandomPosition(__int64 Duration);

    TMasterTileStack    FMasterTileStack;
    TGuiElementStack    FGuiElementStack;

    String              FSfxDir;

    TTileMap            FTileMap;

    shared_ptr<TGuiElement> FGuiMainInterface;

    TCanvas         *FCanvas;
    float           FCanvasWidth;
    float           FCanvasHeight;

    float           FScrollBorderWidth;
    float           FScrollSpeed;

    TPointF         FCameraPos;

    TMouseOptions   FMouseOptions;

    float           FFPS;
    int             FFPSCycle;
    float           FFPSTimer;

    float           FBlinkerOpacity;
    bool            FBlinkerCountDown;

    TSfxOptions     FSfxOptions;

    std::unique_ptr<TMediaPlayer> FSfxMusicPlayer;
};
//---------------------------------------------------------------------------

} // namespace Fmxtiles

#endif
