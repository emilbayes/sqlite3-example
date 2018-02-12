{
  'variables': {
    'target_arch%': '<!(node preinstall.js --print-arch)>'
  },
  'targets': [
    {
      'target_name': 'libsqlite3',
      'include_dirs' : [
        "<!(node -e \"require('nan')\")",
        "/usr/local/opt/sqlite/include"
      ],
      'sources': [
        'binding.cc',
      ],
      'xcode_settings': {
        'OTHER_CFLAGS': [
          '-g',
          '-O3'
        ]
      },
      'cflags': [
        '-g',
        '-O3'
      ],
      'libraries': [
        '/usr/local/opt/sqlite/lib/libsqlite3.dylib'
      ],
      'conditions': [
        ['OS != "mac" and OS != "win"', {
          'link_settings': {
            'libraries': [ "-Wl,-rpath=\\$$ORIGIN/lib"]
          }
        }],
        ['OS != "win"', {
            'product_prefix': 'lib'
        }]
      ],
    }
  ]
}
