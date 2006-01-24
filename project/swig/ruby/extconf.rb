=begin
ruby extconf.rb --with-librets-config=../../../librets-config-inplace
=end
require 'mkmf'

if PLATFORM =~ /darwin/
  CONFIG['LDSHARED'].sub!(/^cc/, "c++")
elsif PLATFORM =~ /linux/
  CONFIG['LDSHARED'].sub!(/\$\(CC\)/, "$(CXX)")
end

if PLATFORM =~ /darwin/ || PLATFORM =~ /linux/
  librets_config = with_config("librets-config", "librets-config")
  $libs += ' ' + `#{librets_config} --libs`.chomp
  $CFLAGS += ' ' + `#{librets_config} --cflags`.chomp
elsif PLATFORM =~ /win32/
  $CFLAGS += ' /MD /DBOOST_USE_WINDOWS_H /GR /GX /nologo -I../../librets/include -Ic:/odbcrets/vendor/boost'
  $libs += ' ../../librets/src/build/librets.lib winmm.lib'
#  $LDFLAGS += ' 
end


create_makefile('librets')

mfile = File.open("Makefile", "a")

mfile.print %{
librets_wrap.cxx: ../librets.i
\tswig -c++ -ruby ../librets.i
}
mfile.close
