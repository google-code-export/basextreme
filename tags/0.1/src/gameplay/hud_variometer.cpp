
#include "headers.h"
#include "hud.h"

/**
 * actor abstracts
 */

void Variometer::onUpdateActivity(float dt)
{
    _timeout -= dt;
    if( _timeout < 0 ) _timeout = 0;

    if( _timeout > 0 )
    {
        resetIndicator( _window->getPanel()->find( "Digit4" ) );
        resetIndicator( _window->getPanel()->find( "Digit3" ) );
        resetIndicator( _window->getPanel()->find( "Digit2" ) );
        resetIndicator( _window->getPanel()->find( "Digit1" ) );
        resetIndicator( _window->getPanel()->find( "Digit0" ) );

        resetIndicator( _window->getPanel()->find( "Digit24" ) );
        resetIndicator( _window->getPanel()->find( "Digit23" ) );
        resetIndicator( _window->getPanel()->find( "Digit22" ) );
        resetIndicator( _window->getPanel()->find( "Digit21" ) );
        resetIndicator( _window->getPanel()->find( "Digit20" ) );

        resetIndicator( _window->getPanel()->find( "Digit31" ) );
        resetIndicator( _window->getPanel()->find( "Digit30" ) );
    }
    else
    {
        // retrieve parent actor altitude
        Vector3f parentVel = _parent->getVel();
        float fall = parentVel[1];
        float move = (float)sqrtf(parentVel[0] * parentVel[0] + parentVel[2] * parentVel[2]);

        // draw signum digit
        setSignumIndicator( _window->getPanel()->find( "Digit4" ), fall );    
        setSignumIndicator( _window->getPanel()->find( "Digit24" ), move );    

        // absolute units conversion
        int fallUnits = int(fabs(fall) * 0.01f * 10.0f);
        int moveUnits = int(move * 0.01f * 10.0f);
        // Are we using feet units?
        if ( Gameplay::iGameplay->_feetModeIsEnabled )
        {
            fallUnits = (int)(fabs(fall) * 0.01f * 10.0f * 3.2808399f);
            moveUnits = (int)(move * 0.01f * 10.0f * 3.2808399f);
        }

        // digits pickup
        int digit = fallUnits % 10;
        setIndicator( _window->getPanel()->find( "Digit0" ), digit );
    
        fallUnits = fallUnits / 10;
        digit = fallUnits % 10;
        setIndicator( _window->getPanel()->find( "Digit1" ), digit );

        fallUnits = fallUnits / 10;
        digit = fallUnits % 10;
        setIndicator( _window->getPanel()->find( "Digit2" ), digit );

        fallUnits = fallUnits / 10;
        digit = fallUnits % 10;
        setIndicator( _window->getPanel()->find( "Digit3" ), digit );

        // digits pickup
        digit = moveUnits % 10;
        setIndicator( _window->getPanel()->find( "Digit20" ), digit );
    
        moveUnits = moveUnits / 10;
        digit = moveUnits % 10;
        setIndicator( _window->getPanel()->find( "Digit21" ), digit );

        moveUnits = moveUnits / 10;
        digit = moveUnits % 10;
        setIndicator( _window->getPanel()->find( "Digit22" ), digit );

        moveUnits = moveUnits / 10;
        digit = moveUnits % 10;
        setIndicator( _window->getPanel()->find( "Digit23" ), digit );

        int ratio = 0;
        if (fall != 0.0f) {
            ratio = (int)((move / fabsf(fall)) * 10.0f);
        }
        // digits pickup
        digit = ratio % 10;
        setIndicator( _window->getPanel()->find( "Digit30" ), digit );
    
        ratio = ratio / 10;
        digit = ratio % 10;
        setIndicator( _window->getPanel()->find( "Digit31" ), digit );
    }
}

void Variometer::onEvent(Actor* initiator, unsigned int eventId, void* eventData)
{
    if( eventId == EVENT_HUD_HIDE )
    {
        if( _window->getPanel()->getParent() )
        {
            _window->getPanel()->getParent()->removePanel( _window->getPanel() );
        }
    }
}

/**
 * class implemetation
 */
   
Variometer::Variometer(Actor* parent) : HUD( parent )
{
    // window
    _window = Gameplay::iGui->createWindow( "Variometer" ); assert( _window );
    Gameplay::iGui->getDesktop()->insertPanel( _window->getPanel() );
    _window->align( gui::atTop, 0, gui::atRight, 0 );

    // timeout
    _timeout = getCore()->getRandToolkit()->getUniform( 1, 3 );
}

Variometer::~Variometer()
{
    Gameplay::iGui->getDesktop()->removePanel( _window->getPanel() );
    _window->getPanel()->release();
}