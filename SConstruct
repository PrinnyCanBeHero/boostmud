import os

env = Environment()
env.Append(CCFLAGS='-std=c++11')
env.Append(CCFLAGS='-g')
env.Append(LIBS=['boost_thread-mt', 'boost_system-mt'])

boostmud = env.Program(target='boostmud',
  source = Glob('*.cpp'))

Default(boostmud)
