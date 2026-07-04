#include <Geode/Geode.hpp>
#include <alphalaneous.badgify/include/Badgify.hpp>
#include "CBConstant.hpp"
#include <unordered_map>

using namespace geode::prelude;

struct RoleCache {
    bool isAdmin = false;
    bool isStaff = false;
};
static std::unordered_map<int, RoleCache> s_roleCache;

void checkAndShowBadge(const alpha::badgify::Badge& badge, bool isAdminBadge) {
    if (!badge.user) return;
    int targetAccountId = badge.user->m_accountID;
    if (targetAccountId <= 0) return;

    if (s_roleCache.contains(targetAccountId)) {
        auto const& roles = s_roleCache[targetAccountId];
        if (isAdminBadge && roles.isAdmin) {
            alpha::badgify::showBadge(badge, CCSprite::createWithSpriteFrameName("CB_admin_badge.png"_spr));
        } else if (!isAdminBadge && roles.isStaff) {
            alpha::badgify::showBadge(badge, CCSprite::createWithSpriteFrameName("CB_staff_badge.png"_spr));
        }
        return;
    }

    arc::spawn([badge, targetAccountId, isAdminBadge]() -> arc::Future<> {
        auto accountData = argon::getGameAccountData();
        int accountId = accountData.accountId;
        if (accountId <= 0) co_return;

        auto authResult = co_await comment::argonToken(accountData);
        if (authResult.empty()) co_return;

        auto req = geode::utils::web::WebRequest();
        auto body = matjson::makeObject({{"accountId", accountId},
            {"argonToken", authResult},
            {"targetAccountId", targetAccountId}});
        auto response = co_await req.bodyJSON(body).post(fmt::format("{}/getUser", comment::baseUrl));
        if (!response.ok()) co_return;
        auto jsonRes = response.json();
        if (jsonRes.isErr()) co_return;
        auto json = jsonRes.unwrap();

        RoleCache roles;
        roles.isAdmin = json["is_admin"].asBool().unwrapOr(false);
        roles.isStaff = json["is_staff"].asBool().unwrapOr(false);

        geode::queueInMainThread([badge, targetAccountId, isAdminBadge, roles]() {
            s_roleCache[targetAccountId] = roles;
            if (isAdminBadge && roles.isAdmin) {
                alpha::badgify::showBadge(badge, CCSprite::createWithSpriteFrameName("CB_admin_badge.png"_spr));
            } else if (!isAdminBadge && roles.isStaff) {
                alpha::badgify::showBadge(badge, CCSprite::createWithSpriteFrameName("CB_staff_badge.png"_spr));
            }
        });
    });
}

$execute {
    alpha::badgify::registerBadge(
        "cb-admin"_spr,
        "Comment Banners Admin",
        "This user is an <cr>Administrator</c> for <cp>Comment Banners</c>. They have the ability to <co>delete/modify</c> banners as well as <cg>promote users to Staff</c>",
        [](const alpha::badgify::Badge& badge) {
            checkAndShowBadge(badge, true);
        });

    alpha::badgify::registerBadge(
        "cb-staff"_spr,
        "Comment Banners Staff",
        "This user is a <cg>Staff</c> for <cp>Comment Banners</c>. They have the ability to <cg>approve pending banners</c> and <cy>feature banners</c>",
        [](const alpha::badgify::Badge& badge) {
            checkAndShowBadge(badge, false);
        });
}