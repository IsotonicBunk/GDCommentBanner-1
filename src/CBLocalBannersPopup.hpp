#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <cue/ListNode.hpp>

using namespace geode::prelude;

class CBLocalBannersPopup : public geode::Popup {
public:
    static CBLocalBannersPopup* create();
    void fetchBanners();

private:
    bool init() override;
    void onAddBanner(CCObject* sender);

    cue::ListNode* m_list = nullptr;
    CCLabelBMFont* m_noBannersLabel = nullptr;
};
