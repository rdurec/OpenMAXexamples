#include "CommonFunctions.h"

#include <iostream>
#include <cstring>
#include <sstream>

#include "IL/OMX_Component.h"

using namespace std;

CommonFunctions::CommonFunctions()
{
}

std::string CommonFunctions::ErrorToString( int errorID )
{
    switch ( errorID ) {
        case OMX_ErrorInsufficientResources:
            return "OMX_ErrorInsufficientResources";
        case OMX_ErrorUndefined:
            return "OMX_ErrorUndefined";
        case OMX_ErrorInvalidComponentName:
            return "OMX_ErrorInvalidComponentName";
        case OMX_ErrorComponentNotFound:
            return "OMX_ErrorComponentNotFound";
        case OMX_ErrorInvalidComponent:
            return "OMX_ErrorInvalidComponent";
        case OMX_ErrorBadParameter:
            return "OMX_ErrorBadParameter";
        case OMX_ErrorNotImplemented:
            return "OMX_ErrorNotImplemented";
        case OMX_ErrorUnderflow:
            return "OMX_ErrorUnderflow";
        case OMX_ErrorOverflow:
            return "OMX_ErrorOverflow";
        case OMX_ErrorHardware:
            return "OMX_ErrorHardware";
        case OMX_ErrorInvalidState:
            return "OMX_ErrorInvalidState";
        case OMX_ErrorStreamCorrupt:
            return "OMX_ErrorStreamCorrupt";
        case OMX_ErrorPortsNotCompatible:
            return "OMX_ErrorPortsNotCompatible";
        case OMX_ErrorResourcesLost:
            return "OMX_ErrorResourcesLost";
        case OMX_ErrorNoMore:
            return "OMX_ErrorNoMore";
        case OMX_ErrorVersionMismatch:
            return "OMX_ErrorVersionMismatch";
        case OMX_ErrorNotReady:
            return "OMX_ErrorNotReady";
        case OMX_ErrorTimeout:
            return "OMX_ErrorTimeout";
        case OMX_ErrorSameState:
            return "OMX_ErrorSameState";
        case OMX_ErrorResourcesPreempted:
            return "OMX_ErrorResourcesPreempted";
        case OMX_ErrorPortUnresponsiveDuringAllocation:
            return "OMX_ErrorPortUnresponsiveDuringAllocation";
        case OMX_ErrorPortUnresponsiveDuringDeallocation:
            return "OMX_ErrorPortUnresponsiveDuringDeallocation";
        case OMX_ErrorPortUnresponsiveDuringStop:
            return "OMX_ErrorPortUnresponsiveDuringStop";
        case OMX_ErrorIncorrectStateTransition:
            return "OMX_ErrorIncorrectStateTransition";
        case OMX_ErrorIncorrectStateOperation:
            return "OMX_ErrorIncorrectStateOperation";
        case OMX_ErrorUnsupportedSetting:
            return "OMX_ErrorUnsupportedSetting";
        case OMX_ErrorUnsupportedIndex:
            return "OMX_ErrorUnsupportedIndex";
        case OMX_ErrorBadPortIndex:
            return "OMX_ErrorBadPortIndex";
        case OMX_ErrorPortUnpopulated:
            return "OMX_ErrorPortUnpopulated";
        case OMX_ErrorComponentSuspended:
            return "OMX_ErrorComponentSuspended";
        case OMX_ErrorDynamicResourcesUnavailable:
            return "OMX_ErrorDynamicResourcesUnavailable";
        case OMX_ErrorMbErrorsInFrame:
            return "OMX_ErrorMbErrorsInFrame";
        case OMX_ErrorFormatNotDetected:
            return "OMX_ErrorFormatNotDetected";
        case OMX_ErrorContentPipeOpenFailed:
            return "OMX_ErrorContentPipeOpenFailed";
        case OMX_ErrorContentPipeCreationFailed:
            return "OMX_ErrorContentPipeCreationFailed";
        case OMX_ErrorSeperateTablesUsed:
            return "OMX_ErrorSeperateTablesUsed";
        case OMX_ErrorTunnelingUnsupported:
            return "OMX_ErrorTunnelingUnsupported";
        default:
            return "unknown error";
    }
}

string CommonFunctions::StateToString( OMX_STATETYPE state )
{
    switch ( state ) {
        case OMX_StateExecuting:
            return "OMX_StateExecuting";
        case OMX_StateIdle:
            return "OMX_StateIdle";
        case OMX_StateInvalid:
            return "OMX_StateInvalid";
        case OMX_StateLoaded:
            return "OMX_StateLoaded";
        case OMX_StateWaitForResources:
            return "OMX_StateWaitForResources";
        case OMX_StatePause:
            return "OMX_StatePause";
        default:
            return "Other OMX_STATETYPE: " + state;
    }
}

