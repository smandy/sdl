env = Environment(LIBS=['SDLmain', 'SDL2', 'SDL2_mixer', 'SDL2_ttf', 'GL','GLU','glut'], 
    CPPFLAGS=['-std=c++1y', '-g'],
                  LIBPATH = ['/usr/lib/x86_64-linux-gnu'],
                  CPPPATH=['/usr/include/SDL2'])


vortex = env.Object('vortex.cpp')
mousey = env.Object('mousey.cpp')

env.Program('helloWorld.cpp')
env.Program('tutorial.cpp')
#env.Program('tutorial2.cpp')
env.Program('tutorial3.cpp')
#env.Program('vortex_main.cpp')

if 1:
    gameApp      = env.Object( 'gameapp.cpp')
    eventHandler = env.Object( 'eventHandler.cpp')
    myHandler    = env.Object( 'myHandler.cpp')
    headerfile    = env.Program( 'headerfile_tutorial.cpp')
    headerfile2    = env.Program( 'headerfile_tutorial2.cpp')
    
    env.Program('platform', ['platform.cpp', gameApp, myHandler, eventHandler, mousey])
    
    vortex_main = env.Program('vortex_main', ['vortex_main.cpp', vortex, mousey])
    #env.Program('pickSticks.cpp')





