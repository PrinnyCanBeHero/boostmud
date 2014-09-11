import os

env = Environment()
env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME']=1
env['OBJSUFFIX'] = '.o'
env['SHOBJSUFFIX'] = '.o'
env.Append(CCFLAGS='-std=c++11')
env.Append(CCFLAGS='-g')
env.Append(CPPPATH='#/src')
env.Append(LIBPATH='#/bin')
env.Append(LIBS=['boost_thread-mt', 'boost_system-mt'])

Export('env')

env.SConscript('#/src/world/SConscript')
env.SConscript('#/src/util/SConscript')

env.SConscript('#/src/SConscript')