std::string CommonFunctions::GetComponentState( OMX_HANDLETYPE handle )
{
    OMX_STATETYPE state;
    OMX_ERRORTYPE err;

    err = OMX_GetState( handle, &state );
    if ( err != OMX_ErrorNone ) {
        cout << ErrorToString( err ) << endl;
        return "Error on getting state";
    }

    switch ( state ) {
        case OMX_StateLoaded:
            return "StateLoaded";
        case OMX_StateIdle:
            return "StateIdle";
        case OMX_StateExecuting:
            return "StateExecuting";
        case OMX_StatePause:
            return "StatePause";
        case OMX_StateWaitForResources:
            return "StateWaitForResources";
        case OMX_StateInvalid:
            return "StateInvalid";
        default:
            return "unknown state: " + state;
    }
}

bool CommonFunctions::DisableAllPorts( OMX_HANDLETYPE handle )
{
    cout << "Trying to disable all ports" << endl;

    OMX_INDEXTYPE types[] = {OMX_IndexParamAudioInit, OMX_IndexParamVideoInit, OMX_IndexParamImageInit, OMX_IndexParamOtherInit};

    for ( int i = 0; i < 4; i++ ) {
        cout << "Disabling ports with type " << i << endl;

        OMX_PORT_PARAM_TYPE param;
        memset( &param, 0, sizeof( OMX_PORT_PARAM_TYPE ) );
        param.nSize = sizeof( OMX_PORT_PARAM_TYPE );
        param.nVersion.nVersion = OMX_VERSION;

        OMX_ERRORTYPE err = OMX_GetParameter( handle, types[i], &param );
        if ( err != OMX_ErrorNone ) {
            cout << ErrorToString( err ) << endl;
            return false;
        }

        int firstPort = param.nStartPortNumber;
        int portCount = param.nPorts;
        for ( int portNumber = firstPort; portNumber < firstPort + portCount; portNumber++ ) {
            OMX_SendCommand( handle, OMX_CommandPortDisable, portNumber, NULL );
        }
    }
    cout << "All ports disabled" << endl;
}

void CommonFunctions::InitStructure( OMX_PORT_PARAM_TYPE& structure )
{
    memset( &structure, 0, sizeof( OMX_PORT_PARAM_TYPE ) );
    structure.nSize = sizeof( OMX_PORT_PARAM_TYPE );
    structure.nVersion.nVersion = OMX_VERSION;
    structure.nVersion.s.nVersionMajor = OMX_VERSION_MAJOR;
    structure.nVersion.s.nVersionMinor = OMX_VERSION_MINOR;
    structure.nVersion.s.nRevision = OMX_VERSION_REVISION;
    structure.nVersion.s.nStep = OMX_VERSION_STEP;
}

void CommonFunctions::InitStructure( OMX_VIDEO_PARAM_PORTFORMATTYPE& structure )
{
    memset( &structure, 0, sizeof( OMX_VIDEO_PARAM_PORTFORMATTYPE ) );
    structure.nSize = sizeof( OMX_VIDEO_PARAM_PORTFORMATTYPE );
    structure.nVersion.nVersion = OMX_VERSION;
    structure.nVersion.s.nVersionMajor = OMX_VERSION_MAJOR;
    structure.nVersion.s.nVersionMinor = OMX_VERSION_MINOR;
    structure.nVersion.s.nRevision = OMX_VERSION_REVISION;
    structure.nVersion.s.nStep = OMX_VERSION_STEP;
}

void CommonFunctions::InitStructure( OMX_PARAM_PORTDEFINITIONTYPE& structure )
{
    memset( &structure, 0, sizeof( OMX_PARAM_PORTDEFINITIONTYPE ) );
    structure.nSize = sizeof( OMX_PARAM_PORTDEFINITIONTYPE );
    structure.nVersion.nVersion = OMX_VERSION;
    structure.nVersion.s.nVersionMajor = OMX_VERSION_MAJOR;
    structure.nVersion.s.nVersionMinor = OMX_VERSION_MINOR;
    structure.nVersion.s.nRevision = OMX_VERSION_REVISION;
    structure.nVersion.s.nStep = OMX_VERSION_STEP;
}

string CommonFunctions::IntToString( int a )
{
    stringstream s;
    s << a;
    return s.str();
}