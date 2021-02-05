#pragma once
#pragma comment(lib, "PluginSDK.lib")
#include "bakkesmod/plugin/bakkesmodplugin.h"

class DoubleTrouble : public BakkesMod::Plugin::BakkesModPlugin
{
private:
    //std::shared_ptr<bool> bEnabled;

public:
	void onLoad() override;
	void onUnload() override;

    void OnCarHitBall();
    void OnBallHitGoal();
};
