#include "VideoDecodeRender.h"

#include <cstdio>

#include "src/core/CommonFunctions.h"
#include "src/core/Logger.h"
#include "src/core/Tunnel.h"
#include "src/components/DecoderH264.h"
#include "src/components/VideoRenderer.h"

#define FILENAME "test.h264"

class VideoDecodeRender::DataClass
{
public:
    DataClass();
    ~DataClass();

    DecoderH264 decoder;
    VideoRenderer renderer;
};

VideoDecodeRender::DataClass::DataClass()
{
}

VideoDecodeRender::DataClass::~DataClass()
{
}

VideoDecodeRender::VideoDecodeRender()
    : TestCase( TESTCASE_NAME_VIDEO_DECODE_RENDER )
{
    d = new DataClass();
}

VideoDecodeRender::~VideoDecodeRender()
{
    delete d;
}

void VideoDecodeRender::Init()
{
    OMX_ERRORTYPE err = OMX_Init();
    if ( err != OMX_ErrorNone ) {
        LOG_ERR( "OMX_Init failed" + CommonFunctions::ErrorToString( err ) );
        return;
    }
    LOG_INFO( "OMX_Init successful" );
}

void VideoDecodeRender::Run()
{
    TestCase::Run();

    FILE* inputFile = fopen( FILENAME, "rb" );
    if ( inputFile == NULL ) {
        LOG_ERR( "Cannot open input file" );
        return;
    }

    bool ok = d->decoder.Init();
    if ( ok == false ) {
        LOG_ERR( "Error init component, destroying.." );
        return;
    }

    ok = d->renderer.Init();
    if ( ok == false ) {
        LOG_ERR( "Error init component, destroying.." );
        return;
    }

    Tunnel tunnelDecoderRenderer( &d->decoder, DecoderH264::OutputPort, &d->renderer, VideoRenderer::InputPort );

    ok = d->decoder.SetVideoParameters();
    if ( ok == false ) {
        LOG_ERR( "Error setting decoder params" );
        return;
    }

    ok = d->decoder.EnablePortBuffers( DecoderH264::InputPort );
    if ( ok == false ) {
        LOG_ERR( "Error enabling port buffers" );
        return;
    }

    ok = d->decoder.ChangeState( OMX_StateExecuting );
    if ( ok == false ) {
        LOG_ERR( "Error changing state to executing" );
        return;
    }

    OMX_BUFFERHEADERTYPE* buffer;
    ok = d->decoder.GetInputBuffer( DecoderH264::InputPort, buffer );
    if ( ok == false ){
        LOG_ERR( "Error get input buffer" );
        return;
    }

    //feed some data

    //wait for port settings changed on decoder

    ok = tunnelDecoderRenderer.SetupTunnel();
    if ( ok == false ){
        LOG_ERR( "Error setup tunnel" );
        return;
    }

    ok = d->renderer.SetRenderParameters();
    if ( ok == false ){
        LOG_ERR( "Error set render parameters" );
        return;
    }

    ok = d->renderer.ChangeState( OMX_StateExecuting );
    if ( ok == false ) {
        LOG_ERR( "Error changing state to executing" );
        return;
    }

    //wait for portsettingschanged on renderer

    bool hasMoreData = true;
    while ( hasMoreData == true ){
        //read data from file

        //set hasMoreData=false if is EOF

        ok = d->decoder.GetInputBuffer( DecoderH264::InputPort, buffer );
        if ( ok == false ){
            LOG_ERR( "error getting input buffer" );
            return;
        }

        //move data to buffer

        //call emptyThisBuffer
    }

    //cleanup
}

void VideoDecodeRender::Destroy()
{
    OMX_ERRORTYPE err = OMX_Deinit();
    if ( err != OMX_ErrorNone ) {
        LOG_ERR( "OMX_Deinit failed" + CommonFunctions::ErrorToString( err ) );
        return;
    }
    LOG_INFO( "OMX_Deinit successful" );
}
