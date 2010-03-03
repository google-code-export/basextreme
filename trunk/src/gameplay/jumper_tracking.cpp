
#include "headers.h"
#include "jumper.h"
#include "imath.h"
#include "../common/istring.h"

/**
 * process constants
 */

const float trackingBlendTime         = 0.5f;
const float steeringBackBoneBendLimit = 15.0f;
const float legPitchTime              = 0.35f;
const float legPitchBendLimit         = 20.0f;

/**
 * related animations
 */

static engine::AnimSequence trackForwardSequence = 
{
    FRAMETIME(335), 
    FRAMETIME(450), 
    engine::ltPeriodic, 
    FRAMETIME(335) 
};

static engine::AnimSequence trackForwardSequenceWings = 
{
    FRAMETIME(1985), 
    FRAMETIME(2004), 
    engine::ltPeriodic, 
    FRAMETIME(1985) 
};

static engine::AnimSequence trackDownSequence =
{
    FRAMETIME(473), 
    FRAMETIME(490), 
    engine::ltPeriodic, 
    FRAMETIME(473) 
};

static engine::AnimSequence trackDownSequenceWings =
{
    FRAMETIME(2005), 
    FRAMETIME(2025), 
    engine::ltPeriodic, 
    FRAMETIME(2005) 
};

static engine::AnimSequence trackLeftSequence =
{
    FRAMETIME(512), 
    FRAMETIME(526), 
    engine::ltPeriodic, 
    FRAMETIME(512)     
};

static engine::AnimSequence trackLeftSequenceWings =
{
    FRAMETIME(2049), 
    FRAMETIME(2071), 
    engine::ltPeriodic, 
    FRAMETIME(2049)     
};

static engine::AnimSequence trackRightSequence =
{
    FRAMETIME(451), 
    FRAMETIME(472), 
    engine::ltPeriodic, 
    FRAMETIME(451) 
};

static engine::AnimSequence trackRightSequenceWings =
{
    FRAMETIME(2026), 
    FRAMETIME(2048), 
    engine::ltPeriodic, 
    FRAMETIME(2026) 
};

/**
 * class implementation
 */

Jumper::Tracking::Tracking(Jumper* jumper, NxActor* phActor, MatrixConversion* mc) :
    JumperAction( jumper )
{
    // set action properties
    _actionTime = 0.0f;
    _blendTime = 0.5f;
    _endOfAction = false;
    _phActor = phActor;
    _matrixConversion = mc;
    _steering = 0.0f;
    _tracking = 0.0f;
    _legPitch = 0.0f;

    if( jumper->_player )
    {
        Gameplay::iGameplay->resetActionChannels( ::iaZoomIn );
    }

    engine::IAnimationController* animCtrl = _clump->getAnimationController();

    // capture blend source
    animCtrl->captureBlendSrc();

    // reset animation mixer
    for( unsigned int i=0; i<engine::maxAnimationTracks; i++ )
    {
        if( animCtrl->getTrackAnimation( i ) ) animCtrl->setTrackActivity( i, false );
    }

    bool useWingsuit = database::Suit::getRecord( _jumper->getVirtues()->equipment.suit.id )->wingsuit;

    // setup animation cycles
    if( useWingsuit )
    {
        animCtrl->setTrackAnimation( 0, &trackForwardSequenceWings );
        animCtrl->setTrackAnimation( 1, &trackDownSequenceWings );
        animCtrl->setTrackAnimation( 2, &trackLeftSequenceWings );
    }
    else
    {
        animCtrl->setTrackAnimation( 0, &trackForwardSequence );
        animCtrl->setTrackAnimation( 1, &trackDownSequence );
        animCtrl->setTrackAnimation( 2, &trackLeftSequence );
    }
    animCtrl->setTrackActivity( 0, true );
    animCtrl->setTrackActivity( 1, true );
    animCtrl->setTrackActivity( 2, true );
    animCtrl->setTrackSpeed( 0, 1.0f );
    animCtrl->setTrackWeight( 0, 1.0f );
    animCtrl->setTrackSpeed( 1, 1.0f );
    animCtrl->setTrackWeight( 1, 0.0f );
    animCtrl->setTrackSpeed( 2, 1.0f );
    animCtrl->setTrackWeight( 2, 0.0f );
    animCtrl->resetTrackTime( 0 );
    animCtrl->resetTrackTime( 1 );
    animCtrl->resetTrackTime( 2 );
    updatePhysics();
    animCtrl->advance( 0.0f );

    // capture blend destination
    animCtrl->captureBlendDst();
    animCtrl->blend( 0.0f );

    if (_jumper->getSpinalCord()->modifier) {
        _phActor->addLocalTorque(NxVec3(0.5f, 0.0f, 0.0f), NxForceMode::NX_VELOCITY_CHANGE);
    }
}

