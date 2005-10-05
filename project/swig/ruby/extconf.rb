=begin
ruby extconf.rb --with-librets-config=../../../librets-config-inplace
=end
require 'mkmf'

if PLATFORM =~ /darwin/
  CONFIG['LDSHARED'].sub!(/^cc/, "c++")
elsif
  CONFIG['LDSHARED'].sub!(/gcc/, "g++")
end

librets_config = with_config("librets-config", "librets-config")
$libs += ' ' + `#{librets_config} --libs`.chomp
$CFLAGS += ' ' + `#{librets_config} --cflags`.chomp

create_makefile('librets')

mfile = File.open("Makefile", "a")

mfile.print %{
librets_wrap.cxx: ../librets.i
\tswig -c++ -ruby ../librets.i
}
mfile.close
