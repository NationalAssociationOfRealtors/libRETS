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
  $CFLAGS += ' $(CFLAGS_STD) $(BOOST_CFLAGS) -I../../librets/include'
  $libs += ' $(LIBRETS_LIB) winmm.lib'
end


create_makefile('librets')

orig_makefile = IO::read("Makefile")
File.open("Makefile", "w") do |mfile|
  mfile.print %{
!include <../../build/Makefile.vc>
LIBRETS_LIB = ../../librets/src/$(BUILD_DIR)/$(LIB_PREFIX)rets$(LIB_RUNTIME)$(LIB_DEBUG_RUNTIME).$(LIB_EXT)
}

  mfile << orig_makefile

  mfile.print %{
librets_wrap.cxx: ../librets.i
\tswig -c++ -ruby ../librets.i

librets_wrap.cpp: ../librets.i
\tswig -c++ -ruby ../librets.i
}
end

