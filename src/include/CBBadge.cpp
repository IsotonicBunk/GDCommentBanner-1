#include <Geode/Geode.hpp>
#include <alphalaneous.badgify/include/Badgify.hpp>

using namespace geode::prelude;

$execute {
    alpha::badgify::registerBadge(
        // A unique ID for your badge.
        "your-badge-id"_spr,
        // The name shown when clicking the badge.
        "Badge Name",
        // The description shown when clicking the badge.
        "This is a description that goes along with the badge.",
        // Show the badge when a Location::Profile, Location::Comment, or Location::InfoPopup is loaded.
        // alpha::badgify::showBadge can be called at any time and requires the Badge object and the node for the badge.
        [](const alpha::badgify::Badge& badge) {
            if (badge.modStatus == ModStatus::Regular) {
                alpha::badgify::showBadge(badge, CCSprite::createWithSpriteFrameName("modBadge_01_001.png"));
            }
        });
}