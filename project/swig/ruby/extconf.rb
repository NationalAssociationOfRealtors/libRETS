=begin
ruby extconf.rb --with-librets-config=../../../librets-config-inplace
=end
require 'mkmf'

if PLATFORM =~ /darwin/
  CONFIG['LDSHARED'].sub!(/^cc/, "c++")
elsif PLATFORM =~ /linux/ || PLATFORM =~ /freebsd/
  CONFIG['LDSHARED'].sub!(/\$\(CC\)/, "$(CXX)")
end

makefile_prefix = ""
if PLATFORM =~ /darwin/ || PLATFORM =~ /linux/ || PLATFORM =~ /freebsd/
  librets_config = with_config("librets-config",
    "../../../librets-config-inplace")
  $libs += ' ' + `#{librets_config} --libs`.chomp
  if ENV['CFLAGS'].nil?
    $CFLAGS += ' ' + `#{librets_config} --cflags`.chomp
  else
    $CFLAGS += ' ' + ENV['CFLAGS'] + ' ' + `#{librets_config} --cflags`.chomp
  end
elsif PLATFORM =~ /win32/
  $CFLAGS += ' $(CFLAGS_STD) $(BOOST_CFLAGS) -I../../librets/include'
  $libs += ' $(LIBRETS_LIB) winmm.lib'
  makefile_prefix = %{
!include <../../build/Makefile.vc>
LIBRETS_LIB = ../../librets/src/$(BUILD_DIR)/$(LIB_PREFIX)rets$(LIB_RUNTIME)$(LIB_DEBUG_RUNTIME).$(LIB_EXT)
}
end

$INSTALLFILES = [['librets.rb', "$(RUBYLIBDIR)", "lib"]]
swig_dir=with_config("swig-dir", "../../../swig")

create_makefile('librets_native')

orig_makefile = IO::read("Makefile")
File.open("Makefile", "w") do |mfile|
  mfile << makefile_prefix
  mfile << orig_makefile

  mfile.print %{
librets_wrap.cxx: #{swig_dir}/librets.i
\tswig -c++ -ruby #{swig_dir}/librets.i

librets_wrap.cpp: #{swig_dir}/librets.i
\tswig -c++ -ruby #{swig_dir}/librets.i
  }

end
