#include "network.h"
#include "mtcnn.h"
#include <time.h>
#include "AL/alut.h"
#include "AL/al.h"
#include "AL/alut.h"
#include "AL/alc.h"

using namespace std;

namespace TE
{
    class TEAudio
    {
    private:
        static TEAudio* m_pAudio;

    public:
        TEAudio(void);
        ~TEAudio(void);

        //! Singleton. Return the single instance;
        static TEAudio* sharedAudio(void);

        //! Load file and play the media(only *.wav...now)
        void loadFile(const char* strFilePath);
    };
}

using namespace TE;

TEAudio* TEAudio::m_pAudio = NULL;

TE::TEAudio::TEAudio( void )
{
    int argc = 1;
    char* argv = "TEApp";
    if (!alutInit (&argc, &argv))
    {
        ALenum error = alutGetError ();
        fprintf (stderr, "%s\n", alutGetErrorString (error));
        exit (EXIT_FAILURE);
    }
}

TE::TEAudio::~TEAudio( void )
{

}

TEAudio* TE::TEAudio::sharedAudio( void )
{
    if (NULL == m_pAudio)
    {
        m_pAudio = new TEAudio();
    }
    return m_pAudio;
}

void TE::TEAudio::loadFile( const char* strFilePath )
{
    ALuint buffer;
    ALuint source;
    ALenum error;
    ALint status;

    /* Create an AL buffer from the given sound file. */
    buffer = alutCreateBufferFromFile (strFilePath);
    if (buffer == AL_NONE)
    {
        error = alutGetError ();
        fprintf (stderr, "Error loading file: '%s'\n",
            alutGetErrorString (error));
        alutExit ();
        exit (EXIT_FAILURE);
    }

    /* Generate a single source, attach the buffer to it and start playing. */
    alGenSources (1, &source);
    alSourcei (source, AL_BUFFER, buffer);
    alSourcePlay (source);

    /* Normally nothing should go wrong above, but one never knows... */
    error = alGetError ();
    if (error != ALUT_ERROR_NO_ERROR)
    {
        fprintf (stderr, "%s\n", alGetString (error));
        alutExit ();
        exit (EXIT_FAILURE);
    }

    /* Check every 0.3 seconds if the sound is still playing. */
    do
    {
        alutSleep (0.3f);
        alGetSourcei (source, AL_SOURCE_STATE, &status);

        std::cout << ".";
    }
    while (status == AL_PLAYING);

}

int main()
{
     Mat image;
     VideoCapture cap(0);
     if(!cap.isOpened())
         cout<<"fail to open!"<<endl;
     cap>>image;
     if(!image.data){
         cout<<"读取视频失败"<<endl;
         return -1;
     }
     TEAudio::sharedAudio()->loadFile("/home/einstein/桌面/1.wav");
     mtcnn find(image.rows, image.cols);
     clock_t start;
     for(;;){
         start = clock();
         cap>>image;
         find.findFace(image);
         resize(image,image,Size(image.cols*1.5,image.rows*1.5));
         imshow("result", image);
         if( waitKey(1)>=0 ) break;
         start = clock() -start;
         cout<<"time is  "<<start/10e3<<endl;
     }

    waitKey(0);
    image.release();
    return 0;
}

