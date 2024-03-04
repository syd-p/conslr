///
///@file conslr/widgets/taggedfloatingtext.hpp
///@brief Defines the TaggedFloatingText widget
///
#pragma once

#include <sstream>
#include <stdexcept>

#include <SDL_rect.h>

#include "conslr/widget.hpp"
#include "conslr/screen.hpp"
#include "conslr/taggedstring.hpp"
#include "conslr/widgetmanager.hpp"
#include "conslr/widgetfactory.hpp"

namespace conslr::widgets
{
    ///
    ///Renders borderless tagged text
    ///
    class TaggedFloatingText : public IWidget, public IRenderable
    {
    public:
        friend class conslr::Screen;
        friend class conslr::WidgetManager;

        //Getters
        constexpr const SDL_Rect& getRegion() const noexcept { return mRegion; }
        constexpr const TaggedString& getString() const noexcept { return mString; }
        constexpr const TagSet& getTags() const noexcept { return mTags; }

        //Setters
        constexpr void setRegion(const SDL_Rect& region) 
        {
            if (region.w <= 0 || region.h <= 0)
            {
                throw std::invalid_argument("Region width and height must be greater than 0, width: " + std::to_string(region.w) + ", height: " + std::to_string(region.h));
            }

            mRegion = region;
            mRerender = true;

            return;
        }
        constexpr void setString(const TaggedString& str) { mString = str; mRerender = true; }
        constexpr void setTags(const TagSet& tags) { mTags = tags; mRerender = true; }

    protected:
        constexpr TaggedFloatingText(int32_t id, int32_t priority) noexcept :
            IWidget{ id, priority },
            mRegion{ 0, 0, 0, 0 }
        {}

        virtual void render(Screen& screen)
        {
            if (mRegion.w <= 0 || mRegion.h <= 0)
            {
                throw std::runtime_error("Region width and height must be greater than 0, width: " + std::to_string(mRegion.w) + ", height: " + std::to_string(mRegion.h));
            }

            screen.renderMultilineTextTagged(
                    mRegion.x, mRegion.y,
                    mRegion.w, mRegion.h,
                    mString,
                    mTags
                    );

            return;
        }

        SDL_Rect mRegion;
        TaggedString mString;
        TagSet mTags;
    };

    inline std::pair<std::string, int32_t> constructTaggedFloatingText(WidgetManager& wm, const WidgetParameterMap& params)
    {
        int priority = 0;
        if (params.contains("priority"))
        {
            priority = std::stoi(params.at("priority"));
        }
        auto wptr = wm.createWidget<TaggedFloatingText>(priority);
        auto ptr = wptr.lock();

        if (params.contains("visible"))
        {
            if (params.at("visible") == "true")
            {
                ptr->show();
            } 
            else if (params.at("visible") == "false")
            {
                ptr->hide();
            }
            else
            {
                throw std::invalid_argument("Param visible must be \"true\" or \"false\"");
            }
        }

        if (params.contains("showtitle"))
        {
            if (params.at("showtitle") == "true")
            {
                ptr->showTitle();
            }
            else if (params.at("showtitle") == "false")
            {
                ptr->hideTitle();
            } 
            else
            {
                throw std::invalid_argument("Param showtitle must be \"true\" or \"false\"");
            }
        }

        if (params.contains("active"))
        {
            if (params.at("active") == "true")
            {
                ptr->setActive(true);
            }
            else if (params.at("active") == "false")
            {
                ptr->setActive(false);
            } 
            else
            {
                throw std::invalid_argument("Param active must be \"true\" or \"false\"");
            }
        }

        if (params.contains("string"))
        {
            //Loads a formatted string
            ptr->setString(TaggedString{ params.at("string") });
        }

        if (params.contains("tags"))
        {
            //Loads a formatted tagset
            ptr->setTags(createTagSetFromString(params.at("tags")));
        }

        if (params.contains("title"))
        {
            ptr->setTitle(params.at("title"));
        }

        if (params.contains("region"))
        {
            std::stringstream ss{ params.at("region") };
            int32_t x;
            int32_t y;
            int32_t w;
            int32_t h;

            ss >> x >> y >> w >> h;
            if (ss.fail())
            {
                throw std::invalid_argument("Param region must be in the format of \"intx inty intw inth\"");
            }

            ptr->setRegion({ x, y, w, h });
        }

        if (params.contains("name"))
        {
            return { params.at("name"), ptr->getId() };
        }
        else
        {
            return { "unnamed", ptr->getId() };
        }
    } 
}
