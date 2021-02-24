#pragma once
#pragma comment(lib, "PluginSDK.lib")
#include "bakkesmod/plugin/bakkesmodplugin.h"

class DoubleTrouble : public BakkesMod::Plugin::BakkesModPlugin
{
private:
    std::shared_ptr<bool> bEnabled;
    std::shared_ptr<bool> bShowPrimaryBall;
    std::shared_ptr<bool> bDebugLog;

    uintptr_t PrimaryBallAddress = 0;

public:
	void onLoad() override;
	void onUnload() override;
    bool IsValid();

    void Render(CanvasWrapper canvas);

    void OnRoundStarted();
    void OnCarHitBall(BallWrapper HitBall, void* Params);
    void OnBallHitGoal(BallWrapper Ball, void* Params);

    Vector GetRelativeVectorComponents(Rotator RootObjectRotation, Vector RootObjectVector, Vector RelativeObjectVector);
};

struct RecordCarHitParams
{
	uintptr_t HitCar; //CarWrapper
	Vector HitLocation;
	Vector HitNormal;
	unsigned char HitType;
};