void Jumper::Tracking::updateBlending(float dt)
{
    // blend animations
    float weight0, weight1, weight2, correct01;
    weight2 = fabs( _steering );
    weight2 *= 0.5f;
    weight1 = _tracking;
    weight0 = 1.0f - weight1;
    correct01 = ( weight0 + weight1 ) * ( 1.0f - weight2 );
    weight0 *= correct01;
    weight1 *= correct01;

    engine::IAnimationController* animCtrl = _clump->getAnimationController();
    animCtrl->setTrackWeight( 0, weight0 );
    animCtrl->setTrackWeight( 1, weight1 );
    animCtrl->setTrackWeight( 2, weight2 );   

    if( _steering < 0 && animCtrl->getTrackAnimation( 2 ) != &trackLeftSequence &&
                         animCtrl->getTrackAnimation( 2 ) != &trackLeftSequenceWings )
    {
        if( database::Suit::getRecord( _jumper->getVirtues()->equipment.suit.id )->wingsuit )
        {
            animCtrl->setTrackAnimation( 2, &trackLeftSequenceWings );
        }
        else
        {
            animCtrl->setTrackAnimation( 2, &trackLeftSequence );
        }
        animCtrl->resetTrackTime( 2 );
    }
    if( _steering > 0 && animCtrl->getTrackAnimation( 2 ) != &trackRightSequence )
    {
        if( database::Suit::getRecord( _jumper->getVirtues()->equipment.suit.id )->wingsuit )
        {
            animCtrl->setTrackAnimation( 2, &trackRightSequenceWings );
        }
        else
        {
            animCtrl->setTrackAnimation( 2, &trackRightSequence );
        }
        animCtrl->resetTrackTime( 2 );
    }
}

void Jumper::Tracking::updateProceduralAnimation(float dt)
{
    // back bone
    engine::IFrame* backBone = Jumper::getBackBone( _clump );
    Matrix4f backBoneMatrix = backBone->getMatrix();
    Vector3f backBoneUp( backBoneMatrix[1][0], backBoneMatrix[1][1], backBoneMatrix[1][2] );
    backBoneUp.normalize();
    float steeringAngle = steeringBackBoneBendLimit * _steering;
    backBoneMatrix = Gameplay::iEngine->rotateMatrix( backBoneMatrix, backBoneUp, steeringAngle );
    backBone->setMatrix( backBoneMatrix );

    float pitchAngle = legPitchBendLimit * -_legPitch * (1.0f - _tracking);

    // left leg    
    engine::IFrame* leftLegBone = Jumper::getLeftLegBone( _clump );    
    Matrix4f matrix = leftLegBone->getMatrix();
    Vector3f axis = Vector3f( matrix[2][0], matrix[2][1], matrix[2][2] );
    axis.normalize();
    matrix = Gameplay::iEngine->rotateMatrix( matrix, axis, pitchAngle );
    leftLegBone->setMatrix( matrix );

    // right leg
    engine::IFrame* rightLegBone = Jumper::getRightLegBone( _clump );
    matrix = rightLegBone->getMatrix();
    axis = Vector3f( matrix[2][0], matrix[2][1], matrix[2][2] );
    axis.normalize();
    matrix = Gameplay::iEngine->rotateMatrix( matrix, axis, pitchAngle );
    rightLegBone->setMatrix( matrix );
}

