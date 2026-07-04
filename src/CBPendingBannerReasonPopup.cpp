#include "CBPendingBannerReasonPopup.hpp"
#include "CBAdminPanelLayer.hpp"
#include <Geode/binding/FLAlertLayer.hpp>

using namespace geode::prelude;

CBPendingBannerReasonPopup* CBPendingBannerReasonPopup::create(int bannerId, bool isReject, CBAdminPanelLayer* adminPanel) {
    auto ret = new CBPendingBannerReasonPopup();
    if (ret && ret->init(bannerId, isReject, adminPanel)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CBPendingBannerReasonPopup::init(int bannerId, bool isReject, CBAdminPanelLayer* adminPanel) {
    if (!Popup::init(320.f, 180.f, "GJ_square02.png")) return false;

    m_bannerId = bannerId;
    m_isReject = isReject;
    m_adminPanel = adminPanel;

    this->setTitle(isReject ? "Reject Banner" : "Approve Banner");
    addSideArt(m_mainLayer, SideArt::All, SideArtStyle::PopupBlue);

    m_reasonInput = TextInput::create(260.f, "Enter reason here...");
    m_reasonInput->setCommonFilter(CommonFilter::Any);
    m_reasonInput->setLabel(isReject ? "Reason for Rejection (Required):" : "Reason for Approval (Optional):");
    m_reasonInput->setMaxCharCount(150);
    m_mainLayer->addChildAtPosition(m_reasonInput, Anchor::Center, {0.f, 0.f});

    auto btnSpr = ButtonSprite::create(isReject ? "Reject" : "Approve", "goldFont.fnt", isReject ? "GJ_button_06.png" : "GJ_button_01.png", 0.8f);
    auto submitBtn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(CBPendingBannerReasonPopup::onConfirm));

    m_buttonMenu->addChildAtPosition(submitBtn, Anchor::Bottom, {0.f, 25.f}, false);

    return true;
}

void CBPendingBannerReasonPopup::onConfirm(CCObject* sender) {
    std::string reason = m_reasonInput->getString();
    if (m_isReject && reason.empty()) {
        Notification::create("A reason is required when rejecting a banner.", NotificationIcon::Error)->show();
        return;
    }

    m_adminPanel->processPendingBanner(m_bannerId, !m_isReject, reason);
    this->onClose(nullptr);
}
