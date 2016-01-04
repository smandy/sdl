env = Environment(LIBS=['SDLmain', 'SDL', 'SDL_mixer', 'GL','GLU','glut'], 
    CPPFLAGS=['-std=c++0x'],
                  LIBPATH = ['/usr/lib/x86_64-linux-gnu'],
                  CPPPATH=['/usr/include/SDL'])

env.Program('helloWorld.cpp')
env.Program('tutorial.cpp')
#env.Program('tutorial2.cpp')
env.Program('tutorial3.cpp')
env.Program('vortex.cpp')

if 1:
    gameApp      = env.Object( 'gameapp.cpp')
    eventHandler = env.Object( 'eventHandler.cpp')
    myHandler    = env.Object( 'myHandler.cpp')
    
    env.Program('platform', ['platform.cpp', gameApp, myHandler, eventHandler])
    #env.Program('pickSticks.cpp')





