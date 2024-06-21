files = ['triangle_frag.glsl', 'triangle_vert.glsl']
files_dict = {
 'triangle.frag': 'frag.psv',
 'triangle.vert': 'vert.psv'
}
env = environment()

message('compiling shaders')
foreach i,j : files_dict
  r = run_command('glslc', i, '-o', j, env:env, check:true)
  #this command below always gives an error, but it works as expected so...
  t = run_command('cp', j, source_root+'/build/shaders/', env:env, check:false)
  output = r.stdout().strip()
  err = r.stderr().strip()
  message(output)
  message(err)
endforeach
