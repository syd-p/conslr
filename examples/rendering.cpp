///Simple rendering program
///
///Renders a bouncing dot
#include <SDL.h>

#include <conslr/console.hpp>
#include <conslr/screen.hpp>
#include <conslr/widget.hpp>

using namespace conslr;

class DotWidget : public IWidget, public IRenderable
{
public:
    constexpr DotWidget(int32_t id, int32_t priority) noexcept :
        IWidget{ id, priority }, IRenderable{}
    {}

    constexpr void update() noexcept
    {
        mStep = (mStep + 1) % mSpeed;
        if (mStep == 0)
        {
            if (mX + mVelocityX < 0 || mX + mVelocityX >= mRegion.w) { mVelocityX = -mVelocityX; }
            if (mY + mVelocityY < 0 || mY + mVelocityY >= mRegion.h) { mVelocityY = -mVelocityY; }

            mX += mVelocityX;
            mY += mVelocityY;

            mRerender = true;
        }

        return;
    }

    virtual void render(Screen& scr)
    {
        scr.fillRect(mRegion, mTheme->background, mTheme->text, ' ');

        scr.setCellBackground(mRegion.x + mX, mRegion.y + mY, { 255, 0, 255, 255 });

        mRerender = false;

        return;
    }

    constexpr void setRegion(const SDL_Rect& region) noexcept { mRegion = region; }

private:
    SDL_Rect mRegion;

    int32_t mX = 0;
    int32_t mY = 0;
    int32_t mVelocityX = 1;
    int32_t mVelocityY = 1;
    int32_t mStep = 1;
    int32_t mSpeed = 10;
};

int main()
{
    //Init console
    conslr::Console console{ 16, 32, 80, 24 };

    int32_t font = console.createFont("res/ibm_vga_fontsheet.bmp", 8, 16);
    console.setCurrentFontIndex(font);

    int32_t scr = console.createScreen();
    console.setCurrentScreenIndex(scr);

    auto& wm = console.getWidgetManager(scr);
    auto dot = wm.createWidget<DotWidget>();
    dot.lock()->setRegion({ 0, 0, 80, 33 });

    SDL_Event event;
    bool running = true;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        dot.lock()->update();
        console.render();
    }

    console.destroy();
    SDL_Quit();

    return 0;
}
