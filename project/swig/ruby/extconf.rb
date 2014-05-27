=begin
ruby extconf.rb --with-librets-config=../../../librets-config-inplace
=end
require 'mkmf'

puts "Platform: " + RUBY_PLATFORM.to_s

if RUBY_PLATFORM =~ /darwin/
  CONFIG['LDSHARED'].sub!(/^cc/, "c++")
elsif RUBY_PLATFORM =~ /linux/ || RUBY_PLATFORM =~ /freebsd/
  CONFIG['LDSHARED'].sub!(/\$\(CC\)/, "$(CXX)")
elsif RUBY_PLATFORM =~ /i386-mingw32/
  CONFIG['LDSHARED'].sub!(/\$\(CC\)/, "$(CXX)")
end

makefile_prefix = ""
if RUBY_PLATFORM =~ /darwin/ || RUBY_PLATFORM =~ /linux/ || RUBY_PLATFORM =~ /freebsd/ 
  librets_config = with_config("librets-config",
    "../../../librets-config-inplace")
  $libs += ' ' + `#{librets_config} --libs`.chomp
  if ENV['CFLAGS'].nil?
    $CFLAGS += ' ' + `#{librets_config} --cflags`.chomp
  else
    $CFLAGS += ' ' + ENV['CFLAGS'] + ' ' + `#{librets_config} --cflags`.chomp
  end
  swig_dir=with_config("swig-dir", "../../../swig")
elsif RUBY_PLATFORM =~ /win32/ 
  $CFLAGS += ' $(CFLAGS_STD) $(BOOST_CFLAGS) -I.. -I../../librets/include'
  $libs += ' $(LIBRETS_LIB) winmm.lib wldap32.lib gdi32.lib'
  makefile_prefix = %{
!include <../../build/Makefile.vc>
LIBRETS_LIB = ../../librets/src/$(BUILD_DIR)/$(LIBRETS_LIB_NAME)
}
  swig_dir=with_config("swig-dir", "..")
elsif RUBY_PLATFORM =~ /i386-mingw32/
  librets_cflags = with_config("librets-cflags")
  if librets_cflags.nil?
    abort("--with-librets-cflags is required")
  end

  librets_libs = with_config("librets-libs")
  if librets_libs.nil?
    abort("--with-librets-libs is required")
  end

  $libs += ' ' + librets_libs.chomp
  if ENV['CFLAGS'].nil?
    $CFLAGS += ' ' + librets_cflags.chomp
  else
    $CFLAGS += ' ' + ENV['CFLAGS'] + ' ' + librets_cflags.chomp
  end
  $CFLAGS += ' -DHAVE_STRUCT_TIMESPEC'
  $CFLAGS += ' -DHAVE_STRUCT_TIMEZONE'

  swig_dir=with_config("swig-dir", "../../../project/swig")
end

$INSTALLFILES = [['librets.rb', "$(RUBYLIBDIR)", "lib"]]

create_makefile('librets_native')

orig_makefile = IO::read("Makefile")
if RUBY_PLATFORM =~ /linux/
  # Make sure the proper version boost libraries are detected first by changing ruby's create_makefile library declaration order
  orig_makefile.gsub!(/LIBS = \$\(LIBRUBYARG_SHARED\)(.+)/, "LIBS = \\1 \$\(LIBRUBYARG_SHARED\)");
  orig_makefile.gsub!(/(-o \$@ \$\(OBJS\))( \$\(LIBPATH\) \$\(DLDFLAGS\) \$\(LOCAL_LIBS\))( \$\(LIBS\))/, "\\1\\3\\2")
  # Ruby 2.0 improperly generates CXXFLAGS without including CFLAGS which causes compilation to fail due to missing librets.h
  orig_makefile.gsub!(/CXXFLAGS = \$\(CCDLFLAGS\)/, "CXXFLAGS = \$\(CFLAGS\)");
end
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
