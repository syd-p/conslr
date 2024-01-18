#include "conslr/taggedstring.hpp"

#include <cassert>

conslr::TaggedString::TaggedString() :
    mCurrentForegroundTag{ 0 }, mCurrentBackgroundTag{ 0 }
{}

conslr::TaggedString& conslr::TaggedString::operator<<(const std::string& str)
{
    mString.reserve(mString.size() + str.size());

    for (const auto& c : str)
    {
        mString.emplace_back(c, (mCurrentBackgroundTag & 0x0F) + (mCurrentForegroundTag << 4));
    }

    return *this;
}

conslr::TaggedString& conslr::TaggedString::operator<<(const char& c)
{
    mString.emplace_back(c, (mCurrentBackgroundTag & 0x0F) + (mCurrentForegroundTag << 4));

    return *this;
}

conslr::TaggedString& conslr::TaggedString::operator<<(const FgTag& tag)
{
    mCurrentForegroundTag = tag.index & 0x0F;

    if (mTags.size() <= mCurrentForegroundTag)
    {
        mTags.resize(mCurrentForegroundTag + 1, { 255, 255, 255, 255 });
    }

    return *this;
}

conslr::TaggedString& conslr::TaggedString::operator<<(const BgTag& tag)
{
    mCurrentBackgroundTag = tag.index & 0x0F;

    if (mTags.size() <= mCurrentBackgroundTag)
    {
        mTags.resize(mCurrentBackgroundTag + 1, { 255, 255, 255, 255 });
    }

    return *this;
}

//Getters
const std::vector<conslr::TaggedString::TaggedChar>& conslr::TaggedString::getString() const { return mString; }
const SDL_Color& conslr::TaggedString::getTag(int32_t index) const { return mTags.at(index); }

//Setters
void conslr::TaggedString::setTag(int32_t index, const SDL_Color& color)
{
    assert((index > 0 && index < MAX_TAGS) && "Tag index is out of bounds");

    if (index >= mTags.size())
    {
        mTags.resize(index + 1);
    }

    mTags.at(index) = color;

    return;
}
