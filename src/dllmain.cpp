#include "includes.h"
#include "custom-layer.hpp"

/*
  Here we define the hook function and the trampoline function, with
  the first being a global variable with a function type, and the other
  is an actual function. The actual calling convention for this function
  is __thiscall, since its a class method, however, c++ doesn't let you
  use __thiscall outside classes and function types, so we have to approximate
  it using __fastcall, which can be done by having `this` be the first
  argument and between actual arguments, an unused arg which is the `edx`
  register. so for example
  
    PlayLayer::init(GJGameLevel*)
  
  would be
  
    PlayLayer_init_H(PlayLayer*, void*, GJGameLevel*)
  
  If the function doesn't have any arguments (which is the case here),
  the `void*` for the edx register is not needed, however i still left it in
  here for the explanation
  
  PS: not every function is a __thiscall, for example, if they're static
  they can be __fastcall, which is much easier to deal with, or sometimes,
  they get really optimized and floats go into xmm registers, and in that
  case you have to do some asm to call/hook the function properly :(
  
  PS2: this calling convention shenanigans only happens on x86, on 64 bit
  (the only 64 bit version of gd being the mac version), you don't have to
  worry about this
*/
    
auto winSize = CCDirector::sharedDirector()->getWinSize();

bool (__thiscall* MenuLayer_init)(CCLayer* self);
bool __fastcall MenuLayer_init_H(CCLayer* self, void*) {
    /*
      In cocos2d-x, init functions return a boolean, which is false if
      the initialization went wrong, so here we check for it. most of the time
      gd just crashes if an init function goes wrong so this is kinda useless,
      except for the part where we return true at the end
    */
    if (!MenuLayer_init(self)) return false;

    auto twitch = CCSprite::createWithSpriteFrameName("gj_twitchIcon_001.png");
    auto twitchbutton = gd::CCMenuItemSpriteExtra::create(
        twitch,
        self,
        menu_selector(CustomLayer::twitch)
    );
    twitchbutton->setPosition({ 0, 38 });

    auto discord = CCSprite::create("gj_discordIcon_001.png");
    auto discordbutton = gd::CCMenuItemSpriteExtra::create(
        discord,
        self,
        menu_selector(CustomLayer::discord)
    );
    discordbutton->setPosition({0, 0});

    auto menu = CCMenu::create();
    menu->addChild(twitchbutton);
    menu->addChild(discordbutton);
    menu->setPosition({ 112, 23.6 });
    menu->setAnchorPoint({ 0, 0 });
    menu->setScale({ 0.82 });

    self->addChild(menu);

    return true;
}

DWORD WINAPI thread_func(void* hModule) {
    // initialize minhook
    MH_Initialize();

    /*
      `uintptr_t` is a type which is always the size of a pointer, in this case it is
      32 bit / 4 bytes. The usage of it here is kinda reduntant since this code is not
      cross-platform, however i think it looks neat and it makes it easy to tell if
      something is an address.
    */
    auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    
    /*
      Here we are hooking MenuLayer::init, by giving the starting address of the function
      and what function to hook it to, as well as a trampoline, which is used to call the
      original function without going through our hook.

      Hooking is quite a complex process and can be confusing to beginners, so if you
      want a more detailed explanation you can watch this video by Guided Hacking
      https://www.youtube.com/watch?v=jTl3MFVKSUM (in the video he makes his own
      hook method, however here we just use MinHook as its a lot simpler to use
      and does the dissassembly part for us).
      To give a basic explanation: hooking a function means overwriting its first few
      instructions, making it redirect code execution to our own function.

      Hooking the init function of a node (in this case a layer) is a common practice as
      that is where you should initialize the elements in the node, and in a hook, you
      can add your own nodes.
    */
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1907b0),
        reinterpret_cast<void*>(&MenuLayer_init_H),
        reinterpret_cast<void**>(&MenuLayer_init) // note the &, this gets the address of the variable
    );

    // enable all hooks you've created with minhook
    MH_EnableHook(MH_ALL_HOOKS);

    return 0;
}

/*
This function runs whenever we inject our dll, this is present
in pretty much every mod, and is just winapi boilerplate
*/
BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        auto h = CreateThread(0, 0, thread_func, handle, 0, 0);
        if (h)
            CloseHandle(h);
        else
            return FALSE;
    }
    return TRUE;
}