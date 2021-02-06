#include "DoubleTrouble.h"
#include "bakkesmod\wrappers\includes.h"

BAKKESMOD_PLUGIN(DoubleTrouble, "Duplicates the ball when a car touches it. Score the original ball to win", "1.0", PLUGINTYPE_FREEPLAY)

void DoubleTrouble::onLoad()
{
	using namespace std::placeholders;
	
    bEnabled = std::make_shared<bool>(false);
    cvarManager->registerCvar("DoubleTrouble_Enabled", "0", "Enable the DoubleTrouble plugin", true, true, 0, true, 1).bindTo(bEnabled);

    gameWrapper->HookEventWithCallerPost<BallWrapper>("Function TAGame.Ball_TA.RecordCarHit", std::bind(&DoubleTrouble::OnCarHitBall, this, _1, _2));
    gameWrapper->HookEventWithCaller<BallWrapper>("Function TAGame.Ball_TA.OnHitGoal", std::bind(&DoubleTrouble::OnBallHitGoal, this, _1, _2));
    gameWrapper->HookEvent("Function GameEvent_Soccar_TA.Active.StartRound", std::bind(&DoubleTrouble::OnRoundStarted, this));

    //If loaded in the middle of a round, get the primary ball address
    OnRoundStarted();
}
void DoubleTrouble::onUnload() {}

bool DoubleTrouble::IsValid()
{
    if(!*bEnabled) { return false; }

    //Make sure server is valid
    ServerWrapper Server = gameWrapper->GetCurrentGameState();
    if(Server.IsNull()) { return false; }
    GameSettingPlaylistWrapper GSPW = Server.GetPlaylist();
    if(GSPW.memory_address == NULL) { return false; }

    //All offline modes are fine
    if(!gameWrapper->IsInOnlineGame()) { return true; }

    //If online AND in LAN match, it's also fine
    if(GSPW.IsLanMatch()) { return true; }

    return false;
}

// Called the instant the countdown hits 0 //
void DoubleTrouble::OnRoundStarted()
{
    //Reset the ball address
    PrimaryBallAddress = 0;

    if(!IsValid()) { return; }

    //Get the ball
    ServerWrapper Server = gameWrapper->GetCurrentGameState();
    if(Server.IsNull()) { return; }
    BallWrapper Ball = Server.GetBall();
    if(Ball.IsNull()) { return; }

    //Store the memory address of the only existing ball
    PrimaryBallAddress = Ball.memory_address;
}

void DoubleTrouble::OnCarHitBall(BallWrapper HitBall, void* Params)
{
    if(!IsValid() || HitBall.IsNull()) { return; }
    if(gameWrapper->IsInOnlineGame()) { return; } //Limit this function to host only

    RecordCarHitParams* CastParams = (RecordCarHitParams*)Params;
    CarWrapper HitCar = CarWrapper(CastParams->HitCar);
    if(HitCar.IsNull()) { return; }
    if(HitCar.GetPlayerController().IsNull() && HitCar.GetAIController().IsNull()) { return; }

    //Get the car's orientation
    Quat CarQuat = RotatorToQuat(HitCar.GetRotation());
    Vector CarForward = RotateVectorWithQuat(Vector{1,0,0}, CarQuat);
    Vector CarRight   = RotateVectorWithQuat(Vector{0,1,0}, CarQuat);
    Vector CarUp      = RotateVectorWithQuat(Vector{0,0,1}, CarQuat);

    //Get the location of the ball relative to the car that hit it    
    Vector BallRelativeLocation = GetRelativeVectorComponents(HitCar.GetRotation(), HitCar.GetLocation(), HitBall.GetLocation());
    bool bIsOnRightSide = BallRelativeLocation.Y > 0.f;

    //Get ball spawn location (inverted Right component)
    Vector BallForward = CarForward *  BallRelativeLocation.X;
    Vector BallRight   = CarRight   * -BallRelativeLocation.Y;
    Vector BallUp      = CarUp      *  BallRelativeLocation.Z;
    Vector SpawnLocation = BallForward + BallRight + BallUp + HitCar.GetLocation();

    //Get the relative velocity of the ball to invert the right component
    Vector BallInvertedVelocity = GetRelativeVectorComponents(HitCar.GetRotation(), Vector(0,0,0), HitBall.GetVelocity());
    BallInvertedVelocity *= -1.f;
    
    //Spawn a new ball
    ServerWrapper Server = gameWrapper->GetCurrentGameState();
    BallWrapper NewBall = Server.SpawnBall(SpawnLocation, true, false);
    NewBall.SetVelocity(BallInvertedVelocity);

    //Apply forces to the balls
    float ForceMagnitude = 1000.f;
    float ForceInversion = bIsOnRightSide ? 1.f : -1.f;
    Vector OrignalBallForce = CarRight * ForceMagnitude * ForceInversion;
    Vector NewBallForce = CarRight * ForceMagnitude * -ForceInversion;
    HitBall.AddForce(OrignalBallForce, 1);
    NewBall.AddForce(NewBallForce, 1);
}

void DoubleTrouble::OnBallHitGoal(BallWrapper Ball, void* Params)
{
    if(!IsValid() || Ball.IsNull()) { return; }

    if(Ball.memory_address != PrimaryBallAddress)
    {
        Ball.DoDestroy();
    }
}

Vector DoubleTrouble::GetRelativeVectorComponents(Rotator RootObjectRotation, Vector RootObjectVector, Vector RelativeObjectVector)
{
    //Get orientation matrix of root
    Quat RootQuat = RotatorToQuat(RootObjectRotation);
    Vector RootForward = RotateVectorWithQuat(Vector{1,0,0}, RootQuat);
    Vector RootRight   = RotateVectorWithQuat(Vector{0,1,0}, RootQuat);
    Vector RootUp      = RotateVectorWithQuat(Vector{0,0,1}, RootQuat);

    //Get relative vector
    Vector Relative = RelativeObjectVector - RootObjectVector;
    float ForwardComp = Vector::dot(Relative, RootForward);
    float RightComp   = Vector::dot(Relative, RootRight);
    float UpComp      = Vector::dot(Relative, RootUp);

    return {ForwardComp, RightComp, UpComp};
}
