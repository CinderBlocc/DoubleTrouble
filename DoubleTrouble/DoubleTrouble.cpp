#include "DoubleTrouble.h"
#include "bakkesmod\wrappers\includes.h"

BAKKESMOD_PLUGIN(DoubleTrouble, "Duplicates the ball when a car touches it. Score the first ball to win", "1.0", PLUGINTYPE_FREEPLAY)


void DoubleTrouble::onLoad()
{
	using namespace std::placeholders;
	
    //bEnabled = std::make_shared<bool>(false);
    //cvarManager->registerCvar("DoubleTrouble_Enabled", "0", "Enable the DoubleTrouble plugin", true, true, 0, true, 1).bindTo(bEnabled);

	//cvarManager->registerNotifier("NOTIFIER", [this](std::vector<std::string> params){FUNCTION();}, "DESCRIPTION", PERMISSION_ALL);
	//cvarManager->registerCvar("CVAR", "DEFAULTVALUE", "DESCRIPTION", true, true, MINVAL, true, MAXVAL);//.bindTo(CVARVARIABLE);
	//gameWrapper->HookEvent("FUNCTIONNAME", std::bind(&DoubleTrouble::FUNCTION, this));
	//gameWrapper->HookEventWithCallerPost<ActorWrapper>("FUNCTIONNAME", std::bind(&DoubleTrouble::FUNCTION, this, _1, _2, _3));
	//gameWrapper->RegisterDrawable(std::bind(&DoubleTrouble::Render, this, _1));
}
void DoubleTrouble::onUnload() {}

void DoubleTrouble::OnCarHitBall()
{
    //Duplicate the ball
    //Shoot off at 45 degrees?
    //When they duplicate, scale each by 0.5?
        //Probably won't replicate change so that wouldnt work unless you want to force everyone to have the plugin
}

void DoubleTrouble::OnBallHitGoal()
{
    //Check which ball it is. If it's the primary ball, explode. If not, destroy?
}