void Jumper::Tracking::update(float dt)
{
    updateAnimation( dt );

    // synchronize physics & render
    _clump->getFrame()->setMatrix( _matrixConversion->convert( wrap( _phActor->getGlobalPose() ) ) );    
    updateProceduralAnimation( dt );
    _clump->getFrame()->getLTM();

    // update controls
    if( _actionTime > _blendTime ) 
    {
        updateBlending( dt );
    }
    else if( _jumper->_player )
    {
        Gameplay::iGameplay->resetActionChannels( ::iaZoomIn );
    }
}

/**
 * physics controller
 */

static float getAirResistancePower(float i)
{
    return ( exp( pow( i, 1.4f ) )  - 1.0f ) / 1.718f;
}

static float getGlidePower(float angle)
{
    float i = cos( angle * 3.1415926f / 180.0f );
    return ( pow( cos( i ), 7.0f ) * pow( i, 8.0f ) ) / 0.01544f;
}


#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif


void Jumper::Tracking::updatePhysics(void)
{
    // update controls
    if( _actionTime > _blendTime )
    {
        // update tracking 
        if( !_jumper->getSpinalCord()->modifier )
        {   
            if( _tracking < simulationStepTime / trackingBlendTime ) _tracking = 0;
            else _tracking -= simulationStepTime / trackingBlendTime;
        }
        // update tracking 
        else
        {
            _tracking += simulationStepTime / trackingBlendTime;
            if( _tracking > 1.0f ) _tracking = 1.0f;
        }

        // update steering
        _steering = 0;
        _steering += _jumper->getSpinalCord()->left;
        _steering -= _jumper->getSpinalCord()->right;
        if( _steering == 0.0f )
        {
            _clump->getAnimationController()->resetTrackTime( 2 );
        }
    }

    // leg pitch
    if( _jumper->getSpinalCord()->up != 0.0f ) _legPitch -= simulationStepTime / legPitchTime;
    if( _jumper->getSpinalCord()->down != 0.0f ) _legPitch += simulationStepTime / legPitchTime;
    if( _jumper->getSpinalCord()->up == 0.0f && _jumper->getSpinalCord()->down == 0.0f )
    {
        if( _legPitch < 0 )
        {
            _legPitch += simulationStepTime / legPitchTime;
            if( _legPitch > 0 ) _legPitch = 0.0f;
        }
        if( _legPitch > 0 )
        {
            _legPitch -= simulationStepTime / legPitchTime;
            if( _legPitch < 0 ) _legPitch = 0.0f;
        }
    }
    if( _legPitch < -1 ) _legPitch = -1;
    if( _legPitch > 1 ) _legPitch = 1;

    // velocity of base jumper's body
    NxVec3 velocity = _phActor->getLinearVelocity();

    // horizontal velocity of base jumper's body
    NxVec3 velocityH( velocity.x, 0, velocity.z );

    // vectical velocity of base jumper's body
    NxVec3 velocityV( 0, velocity.y, 0 );

    // local coordinate system of base jumper
    NxMat34 freeFallPose = _phActor->getGlobalPose();
    NxVec3 x = freeFallPose.M.getColumn(0);
    NxVec3 y = freeFallPose.M.getColumn(1);
    NxVec3 z = freeFallPose.M.getColumn(2);

    // air resistance coefficients
    float ARfrog = _jumper->getVirtues()->getFrogAirResistance();
    float ARtrac = _jumper->getVirtues()->getTrackingAirResistance();
    float ARhead = _jumper->getVirtues()->getHeadfirstAirResistance();
    float ARlift = 1.2f;

    // retrieve final resistance coefficient
    float AR = ARfrog * ( 1.0f - _tracking ) + ARtrac * _tracking;
    float headfirstFactor = z.dot( NxVec3(0,-1,0) );
    if( headfirstFactor < 0 ) headfirstFactor = 0.0f;
    AR = AR * ( 1.0f - headfirstFactor ) + ARhead * headfirstFactor;   

    // terminal velocity
    float Vt = sqrt( 9.8f * _phActor->getMass() / AR );
    float It = velocityV.magnitude() / Vt;

    // steering coefficients
    float Kslide = _jumper->getVirtues()->getSteerSlide();
    float Kroll  = _jumper->getVirtues()->getSteerRoll();
    float Kalign = _jumper->getVirtues()->getSteerAlign();

    // control torque
    NxVec3 Tctrl( 0,0,0 );

    float leftRate  = ( _jumper->getSpinalCord()->leftWarp > 0.0f ) ? _jumper->getSpinalCord()->leftWarp * 5.0f : _jumper->getSpinalCord()->left * 2.0f;
    float rightRate = ( _jumper->getSpinalCord()->rightWarp > 0.0f ) ? _jumper->getSpinalCord()->rightWarp * 5.0f : _jumper->getSpinalCord()->right * 2.0f;

    Tctrl += NxVec3(1,0,0) * -_jumper->getVirtues()->getSteerPitch() * _jumper->getSpinalCord()->down;
    Tctrl += NxVec3(1,0,0) * _jumper->getVirtues()->getSteerPitch() * _jumper->getSpinalCord()->up;
    Tctrl += NxVec3(0,0,1) * leftRate * -_jumper->getVirtues()->getSteerTurn();
    Tctrl += NxVec3(0,0,1) * rightRate * _jumper->getVirtues()->getSteerTurn();
    Tctrl *= It;

    // calculate steering torque
    NxVec3 signum = z; signum.y = 0.0f; signum.normalize();
    NxVec3 vertical(0,1,0);
    float steerAngle = calcAngle( y, vertical, signum );
    float inclinationAngle = fabs( calcAngle( z, signum ) );
    if( steerAngle > 0 ) inclinationAngle *= -1;
    steerAngle += inclinationAngle;
    if (_jumper->_player) {
        Kalign = 0.0f;
        //Kroll = 0.0f;
    }
    NxVec3 Tsteer = ( y * sqrt( velocity.magnitude() * Kslide ) * -steerAngle )+
                    ( vertical * sqrt( velocity.magnitude() * Kroll ) * -steerAngle ) +
                    ( z * sqrt( velocity.magnitude() * Kalign ) * -steerAngle );
    Tsteer *= getAirResistancePower( It );

    // gyration resistance
    float GRfrog = 20.0f;
    float GRtrac = 40.0f;
    float GR = GRfrog * ( 1.0f - _tracking ) + GRtrac * _tracking;
    float GRV = velocity.magnitude() / 25.0f;
    if( GRV > 1.0f ) GRV = 1.0f;
    GR *= GRV;
    NxVec3 Tr = _phActor->getAngularVelocity() * -GR; 


    database::Suit* suit = database::Suit::getRecord(_jumper->getVirtues()->equipment.suit.id);

    const float airDensity = 1.225f;

    const float wingAreaBox = suit->mWingAreaBox;
    const float wingAreaTrack = suit->mWingAreaTrack;

    const float wingLiftCoeffBox = suit->mWingLiftCoeffBox;
    const float wingLiftCoeffTrack = suit->mWingLiftCoeffTrack;
    const float wingLiftBackTrackEfficiency = suit->mWingLiftBackTrackEfficiency;

    const float dragCoeffBoxFront = suit->mDragCoeffBoxFront;
    const float dragCoeffBoxSide = suit->mDragCoeffBoxSide;
    const float dragCoeffBoxTop = suit->mDragCoeffBoxTop;
    const float dragCoeffTrackFront = suit->mDragCoeffTrackFront;
    const float dragCoeffTrackSide = suit->mDragCoeffTrackSide;
    const float dragCoeffTrackTop = suit->mDragCoeffTrackTop;

    const float wingArea = wingAreaBox + (wingAreaTrack - wingAreaBox) * _tracking;
    const float wingLiftCoeff = wingLiftCoeffBox + (wingLiftCoeffTrack - wingLiftCoeffBox) * _tracking;
    const float dragCoeffFront = dragCoeffBoxFront + (dragCoeffTrackFront - dragCoeffBoxFront) * _tracking;
    const float dragCoeffSide = dragCoeffBoxSide + (dragCoeffTrackSide - dragCoeffBoxSide) * _tracking;
    const float dragCoeffTop = dragCoeffBoxTop + (dragCoeffTrackTop - dragCoeffBoxTop) * _tracking;

    // lift force
    NxVec3 airFlowDirection = -velocity;
    airFlowDirection.normalize();
    NxVec3 Flift(0.0f, 0.0f, 0.0f);
    float squaredVelocity = velocity.magnitudeSquared();
    float wingAngleOfAttack = y.dot(airFlowDirection);
    if ((squaredVelocity != 0.0f) && (wingAngleOfAttack != 0.0f)) {
        float angle = fabsf(3.14159f - fabsf(acosf(wingAngleOfAttack) * 2.0f));
        float liftCoeff = 
                sinf(angle) *
                ((sinf(min(angle * angle * 9.0f + 2.5f, 3.14159f * 1.5f)) + 1.0f) * 1.0f * wingLiftCoeff + 1.0f)*
                (exp(3.14159f - angle * 3.0f + 2.5f) * 0.007f + 1.0f) * max(0.2f, 1.0f - fabsf(x.dot(airFlowDirection)));
        float lift = 0.5f * airDensity * squaredVelocity * liftCoeff * wingArea;

        // Backtracking gives worse performance
        if (z.dot(airFlowDirection) > 0.0f) {
                lift *= wingLiftBackTrackEfficiency;
        }

        NxVec3 side = y.cross(airFlowDirection);
        if (!side.isZero()) {
                Flift = airFlowDirection.cross(side);
                Flift.normalize();
                Flift *= (lift * sgn(wingAngleOfAttack));
        }
    }
    //Flift.zero();


    // drag force
    //const float airDensity = 1.225f;
    float xComponent = fabsf(x.dot(airFlowDirection));
    float yComponent = fabsf(y.dot(airFlowDirection));
    float zComponent = fabsf(z.dot(airFlowDirection));
    float t = fabsf(wingAngleOfAttack);
    const float dragCoeff = dragCoeffSide * xComponent + dragCoeffTop * yComponent + dragCoeffFront * zComponent;
    float drag = 0.5f * airDensity * squaredVelocity * dragCoeff;
    NxVec3 Fdrag = airFlowDirection * drag;
    //Fdrag.zero();

    // finalize motion equation    
    //_phActor->addForce( Far + Fg + Fgr + Flift);
    _phActor->addForce(Flift + Fdrag);
    _phActor->addTorque(Tr * It + Tsteer);
    _phActor->addLocalTorque( Tctrl );

/*
    // fake physics : horizontal velocity aligment
    float kHAlign = _tracking * ::simulationStepTime * _jumper->getVirtues()->getHorizontalAligment();
    NxVec3 lVel = _phActor->getLinearVelocity();
    NxVec3 vVel( 0,1,0 );
    vVel *= vVel.dot( lVel );
    NxVec3 hVel = lVel - vVel;
    float hVelMagnitude = hVel.magnitude();
    hVel.normalize();
    hVel = hVel + g * kHAlign;
    hVel.normalize();
    hVel *= hVelMagnitude;
    _phActor->setLinearVelocity( hVel + vVel );
//*/
}
