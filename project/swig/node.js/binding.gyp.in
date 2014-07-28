{
  "targets": [
    {
      "target_name": "librets",
      "sources": [ "librets_wrap.cpp" ],
      "include_dirs": [ '/Users/kgarner/src/odbcrets/librets/project/librets/include', '/opt/local/include' ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'libraries': ['/Users/kgarner/src/odbcrets/librets/build/librets/lib/librets.a /opt/local/lib/libboost_filesystem-mt.a /opt/local/lib/libboost_system-mt.a /opt/local/lib/libcurl.a /opt/local/lib/libcurl.a /opt/local/lib/libidn.a /opt/local/lib/libssl.a /opt/local/lib/libcrypto.a /opt/local/lib/libssl.a /opt/local/lib/libz.a /opt/local/lib/libexpat.a'],
       'conditions': [
         ['OS=="mac"', {
       	    'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
    }
  ]
}
