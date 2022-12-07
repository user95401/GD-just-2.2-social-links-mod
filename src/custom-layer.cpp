#include "custom-layer.hpp"

void CustomLayer::twitch(CCObject* object) {
    LPCWSTR a = L"https://www.twitch.tv/robtopgames";
    ShellExecuteW(NULL, L"open", a, NULL, NULL, SW_SHOWNORMAL);
}
void CustomLayer::discord(CCObject* object) {
    LPCWSTR a = L"https://discord.com/invite/geometrydash";
    ShellExecuteW(NULL, L"open", a, NULL, NULL, SW_SHOWNORMAL);
}